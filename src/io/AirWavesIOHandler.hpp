// AirWavesIOHandler.hpp
#pragma once


#include <string>
#include "shared/core/ImguiOpenGLIOHandler.hpp"


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
class AirWavesIOHandler : public shared::ImguiOpenGLIOHandler
{

public:

  ///////////////////////////////////////////////////////////////
  /// \brief AirWavesIOHandler
  ///////////////////////////////////////////////////////////////
  AirWavesIOHandler( shared::World &world );


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
