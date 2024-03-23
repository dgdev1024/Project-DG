/** @file DG/Graphics/GraphicsBuffers.hpp */

#pragma once

#include <DG/Graphics/VertexLayout.hpp>

namespace dg
{

  /**
   * @brief The @a `IndexType` enum enumerates the unsigned integer types of indices which can
   *        be stored in an @a `IndexBufferBase` on the graphics card.
   */
  enum class IndexType
  {
    None,
    UnsignedByte,
    UnsignedShort,
    UnsignedInt
  };

  /**
   * @brief The @a `ToUnsigned` type-traits struct is used to deduce an unsigned integer type
   *        from the given, templated @a `IndexType`.
   * 
   * @tparam T  The templated @a `IndexType` enumeration to be deduced. 
   */
  template <IndexType T>
  struct ToUnsigned
  {
    using Type = void;
  };

  // Type traits specializations for each `IndexType` enumeration.
  template <> struct ToUnsigned<IndexType::UnsignedByte>  { using Type = Uint8;  };
  template <> struct ToUnsigned<IndexType::UnsignedShort> { using Type = Uint16; };
  template <> struct ToUnsigned<IndexType::UnsignedInt>   { using Type = Uint32; };

  /**
   * @brief The @a `VertexBuffer` class describes an interface for allocating and using vertex data
   *        on the graphics card.
   */
  class VertexBuffer
  {
  public:
    VertexBuffer (Bool dynamic);
    ~VertexBuffer ();

    /**
     * @brief Creates a new @a `VertexBuffer` on the graphics card.
     * 
     * @param dynamic Is this vertex buffer a @a `GL_DYNAMIC_DRAW` vertex buffer?
     *  
     * @return  A shared pointer to the newly-created @a `VertexBuffer`. 
     */
    static Ref<VertexBuffer> make (Bool dynamic);

    /**
     * @brief Sets this @a `VertexBuffer` as the active @a `GL_ARRAY_BUFFER`.
     */
    void bind () const;

    /**
     * @brief Un-sets the active @a `GL_ARRAY_BUFFER`.
     */
    static void unbind ();

    /**
     * @brief Indicates whether or not this @a `VertexBuffer` is a @a `GL_DYNAMIC_DRAW` vertex
     *        buffer.
     * 
     * @return  @a `true` if this @a `VertexBuffer` is a dynamic draw buffer;
     *          @a `false` otherwise. 
     */
    Bool isDynamic () const;

    /**
     * @brief Retrieves the @a `VertexLayout` of this @a `VertexBuffer`.
     * 
     * @return  The vertex buffer's attribute layout.
     */
    const VertexLayout& getLayout () const;

    /**
     * @brief Sets the @a `VertexLayout` of this @a `VertexBuffer`.
     * 
     * @param layout  The vertex buffer's new attribute layout.
     */
    void setLayout (const VertexLayout& layout);

    /**
     * @brief Reserves the given number of vertices of type @a `T` on this @a `VertexBuffer`
     *        on the graphics card.
     * 
     * @tparam T  The standard-layout struct type of the vertex data to be reserved.
     *  
     * @param count The number of vertices to be reserved.
     */
    template <typename T>
    inline void reserve (const Count count)
    {
      static_assert(std::is_standard_layout_v<T>,
        "[VertexBuffer::reserve] 'T' must be of a standard layout.");

      reserveRaw(count * sizeof(T));
    }

    /**
     * @brief Allocates the given collection of vertices of type @a `T` to this
     *        @a `VertexBuffer` on the graphics card.
     * 
     * @tparam T  The standard-layout struct type of the vertex data to be allocated.
     *  
     * @param vertices  The collection of vertices to be uploaded. 
     * @param count     The number of vertices to be uploaded.
     */
    template <typename T>
    inline void allocate (const Collection<T>& vertices, Count count = -1)
    {
      static_assert(std::is_standard_layout_v<T>,
        "[VertexBuffer::allocate] 'T' must be of a standard layout.");

      if (count > vertices.size()) { count = vertices.size(); }

      allocateRaw(vertices.data(), count * sizeof(T));
    }

    /**
     * @brief Uploads the given collection of vertices of type @a `T` to this
     *        @a `VertexBuffer` on the graphics card.
     * 
     * @tparam T  The standard-layout struct type of the vertex data to be allocated.
     *  
     * @param vertices  The collection of vertices to be uploaded. 
     * @param count     The number of vertices to be uploaded.
     */
    template <typename T>
    inline void upload (const Collection<T>& vertices, Count count = -1)
    {
      static_assert(std::is_standard_layout_v<T>,
        "[VertexBuffer::upload] 'T' must be of a standard layout.");

      if (count > vertices.size()) { count = vertices.size(); }

      uploadRaw(vertices.data(), count * sizeof(T));
    }

  private:
    /**
     * @brief Allocates the given number of bytes to this @a `VertexBuffer` on the graphics card.
     * 
     * @param size  The number of bytes to be allocated. 
     */
    void reserveRaw (const Size size);

    /**
     * @brief Allocates, then uploads, raw data of the given size to this @a `VertexBuffer` on the 
     *        graphics card.
     * 
     * @param data  Points to the raw vertex data to be uploaded. 
     * @param size  The size, in bytes, of the raw vertex data to be allocated and uploaded.
     */
    void allocateRaw (const void* data, const Size size);

    /**
     * @brief Uploads raw data of the given size to this @a `VertexBuffer` on the graphics card.
     * 
     * @param data  Points to the raw vertex data to be uploaded. 
     * @param size  The size, in bytes, of the raw vertex data to be uploaded.
     */
    void uploadRaw (const void* data, const Size size);

  private:
    /**
     * @brief An integer handle pointing to this @a `VertexBuffer`'s data on the graphics card.
     */
    Uint32 m_handle = 0;

    /**
     * @brief Indicates whether or not this is a @a `GL_DYNAMIC_DRAW` vertex buffer.
     */
    Boolean m_dynamic = false;

    /**
     * @brief The size, in bytes, of the vertex data allocated to this @a `VertexBuffer` on the
     *        graphics card.
     */
    Size m_bufferSize = 0;

    /**
     * @brief Indicates how the vertex data in this @a `VertexBuffer` on the graphics card is laid 
     *        out.
     */
    VertexLayout m_layout;

  };

  /**
   * @brief The @a `IndexBuffer` class, also called an @a `ElementBuffer`, describes an interface
   *        for allocating, storing and using unsigned integer indices on the graphics card, with
   *        each index pointing to a vertex contained in a @a `VertexBuffer`.
   */
  class IndexBuffer
  {
  public:
    IndexBuffer (Bool dynamic);
    ~IndexBuffer ();

    /**
     * @brief Creates a new @a `IndexBuffer` on the graphics card.
     * 
     * @param dynamic Is this vertex buffer a @a `GL_DYNAMIC_DRAW` vertex buffer?
     *  
     * @return  A shared pointer to the newly-created @a `IndexBuffer`. 
     */
    static Ref<IndexBuffer> make (Bool dynamic);

    /**
     * @brief Sets this @a `IndexBuffer` as the active @a `GL_ELEMENT_ARRAY_BUFFER`.
     */
    void bind () const;

    /**
     * @brief Un-sets the active @a `GL_ELEMENT_ARRAY_BUFFER`.
     */
    static void unbind ();    

    /**
     * @brief Retrieves whether or not this @a `IndexBuffer` is a "dynamic draw" index
     *        buffer.
     * @return 
     */
    Boolean isDynamic () const;

    /**
     * @brief Retrieves the number of indices allocated to this @a `IndexBuffer` on the
     *        graphics card.
     * 
     * @return  The number of indices allocated. 
     */
    Count getIndexCount () const;

    /**
     * @brief Retrieves the type of the unsigned integer indices in this @a `IndexBuffer`.
     * 
     * @return  This @a `IndexBuffer`'s @a `IndexType`.
     */
    IndexType getIndexType () const;

    /**
     * @brief   Resolves the type of the unsigned integer indices in this @a `IndexBuffer` to the
     *          proper GL enumeration.
     * 
     * @return  The resolved GL enumeration. 
     */
    GLenum resolveIndexType () const;

    /**
     * @brief Reserves the given number of indices of type @a `T` on this @a `IndexBuffer`
     *        on the graphics card.
     * 
     * @tparam T  The @a `IndexType` enumerated type of the index data to be reserved.
     *  
     * @param count The number of indices to be reserved.
     */
    template <IndexType T = IndexType::UnsignedInt>
    inline void reserve (const Count count)
    {
      m_indexCount = count;
      m_indexType  = T;
      m_integerSize  = sizeof(typename ToUnsigned<T>::Type);
      reserveRaw(count * sizeof(typename ToUnsigned<T>::Type));
    }

    /**
     * @brief Allocates the given collection of indices of type @a `T` to this
     *        @a `IndexBuffer` on the graphics card.
     * 
     * @tparam T  The @a `IndexType` enumerated type of the index data to be allocated.
     *  
     * @param indices  The collection of indices to be uploaded. 
     */
    template <IndexType T = IndexType::UnsignedInt>
    inline void allocate (const Collection<typename ToUnsigned<T>::Type>& indices)
    {
      m_indexCount = indices.size();
      m_indexType  = T;
      m_integerSize  = sizeof(typename ToUnsigned<T>::Type);
      allocateRaw(indices.data(), indices.size() * sizeof(typename ToUnsigned<T>::Type));
    }

    /**
     * @brief Uploads the given collection of indices of type @a `T` to this
     *        @a `IndexBuffer` on the graphics card.
     * 
     * @tparam T  The @a `IndexType` enumerated type of the index data to be allocated.
     *  
     * @param indices  The collection of indices to be uploaded. 
     */
    template <IndexType T = IndexType::UnsignedInt>
    inline void upload (const Collection<typename ToUnsigned<T>::Type>& indices)
    {
      uploadRaw(indices.data(), indices.size() * sizeof(typename ToUnsigned<T>::Type));
    }

  private:
    /**
     * @brief Allocates the given number of bytes to this @a `IndexBuffer` on the graphics card.
     * 
     * @param size  The number of bytes to be allocated. 
     */
    void reserveRaw (const Size size);

    /**
     * @brief Allocates, then uploads, raw data of the given size to this @a `IndexBuffer` on the 
     *        graphics card.
     * 
     * @param data  Points to the raw index data to be uploaded. 
     * @param size  The size, in bytes, of the raw index data to be allocated and uploaded.
     */
    void allocateRaw (const void* data, const Size size);

    /**
     * @brief Uploads raw data of the given size to this @a `IndexBuffer` on the graphics card.
     * 
     * @param data  Points to the raw index data to be uploaded. 
     * @param size  The size, in bytes, of the raw index data to be uploaded.
     */
    void uploadRaw (const void* data, const Size size);

  private:
    /**
     * @brief An integer handle pointing to this @a `IndexBuffer`'s data on the graphics card.
     */
    Uint32 m_handle = 0;

    /**
     * @brief Indicates whether or not this is a @a `GL_DYNAMIC_DRAW` index buffer.
     */
    Boolean m_dynamic = false;

    /**
     * @brief The size, in bytes, of the index data allocated to this @a `IndexBuffer` on the
     *        graphics card.
     */
    Size m_bufferSize = 0;

    /**
     * @brief The number of indices allocated to this @a `IndexBuffer` on the graphics card.
     */
    Count m_indexCount = 0;

    /**
     * @brief The @a `IndexType` enumerator indicating the type of the unsigned integer indices
     *        in this @a `IndexBuffer`.
     */
    IndexType m_indexType = IndexType::None;

    /**
     * @brief The size, in bytes, of a single unsigned integer index in this @a `IndexBuffer`.
     */
    Size m_integerSize = 0;

  };

  using ElementBuffer = IndexBuffer;

}
