#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream>

namespace test {
  using namespace std;
  
  struct CallRecorder
  {
    mutable map<string, vector<string>> __calls;
    
    void __rc( const string &function_name ) const
    {
      __calls[function_name].push_back( "" );
    }

    template <typename T>
      void __rc( const string &function_name, const T &desc ) const
      {
        stringstream buf;
        buf << desc;
        __calls[function_name].push_back( { buf.str() } );
      }

    const bool __called( const string &function_name )
    {
      try {
        __calls.at( function_name );
        return true;
      } catch ( const out_of_range &e ) {
        return false;
      }
    }
  };
  
}
