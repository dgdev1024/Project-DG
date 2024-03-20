/** @file DGStudio/StudioLayer.hpp */

#pragma once

#include <DGStudio_Pch.hpp>

namespace dgstudio
{

  class StudioLayer : public dg::Layer
  {
  public:
    StudioLayer ();
    ~StudioLayer ();

    void onAttach () override;
    void onDetach () override;
    void processEvent (dg::Event& ev) override;
    void fixedUpdate (const dg::Float32 timestep) override;
    void update () override;
    void guiUpdate () override;

  };

}
