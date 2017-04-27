// AirWavesIOHandler.hpp
#pragma once

#include "shared/core/ImguiOpenGLIOHandler.hpp"
#include <string>


namespace shared
{

class World;

}


namespace vmp
{


/////////////////////////////////////////////
/// \brief The AirWavesIOHandler class
///
/// \author Logan Barnes
/////////////////////////////////////////////
class AirWavesIOHandler : public shs::ImguiOpenGLIOHandler
{

public:

  ///////////////////////////////////////////////////////////////
  /// \brief AirWavesIOHandler
  ///////////////////////////////////////////////////////////////
  AirWavesIOHandler( shs::World &world );


  ///////////////////////////////////////////////////////////////
  /// \brief ~AirWavesIOHandler
  ///////////////////////////////////////////////////////////////
  ~AirWavesIOHandler( );


protected:

private:

  virtual
  void _onRender ( const double alpha ) final;

  virtual
  void _onGuiRender ( ) final;


};


} // namespace vmp
