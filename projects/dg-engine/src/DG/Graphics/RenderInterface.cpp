/** @file DG/Graphics/RenderInterface.cpp */

#include <DG/Graphics/RenderInterface.hpp>

namespace dg
{

  RenderPrimitiveType RenderInterface::s_primitiveType = RenderPrimitiveType::Triangles;

  void RenderInterface::initialize ()
  {
    GLenum result = glewInit();
    if (result != GLEW_OK) {
      DG_ENGINE_CRIT("Error initializing GLEW - {}: {}!", result, glewGetErrorString(result));
      throw std::runtime_error { "Error initializing GLEW!" };
    }
  }

  void RenderInterface::setViewport (const Vector2u& size)
  {
    glViewport(0, 0, size.x, size.y);
  }

  void RenderInterface::setClearColor (const Color& color)
  {
    glClearColor(color.red, color.green, color.blue, color.alpha);
  }

  void RenderInterface::clear ()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void RenderInterface::drawIndexed (const Ref<VertexArray>& vao, Count indexCount)
  {

    // Ensure that a vertex array object is provided!
    if (vao == nullptr) {
      throw std::invalid_argument { "Attempted 'drawIndexed' on null vertex array object!" };
    }

    // Since this is an indexed draw call, ensure that the given VAO has an index buffer object
    // attached to it!
    auto ibo = vao->getIndexBuffer();
    if (ibo == nullptr) {
      throw std::invalid_argument { 
        "Attempted 'drawIndexed' on vertex array object with no index buffer attached!" 
      };
    }

    // If the given index count is greater than the attached IBO's index count, then correct
    // the index count.
    if (indexCount > ibo->getIndexCount()) {
      indexCount = ibo->getIndexCount();
    }

    // Bind the VAO and perform the draw call.
    vao->bind();
    glDrawElements(resolvePrimitiveType(), indexCount, ibo->resolveIndexType(), nullptr);

  }

  RenderPrimitiveType RenderInterface::getPrimitiveType ()
  {
    return s_primitiveType;
  }

  void RenderInterface::setPrimitiveType (const RenderPrimitiveType type)
  {
    s_primitiveType = type;
  }

  GLenum RenderInterface::resolvePrimitiveType ()
  {
    switch (s_primitiveType)
    {
      case RenderPrimitiveType::Triangles:  return GL_TRIANGLES;
      case RenderPrimitiveType::Points:     return GL_POINTS;
      case RenderPrimitiveType::Lines:      return GL_LINES;
      default:                              return GL_TRIANGLES;
    }
  }

}
