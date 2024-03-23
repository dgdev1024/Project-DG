/** @file DG/Graphics/GraphicsBuffers.cpp */

#include <DG/Graphics/GraphicsBuffers.hpp>

namespace dg
{

  /** Vertex Buffer *******************************************************************************/

  VertexBuffer::VertexBuffer (Bool dynamic) :
    m_dynamic { dynamic }
  {
    glGenBuffers(1, &m_handle);
  }

  VertexBuffer::~VertexBuffer ()
  {
    glDeleteBuffers(1, &m_handle);
  }

  Ref<VertexBuffer> VertexBuffer::make (Bool dynamic)
  {
    return makeRef<VertexBuffer>(dynamic);
  }

  void VertexBuffer::bind () const
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
  }

  void VertexBuffer::unbind ()
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  Boolean VertexBuffer::isDynamic () const
  {
    return m_dynamic;
  }

  const VertexLayout& VertexBuffer::getLayout () const
  {
    return m_layout;
  }

  void VertexBuffer::setLayout (const VertexLayout& layout)
  {
    m_layout = layout;
  }

  void VertexBuffer::reserveRaw (const Size size)
  {
    // Data reservation can only be done with dynamic draw vertex buffers.
    if (m_dynamic == false) {
      throw std::invalid_argument { "Attempted 'reserveRaw' on non-dynamic vertex buffer!" };
    }

    // Data reservation can only be done once.
    if (m_bufferSize != 0) {
      throw std::invalid_argument { "Attempted 'reserveRaw' on already-allocated vertex buffer!" };
    }

    // Bind the buffer, then allocate the vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

    // Set the buffer size.
    m_bufferSize = size;   
  }

  void VertexBuffer::allocateRaw (const void* data, const Size size)
  {
    // Data allocation can only be done on static draw vertex buffers.
    if (m_dynamic == true) {
      throw std::invalid_argument { "Attempted 'allocateRaw' on dynamic vertex buffer!" };
    }

    // Data allocation can only be done on non-allocated vertex buffers.
    if (m_bufferSize != 0) {
      throw std::invalid_argument { "Attempted 'allocateRaw' on dynamic vertex buffer!" };
    }

    // Ensure that the raw data is not null!
    if (data == nullptr || size == 0) {
      throw std::invalid_argument { "Attempted 'allocateRaw' of null pointer to vertex buffer!" };
    }

    // Bind the buffer, then allocate and upload the vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    // Set the buffer size.
    m_bufferSize = size;    
  }

  void VertexBuffer::uploadRaw (const void* data, const Size size)
  {
    // Data upload can only be done on dynamic draw vertex buffers.
    if (m_dynamic == false) {
      throw std::invalid_argument { "Attempted 'uploadRaw' on non-dynamic vertex buffer!" };
    }

    // Data upload can only be done on allocated vertex buffers.
    if (m_bufferSize == 0) {
      throw std::invalid_argument { "Attempted 'uploadRaw' on non-allocated vertex buffer!" };
    }

    // Data upload cannot be done if the allocated vertex buffer is too small.
    if (size >= m_bufferSize) {
      DG_ENGINE_CRIT("Attempted 'uploadRaw' of {} bytes to vertex buffer with only {} byte(s)!",
        size, m_bufferSize);
      throw std::invalid_argument { "Attempted 'uploadRaw' of overflowing data!" };
    }

    // Bind the buffer, then upload the vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);    
  }

  /** Index Buffer ********************************************************************************/

  IndexBuffer::IndexBuffer (Bool dynamic) :
    m_dynamic { dynamic }
  {
    glGenBuffers(1, &m_handle);
  }

  IndexBuffer::~IndexBuffer ()
  {
    glDeleteBuffers(1, &m_handle);
  }

  Ref<IndexBuffer> IndexBuffer::make (Bool dynamic)
  {
    return makeRef<IndexBuffer>(dynamic);
  }

  void IndexBuffer::bind () const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
  }

  void IndexBuffer::unbind ()
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  Boolean IndexBuffer::isDynamic () const
  {
    return m_dynamic;
  }

  Count IndexBuffer::getIndexCount () const
  {
    return m_indexCount;
  }

  IndexType IndexBuffer::getIndexType () const
  {
    return m_indexType;
  }

  GLenum IndexBuffer::resolveIndexType () const
  {
    switch (m_indexType) {
      case IndexType::UnsignedByte:   return GL_UNSIGNED_BYTE;
      case IndexType::UnsignedShort:  return GL_UNSIGNED_SHORT;
      case IndexType::UnsignedInt:    return GL_UNSIGNED_INT;
      default:                        return GL_UNSIGNED_INT;
    }
  }

  void IndexBuffer::reserveRaw (const Size size)
  {
    // Data reservation can only be done with dynamic draw index buffers.
    if (m_dynamic == false) {
      throw std::invalid_argument { "Attempted 'reserveRaw' on non-dynamic index buffer!" };
    }

    // Data reservation can only be done once.
    if (m_bufferSize != 0) {
      throw std::invalid_argument { "Attempted 'reserveRaw' on already-allocated index buffer!" };
    }

    // Ensure that the given size is a multiple of the unsigned integer size.
    if (size % m_integerSize != 0) {
      DG_ENGINE_CRIT("Attempted 'reserveRaw' of non-uniform byte size {} (must be a multiple of {})!",
        size, m_integerSize);
      throw std::invalid_argument { "Attempted 'reserveRaw' of non-uniform byte size to index buffer!" };
    }

    // Bind the buffer, then allocate the index data.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

    // Set the buffer size.
    m_bufferSize = size;   
  }

  void IndexBuffer::allocateRaw (const void* data, const Size size)
  {
    // Data allocation can only be done on static draw index buffers.
    if (m_dynamic == true) {
      throw std::invalid_argument { "Attempted 'allocateRaw' on dynamic index buffer!" };
    }

    // Data allocation can only be done on non-allocated index buffers.
    if (m_bufferSize != 0) {
      throw std::invalid_argument { "Attempted 'allocateRaw' on dynamic index buffer!" };
    }

    // Ensure that the raw data is not null!
    if (data == nullptr || size == 0) {
      throw std::invalid_argument { "Attempted 'allocateRaw' of null pointer to index buffer!" };
    }

    // Ensure that the given size is a multiple of the unsigned integer size.
    if (size % m_integerSize != 0) {
      DG_ENGINE_CRIT("Attempted 'allocateRaw' of non-uniform byte size {} (must be a multiple of {})!",
        size, m_integerSize);
      throw std::invalid_argument { "Attempted 'allocateRaw' of non-uniform byte size to index buffer!" };
    }

    // Bind the buffer, then allocate and upload the index data.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    // Set the buffer size.
    m_bufferSize = size;    
  }

  void IndexBuffer::uploadRaw (const void* data, const Size size)
  {
    // Data upload can only be done on dynamic draw index buffers.
    if (m_dynamic == false) {
      throw std::invalid_argument { "Attempted 'uploadRaw' on non-dynamic index buffer!" };
    }

    // Data upload can only be done on allocated index buffers.
    if (m_bufferSize == 0) {
      throw std::invalid_argument { "Attempted 'uploadRaw' on non-allocated index buffer!" };
    }

    // Data upload cannot be done if the allocated index buffer is too small.
    if (size >= m_bufferSize) {
      DG_ENGINE_CRIT("Attempted 'uploadRaw' of {} bytes to index buffer with only {} byte(s)!",
        size, m_bufferSize);
      throw std::invalid_argument { "Attempted 'uploadRaw' of overflowing data!" };
    }

    // Ensure that the given size is a multiple of the unsigned integer size.
    if (size % m_integerSize != 0) {
      DG_ENGINE_CRIT("Attempted 'uploadRaw' of non-uniform byte size {} (must be a multiple of {})!",
        size, m_integerSize);
      throw std::invalid_argument { "Attempted 'uploadRaw' of non-uniform byte size to index buffer!" };
    }

    // Bind the buffer, then upload the index data.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);    
  }

}
