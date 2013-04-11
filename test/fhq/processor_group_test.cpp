#include "gtest/gtest.h"
#include "util/call_recorder.h"

#include "mongo/client/dbclient.h"
#include "fhq/processor_group.h"

namespace {
  using namespace std;
  using ::testing::Test;
  using fhq::ProcessorGroup;

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

  namespace processing {
    using fhq::processor_t;
    using mongo::BSONObj;
    using mongo::BSONArray;
    using mongo::BSONArrayBuilder;
    using mongo::GENOID;

    struct ProcessorMock : public test::CallRecorder
    {
      ProcessorMock( const int id ) : __id( id ) {}
      
      void operator() ( BSONObj &obj )
      {
        BSONArrayBuilder b;
        for ( auto &v : obj["pmids"].Array() )
          b.append( v.Int() );
        b.append( __id );
        obj = BSON( "_id" << obj["_id"] << "pmids" << b.arr() );
        __rc( "()", obj["_id"] );
      }
      
      const string desc() const { return ""; }
    private:
      int __id;
    };
    
    struct ProcessorGroupMock : public ProcessorGroup
    {
      ProcessorGroupMock( const vector<processor_t> &processors )
      {
        for ( const auto &p : processors )
          add_processor( p );
      }
    };

    struct ProcessorGroupProcessingTest : public Test
    {
    protected:
      BSONObj __obj = BSON( GENOID << "pmids" << BSONArray() );
      vector<processor_t> __processors = { ProcessorMock( 0 ),
                                           ProcessorMock( 1 ),
                                           ProcessorMock( 2 ) };
      ProcessorGroupMock __pgroup = { __processors };

      ProcessorGroupProcessingTest()
      {
        __pgroup( __obj );
      }
    };

    // Should invoke each of the processors it contains on the object
    TEST_F( ProcessorGroupProcessingTest,
            ShouldInvokeEachOfTheProcessorsItContainsOnTheObject ) {
      for ( auto &p : __processors )
        EXPECT_TRUE( dynamic_cast<fhq::processor_w<ProcessorMock> &>( p.p() ).t()
                       .__called( "()", __obj["_id"] ) );
    }

    // Should collect cumulative processes on the object
    TEST_F( ProcessorGroupProcessingTest,
            ShouldCollectCumulativeProcessesOnTheObject ) {
      vector<int> pmids;
      for ( auto &v : __obj["pmids"].Array() )
        pmids.push_back( v.Int() );
      EXPECT_EQ( static_cast<size_t>( 3 ), pmids.size() );
      for ( int i( 0 ) ; i < static_cast<int>( pmids.size() ) ; ++ i )
        EXPECT_EQ( i, pmids[i] );
    }
  } // processing
  
}
