#pragma once

#include "fhq/processor_concept.h"
#include "fhq/processors/time.h"

#include <string>
#include <stdexcept>

namespace fhq {
  using namespace std;

  struct UnknownProcessorError : public runtime_error
  {
    UnknownProcessorError( const string &desc ) :
      runtime_error( ( string( "Unknown processor: " ) + desc ).c_str() ) {}
  };

  class ProcessorGroup
  {
  public:
    VIRTUAL ~ProcessorGroup() {}
    
    ProcessorGroup & operator << ( const string &desc )
    {
      if ( desc.compare( "time" ) == 0 )
        __processors.push_back( processors::Time() );
      else
        throw UnknownProcessorError( desc );
      return *this;
    }
    
    VIRTUAL void operator() ( BSONObj &obj )
    {
      for ( auto &processor : __processors )
        processor( obj );
    }
    
  PRIVATE:
    vector<processor_t> __processors;

    TEST_ONLY(
    public:
      const vector<processor_t> & processors() const { return __processors; }
      const string desc() const { return "group"; }
      void add_processor( const processor_t &p ) { __processors.push_back( p ); }
    ) // TEST_ONLY
  };
}
