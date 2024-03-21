/** @file DGStudio/StudioLayer.cpp */

#include <DGStudio/StudioLayer.hpp>

namespace dgstudio
{

  StudioLayer::StudioLayer () :
    dg::Layer { "Studio Layer" }
  {

  }

  StudioLayer::~StudioLayer ()
  {
    
  }

  void StudioLayer::onAttach ()
  {
    dg::ColorPalette palette { "assets/background.pal" };
    dg::Application::getRenderer().setClearColor(palette[1]);
  }

  void StudioLayer::onDetach ()
  {

  }

  void StudioLayer::processEvent (dg::Event& ev)
  {

  }

  void StudioLayer::fixedUpdate (const dg::Float32 timestep)
  {

  }

  void StudioLayer::update ()
  {

  }

  void StudioLayer::guiUpdate ()
  {
    ImGui::ShowDemoWindow();
  }
  
}
