#include "gtest/gtest.h"

#include <map>
#include <string>

namespace {
  using namespace std;
  using ::testing::Test;

  // Maps should be comparable in tests
  TEST( SanityChecks,
        MapsShouldBeComparableInTests ) {
    map<string, string> m1 { { "hello", "world" } };
    map<string, string> m2 { { "hello", "world" } };
    map<string, string> m3 { { "hello", "wonky" } };
    map<string, string> m4, m5;
    EXPECT_EQ( m1, m2 );
    EXPECT_NE( m1, m3 );
    EXPECT_NE( m1, m4 );
    EXPECT_EQ( m4, m5 );
  }
}
