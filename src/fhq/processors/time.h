#pragma once

#include "fhq/processor_concept.h"
#include <string>

namespace fhq {
  namespace processors {
    using namespace std;

    class Time
    {
    public:
      void operator() ( BSONObj &obj )
      {
        if ( ! obj.hasField( "time" ) ) {
          BSONObjBuilder b;
          for ( auto i( obj.begin() ) ; i.more() ; )
            b << i.next();
          b << "time" << DATENOW;
          obj = b.obj();
        }
      }

    PRIVATE:
      const string desc() const { return "time"; }
    };
  }
}
