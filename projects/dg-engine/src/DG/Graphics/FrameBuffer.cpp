/** @file DG/Graphics/FrameBuffer.cpp */

#include <DG/Graphics/RenderInterface.hpp>
#include <DG/Graphics/FrameBuffer.hpp>

namespace dg
{

  /** Static Functions ****************************************************************************/

  namespace Private
  {

    static Uint32 resolveTextureTarget (Bool isMultisampled)
    {
      return isMultisampled == true ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }   

    static void resolveTextureFormat (const FrameBufferTextureFormat& format, 
      GLenum& internalFormat, GLenum& pixelFormat, GLenum& dataType)
    {

      switch (format)
      {
        case FrameBufferTextureFormat::ColorR32:
          internalFormat = GL_R32I;
          pixelFormat = GL_RED_INTEGER;
          dataType = GL_INT;
          break;

        case FrameBufferTextureFormat::ColorRGBA8:
          internalFormat = GL_RGBA8;
          pixelFormat = GL_RGBA;
          dataType = GL_UNSIGNED_BYTE;
          break;

        case FrameBufferTextureFormat::Depth24Stencil8:
          internalFormat = GL_DEPTH24_STENCIL8;
          pixelFormat = GL_DEPTH_STENCIL;
          dataType = GL_UNSIGNED_INT_24_8;
          break;

        default:
          internalFormat = 0;
          pixelFormat = 0;
          dataType = 0;
          break;
      }

    }

    static GLenum resolveAttachPoint (const FrameBufferTextureFormat& format)
    {

      switch (format)
      {
        case FrameBufferTextureFormat::Depth24Stencil8:
          return GL_DEPTH_STENCIL_ATTACHMENT;

        default:
          return 0;
      }

    }

    static void generateTextures (Uint32* handles, const Count count)
    {
      glGenTextures(count, handles);
    }

    static void bindTexture (Bool isMultisampled, Uint32 handle)
    {
      glBindTexture(resolveTextureTarget(isMultisampled), handle);
    }

    static void attachColorTexture (Uint32 handle, const FrameBufferSpecification& framebufferSpec,
      const FrameBufferTextureSpecification& textureSpec, const Index index)
    {

      // Deduce the proper GL type enums from the texture format.
      GLenum internalFormat = 0, pixelFormat = 0, dataType = 0;
      resolveTextureFormat(textureSpec.textureFormat, internalFormat, pixelFormat, dataType);

      // Determine if the frame buffer is multisampled.
      Bool isMultisampled = framebufferSpec.sampleCount > 1;

      // Create the texture according to the number of samples.
      if (isMultisampled == true) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, framebufferSpec.sampleCount, 
          internalFormat, framebufferSpec.size.x, framebufferSpec.size.y, GL_FALSE);
      } else {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, framebufferSpec.size.x, 
          framebufferSpec.size.y, 0, pixelFormat, dataType, nullptr);

        // Set the texture's filtering and wrapping parameters.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }

      // Attach the color texture.
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, 
        resolveTextureTarget(isMultisampled), handle, 0);

    }

    static void attachDepthTexture (Uint32 handle, const FrameBufferSpecification& framebufferSpec,
      const FrameBufferTextureSpecification& textureSpec)
    {

      // Deduce the proper GL type enums from the texture format.
      GLenum internalFormat = 0, pixelFormat = 0, dataType = 0;
      resolveTextureFormat(textureSpec.textureFormat, internalFormat, pixelFormat, dataType);

      // Determine if the frame buffer is multisampled.
      Bool isMultisampled = framebufferSpec.sampleCount > 1;

      // Create the texture according to the number of samples.
      if (isMultisampled == true) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, framebufferSpec.sampleCount, 
          internalFormat, framebufferSpec.size.x, framebufferSpec.size.y, GL_FALSE);
      } else {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, framebufferSpec.size.x, 
          framebufferSpec.size.y, 0, pixelFormat, dataType, nullptr);

        // Set the texture's filtering and wrapping parameters.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }

      // Attach the depth texture.
      glFramebufferTexture2D(GL_FRAMEBUFFER, resolveAttachPoint(textureSpec.textureFormat), 
        resolveTextureTarget(isMultisampled), handle, 0);

    }

  }

  /** Frame Buffer Texture Specification **********************************************************/

  Bool FrameBufferTextureSpecification::isDepthTextureFormat () const
  {
    switch (textureFormat)
    {
      case FrameBufferTextureFormat::Depth24Stencil8:   
        return true;
      default: 
        return false;
    }
  }

  /** Frame Buffer Class **************************************************************************/

  FrameBuffer::FrameBuffer (const FrameBufferSpecification& spec)
  {
    // Sort out the framebuffer's attachment texture formats.
    for (const auto& attachment : m_spec.attachmentSpec.attachments) {
      if (attachment.isDepthTextureFormat() == true) {
        m_depthAttachmentSpec = attachment;
      } else {
        m_colorAttachmentSpecs.push_back(attachment);
      }
    }

    build();
  }

  FrameBuffer::~FrameBuffer ()
  {
    glDeleteFramebuffers(1, &m_handle);
    glDeleteTextures(m_colorHandles.size(), m_colorHandles.data());
    glDeleteTextures(1, &m_depthHandle);

    m_colorHandles.clear();
    m_depthHandle = 0;
    m_handle = 0;
  }

  Ref<FrameBuffer> FrameBuffer::make (const FrameBufferSpecification& spec)
  {
    return makeRef<FrameBuffer>(spec);
  }

  void FrameBuffer::bind (const FrameBufferTarget target) const
  {
    if (target == FrameBufferTarget::Drawing || target == FrameBufferTarget::Both) {
      glBindFramebuffer(target == FrameBufferTarget::Both ? GL_FRAMEBUFFER : GL_DRAW_FRAMEBUFFER,
        m_handle);
      
      RenderInterface::setViewport(m_spec.size);
      RenderInterface::clear();
    } else {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, m_handle);
    }
  }

  void FrameBuffer::unbind (const FrameBufferTarget target)
  {
    switch (target) {
      case FrameBufferTarget::Reading: glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); break;
      case FrameBufferTarget::Drawing: glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); break;
      case FrameBufferTarget::Both:    glBindFramebuffer(GL_FRAMEBUFFER, 0); break;
    }
  }

  void FrameBuffer::clearColorAttachment (const Index index, Int32 clearData)
  {
    if (index >= m_colorHandles.size()) {
      DG_ENGINE_CRIT("GL Framebuffer color attachment index {} is out of range!", index);
      throw std::out_of_range { "Attempt to clear framebuffer at attachment index out of range!" };
    }

    // Get the texture spec and resolve it.
    const auto& spec = m_colorAttachmentSpecs.at(index);
    GLenum internalFormat = 0, pixelFormat = 0, dataType = 0;
    Private::resolveTextureFormat(spec.textureFormat, internalFormat, pixelFormat, dataType);

    glClearTexImage(m_colorHandles[index], 0, pixelFormat, dataType, &clearData);
  }

  Int32 FrameBuffer::readPixel (const Index index, Int32 x, Int32 y)
  {
    if (index >= m_colorHandles.size()) {
      DG_ENGINE_CRIT("GL Framebuffer color attachment index {} is out of range!", index);
      throw std::out_of_range { 
        "Attempt to read pixels from framebuffer at attachment index out of range!" 
      };
    }

    if (x < 0 || y < 0 || x >= m_spec.size.x || y >= m_spec.size.y)
    {
      return -1;
    }

    // We will need the pixel format and data type.
    GLenum pixelFormat = 0, pixelDataType = 0, unusedInternalFormat = 0;
    Private::resolveTextureFormat(m_colorAttachmentSpecs[index].textureFormat, unusedInternalFormat,
      pixelFormat, pixelDataType);

    // Set this framebuffer as the active `GL_READ_FRAMEBUFFER`.
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_handle);

    // Read the requested color attachment.
    glReadBuffer(GL_COLOR_ATTACHMENT0 + index);

    // Read in the pixel data.
    Int32 pixelData = 0;
    glReadPixels(x, y, 1, 1, pixelFormat, pixelDataType, &pixelData);
    
    // Un-set the read framebuffer.
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    return pixelData;
  }

  Int32 FrameBuffer::readPixel (const Index index, Float32 x, Float32 y)
  {
    return readPixel(index, static_cast<Int32>(x), static_cast<Int32>(y));
  }

  Uint32 FrameBuffer::getColorHandle (const Index index) const
  {
    if (index >= m_colorHandles.size()) {
      DG_ENGINE_CRIT("GL Framebuffer color attachment index {} is out of range!", index);
      throw std::out_of_range { "GL Framebuffer color attachment index out of range!" };
    }

    return m_colorHandles.at(index);
  }

  Uint32 FrameBuffer::getDepthHandle () const
  {
    return m_depthHandle;
  }

  void* FrameBuffer::getTextureHandle (const Index index) const
  {
    if (index >= m_colorHandles.size()) {
      DG_ENGINE_CRIT("GL Framebuffer color attachment index {} is out of range!", index);
      throw std::out_of_range { "GL Framebuffer color attachment index out of range!" };
    }

    return (void*) (intptr_t) m_colorHandles.at(index);
  }

  void FrameBuffer::build ()
  {

    // If there is a frame buffer built already, then delete that buffer and its attachments, first.
    if (m_handle != 0) {
      glDeleteFramebuffers(1, &m_handle);
      glDeleteTextures(m_colorHandles.size(), m_colorHandles.data());
      glDeleteTextures(1, &m_depthHandle);

      m_colorHandles.clear();
      m_depthHandle = 0;
      m_handle = 0;
    }

    // A framebuffer is "multisampled" if its sample count is greater than one.
    Bool isMultisampled = m_spec.sampleCount > 1;

    // Generate, then bind, the new frame buffer.
    glGenFramebuffers(1, &m_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

    // Check to see if there are color attachments to attach to this framebuffer.
    if (m_colorAttachmentSpecs.empty() == false) {

      // Resize the color attachment handles collection as appropriate, then generate the color 
      // attachment textures.
      m_colorHandles.resize(m_colorAttachmentSpecs.size());
      Private::generateTextures(m_colorHandles.data(), m_colorHandles.size());

      // Now iterate over the color attachment specifications.
      for (std::size_t i = 0; i < m_colorAttachmentSpecs.size(); ++i) {

        // Bind, then attach, the color attachment texture.
        Private::bindTexture(isMultisampled, m_colorHandles[i]);
        Private::attachColorTexture(m_colorHandles[i], m_spec, m_colorAttachmentSpecs[i], i);

      }

    }

    // Now check for the presence of a depth attachment texture.
    if (m_depthAttachmentSpec.textureFormat != FrameBufferTextureFormat::None) {
      
      // Create, bind, then attach the depth texture.
      Private::generateTextures(&m_depthHandle, 1);
      Private::bindTexture(isMultisampled, m_depthHandle);
      Private::attachDepthTexture(m_depthHandle, m_spec, m_depthAttachmentSpec);

    }

    // If at least one color attachment texture was provided...
    if (m_colorHandles.size() > 1) {

      // Ensure that no more than the maximum number of color attachments are provided.
      if (m_colorHandles.size() > FRAMEBUFFER_COLOR_ATTACHMENT_COUNT) {
        DG_ENGINE_CRIT("Too many color attachments ({}) on this framebuffer!", m_colorHandles.size());
        throw std::runtime_error { "Exceeded framebuffer color attachment limit!" };
      }

      // Create an array of the symbolic constants pointing to the color attachment textures to be 
      // drawn.
      Uint32 colorAttachmentBuffers[FRAMEBUFFER_COLOR_ATTACHMENT_COUNT] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3,
      };

      // Map the color attachments in our framebuffer to the above-defined color attachment buffers 
      // array.
      glDrawBuffers(m_colorHandles.size(), colorAttachmentBuffers);
    } else if (m_colorHandles.empty()) {
      // Only a depth buffer is being presented.
      glDrawBuffer(GL_NONE);
    }

    // Ensure that the framebuffer is complete and is ready to be used.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      throw std::runtime_error { "Unable to build a complete framebuffer!" };
    }

    // Building done, unbind the framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

  }

}
