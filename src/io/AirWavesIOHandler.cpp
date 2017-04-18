#include "AirWavesIOHandler.hpp"

// shared
#include "imgui.h"
#include "shared/graphics/OpenGLWrapper.hpp"
#include "shared/graphics/ImguiCallback.hpp"

// project
#include "AirWavesConfig.hpp"


namespace vmp
{


namespace
{

constexpr int defaultWidth  = 1024;
constexpr int defaultHeight = 720;

}


/////////////////////////////////////////////
/// \brief AirWavesIOHandler::AirWavesIOHandler
///
/// \author Logan Barnes
/////////////////////////////////////////////
AirWavesIOHandler::AirWavesIOHandler( shared::World &world )
  : ImguiOpenGLIOHandler( world, true, defaultWidth, defaultHeight )
{}



AirWavesIOHandler::~AirWavesIOHandler( )
{}



/////////////////////////////////////////////
/// \brief AirWavesIOHandler::onRender
/// \param alpha
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
AirWavesIOHandler::_onRender( const double )
{
  upGLWrapper_->clearWindow( );

} // AirWavesIOHandler::onRender



/////////////////////////////////////////////
/// \brief AirWavesIOHandler::_onGuiRender
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
AirWavesIOHandler::_onGuiRender( )
{
  bool alwaysOpen = true;

  ImGui::SetNextWindowPos ( ImVec2( 0, 0 ), ImGuiSetCond_FirstUseEver );
  ImGui::SetNextWindowSize( ImVec2( 0, 0 ), ImGuiSetCond_FirstUseEver ); // auto scale size not working right now?

  ImGui::Begin( "Settings", &alwaysOpen );

  //
  // FPS
  //
  ImGui::Text(
              "Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO( ).Framerate,
              ImGui::GetIO( ).Framerate
              );

  ImGui::End( );
} // AirWavesIOHandler::onGuiRender



} // namespace vmp
