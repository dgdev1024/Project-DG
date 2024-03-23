/** @file DGStudio/StudioLayer.cpp */

#include <DGStudio/StudioLayer.hpp>

namespace dgstudio
{

  struct TestVertex
  {
    dg::Vector3f pos;
    dg::Vector2f texcoord;
  };

  StudioLayer::StudioLayer () :
    dg::Layer { "Studio Layer" }
  {

  }

  StudioLayer::~StudioLayer ()
  {
    
  }

  void StudioLayer::onAttach ()
  {

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
    // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    ImGui::ShowDemoWindow();
  }
  
}
