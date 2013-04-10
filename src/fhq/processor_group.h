#pragma once

#include "fhq/processor_concept.h"

namespace fhq {
  using namespace std;

  class ProcessorGroup {
  public:
    VIRTUAL void operator() ( BSONObj &obj )
    {
      throw runtime_error( "ProcessorGroup() not implemented." );
    }
  };
}
