#include "gtest/gtest.h"

#include "fhq/processors/time.h"
#include <string>

namespace {
  using namespace std;
  using ::testing::Test;
  using fhq::processors::Time;
  using mongo::BSONObj;
  using mongo::Date_t;
  using mongo::DATENOW;

  namespace processing {

    struct TimeProcessingTest : public Test
    {
    protected:
      Time __proc;
      void operator() ( BSONObj &obj )
      {
        __proc( obj );
      }
    };

    // Should add a time field an object with no time field
    TEST_F( TimeProcessingTest,
            ShouldAddATimeFieldAnObjectWithNoTimeField ) {
      BSONObj obj( BSON( "object" << string( "with" ) << "no" << string( "time" ) ) );
      ( *this )( obj );
      EXPECT_EQ( string( "with" ), obj.getStringField( "object" ) );
      EXPECT_EQ( string( "time" ), obj.getStringField( "no" ) );
      EXPECT_TRUE( obj.hasField( "time" ) );
    }

    // Should leave an object with a time field alone
    TEST_F( TimeProcessingTest,
            ShouldLeaveAnObjectWithATimeFieldAlone ) {
      BSONObj obj( BSON( "object" << string( "with" ) << "time" << Date_t( 1365709002614 ) ) );
      Date_t orig_date( obj["time"].Date() );
      ( *this )( obj );
      EXPECT_EQ( string( "with" ), obj.getStringField( "object" ) );
      EXPECT_EQ( orig_date, obj["time"].Date() );
    }
    
  } // processing
}
