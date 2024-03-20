/** @file DG/Graphics/Renderer.hpp */

#pragma once

#include <DG/Graphics/RenderInterface.hpp>

namespace dg
{

  /**
   * @brief The @a `RendererSpecification` struct describes attributes defining the @a `Renderer`.
   */
  struct RendererSpecification
  {

  };

  /**
   * @brief The @a `Renderer` class is responsible for interfacing with the rendering interface to
   *        draw objects.
   */
  class Renderer
  {
  public:
    Renderer (const RendererSpecification& spec);
    ~Renderer ();

    /**
     * @brief Creates a new @a `Renderer`.
     * 
     * @return  A pointer to the newly-created @a `Renderer`. 
     */
    static Scope<Renderer> make (const RendererSpecification& spec = {});

    /**
     * @brief Sets the color to clear the current framebuffer to.
     *
     * @param color The color to clear to.
     */
    void setClearColor (const Color& color);

    /**
     * @brief Clears the current framebuffer, or the window if none is set.
     */
    void clear ();

  };

}
