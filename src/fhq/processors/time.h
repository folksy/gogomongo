#pragma once

#include "fhq/processor_concept.h"

namespace fhq {
  namespace processors {
    using namespace std;

    class Time
    {
    public:
      void operator() ( BSONObj &obj )
      {
        throw runtime_error( "fhq::processors::Time( ... ) not implemented." );
      }

      TEST_ONLY( const string desc() const { return "time"; } )
    };
  }
}
