/** @file DG/Graphics/RenderInterface.hpp */

#pragma once

#include <DG_Pch.hpp>
#include <DG/Graphics/Color.hpp>
#include <DG/Graphics/VertexArray.hpp>

namespace dg
{

  /**
   * @brief The @a `RenderPrimitiveType` enum enumerates the different types of primitives into
   *        which the renderer can group vertices when performing draw calls.
   */
  enum class RenderPrimitiveType
  {
    Triangles,
    Points,
    Lines
  };

  /**
   * @brief The @a `RenderInterface` class is a static helper class which is used by the
   *        @a `Renderer` to directly interface with OpenGL.
   */
  class RenderInterface
  {
  public:

    /**
     * @brief Initializes the render interface.
     */
    static void initialize ();

    /**
     * @brief   Sets the viewport of the current framebuffer.
     * 
     * @param   size  The viewport's size, in pixels. 
     */
    static void setViewport (const Vector2u& size);

    /**
     * @brief Sets the color to clear the current framebuffer to.
     *
     * @param color The color to clear to.
     */
    static void setClearColor (const Color& color);

    /**
     * @brief Clears the current framebuffer, or the window if none is set.
     */
    static void clear ();

    /**
     * @brief Performs an indexed draw call, rendering the vertices found in the given
     *        @a `VertexArray` as indexed by its bound @a `IndexBuffer`.
     * 
     * @param vao         Points to the @a `VertexArray` object containing the vertices to render.
     * @param indexCount  The number of vertex indices to render.
     */
    static void drawIndexed (const Ref<VertexArray>& vao, Count indexCount = -1);

    /**
     * @brief   Retrieves the type of primitive into which vertices should be grouped.
     * 
     * @return  The current @a `RenderPrimitiveType`. 
     */
    static RenderPrimitiveType getPrimitiveType ();

    /**
     * @brief   Sets the type of primitive into which vertices should be grouped.
     * 
     * @param   type  The new @a `RenderPrimitiveType`. 
     */
    static void setPrimitiveType (const RenderPrimitiveType type);

  private:
    /**
     * @brief   Resolves the @a `RenderInterface`'s current primitive type into the proper GL
     *          enumeration.
     * 
     * @return  The resolved GL enumeration. 
     */
    static GLenum resolvePrimitiveType ();

  private:
    /**
     * @brief The type of primitive into which vertices should be grouped.
     */
    static RenderPrimitiveType s_primitiveType;

  };

}
