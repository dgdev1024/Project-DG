/** @file DG/Graphics/Texture.hpp */

#pragma once

#include <DG_Pch.hpp>

namespace dg
{

  /**
   * @brief The maximum number of texture slots which can be occupied. On most machines, this number
   *        is 16.
   */
  constexpr Size TEXTURE_SLOT_COUNT = 16;

  /**
   * @brief The @a `TextureWrapMode` enum enumerates the modes by which the @a `Renderer` can handle
   *        texture coordinates residing outside the normal range of @a `(0, 0)` and @a `(1, 1)`
   *        when rendering a given @a `Texture`.
   */
  enum class TextureWrapMode
  {
    Repeat,
    MirroredRepeat,
    ClampToEdge
  };

  /**
   * @brief The @a `TextureFilterMode` enum enumerates the modes by which a texture coordinate is
   *        resolved to a pixel in a @a `Texture`.
   */
  enum class TextureFilterMode
  {
    Nearest,
    Linear
  };

  /**
   * @brief The @a `TextureSpecification` struct contains attributes which define a @a `Texture`.
   */
  struct TextureSpecification
  {
    /**
     * @brief The width and height of the @a `Texture`, in pixels.
     */
    Vector2u size = { 1, 1 };

    /**
     * @brief The number of color channels in this @a `Texture`. For instance, RGBA textures have
     *        four color channels (red, green, blue and alpha).
     */
    Uint32 colorChannels = 4;

    /**
     * @brief The @a `Texture`'s wrapping mode.
     */
    TextureWrapMode wrap = TextureWrapMode::Repeat;

    /**
     * @brief The @a `Texture`'s minification filter mode.
     */
    TextureFilterMode magnify = TextureFilterMode::Nearest;

    /**
     * @brief The @a `Texture`'s magnitification filter mode.
     */
    TextureFilterMode minify = TextureFilterMode::Nearest;

  };  

  /**
   * @brief The @a `Texture` class describes, among other purposes, image data which can be used to
   *        add detail to a primitive or mesh being rendered.
   */
  class Texture
  {
  public:
    Texture ();
    ~Texture ();

    /**
     * @brief Creates a new @a `Texture` from the given image file.
     * 
     * @param path  The path the texture file to load.
     *  
     * @return  A shared pointer to the newly-created @a `Texture`. 
     */
    static Ref<Texture> make (const Path& path);

    /**
     * @brief Sets this @a `Texture` as the active texture at the given texture slot.
     * 
     * @param slot The number of the texture slot to bind this texture to.
     */
    void bind (const Index slot = 0) const;

    /**
     * @brief Un-sets the active texture at the given texture slot.
     * 
     * @param slot The number of the texture slot to unbind.
     */
    void unbind (const Index slot = 0) const;

    /**
     * @brief Attempts to create a new @a `Texture` from the given @a `TextureSpecification`.
     * 
     * @param spec  The new texture's specification.
     *  
     * @return  @a `true` if the @a `Texture` is created with no errors; @a `false` otherwise. 
     */
    Boolean createFromSpecification (const TextureSpecification& spec);

    /**
     * @brief Attempts to create a new @a `Texture` by loading image data from the given file.
     * 
     * @param path  The path to the image file to load.
     *  
     * @return  @a `true` if the @a `Texture` is created, and its image data loaded, successfully;
     *          @a `false` otherwise. 
     */
    Boolean loadFromFile (const Path& path);

    /**
     * @brief Attempts to upload raw data to this @a `Texture`.
     * 
     * @param data  Points to the raw data to be uploaded.
     * @param size  The size of the data to be uploaded.
     */
    void uploadData (const void* data, const Size size);

    /**
     * @brief Resolves the given position within this @a `Texture`s bounds to a texture coordinate.
     * 
     * @param position  The position within the texture to resolve.
     * 
     * @return  The resolved texture coordinate.
     */
    Vector2f getTextureCoordinate (const Vector2f& position) const;

    /**
     * @brief Retrieves whether or not this @a `Texture` is valid and ready to be used.
     * 
     * @return  @a `true` if this is a valid @a `Texture`; @a `false` otherwise. 
     */
    Boolean isValid () const;

  private:
    /**
     * @brief The integer ID pointing to the @a `Texture` on the graphics card.
     */
    Uint32 m_handle = 0;

    /**
     * @brief Indicates the internal and public pixel formats of this @a `Texture`.
     */
    GLenum m_internalFormat = 0, m_pixelFormat = 0;

    /**
     * @brief Indicates whether or not this @a `Texture` is ready to be used.
     */
    Boolean m_valid = false;

    /**
     * @brief If this @a `Texture` was loaded from an image file, this contains the absolute path
     *        to that image file.
     */
    Path m_filepath = "";

    /**
     * @brief The @a `Texture`'s specification.
     */
    TextureSpecification m_spec;
    
  };

  /**
   * @brief The @a `TextureManager` class is a static helper class used for keeping track of
   *        and easily accessing currently-loaded @a `Texture` assets.
   */
  class TextureManager
  {
  public:
    /**
     * @brief Retrieves a loaded @a `Texture` asset which is mapped to the given relative filename
     *        string. If no such asset is found, then a new @a `Texture` asset is created, loaded
     *        from that file, then mapped to the given filename string.
     * 
     * @param filename  The relative filename of the asset to get or emplace.
     * 
     * @return  A shared pointer to the mapped (or newly-created) @a `Texture` asset. 
     */
    static Ref<Texture> getOrEmplace (const String& filename);

    /**
     * @brief Checks to see if a loaded @a `Texture` asset has been mapped to the given relative
     *        filename string.
     * 
     * @param filename  The relative filename of the asset to check for.
     * 
     * @return  @a `true` if a @a `Texture` asset is mapped to the given filename string; @a `false`
     *          otherwise. 
     */
    static Boolean contains (const String& filename);

    /**
     * @brief Retrieves a loaded @a `Texture` asset which is mapped to the given relative filename
     *        string.
     * 
     * @param filename  The relative filename of the asset to retrieve.
     *  
     * @return  A shared pointer to the mapped @a `Texture` asset. 
     */
    static Ref<Texture> get (const String& filename);

    /**
     * @brief Un-maps the loaded @a `Texture` asset which is mapped to the given relative filename 
     *        string.
     * 
     * @param filename  The relative filename of the asset to remove.
     *  
     * @return  A shared pointer to the removed @a `Texture` asset. 
     */
    static Ref<Texture> remove (const String& filename);

    /**
     * @brief Un-maps all loaded, currently-mapped @a `Texture` assets.
     */
    static void clear ();
    
  private:
    /**
     * @brief The collection of currently loaded @a `Texture` assets.
     */
    static Dictionary<Ref<Texture>> s_assets;

  };

}
