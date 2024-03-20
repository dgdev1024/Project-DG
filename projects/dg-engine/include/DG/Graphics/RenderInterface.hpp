/** @file DG/Graphics/RenderInterface.hpp */

#pragma once

#include <DG_Pch.hpp>
#include <DG/Graphics/Color.hpp>

namespace dg
{

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
     * @brief Sets the color to clear the current framebuffer to.
     *
     * @param color The color to clear to.
     */
    static void setClearColor (const Color& color);

    /**
     * @brief Clears the current framebuffer, or the window if none is set.
     */
    static void clear ();

  };

}
