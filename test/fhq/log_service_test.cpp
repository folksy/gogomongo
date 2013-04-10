#include "gtest/gtest.h"
#include "util/call_recorder.h"

#include "fhq/log_service.h"

namespace {
  using namespace std;
  using ::testing::Test;
  using test::CallRecorder;
  using fhq::config_t;
  using fhq::configs_t;
  using fhq::LogService;
  using fhq::configs_t;
  using fhq::processors_t;

  namespace log_service_construction {
    // struct LogServiceMock : public LogService, public CallRecorder
    // {
    //   LogServiceMock( const string &host, const int port, const string &ns,
    //                  const configs_t &configs, const string &errlogfname )
    //     : LogService( host, port, ns, configs, errlogfname )
    //   {}

    //   virtual processors_t & _generate_processors( const configs_t &configs ) const
    //   {
    //     cout << "_generate_processors: " << &configs << endl;
    //     __rc( "_generate_processors", &configs );
    //   }
    // };

    struct LogServiceConstructionTest : public Test
    {
      using log_server_t = LogService;
    protected:
      string __host = "localheist";
      int __port = 12345;
      string __ns = "somedb.somecollection";
      configs_t __configs = { { "thename", { "the.namespace", { "time" } } } };
      string __errlogfname = "some/error/file.log";
      log_server_t __server;

      LogServiceConstructionTest()
        : __server( __host, __port, __ns, __configs, __errlogfname )
      {}
    };

    // Should store the host
    TEST_F( LogServiceConstructionTest,
            ShouldStoreTheHost ) {
      EXPECT_EQ( __host, __server.host() );
    }

    // Should store the port
    TEST_F( LogServiceConstructionTest,
            ShouldStoreThePort ) {
      EXPECT_EQ( __port, __server.port() );
    }

    // Should store the namespace
    TEST_F( LogServiceConstructionTest,
            ShouldStoreTheNamespace ) {
      EXPECT_EQ( __ns, __server.ns() );
    }

    // Should store the error log filename
    TEST_F( LogServiceConstructionTest,
            ShouldStoreTheErrorLogFilename ) {
      EXPECT_EQ( __errlogfname, __server.errlogfname() );
    }

    // Should generate processors to match passed-in configs
    TEST_F( LogServiceConstructionTest,
            ShouldGenerateProcessorsToMatchPassedInConfigs ) {
      processors_t p( const_cast<processors_t &>( __server.processors() ) );
      EXPECT_EQ( static_cast<size_t>( 1 ), p.size() );
      EXPECT_EQ( "the.namespace", p["thename"].first );
      EXPECT_EQ( "group", p["thename"].second.desc() );
    }
    
  }
}
