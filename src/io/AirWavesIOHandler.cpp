#include "AirWavesIOHandler.hpp"

// project
#include "AirWavesConfig.hpp"

// shared
#include "shared/graphics/OpenGLWrapper.hpp"
#include "shared/graphics/ImguiCallback.hpp"
#include <imgui.h>


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
AirWavesIOHandler::AirWavesIOHandler( shs::World &world )
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

  ImGui::SetNextWindowPos ( ImVec2( 0, 0 ) );
  ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );

  ImGui::Begin( "Settings", &alwaysOpen, ImGuiWindowFlags_AlwaysAutoResize );


  //
  // FPS
  //
  ImGui::Text(
              "Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO( ).Framerate,
              ImGui::GetIO( ).Framerate
              );

  ImGui::End( );
  ImGui::PopStyleVar( );

} // AirWavesIOHandler::onGuiRender



} // namespace vmp
