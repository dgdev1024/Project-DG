/** @file DGStudio/StudioApplication.cpp */

#include <DGStudio/StudioApplication.hpp>

namespace dgstudio
{

  StudioApplication::StudioApplication (const dg::ApplicationSpecification& spec) :
    dg::Application { spec }
  {
    m_renderer->setClearColor({ 0.2f, 0.3f, 0.3f });

    m_studioLayer = dg::makeScope<StudioLayer>();
    attachLayer(*m_studioLayer);
  }

  StudioApplication::~StudioApplication ()
  {
    detachLayer(*m_studioLayer);
    m_studioLayer.reset();
  }
  
}
