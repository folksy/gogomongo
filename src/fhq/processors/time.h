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
          for ( auto i( obj.begin() ) ; i.more() ; ) {
            auto e( i.next() );
            b << e;
          }
          b << "time" << DATENOW;
          obj = b.obj();
        }
      }

    PRIVATE:
      const string desc() const { return "time"; }
    };
  }
}
