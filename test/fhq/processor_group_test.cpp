#include "gtest/gtest.h"

#include "fhq/processor_group.h"

namespace {
  using namespace std;
  using ::testing::Test;
  using fhq::ProcessorGroup;

  template <typename T>
    const string class_name( const T & );
  template <>
    const string class_name<ProcessorGroup>( const ProcessorGroup &pg )
    {
      return "ProcessorGroup";
    }

  struct ProcessorGroupPropertiesTest : public Test
  {
  protected:
    ProcessorGroup __pgroup;
  };

  // Should know its typename for the tests
  TEST_F( ProcessorGroupPropertiesTest,
          ShouldKnowItsTypenameForTheTests ) {
    EXPECT_EQ( "ProcessorGroup", class_name( __pgroup ) );
  }

  namespace appending_processors {
    
    struct ProcessorGroupAppendingProcessorsTest : public Test
    {
    protected:
      ProcessorGroup __pgroup;
    };

    // Should be able to append arbitrary numbers of time processors
    TEST_F( ProcessorGroupAppendingProcessorsTest,
            ShouldBeAbleToAppendArbitraryNumbersOfTimeProcessors ) {
      EXPECT_EQ( static_cast<size_t>( 0 ), __pgroup.processors().size() );
      __pgroup << "time";
      ASSERT_EQ( static_cast<size_t>( 1 ), __pgroup.processors().size() );
      EXPECT_EQ( "time", __pgroup.processors()[0].desc() );
      __pgroup << "time";
      __pgroup << "time";
      __pgroup << "time";
      __pgroup << "time";
      ASSERT_EQ( static_cast<size_t>( 5 ), __pgroup.processors().size() );
      for ( const auto &processor : __pgroup.processors() )
        EXPECT_EQ( "time", processor.desc() );
    }

    // Should throw unknown processor error if processor desc unrecognised
    TEST_F( ProcessorGroupAppendingProcessorsTest,
            ShouldThrowUnknownProcessorErrorIfProcessorDescUnrecognised ) {
      EXPECT_THROW( __pgroup << "moose", fhq::UnknownProcessorError );
    }
  } // appending_processors
  
}
