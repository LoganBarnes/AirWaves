// AirWaves.cpp

#include <iostream>
#include <memory>

#include "shared/core/ContinuousDriver.hpp"
#include "shared/core/World.hpp"
#include "shared/core/VulkanIOHandler.hpp"

#include "AirWavesConfig.hpp"


/////////////////////////////////////////////
/// \brief main
/// \return
///
/// \author Logan Barnes
/////////////////////////////////////////////
int
main(
     const int    argc, ///< number of arguments
     const char **argv  ///< array of argument strings
     )
{

  shared::Driver::printProjectInfo(
                                   vmp::PROJECT_NAME,
                                   vmp::VERSION_MAJOR,
                                   vmp::VERSION_MINOR,
                                   vmp::VERSION_PATCH
                                   );

  try
  {
    //
    // create world to handle physical updates
    // and ioHandler to interface between the
    // world and the user
    //
    shared::World world;
    shared::VulkanIOHandler io( world );

    //
    // pass world and ioHandler to driver
    // to manage event loop
    //
    shared::ContinuousDriver driver( world, io );

    //
    // run program
    //
    return driver.exec( argc - 1, &argv[ 1 ] );
  }
  catch ( const std::exception &e )
  {
    std::cerr << "ERROR: program failed: " << e.what( ) << std::endl;
  }

  // should only reach this point after an exception
  return EXIT_FAILURE;

} // main
