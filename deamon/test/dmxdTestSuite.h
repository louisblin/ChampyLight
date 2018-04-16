#include <cxxtest/TestSuite.h>

class DmxdTestSuite : public CxxTest::TestSuite 
{
  public:
    // Dummy test to make sure everything works
    void testAddition( void )
    {
      TS_ASSERT( 1 + 1 > 1 );
      TS_ASSERT_EQUALS( 1 + 1, 2 );
    }
};

