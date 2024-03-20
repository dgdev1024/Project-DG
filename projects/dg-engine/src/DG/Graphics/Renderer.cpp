/** @file DG/Graphics/Renderer.cpp */

#include <DG/Graphics/Renderer.hpp>

namespace dg
{

  Renderer::Renderer (const RendererSpecification& spec)
  {
    RenderInterface::initialize();
  }

  Renderer::~Renderer ()
  {

  }

  Scope<Renderer> Renderer::make (const RendererSpecification& spec)
  {
    return makeScope<Renderer>(spec);
  }

  void Renderer::setClearColor (const Color& color)
  {
    RenderInterface::setClearColor(color);
  }

  void Renderer::clear ()
  {
    RenderInterface::clear();
  }

}
