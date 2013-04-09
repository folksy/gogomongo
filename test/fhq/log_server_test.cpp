#include "gtest/gtest.h"

#include "fhq/log_server.h"

namespace {
  using namespace std;
  using ::testing::Test;
  using fhq::config_t;
  using fhq::configs_t;
  using fhq::LogServer;

  namespace log_server_construction {

    struct LogServerConstructionTest : public Test
    {
    protected:
      string __host = "localheist";
      int __port = 12345;
      string __ns = "somedb.somecollection";
      configs_t __configs;
      string __errlogfname = "some/error/file.log";
      LogServer __server;

      LogServerConstructionTest()
        : __server( __host, __port, __ns, __configs, __errlogfname )
      {}
    };

    // Should store the host
    TEST_F( LogServerConstructionTest,
            ShouldStoreTheHost ) {
      EXPECT_EQ( __host, __server.host() );
    }

    // Should store the port
    TEST_F( LogServerConstructionTest,
            ShouldStoreThePort ) {
      EXPECT_EQ( __port, __server.port() );
    }

    // Should store the namespace
    TEST_F( LogServerConstructionTest,
            ShouldStoreTheNamespace ) {
      EXPECT_EQ( __ns, __server.ns() );
    }

    // Should store the error log filename
    TEST_F( LogServerConstructionTest,
            ShouldStoreTheErrorLogFilename ) {
      EXPECT_EQ( __errlogfname, __server.errlogfname() );
    }
    
  }
}
