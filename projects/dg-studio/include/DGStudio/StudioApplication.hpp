/** @file DGStudio/StudioApplication.hpp */

#pragma once

#include <DGStudio/StudioLayer.hpp>

namespace dgstudio
{

  class StudioApplication : public dg::Application
  {
  public:
    StudioApplication (const dg::ApplicationSpecification& spec = {});
    ~StudioApplication ();

  private:
    dg::Scope<StudioLayer> m_studioLayer = nullptr;

  };

}
