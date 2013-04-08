#pragma once

#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include "mongo/client/dbclient.h"

namespace fhq {
  using namespace std;
  using namespace mongo;

  class LogServer
  {
  public:
    using config_t = pair<string, vector<string>>;
    using configs_t = map<string, config_t>;
    
    LogServer( const string &host, const string &db, const int port,
               const string &input_collection,
               const configs_t &configs, const string &errlogfname ) {}

    void operator() ()
    {
      throw runtime_error( "fhq::LogServer::() not implemented." );
      DBClientConnection conn( true );
      conn.connect( HostAndPort( __host, __port ) );
      Query query = QUERY( "message" << BSON( "$exists" << true ) )
        .hint( BSON( "$natural" << 1 ) );
      while ( true ) {
        auto cursor( conn.query( __ns, query, 0, 0, 0,
                                 QueryOption_CursorTailable | QueryOption_AwaitData ) );
        while ( true ) {
          if ( ! cursor->more() ) {
            if ( cursor->isDead() )
              break;
            else
              continue;
          }
          BSONObj obj = cursor->next();
          throw runtime_error( "TODO -- remove message from original" );
          throw runtime_error( "TODO -- get processor based on object logname" );
          throw runtime_error( "TODO -- dispatch to processor" );
          throw runtime_error( "TODO -- save processed obj to processor ns" );
        } // wend
        query = QUERY( "message" << BSON( "$exists" << true ) )
          .hint( BSON( "$natural" << 1 ) );
      } // wend
    }
    
  private:
  };
}
