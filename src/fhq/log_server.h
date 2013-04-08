#pragma once

#include <map>
#include <vector>
#include <string>
#include <stdexcept>

namespace fhq {
  using namespace std;

  class LogServer
  {
  public:
    using config_t = pair<string, vector<string>>;
    using configs_t = map<string, config_t>;
    
    LogServer( const string &connection_string,
               const string &db,
               const string &input_collection,
               const configs_t &configs,
               const string &errlogfname ) {}

    void operator() ()
    {
      throw runtime_error( "fhq::LogServer::() not implemented." );
    }
    
  private:
  };
}
