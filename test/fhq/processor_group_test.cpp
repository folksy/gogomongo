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
}
