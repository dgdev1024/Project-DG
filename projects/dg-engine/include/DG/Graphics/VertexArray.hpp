/** @file DG/Graphics/VertexArray.hpp */

#pragma once

#include <DG/Graphics/GraphicsBuffers.hpp>

namespace dg
{

  /**
   * @brief The @a `VertexArray` class describes an interface for encapsulating the setup and
   *        management of vertex buffers and attributes, as well as index buffers, for rendering.
   */
  class VertexArray
  {
  public:
    VertexArray ();
    ~VertexArray ();

    /**
     * @brief Creates a new @a `VertexArray`.
     * 
     * @return  A shared pointer to the newly-created @a `VertexArray`. 
     */
    static Ref<VertexArray> make ();

    /**
     * @brief Designates this @a `VertexArray` as the active vertex array object.
     */
    void bind () const;

    /**
     * @brief Un-sets the active @a `VertexArray` object.
     */
    static void unbind ();

    /**
     * @brief Adds a new @a `VertexBuffer` for this @a `VertexArray` to manage.
     * 
     * @param buffer  Points to the vertex buffer to be added and managed.
     */
    void addVertexBuffer (const Ref<VertexBuffer>& buffer);

    /**
     * @brief Sets the @a `IndexBuffer` to be used and managed by this @a `VertexArray`.
     * 
     * @param buffer  Points to the index buffer to be used and managed.
     */
    void setIndexBuffer (const Ref<IndexBuffer>& buffer);

    /**
     * @brief Retrieves the @a `IndexBuffer` bound to this @a `VertexArray`, 
     *        if one is bound.
     * 
     * @return  A pointer to the bound @a `IndexBuffer`; @a `nullptr` if no
     *          such buffer is bound.
     */
    const Ref<IndexBuffer>& getIndexBuffer () const;

  private:
    /**
     * @brief The unique ID of the @a `VertexArray` on the graphics card.
     */
    Uint32 m_handle = 0;

    /**
     * @brief The collection of pointers to @a `VertexBuffers` managed by this
     * @a `VertexArray`.
     */
    Collection<Ref<VertexBuffer>> m_vertexBuffers;

    /**
     * @brief Points to the @a `IndexBuffer`, if any, managed by this @a `VertexArray`.
     */
    Ref<IndexBuffer> m_indexBuffer = nullptr;

  };

}
