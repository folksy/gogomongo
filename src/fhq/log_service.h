#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include "mongo/client/dbclient.h"
#include "fhq/processor_concept.h"
#include "fhq/processor_group.h"

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
      : __host( host ), __port( port ), __ns( ns ),
        __configs( configs ), __errlogfname( errlogfname )
    {}
    LogService( const LogService & ) = delete;
    LogService( LogService && ) = delete;

    const string & host() const { return __host; }
    const int port() const { return __port; }
    const string & ns() const { return __ns; }
    const configs_t & configs() const { return __configs; }
    const string & errlogfname() const { return __errlogfname; }

    void operator() ()
    {
      throw runtime_error( "fhq::LogService::() not implemented." );
      /* DBClientConnection conn( true ); */
      /* conn.connect( HostAndPort( __host, __port ) ); */
      /* Query query = QUERY( "message" << BSON( "$exists" << true ) ) */
      /*   .hint( BSON( "$natural" << 1 ) ); */
      /* while ( true ) { */
      /*   auto cursor( conn.query( __ns, query, 0, 0, 0, */
      /*                            QueryOption_CursorTailable | QueryOption_AwaitData ) ); */
      /*   while ( true ) { */
      /*     if ( ! cursor->more() ) { */
      /*       if ( cursor->isDead() ) */
      /*         break; */
      /*       else */
      /*         continue; */
      /*     } */
      /*     BSONObj obj = cursor->next(); */
      /*     throw runtime_error( "TODO -- remove message from original" ); */
      /*     throw runtime_error( "TODO -- get processor based on object logname" ); */
      /*     throw runtime_error( "TODO -- dispatch to processor" ); */
      /*     throw runtime_error( "TODO -- save processed obj to processor ns" ); */
      /*   } // wend */
      /*   query = QUERY( "message" << BSON( "$exists" << true ) ) */
      /*     .hint( BSON( "$natural" << 1 ) ); */
      /* } // wend */
    }

  PROTECTED:
    VIRTUAL processors_t & _processors()
    {
      throw runtime_error( "LogService::_processors( ... ) not implemented." );
      if ( __processors == nullptr ) {
        __processors = make_shared<processors_t>();
        for ( const auto &kv : __configs ) {
          ProcessorGroup pg;
          for ( const auto &desc : kv.second.second ) {
            if ( desc.compare( "time" ) == 0 ) {
              // TODO
            } else if ( desc.compare( "original-message" ) == 0 ) {
              // TODO
            } else
              throw runtime_error( "Unrecognised processor desc" );
          } // descs
          ( *__processors )[kv.first] = { kv.second.first, std::move( pg ) };
        } // __configs
      }
      return *__processors;
    }
    
  private:
    string __host;
    int __port;
    string __ns;
    configs_t __configs;
    string __errlogfname;
    shared_ptr<processors_t> __processors;
  };
}
