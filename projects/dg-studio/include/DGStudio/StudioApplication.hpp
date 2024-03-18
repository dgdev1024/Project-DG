/** @file DGStudio/StudioApplication.hpp */

#pragma once

#include <DGStudio_Pch.hpp>

namespace dgstudio
{

  class StudioApplication : public dg::Application
  {
  public:
    StudioApplication (const dg::ApplicationSpecification& spec = {});
    ~StudioApplication ();

  };

}
