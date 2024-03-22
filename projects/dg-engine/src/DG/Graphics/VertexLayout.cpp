/** @file DG/Graphics/VertexLayout.cpp */

#include <DG/Graphics/VertexLayout.hpp>

namespace dg
{

  /** Vertex Attribute Struct *********************************************************************/

  VertexAttribute::VertexAttribute (
    const String& name,
    const VertexAttributeType type,
    Boolean normalized
  ) :
    name { name },
    type { type },
    normalized { normalized },
    offset { 0 }
  {

  }

  Size VertexAttribute::getSize () const
  {
    switch (type) {
      case VertexAttributeType::Float:     return 4;   
      case VertexAttributeType::Double:    return 8;    
      case VertexAttributeType::Int:       return 4; 
      case VertexAttributeType::Uint:      return 4;  
      case VertexAttributeType::Bool:      return 1;  
      case VertexAttributeType::Float2:    return 4 * 2;    
      case VertexAttributeType::Double2:   return 8 * 2;     
      case VertexAttributeType::Int2:      return 4 * 2;  
      case VertexAttributeType::Uint2:     return 4 * 2;   
      case VertexAttributeType::Bool2:     return 1 * 2;   
      case VertexAttributeType::Float3:    return 4 * 3;    
      case VertexAttributeType::Double3:   return 8 * 3;     
      case VertexAttributeType::Int3:      return 4 * 3;  
      case VertexAttributeType::Uint3:     return 4 * 3;   
      case VertexAttributeType::Bool3:     return 1 * 3;   
      case VertexAttributeType::Float4:    return 4 * 4;    
      case VertexAttributeType::Double4:   return 8 * 4;     
      case VertexAttributeType::Int4:      return 4 * 4;  
      case VertexAttributeType::Uint4:     return 4 * 4;   
      case VertexAttributeType::Bool4:     return 1 * 4;   
      case VertexAttributeType::Float3x3:  return 4 * 3 * 3;      
      case VertexAttributeType::Double3x3: return 8 * 3 * 3;      
      case VertexAttributeType::Float4x4:  return 4 * 4 * 4;      
      case VertexAttributeType::Double4x4: return 8 * 4 * 4;       
      default:                             return 0;
    }
  }

  Size VertexAttribute::getElementCount () const
  {
    switch (type) {
      case VertexAttributeType::Float:     return 1;            
      case VertexAttributeType::Double:    return 1;             
      case VertexAttributeType::Int:       return 1;          
      case VertexAttributeType::Uint:      return 1;           
      case VertexAttributeType::Bool:      return 1;           
      case VertexAttributeType::Float2:    return 2;             
      case VertexAttributeType::Double2:   return 2;              
      case VertexAttributeType::Int2:      return 2;           
      case VertexAttributeType::Uint2:     return 2;            
      case VertexAttributeType::Bool2:     return 2;            
      case VertexAttributeType::Float3:    return 3;             
      case VertexAttributeType::Double3:   return 3;              
      case VertexAttributeType::Int3:      return 3;           
      case VertexAttributeType::Uint3:     return 3;            
      case VertexAttributeType::Bool3:     return 3;            
      case VertexAttributeType::Float4:    return 4;             
      case VertexAttributeType::Double4:   return 4;              
      case VertexAttributeType::Int4:      return 4;           
      case VertexAttributeType::Uint4:     return 4;            
      case VertexAttributeType::Bool4:     return 4;            
      case VertexAttributeType::Float3x3:  return 3 * 3;               
      case VertexAttributeType::Double3x3: return 3 * 3;               
      case VertexAttributeType::Float4x4:  return 4 * 4;               
      case VertexAttributeType::Double4x4: return 4 * 4;           
      default: return 0;     
    }
  }

  /** Vertex Layout Class *************************************************************************/

  VertexLayout::VertexLayout (const InitList<VertexAttribute>& attributes) :
    m_attributes { attributes },
    m_stride { 0 }
  {
    updateLayout();
  }

  const Collection<VertexAttribute>& VertexLayout::getAttributes () const
  {
    return m_attributes;
  }

  Size VertexLayout::getStride () const
  {
    return m_stride;
  }

  void VertexLayout::updateLayout ()
  {
    // Reset the layout's stride, then keep track of the offset of the current vertex attribute.
    m_stride = 0;
    Size offset = 0;

    // Iterate over each vertex attribute, updating their byte offsets.
    // Update the vertex layout's stride based on the newly-calculated offsets.
    for (auto& attribute : m_attributes) {
      // Retrieve the attribute's size, first.
      auto size = attribute.getSize();

      attribute.offset = offset;
      offset += size;
      m_stride += size;
    }
  }

}
