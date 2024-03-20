/** @file DG/Graphics/RenderInterface.cpp */

#include <DG/Graphics/RenderInterface.hpp>

namespace dg
{

  void RenderInterface::initialize ()
  {
    GLenum result = glewInit();
    if (result != GLEW_OK) {
      DG_ENGINE_CRIT("Error initializing GLEW - {}: {}!", result, glewGetErrorString(result));
      throw std::runtime_error { "Error initializing GLEW!" };
    }
  }

  void RenderInterface::setClearColor (const Color& color)
  {
    glClearColor(color.red, color.green, color.blue, color.alpha);
  }

  void RenderInterface::clear ()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

}
