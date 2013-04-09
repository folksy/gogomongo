#include "gtest/gtest.h"

#include "fhq/log_service.h"

namespace {
  using namespace std;
  using ::testing::Test;
  using fhq::config_t;
  using fhq::configs_t;
  using fhq::LogService;

  namespace log_server_construction {
    struct LogServiceMock : public LogService
    {
      LogServiceMock( const string &host, const int port, const string &ns,
                     const configs_t &configs, const string &errlogfname )
        : LogService( host, port, ns, configs, errlogfname )
      {}
    };

    struct LogServiceConstructionTest : public Test
    {
      using log_server_t = LogServiceMock;
    protected:
      string __host = "localheist";
      int __port = 12345;
      string __ns = "somedb.somecollection";
      configs_t __configs;
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
    
  }
}
