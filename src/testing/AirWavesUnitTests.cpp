// AirWavesUnitTestsUnitTests.cpp
#include "gmock/gmock.h"


namespace
{


///
/// \brief The AirWavesUnitTestsUnitTests class
///
class AirWavesUnitTestsUnitTests : public ::testing::Test
{

protected:

  /////////////////////////////////////////////////////////////////
  /// \brief AirWavesUnitTestsUnitTests
  /////////////////////////////////////////////////////////////////
  AirWavesUnitTestsUnitTests( )
  {}


  /////////////////////////////////////////////////////////////////
  /// \brief ~AirWavesUnitTestsUnitTests
  /////////////////////////////////////////////////////////////////
  virtual
  ~AirWavesUnitTestsUnitTests( )
  {}

};


/////////////////////////////////////////////////////////////////
/// \brief UnimplementedTest
/////////////////////////////////////////////////////////////////
TEST_F( AirWavesUnitTestsUnitTests, UnimplementedTest )
{
  ASSERT_TRUE( false ) << "Test not yet implemented.";
}



} // namespace
