#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include "mongo/client/dbclient.h"
#include "b23/file.h"
#include "fhq/processor_concept.h"
#include "fhq/processor_group.h"

#define ERROR(f,n) b23::file::appender_t( f ) << n << "\n";
#define DEBUG(n) b23::file::appender_t( "log/debug.log" ) << n << "\n";

namespace fhq {
  using namespace std;
  using namespace mongo;
  using config_t = pair<string, vector<string>>;
  using configs_t = map<string, config_t>;
  using processors_t = map<string, pair<string, processor_t>>;

  class LogService
  {
  public:
    LogService( const string &host, const int port, const string &ns,
               const configs_t &configs, const string &errlogfname )
      : __host( host ), __port( port ), __ns( ns ), __errlogfname( errlogfname )
    {
      for ( const auto &kv : configs ) {
        ProcessorGroup pg;
        for ( const auto &desc : kv.second.second )
          pg << desc;
        __processors[kv.first] = { kv.second.first, std::move( pg ) };
      }
    }
    
    LogService( const LogService & ) = delete;
    LogService( LogService && ) = delete;

    void operator() ()
    {
      DBClientConnection conn( true );
      conn.connect( HostAndPort( host(), port() ) );
      Query query = QUERY( "message" << BSON( "$exists" << true ) )
        .hint( BSON( "$natural" << 1 ) );
      while ( true ) {
        auto cursor( conn.query( ns(), query, 0, 0, 0,
                                 QueryOption_CursorTailable | QueryOption_AwaitData ) );
        while ( true ) {
          if ( ! cursor->more() ) {
            if ( cursor->isDead() )
              break;
            else
              continue;
          }
          BSONObj obj = cursor->next();
          conn.update( ns(),
                       BSON( "_id" << obj["_id"] ),
                       BSON( "$unset" << BSON( "message" << true ) ) );
          try {
            auto ns_and_proc( __processors.at( obj.getStringField( "logid" ) ) );
            ns_and_proc.second( obj );
            conn.insert( ns_and_proc.first, obj );
          } catch ( const out_of_range &e ) {
            ERROR( errlogfname(), "Unhandled logid: " << obj["logid"] );
          }
        } // wend
        query = QUERY( "message" << BSON( "$exists" << true ) )
          .hint( BSON( "$natural" << 1 ) );
      } // wend
    }

  PRIVATE:
    const string & host() const { return __host; }
    const int port() const { return __port; }
    const string & ns() const { return __ns; }
    const string & errlogfname() const { return __errlogfname; }
    const processors_t & processors() const { return __processors; }
    
  private:
    string __host;
    int __port;
    string __ns;
    configs_t __configs;
    string __errlogfname;
    processors_t __processors;
  };
}
