/** @file DG/Graphics/VertexArray.cpp */

#include <DG/Graphics/VertexArray.hpp>

namespace dg
{

  namespace Private
  {

    /**
     * @brief Helper static function used for resolving the given @a `VertexAttributeType` to
     *        a valid OpenGL data type enumeration.
     * 
     * @param type  The @a `VertexAttributeType` to resolve.
     *  
     * @return  The resolved OpenGL data type enumeration if it could be resolved; @a `0`
     *          otherwise. 
     */
    static GLenum resolveGLType (VertexAttributeType type)
    {
      switch (type)
      {
        case VertexAttributeType::Float:     return GL_FLOAT;      
        case VertexAttributeType::Double:    return GL_DOUBLE;       
        case VertexAttributeType::Int:       return GL_INT;    
        case VertexAttributeType::Uint:      return GL_UNSIGNED_INT;     
        case VertexAttributeType::Bool:      return GL_BOOL;    
        case VertexAttributeType::Float2:    return GL_FLOAT;       
        case VertexAttributeType::Double2:   return GL_DOUBLE;        
        case VertexAttributeType::Int2:      return GL_INT;     
        case VertexAttributeType::Uint2:     return GL_UNSIGNED_INT;      
        case VertexAttributeType::Bool2:     return GL_BOOL;     
        case VertexAttributeType::Float3:    return GL_FLOAT;       
        case VertexAttributeType::Double3:   return GL_DOUBLE;        
        case VertexAttributeType::Int3:      return GL_INT;     
        case VertexAttributeType::Uint3:     return GL_UNSIGNED_INT;      
        case VertexAttributeType::Bool3:     return GL_BOOL;     
        case VertexAttributeType::Float4:    return GL_FLOAT;       
        case VertexAttributeType::Double4:   return GL_DOUBLE;        
        case VertexAttributeType::Int4:      return GL_INT;     
        case VertexAttributeType::Uint4:     return GL_UNSIGNED_INT;      
        case VertexAttributeType::Bool4:     return GL_BOOL;     
        case VertexAttributeType::Float3x3:  return GL_FLOAT;         
        case VertexAttributeType::Double3x3: return GL_DOUBLE;          
        case VertexAttributeType::Float4x4:  return GL_FLOAT;         
        case VertexAttributeType::Double4x4: return GL_DOUBLE; 
        default: return 0;        
      }
    }

  }

  VertexArray::VertexArray ()
  {
    glGenVertexArrays(1, &m_handle);
  }

  VertexArray::~VertexArray ()
  {
    glDeleteVertexArrays(1, &m_handle);
  }

  Ref<VertexArray> VertexArray::make ()
  {
    return makeRef<VertexArray>();
  }

  void VertexArray::bind () const
  {
    glBindVertexArray(m_handle);
  }

  void VertexArray::unbind ()
  {
    glBindVertexArray(0);
  }

  void VertexArray::addVertexBuffer (const Ref<VertexBuffer>& buffer)
  {
    // Ensure that the vertex buffer given is not null.
    if (buffer == nullptr) {
      throw std::invalid_argument { "Attempted 'addVertexBuffer' with null vertex buffer pointer!" };
    }

    // Bind the vertex array, then the vertex buffer.
    glBindVertexArray(m_handle);
    buffer->bind();

    // Get the vertex buffer's layout. Ensure that it is not empty.
    const auto& layout = buffer->getLayout();
    if (layout.getAttributes().empty()) {
      throw std::invalid_argument { "Attempted 'addVertexBuffer' with vertex buffer with no layout!" };
    }
    
    // Keep track of the index of the next vertex attribute.
    Index index = 0;

    // Iterate over the vertex buffer's layout.
    for (const auto& attribute : layout)
    {
      // Define the GL vertex attribute.
      glVertexAttribPointer(
        index,
        attribute.getElementCount(),
        Private::resolveGLType(attribute.type),
        attribute.normalized ? GL_TRUE : GL_FALSE,
        layout.getStride(),
        (const void*) attribute.offset
      );

      // Inform the bound vertex array of the attribute and how it's laid out.
      glEnableVertexAttribArray(index++);
    }

    // Add the vertex buffer.
    m_vertexBuffers.push_back(buffer);
  }

  void VertexArray::setIndexBuffer (const Ref<IndexBuffer>& buffer)
  {
    // Ensure that the index buffer given is not null.
    if (buffer == nullptr) {
      throw std::invalid_argument { "Attempted 'setIndexBuffer' with null index buffer pointer!" };
    }

    // Bind the vertex array, then the index buffer.
    glBindVertexArray(m_handle);
    buffer->bind();

    // Inform this vertex array of the buffer.
    m_indexBuffer = buffer;
  }

  const Ref<IndexBuffer>& VertexArray::getIndexBuffer () const
  {
    return m_indexBuffer;
  }


}
