/** @file DG/Graphics/FrameBuffer.hpp */

#pragma once

#include <DG_Pch.hpp>

namespace dg
{

  /** Frame Buffer Constants and Enumerations *****************************************************/

  /**
   * @brief The maximum allowed number of color attachment textures in a @a `FrameBuffer`.
   */
  constexpr Count FRAMEBUFFER_COLOR_ATTACHMENT_COUNT = 4;

  /**
   * @brief The @a `FrameBufferTextureFormat` enum enumerates the different kinds of formats of
   *        color and depth texture attachments used by a @a `FrameBuffer`.
   */
  enum class FrameBufferTextureFormat
  {
    None = 0,
    ColorR32,
    ColorRGBA8,
    Depth24Stencil8,

    Color = ColorRGBA8,
    DepthSetncil = Depth24Stencil8,
    Depth = Depth24Stencil8
  };

  enum class FrameBufferTarget
  {
    Reading,
    Drawing,
    Both
  };

  /** Frame Buffer Specifications *****************************************************************/

  /**
   * @brief The @a `FrameBufferTextureSpecification` struct describes the attributes used to define
   *        a texture attachment to a @a `FrameBuffer`.
   */
  struct FrameBufferTextureSpecification
  {

    FrameBufferTextureSpecification () = default;
    FrameBufferTextureSpecification (FrameBufferTextureFormat format) :
      textureFormat { format }
    {
      
    }


    /**
     * @brief The attachment texture's format.
     */
    FrameBufferTextureFormat textureFormat = FrameBufferTextureFormat::None;

    /**
     * @brief   Retrieves whether or not this framebuffer texture attachment is a depth texture.
     * 
     * @return  @a `true` if this is a depth texture; 
     *          @a `false` otherwise.
     */
    Bool isDepthTextureFormat () const;

  };

  /**
   * @brief The @a `FrameBufferAttachmentSpecification` struct describes attributes used to
   *        collectively define all of the attachments to a @a `FrameBuffer`.
   */
  struct FrameBufferAttachmentSpecification
  {
    
    FrameBufferAttachmentSpecification () = default;

    /**
     * @brief   Constructs a new @a `FrameBufferAttachmentSpecification` with the given initializer
     *          list of texture specifications.
     * 
     * @param   specs The initializer list of texture specifications.
     */
    FrameBufferAttachmentSpecification (const InitList<FrameBufferTextureSpecification>& specs) :
      attachments { specs }
    {

    }

    /**
     * @brief A collection of all of the attachments' individual specifications.
     */
    Collection<FrameBufferTextureSpecification> attachments;

  };

  /**
   * @brief The @a `FrameBufferSpecification` struct describes attributes used to define a
   *        @a `FrameBuffer` and its attachments.
   */
  struct FrameBufferSpecification
  {

    /**
     * @brief Contains the size of the @a `FrameBuffer`, in pixels.
     */
    Vector2u size = { 1280, 720 };

    /**
     * @brief The number of texture samples making up this @a `FrameBuffer`.
     */
    Uint32 sampleCount = 1;

    /**
     * @brief The @a `FrameBuffer`'s attachment specification.
     */
    FrameBufferAttachmentSpecification attachmentSpec;

  };

  /** Frame Buffer Class **************************************************************************/

  /**
   * @brief The @a `FrameBuffer` class, also sometimes called the @a `RenderTarget` class, is a
   *        graphics buffer which can be used to render off-screen to a target texture.
   */
  class FrameBuffer
  {
  public:
    FrameBuffer (const FrameBufferSpecification& spec);
    ~FrameBuffer ();

    /**
     * @brief   Creates a new @a `FrameBuffer` with the given specification.
     * 
     * @param   spec  The new frame buffer's specification.
     *  
     * @return  A shared pointer to the newly-created @a `FrameBuffer`.
     */
    static Ref<FrameBuffer> make (const FrameBufferSpecification& spec = {});

    /**
     * @brief   Sets this @a `FrameBuffer` as the current @a `GL_FRAMEBUFFER` for drawing, reading
     *          or both.
     * 
     * @param   target  The type of target to set this @a `FrameBuffer` as.
     */
    void bind (const FrameBufferTarget target = FrameBufferTarget::Drawing) const;

    /**
     * @brief   Un-sets the current @a `GL_FRAMEBUFFER` for drawing, reading or both.
     * 
     * @param   target  The type of target to un-set.
     */
    static void unbind (const FrameBufferTarget target = FrameBufferTarget::Drawing);

    /**
     * @brief   Clears the color attachment with the given index in this @a `FrameBuffer`, using the
     *          given integer as clear data.
     * 
     * @param   index     The index of the color attachment texture to clear. 
     * @param   clearData The integer data to clear the color attachment texture to.
     */
    void clearColorAttachment (const Index index, Int32 clearData);

    /**
     * @brief   Reads the value of a pixel from a color attachment in this @a `FrameBuffer` at the
     *          given index.
     * 
     * @param   index The index of the color attachment texture to read.
     * @param   x     The target pixel's X coordinate.
     * @param   y     The target pixel's Y coordinate.
     * 
     * @return  The value of the pixel requested.
     */
    Int32 readPixel (const Index index, Int32 x, Int32 y);
    Int32 readPixel (const Index index, Float32 x, Float32 y);

    /**
     * @brief   Retrieves the unique ID coresponding to one of the @a `FrameBuffer`'s color 
     *          attachment handles on the graphics card.
     * 
     * @param   index   The index of the color handle to retrieve.
     * 
     * @return  The requested color handle.
     */
    Uint32 getColorHandle (const Index index = 0) const;

    /**
     * @brief   Retrieves the unique ID coresponding to the @a `FrameBuffer`'s depth / stencil 
     *          attachment handle on the graphics card.
     * 
     * @return  The depth handle.
     */
    Uint32 getDepthHandle () const;

    /**
     * @brief   Retrieves a pointer to one of the @a `FrameBuffer`'s underlying color attachment 
     *          textures.
     * 
     * @param   index   The index of the color attachment texture to retrieve.
     * 
     * @return  A pointer to the frame buffer's underlying texture.
     * 
     * @note    This method is generally intended for use by client applications which use Dear 
     *          ImGui.
     */
    void* getTextureHandle (const Index index = 0) const;

  public:

    /**
     * @brief     Retrieves the @a `FrameBuffer`'s specification.
     * 
     * @returns   The frame buffer's specification.
     */
    inline const FrameBufferSpecification& getSpecification () const
    {
      return m_spec;
    }

    /**
     * @brief   Retrieves the width of this @a `FrameBuffer`.
     * 
     * @return  The frame buffer's width, in pixels.
     */
    inline Uint32 getWidth () const { return m_spec.size.x; }

    /**
     * @brief   Retrieves the height of this @a `FrameBuffer`.
     * 
     * @return  The frame buffer's height, in pixels.
     */
    inline Uint32 getHeight () const { return m_spec.size.y; }

    /**
     * @brief   Retrieves the size of this @a `FrameBuffer`.
     * 
     * @return  The frame buffer's size, in pixels. 
     */
    inline const Vector2u& getSize () const { return m_spec.size; }

    /**
     * @brief   Sets the size of this @a `FrameBuffer`.
     * 
     * @param   width   The frame buffer's new width, in pixels.
     * @param   height  The frame buffer's new height, in pixels.
     * 
     * @return  @a `true` if the framebuffer's width or height is changed to a different value, 
     *          causing the framebuffer to be rebuilt; 
     *          @a `false` otherwise.
     */
    inline Bool setSize (const Uint32 width, const Uint32 height)
    {
      if (width > 0 && height > 0 && (width != m_spec.size.x || 
        height != m_spec.size.y)) 
      {
        m_spec.size.x = width;
        m_spec.size.y = height;
        build();

        return true;
      }

      return false;
    }

    /**
     * @brief   Sets the size of this @a `FrameBuffer`.
     * 
     * @param   size  The frame buffer's new size, in pixels.
     * 
     * @return  @a `true` if the framebuffer's width or height is changed to a different value, 
     *          causing the framebuffer to be rebuilt; 
     *          @a `false` otherwise.
     */
    inline Bool setSize (const Vector2u& size)
    {
      return setSize(size.x, size.y);
    }

  private:

    /**
     * @brief Builds (or rebuilds) the @a `FrameBuffer`.
     */
    void build ();

  private:
    /**
     * @brief The integer pointing to this @a `FrameBuffer` on the graphics card.
     */
    Uint32 m_handle = 0;

    /**
     * @brief A collection of integers pointing to the @a `FrameBuffer`'s color attachment textures
     *        on the graphics card.
     */
    Collection<Uint32> m_colorHandles;

    /**
     * @brief The integer pointing to the @a `FrameBuffer`'s depth attachment texture, if any, on
     *        the graphics card.
     */
    Uint32 m_depthHandle = 0;

    /**
     * @brief The @a `FrameBuffer`'s specification.
     */
    FrameBufferSpecification m_spec;

    /**
     * @brief A collection of specifications describing the @a `FrameBuffer`'s color attachment
     *        textures.
     */
    Collection<FrameBufferTextureSpecification> m_colorAttachmentSpecs;

    /**
     * @brief The specification describing the @a `FrameBuffer`'s depth attachment texture, if one
     *        is attached.
     */
    FrameBufferTextureSpecification m_depthAttachmentSpec;

  };

  using RenderTarget = FrameBuffer;

}
