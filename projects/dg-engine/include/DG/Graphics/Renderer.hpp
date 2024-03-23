/** @file DG/Graphics/Renderer.hpp */

#pragma once

#include <DG/Graphics/FrameBuffer.hpp>
#include <DG/Graphics/Color.hpp>
#include <DG/Graphics/VertexArray.hpp>
#include <DG/Graphics/Shader.hpp>
#include <DG/Graphics/Texture.hpp>
#include <DG/Graphics/RenderInterface.hpp>

namespace dg
{

  /**
   * @brief The @a `QuadVertex2D` struct describes a vertex which is used to render a quad in
   *        two-dimensional space.
   */
  struct QuadVertex2D
  {
    Vector3f  position;
    Vector2f  textureCoords;
    Float32   textureIndex;
    Vector4f  color;
    Float32   entityId;
  };

  /**
   * @brief The @a `RenderData2D` struct describes the data which is needed to render a scene in
   *        two-dimensional space.
   */
  struct RenderData2D
  {

    /**
     * @brief This is the maximum number of quads which can be rendered in a single rendering batch.
     *        This dictates the maximum number of vertices and indices which can be processed in
     *        that single batch.
     */
    static constexpr Count QUADS_PER_BATCH    = 25000;
    static constexpr Count VERTICES_PER_BATCH = QUADS_PER_BATCH * 4;
    static constexpr Count INDICES_PER_BATCH  = QUADS_PER_BATCH * 6;

    /**
     * @brief Indicates whether or not a scene has started rendering.
     */
    Bool sceneHasStarted = false;

    /**
     * @brief The "camera product" refers to the product of a view matrix times a projection matrix.
     */
    Matrix4f cameraProduct = Matrix4f { 1.0f };

    /**
     * @brief Points to a @a `FrameBuffer` to which the 2D scene will be rendered.
     */
    Ref<FrameBuffer> framebuffer = nullptr;

    /**
     * @brief Points to a blank, white, 1x1 pixel texture. This texture is to be used when rendering
     *        non-textured primitives.
     */
    Ref<Texture> blankTexture = nullptr;

    /**
     * @brief Points to a series of @a `VertexArray` objects used for rendering certain primitives.
     */
    Ref<VertexArray>  quadVertexArray = nullptr;

    /**
     * @brief Points to a series of @a `VertexBuffer` objects which are bound to their respsective
     *        @a `VertexArray` objects, used for rendering certain primitives.
     */
    Ref<VertexBuffer> quadVertexBuffer = nullptr;

    /**
     * @brief Points to a series of @a `Shader` assets used for rendering certain primitives.
     */
    Ref<Shader> quadShader = nullptr;

    /**
     * @brief A collection of pointers to textures and texture atlases which are used to add detail
     *        to primitives in the current rendering batch. This collection will always at least
     *        contain the blank, white texture pointed to by @a `blankTexture`.
     * 
     * @sa    @a `RenderData2D::blankTexture`
     */
    Collection<Ref<Texture>> textures;

    /**
     * @brief A series of collections of vertices submitted for rendering in the current batch.
     */
    Collection<QuadVertex2D> quadVertices;

    /**
     * @brief A series of arrays of vertex positions. These vertex positions are transformed when
     *        submitting their respective vertices.
     */
    Vector4f quadVertexPositions[4];

    /**
     * @brief A series of arrays of texture coordinates, each used for submitting their respective
     *        vertices for rendering.
     */
    Vector2f quadTextureCoordinates[4];

    /**
     * @brief A series of counts keeping track of the number of respective vertices submitted in
     *        the current rendering batch.
     */
    Count quadVertexCount = 0;

    /**
     * @brief A series of counts keeping track of the number of respective vertex indices submitted
     *        in the current rendering batch.
     */
    Count quadIndexCount = 0;

    /**
     * @brief The total number of vertices and indices submitted in the current rendering batch.
     */
    Count batchVertexCount = 0,
          batchIndexCount = 0;

    /**
     * @brief The total number of vertices and indices rendered in the current 2D scene.
     */
    Count totalVertexCount = 0,
          totalIndexCount = 0;

    /**
     * @brief The number of textures slotted in this rendering batch. There will always be at least
     *        one texture - the blank, white texture - in a batch.
     * 
     * @sa    @a `RenderData2D::blankTexture`
     */
    Count batchTextureCount = 1;

    /**
     * @brief The total number of batches needed to render the current 2D scene.
     */
    Count batchCount = 0;

  };

  /**
   * @brief The @a `RenderDrawSpecification2D` struct describes how a primitive should be rendered
   *        in two-dimensional space.
   */
  struct RenderDrawSpecification2D
  {

    /**
     * @brief Indicates the color of the primitive being drawn. If a texture is being rendered over
     *        the primitive, then this presents a tint over that texture.
     */
    Color color = Color::White;

    /**
     * @brief Points to a @a `Texture` to be rendered over the top of the primitive.
     */
    Ref<Texture> texture = nullptr;

    /**
     * @brief Indicates the ID of an entity, if any, to which the primitive being rendered may
     *        belong.
     */
    Int32 entityId = -1;

  };

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

  public: // Use Shader / Frame Buffer

    /**
     * @brief   Sets the frame buffer into which a 2D scene will be rendered.
     * 
     * @param   framebuffer Points to the @a `FrameBuffer` to be used.
     */
    void useFrameBuffer2D (const Ref<FrameBuffer>& framebuffer);

    /**
     * @brief   Sets the shader to be used for rendering quads in two-dimensional space.
     * 
     * @param   shader  Points to the @a `Shader` to be used.
     */
    void useQuadShader2D (const Ref<Shader>& shader);

  public: // Begine / End Scene / Flush

    /**
     * @brief   Begins rendering a new scene in two-dimensional space.
     * 
     * @param   cameraProduct A transformation matrix produced by multiplying a view matrix and a
     *                        projection matrix. 
     */
    void beginScene2D (const Matrix4f& cameraProduct);

    /**
     * @brief   Begins rendering a new scene in two-dimensional space.
     * 
     * @param   projection  A transformation matrix used to project the scene onto the render target.
     * @param   view        A transformation matrix used to indicate which part of the scene is
     *                      viewable.
     */
    void beginScene2D (const Matrix4f& projection, const Matrix4f& view);

    /**
     * @brief   Flushes the current 2D rendering batch, rendering any vertices which have been
     *          submitted.
     * 
     * @param   flushingEarly Is the batch being flushed before the end of the scene?
     */
    void flushScene2D (Bool flushingEarly = false);

    /**
     * @brief   Finishes rendering the current 2D scene, flushing the current rendering batch in
     *          the process.
     */
    void endScene2D ();

  public: // 2D Submission

    /**
     * @brief   Submits a quad to be rendered in two-dimensional space.
     * 
     * @param   transform A transformation matrix serving as the quad's model matrix, used to
     *                    translate, scane and rotate the quad.
     * @param   spec      Describes how the quad should be rendered.
     */
    void submitQuad2D (const Matrix4f& transform, const RenderDrawSpecification2D& spec = {});

    /**
     * @brief   Submits a quad to be rendered in two-dimensional space.
     * 
     * @param   position  The quad's position in the world.
     * @param   size      The quad's size.
     * @param   rotation  The quad's orientation, in degrees.
     * @param   spec      Describes how the quad should be rendered.
     */
    void submitQuad2D (const Vector3f& position, const Vector2f& size, const Float32 rotation,
      const RenderDrawSpecification2D& spec = {});

  public: // Getters / Setters

    inline Count getVertexCount2D () const { return m_renderData2D.totalVertexCount; }
    inline Count getIndexCount2D () const { return m_renderData2D.totalIndexCount; }
    inline Count getBatchCount2D () const { return m_renderData2D.batchCount; }

  private: // Vertex Submission Functions

    void submitQuadVertex2D (const QuadVertex2D& vertex);

  private: // Other Private Functions

    Index slotTexture2D (const Ref<Texture>& texture);

  private:
    /**
     * @brief Contains the data needed to render a scene in two-dimensional space.
     */
    RenderData2D m_renderData2D;

  };

}
