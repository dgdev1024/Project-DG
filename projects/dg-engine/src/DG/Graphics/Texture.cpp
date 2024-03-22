/** @file DG/Graphics/Texture.cpp */

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

#include <DG/Core/FileIo.hpp>
#include <DG/Graphics/Texture.hpp>

namespace dg
{

  namespace Private
  {

    GLenum resolveGLTextureWrap (TextureWrapMode mode)
    {
      switch (mode) {
        case TextureWrapMode::Repeat:         return GL_REPEAT;
        case TextureWrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case TextureWrapMode::ClampToEdge:    return GL_CLAMP_TO_EDGE;
        default: return 0;
      }
    }

    GLenum resolveGLTextureFilter (TextureFilterMode mode)
    {
      switch (mode) {
        case TextureFilterMode::Linear:   return GL_LINEAR;
        case TextureFilterMode::Nearest:  return GL_NEAREST;
        default: return 0;
      }
    }

    Boolean resolveGLTextureFormat (Uint32 channelCount, GLenum& internalFormat, 
      GLenum& pixelFormat)
    {
      switch (channelCount) {
        case 1: internalFormat = GL_R8;     pixelFormat = GL_RED;   break;
        case 2: internalFormat = GL_RG8;    pixelFormat = GL_RG;    break;
        case 3: internalFormat = GL_RGB8;   pixelFormat = GL_RGB;   break;
        case 4: internalFormat = GL_RGBA8;  pixelFormat = GL_RGBA;  break;
        default: return false;
      }

      return true;
    }

  }

  Texture::Texture ()
  {
    glGenTextures(1, &m_handle);
  }

  Texture::~Texture ()
  {
    glDeleteTextures(1, &m_handle);
  }

  Ref<Texture> Texture::make (const Path& path)
  {
    auto texture = makeRef<Texture>();
    texture->loadFromFile(path);
    return texture;
  }

  void Texture::bind (const Index slot) const
  {
    if (slot >= TEXTURE_SLOT_COUNT) {
      DG_ENGINE_CRIT("Attempted 'bind' of GL texture to invalid texture slot {}!", slot);
      throw std::out_of_range { "Attempted 'bind' of GL texture to invalid texture slot!" };
    }
    
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_handle);
  }

  void Texture::unbind (const Index slot) const
  {
    if (slot >= TEXTURE_SLOT_COUNT) {
      DG_ENGINE_CRIT("Attempted 'bind' of GL texture to invalid texture slot {}!", slot);
      throw std::out_of_range { "Attempted 'bind' of GL texture to invalid texture slot!" };
    }
    
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  Boolean Texture::createFromSpecification (const TextureSpecification& spec)
  {
    // Determine the pixel format from the given color channel count.
    if (
      Private::resolveGLTextureFormat(spec.colorChannels, m_internalFormat, m_pixelFormat) == false
    ) {
      DG_ENGINE_ERROR("Blank texture has invalid color channel count {}.", spec.colorChannels);
      return false;
    }

    // Bind the texture, then set its wrap and filter modes.
    glBindTexture(GL_TEXTURE_2D, m_handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 
      Private::resolveGLTextureWrap(spec.wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 
      Private::resolveGLTextureWrap(spec.wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
      Private::resolveGLTextureFilter(spec.minify));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
      Private::resolveGLTextureFilter(spec.magnify));
  
    // Set aside storage for the texture on the graphics card.
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, spec.size.x, spec.size.y, 0, m_pixelFormat,
      GL_UNSIGNED_BYTE, nullptr);
      
    m_spec = spec;
    m_valid = true;
    return true;
  }

  Boolean Texture::loadFromFile (const Path& path)
  {
    if (path.empty()) {
      DG_ENGINE_ERROR("No image filename specified to load into the texture.");
      return false;
    }

    if (fs::exists(path) == false) {
      DG_ENGINE_ERROR("Image filename '{}' not found.", path);
      return false;
    }

    // Make sure that images are correctly flipped before any loading is done.
    stbi_set_flip_vertically_on_load(true);

    // Store the image's width, height and color channel count here.
    Int32 width = 0, height = 0, colorChannels = 0;
    Uint8* data = stbi_load(path.c_str(), &width, &height, &colorChannels, 0);
    if (data == nullptr) {
      DG_ENGINE_ERROR("Could not load image file '{}' - {}", path,
        stbi_failure_reason());
      return false;
    }

    // Determine the pixel format from the given color channel count.
    if (Private::resolveGLTextureFormat(colorChannels, m_internalFormat, m_pixelFormat) == false)
    {
      DG_ENGINE_ERROR("Image file '{}' has invalid color channel count {}.", path, colorChannels);
      stbi_image_free(data);
      return false;
    }

    m_spec.size = { static_cast<Uint32>(width), static_cast<Uint32>(height) };
    m_spec.colorChannels = static_cast<Uint32>(colorChannels);

    // Bind the texture, then set its wrap and filter modes.
    glBindTexture(GL_TEXTURE_2D, m_handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Private::resolveGLTextureWrap(m_spec.wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Private::resolveGLTextureWrap(m_spec.wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Private::resolveGLTextureFilter(m_spec.minify));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Private::resolveGLTextureFilter(m_spec.magnify));
  
    // Set aside storage for the texture on the graphics card.
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_spec.size.x, m_spec.size.y, 0, m_pixelFormat,
      GL_UNSIGNED_BYTE, data);
      
    m_valid = true;
    return true;
  }

  void Texture::uploadData (const void* data, const Size size)
  {
    if (data == nullptr || size == 0) {
      throw std::invalid_argument { "Attempted 'uploadData' with null image data!" };
    } else if (size != m_spec.size.x * m_spec.size.y * m_spec.colorChannels) {
      throw std::invalid_argument { "Attempted 'uploadData' of mismatched texture size!" };
    }

    glBindTexture(GL_TEXTURE_2D, m_handle);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_spec.size.x, m_spec.size.y, m_pixelFormat,
      GL_UNSIGNED_BYTE, data);
  }

  Vector2f Texture::getTextureCoordinate (const Vector2f& position) const
  {
    if (m_spec.size.x == 0 || m_spec.size.y == 0) { return { 0, 0 }; }
    return {
      position.x / m_spec.size.x,
      position.y / m_spec.size.y
    };
  }

  Boolean Texture::isValid () const
  {
    return m_valid;
  }

  Dictionary<Ref<Texture>> TextureManager::s_assets;

  Ref<Texture> TextureManager::getOrEmplace (const String& filename)
  {
    // Ensure that the relative filename string is provided.
    if (filename.empty()) {
      throw std::invalid_argument { "Attempted 'getOrEmplace' with a blank filename string!" };
    }

    // Check to see if the texture asset is present. If so, then return it.
    auto iter = s_assets.find(filename);
    if (iter != s_assets.end()) { return iter->second; }

    // Create the texture. Ensure that it is valid.
    Ref<Texture> texture = Texture::make(FileIo::getAbsolute(filename));
    if (texture->isValid() == false) {
      DG_ENGINE_CRIT("Could not load texture asset file '{}'!", filename);
      throw std::runtime_error { "Could not load texture asset file!" };
    }

    // Emplace, then return the texture.
    s_assets.emplace(filename, texture);
    return texture;
  }

  Boolean TextureManager::contains (const String& filename)
  {
    // Ensure that the relative filename string is provided.
    if (filename.empty()) {
      throw std::invalid_argument { "Attempted 'contains' with a blank filename string!" };
    }

    return s_assets.contains(filename);
  }

  Ref<Texture> TextureManager::get (const String& filename)
  {
    if (filename.empty()) {
      throw std::invalid_argument { "Attempted 'get' with blank string filename!" };
    }

    // Ensure the texture asset exists.
    auto iter = s_assets.find(filename);
    if (iter == s_assets.end()) {
      DG_ENGINE_CRIT("Texture asset '{}' not found!", filename);
      throw std::out_of_range { "Texture asset not found!" };
    }

    return iter->second;    
  }

  Ref<Texture> TextureManager::remove (const String& filename)
  {
    if (filename.empty()) {
      throw std::invalid_argument { "Attempted 'remove' with blank string filename!" };
    }

    // Ensure the texture asset exists.
    auto iter = s_assets.find(filename);
    if (iter == s_assets.end()) {
      DG_ENGINE_CRIT("Texture asset '{}' not found!", filename);
      throw std::out_of_range { "Texture asset not found!" };
    }

    // Get a copy of the texture before removing it.
    auto removed = s_assets.at(filename);
    s_assets.erase(iter);

    return removed;    
  }

  void TextureManager::clear ()
  {
    s_assets.clear();
  }

}
