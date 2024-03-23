/** @file DG/Graphics/Renderer.cpp */

#include <DG/Graphics/Renderer.hpp>

namespace dg
{

  /** Renderer Initialization / Shutdown **********************************************************/

  Renderer::Renderer (const RendererSpecification& spec)
  {
    RenderInterface::initialize();

    // First, create the blank, white texture(s).
    Uint32 blankTextureData = 0xFFFFFFFF;
    m_renderData2D.blankTexture = Texture::make();
    m_renderData2D.blankTexture->uploadData(&blankTextureData, sizeof(Uint32));

    // Allocate the textures container. Place the blank white texture we just created as the first
    // entry.
    m_renderData2D.textures.resize(TEXTURE_SLOT_COUNT, nullptr);
    m_renderData2D.textures[0] = m_renderData2D.blankTexture;

    // Allocate and populate an array of vertex indices. Feed the indices into a new index buffer.
    // Allocate just a few more indices than is necessary to prevent any bounds issues.
    Collection<Uint32> indices(RenderData2D::INDICES_PER_BATCH + 30);
    for (Index i = 0, offset = 0; i < indices.size(); i += 6, offset += 4) {
      indices[i + 0] = offset + 0;
      indices[i + 1] = offset + 1;
      indices[i + 2] = offset + 2;
      indices[i + 3] = offset + 2;
      indices[i + 4] = offset + 3;
      indices[i + 5] = offset + 0;
    }
    Ref<IndexBuffer> indexBuffer = IndexBuffer::make(false);
    indexBuffer->allocate<IndexType::UnsignedInt>(indices);

    // Allocate the array of quad vertices, feed them into the vertex buffer, then bind the vertex
    // buffer to the vertex array object.
    m_renderData2D.quadVertices.resize(RenderData2D::VERTICES_PER_BATCH + 20);
    m_renderData2D.quadVertexBuffer = VertexBuffer::make(true);
    m_renderData2D.quadVertexBuffer->reserve<QuadVertex2D>(m_renderData2D.quadVertices.size());
    m_renderData2D.quadVertexBuffer->setLayout({
      { "in_Position",  VertexAttributeType::Float3  },
      { "in_TexCoords", VertexAttributeType::Float2  },
      { "in_TexIndex",  VertexAttributeType::Float   },
      { "in_Color",     VertexAttributeType::Float4  },
      { "in_EntityId",  VertexAttributeType::Float   }    
    });
    m_renderData2D.quadVertexArray = VertexArray::make();
    m_renderData2D.quadVertexArray->addVertexBuffer(m_renderData2D.quadVertexBuffer);
    m_renderData2D.quadVertexArray->setIndexBuffer(indexBuffer);

    // Set up the transformation vertices.
    m_renderData2D.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    m_renderData2D.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    m_renderData2D.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    m_renderData2D.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

    // Set up the quad texture coordinates.
    m_renderData2D.quadTextureCoordinates[0] = { 0.0f, 0.0f };
    m_renderData2D.quadTextureCoordinates[1] = { 1.0f, 0.0f };
    m_renderData2D.quadTextureCoordinates[2] = { 1.0f, 1.0f };
    m_renderData2D.quadTextureCoordinates[3] = { 0.0f, 1.0f };
  }

  Renderer::~Renderer ()
  {
    
  }

  Scope<Renderer> Renderer::make (const RendererSpecification& spec)
  {
    return makeScope<Renderer>(spec);
  }

  /** Use Shaders / Frame Buffer ******************************************************************/

  void Renderer::useFrameBuffer2D (const Ref<FrameBuffer>& framebuffer)
  {
    // Ensure that a valid frame buffer has been provided!
    if (framebuffer == nullptr) {
      throw std::invalid_argument { "Null frame buffer provided for 2D rendering!" };
    }

    // If the frame buffer is being swapped out in the middle of rendering a scene, then we need to
    // flush the current rendering batch, first.
    if (m_renderData2D.sceneHasStarted == true) {
      flushScene2D(true);
    }

    // Swap out, then bind the new framebuffer.
    m_renderData2D.framebuffer = framebuffer;
    m_renderData2D.framebuffer->bind();
  }

  void Renderer::useQuadShader2D (const Ref<Shader>& shader)
  {
    // Ensure that a valid shader is provided!
    if (shader == nullptr || shader->isValid() == false) {
      throw std::invalid_argument { "Null or invalid shader provided for rendering 2D quads!" };
    }

    // If the shader is being swapped out in the middle of a scene, then the current batch will need
    // to be flushed, first.
    if (m_renderData2D.sceneHasStarted == true) {
      flushScene2D(true);
    }

    // Un-bind the current shader if one is currently bound, then swap out the shader.
    Shader::unbind();
    m_renderData2D.quadShader = shader;

    // Send the texture slot uniforms over to the new shader.
    for (Index i = 0; i < TEXTURE_SLOT_COUNT; ++i) {
      m_renderData2D.quadShader->setUniform<Int32>("uni_TexSlots[" + std::to_string(i) + "]", i);
    }

    // Also, if this is being done in the middle of a scene, we need to send the camera product to
    // the new shader as well.
    if (m_renderData2D.sceneHasStarted == true) {
      m_renderData2D.quadShader->setUniform<Matrix4f>("uni_CameraProduct", m_renderData2D.cameraProduct);
    }
  }

  /** Begin / End Scene / Flush *******************************************************************/

  void Renderer::beginScene2D (const Matrix4f& cameraProduct)
  {
    // Ensure that a scene has not already started rendering!
    if (m_renderData2D.sceneHasStarted == true) {
      throw std::runtime_error { "Attempt to begin 2D scene when one is already started!" };
    }

    // Ensure that the render target has been set!
    if (m_renderData2D.framebuffer == nullptr) {
      throw std::runtime_error { "Attempt to begin 2D scene with no render target frame buffer!" };
    }

    // Ensure that all shaders are provided before starting the scene!
    if (
      m_renderData2D.quadShader == nullptr
    ) {
      throw std::runtime_error { "Attempt to begin 2D scene without sufficient shaders!" };
    }

    // Use the provided projection and view matrices to produce the camera product. Send that
    // matrix to the shaders.
    m_renderData2D.cameraProduct = cameraProduct;
    m_renderData2D.quadShader->setUniform<Matrix4f>("uni_CameraProduct", m_renderData2D.cameraProduct);

    // Reset the rendering statistics and mark the scene as started.
    m_renderData2D.quadVertexCount = 0;
    m_renderData2D.batchVertexCount = 0;
    m_renderData2D.totalVertexCount = 0;
    m_renderData2D.quadIndexCount = 0;
    m_renderData2D.batchIndexCount = 0;
    m_renderData2D.totalIndexCount = 0;
    m_renderData2D.batchTextureCount = 1;
    m_renderData2D.batchCount = 0;
    m_renderData2D.sceneHasStarted = true;
  }

  void Renderer::beginScene2D (const Matrix4f& projection, const Matrix4f& view)
  {
    beginScene2D(projection * glm::inverse(view));
  }

  void Renderer::flushScene2D (Bool flushingEarly)
  {

    // A scene cannot be flushed if it hasn't even been started!
    if (m_renderData2D.sceneHasStarted == false) {
      throw std::runtime_error { "Attempt to flush a 2D scene batch when no scene has started!" };
    }

    // Slot the textures needed to render the batch.
    for (Index i = 0; i < m_renderData2D.batchTextureCount; ++i) {
      m_renderData2D.textures[i]->bind(i);
    }

    // If there were quad vertices rendered, then render those vertices here.
    if (m_renderData2D.quadVertexCount > 0) {

      // Upload the vertices to its respective vertex buffer here.
      m_renderData2D.quadVertexBuffer->upload<QuadVertex2D>(m_renderData2D.quadVertices,
        m_renderData2D.quadVertexCount);

      // Bind the quad shader and draw.
      m_renderData2D.quadShader->bind();
      RenderInterface::drawIndexed(m_renderData2D.quadVertexArray, m_renderData2D.quadIndexCount);

    }

    // If this flush is happening before the end of this scene, then reset the batch-specific
    // rendering statistics.
    if (flushingEarly == true) {
      m_renderData2D.quadVertexCount = 0;
      m_renderData2D.batchVertexCount = 0;
      m_renderData2D.quadIndexCount = 0;
      m_renderData2D.batchIndexCount = 0;
      m_renderData2D.batchTextureCount = 1;
    }
    
    // Increment the batch count.
    m_renderData2D.batchCount++;

  }

  void Renderer::endScene2D ()
  {
    // Ensure that a 2D scene has started, first!
    if (m_renderData2D.sceneHasStarted == false) {
      throw std::runtime_error { "Attempt to end a 2D scene without first starting one!" };
    }

    // Flush the current rendering batch.
    flushScene2D(false);

    m_renderData2D.sceneHasStarted = false;

  }

  /** 2D Submission Functions *********************************************************************/

  void Renderer::submitQuad2D (const Matrix4f& transform, const RenderDrawSpecification2D& spec)
  {
    // Ensure that a scene is currently underway!
    if (m_renderData2D.sceneHasStarted == false) {
      throw std::runtime_error { "Attempt to submit a 2D scene with no scene started!" };
    }

    // Slot the texture into place, if provided. Cast the slot number, and the entity ID into a
    // floating point.
    Float32 textureIndex  = static_cast<Float32>(slotTexture2D(spec.texture));
    Float32 entityId      = static_cast<Float32>(spec.entityId);

    // Submit the quad's vertices.
    submitQuadVertex2D(QuadVertex2D {
      transform * m_renderData2D.quadVertexPositions[0],
      m_renderData2D.quadTextureCoordinates[0],
      textureIndex, spec.color, entityId
    });
    submitQuadVertex2D(QuadVertex2D {
      transform * m_renderData2D.quadVertexPositions[1],
      m_renderData2D.quadTextureCoordinates[1],
      textureIndex, spec.color, entityId
    });
    submitQuadVertex2D(QuadVertex2D {
      transform * m_renderData2D.quadVertexPositions[2],
      m_renderData2D.quadTextureCoordinates[2],
      textureIndex, spec.color, entityId
    });
    submitQuadVertex2D(QuadVertex2D {
      transform * m_renderData2D.quadVertexPositions[3],
      m_renderData2D.quadTextureCoordinates[3],
      textureIndex, spec.color, entityId
    });

    // Update the quad index counts.
    m_renderData2D.quadIndexCount += 6;
    m_renderData2D.batchIndexCount += 6;
    m_renderData2D.totalIndexCount += 6;

    // Check to see if it's time to flush the rendering batch.
    if (
      m_renderData2D.quadVertexCount >= RenderData2D::VERTICES_PER_BATCH ||
      m_renderData2D.quadIndexCount >= RenderData2D::INDICES_PER_BATCH ||
      m_renderData2D.batchTextureCount >= TEXTURE_SLOT_COUNT
    ) {
      flushScene2D(true);
    }
  }

  void Renderer::submitQuad2D (const Vector3f& position, const Vector2f& size, 
    const Float32 rotation, const RenderDrawSpecification2D& spec)
  {
    Matrix4f transform = 
      glm::translate(Matrix4f(1.0f), position) *
      glm::rotate(Matrix4f(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
      glm::scale(Matrix4f(1.0f), { size.x, size.y, 1.0f });

    submitQuad2D(transform, spec);
  }

  /** Vertex Submission Functions *****************************************************************/

  void Renderer::submitQuadVertex2D (const QuadVertex2D& vertex)
  {
    m_renderData2D.quadVertices[m_renderData2D.quadVertexCount++] = vertex;
    m_renderData2D.batchVertexCount++;
    m_renderData2D.totalVertexCount++;
  }

  /** Other Private Functions *********************************************************************/

  Index Renderer::slotTexture2D (const Ref<Texture>& texture)
  {

    // If the texture given is null or invalid, then return the slot of the blank, white texture.
    if (texture == nullptr || texture->isValid() == false) {
      return 0;
    }

    // Iterate over the collection of textures which have been queued up, looking for the next
    // null/invalid texture slot.
    for (Index i = 0; i < m_renderData2D.batchTextureCount; ++i) {

      // If this texture slot is empty, then slot this texture in and return this index.
      if (m_renderData2D.textures[i] == nullptr) {
        m_renderData2D.textures[i] = texture;
        return i;
      }

      // Compare the memory addresses of the texture in this slot and of the one given.
      // If the addresses are the same, then return this texture's index.
      if (m_renderData2D.textures[i].get() == texture.get()) {
        return i;
      }

    }

    if (m_renderData2D.batchTextureCount == TEXTURE_SLOT_COUNT) {
      flushScene2D(true);
    }

    // Slot this texture into a new slot, return its index and increment the
    // next slot.
    m_renderData2D.textures[m_renderData2D.batchTextureCount] = texture;
    return m_renderData2D.batchTextureCount++;

  }

}
