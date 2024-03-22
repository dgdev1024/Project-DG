/** @file DG/Graphics/VertexLayout.hpp */

#pragma once

#include <DG_Pch.hpp>

namespace dg
{

  /**
   * @brief The @a `VertexAttribType` enum enumerates the different types of data which can make
   *        up a vertex on the graphics card.
   */
  enum class VertexAttributeType
  {
    None = 0,
    Float, 
    Double, 
    Int, 
    Uint, 
    Bool,
    Float2, 
    Double2, 
    Int2, 
    Uint2, 
    Bool2,
    Float3, 
    Double3, 
    Int3, 
    Uint3, 
    Bool3,
    Float4, 
    Double4, 
    Int4, 
    Uint4, 
    Bool4,
    Float3x3, 
    Double3x3, 
    Float4x4, 
    Double4x4
  };

  /**
   * @brief The @a `VertexAttribute` struct defines a piece of data contained in a vertex on the
   *        graphics card.
   */
  struct VertexAttribute
  {
    VertexAttribute () = default;

    /**
     * @brief Constructs a new @a `VertexAttribute` with the given name and of the given type.
     * 
     * @param name        A string identifying the vertex attribute.
     * @param type        The type of the vertex attribute's value(s).
     * @param normalized  Should the vertex attribute's value(s) be normalized into a unit range?
     */
    VertexAttribute (
      const String& name,
      const VertexAttributeType type,
      Bool normalized = false
    );

    /**
     * @brief Retrieves the size of this @a `VertexAttribute` based on its stored type.
     * 
     * @return  The size of this @a `VertexAttribute`, in bytes. 
     */
    Size getSize () const;

    /**
     * @brief Retrieves the number of primitive elements in this @a `VertexAttribute`, based on its
     *        stored type.
     * 
     * @return  The number of primitive elements in this @a `VertexAttribute`. 
     */
    Size getElementCount () const;

    /**
     * @brief A string identifying the vertex attribute.
     */
    String name = "";

    /**
     * @brief The type of this vertex attribute's value(s).
     * @sa @a `dg::VertexAttributeType`
     */
    VertexAttributeType type = VertexAttributeType::None;

    /**
     * @brief Should this vertex attribute's value(s) be normalized into a unit range?
     */
    Bool normalized;

    /**
     * @brief The position, in bytes, of the vertex attribute relative to the starting point of the
     *        vertex on the graphics card.
     */
    Size offset;

  };

  /**
   * @brief The @a `VertexLayout` class describes how the data in a vertex on the graphics card is
   *        laid out.
   */
  class VertexLayout
  {
  public:
    VertexLayout () = default;

    /**
     * @brief Constructs a new @a `VertexLayout` with the given vertex attributes.
     * 
     * @param attributes  An initializer list containing the new @a `VertexLayout`'s attributes. 
     */
    VertexLayout (const InitList<VertexAttribute>& attributes);

    /**
     * @brief Retrieves the collection of @a `VertexAttribute`s which make up this @a `VertexLayout`.
     * 
     * @return  A handle to the @a `VertexAttribute` collection. 
     */
    const Collection<VertexAttribute>& getAttributes () const;

    /**
     * @brief Retrieves the distance between the starting point of one vertex with this
     *        @a `VertexLayout`, and the starting point of the next.
     * 
     * @return  This @a `VertexLayout`'s stride, in bytes. 
     */
    Size getStride () const;

  public: // Ranged-Based For Support
    inline Collection<VertexAttribute>::iterator begin () { return m_attributes.begin(); }
    inline Collection<VertexAttribute>::iterator end () { return m_attributes.end(); }
    inline Collection<VertexAttribute>::const_iterator begin () const { return m_attributes.begin(); }
    inline Collection<VertexAttribute>::const_iterator end () const { return m_attributes.end(); }

  private:
    /**
     * @brief Updates the stride of this @a `VertexLayout`, and the offsets of its attributes.
     */
    void updateLayout ();

  private:
    /**
     * @brief The collection of vertex attributes which make up this @a `VertexLayout`.
     */
    Collection<VertexAttribute> m_attributes;

    /**
     * @brief The distance, in bytes, between the starting point of one vertex with this
     *        @a `VertexLayout`, and the starting point of the next.
     */
    Size m_stride = 0;

  };

}
