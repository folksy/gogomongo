#pragma once

#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include "mongo/client/dbclient.h"

namespace fhq {
  using namespace std;
  using namespace mongo;
  using config_t = pair<string, vector<string>>;
  using configs_t = map<string, config_t>;

  class LogServer
  {
  public:
    LogServer( const string &host, const int port, const string &ns,
               const configs_t &configs, const string &errlogfname )
      : __host( host ), __port( port ), __ns( ns ), __errlogfname( errlogfname )
    {}
    LogServer( const LogServer & ) = delete;
    LogServer( LogServer && ) = delete;

    const string & host() const { return __host; }
    const int port() const { return __port; }
    const string & ns() const { return __ns; }
    const string & errlogfname() const { return __errlogfname; }

    void operator() ()
    {
      throw runtime_error( "fhq::LogServer::() not implemented." );
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
    
  private:
    string __host;
    int __port;
    string __ns;
    string __errlogfname;
  };
}
