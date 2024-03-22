/** @file DG/Graphics/Shader.hpp */

#pragma once

#include <DG/Graphics/GraphicsBuffers.hpp>

namespace dg
{

  /**
   * @brief The @a `Shader` class describes a small program which instructs the graphics card on how
   *        to render the vertices in a @a `VertexBuffer`.
   */
  class Shader
  {
  public:
    Shader ();
    ~Shader ();

    /**
     * @brief Creates a new @a `Shader` from the given vertex and fragment source code.
     * 
     * @param vertexCode    The source code for the vertex shader. 
     * @param fragmentCode  The source code for the fragment/pixel shader.
     * 
     * @return  A shared pointer to the newly-created @a `Shader`. 
     */
    static Ref<Shader> make (const String& vertexCode, const String& fragmentCode);

    /**
     * @brief Creates a new @a `Shader` from the given shader source file.
     * 
     * @param path        The path the shader file to load.
     *  
     * @return  A shared pointer to the newly-created @a `Shader`. 
     */
    static Ref<Shader> make (const Path& path);

    /**
     * @brief Sets this @a `Shader` as the active shader program.
     */
    void bind () const;

    /**
     * @brief Un-sets the active shader program.
     */
    static void unbind ();

    /**
     * @brief Attempts to build a shader program from the given source code strings.
     * 
     * @param vertexCode    The source code of the vertex shader. 
     * @param fragmentCode  The source code of the fragment (or pixel) shader.
     * 
     * @return  @a `true` if the shader program is compiled and linked successfully;
     *          @a `false` otherwise. 
     */
    Boolean loadFromSources (const String& vertexCode, const String& fragmentCode);

    /**
     * @brief Attempts to build a shader program from source code loaded from the given file.
     * 
     * @param path  The path to the shader file to load.
     * 
     * @return  @a `true` if the source file's contents are loaded, and the shader program is
     *          compiled from that code and linked successfully; @a `false` otherwise.
     */
    Boolean loadFromFile (const Path& path);

    /**
     * @brief Sets the value of a shader uniform of type @a `T` mapped to the given string name.
     * 
     * @tparam T  The type of the shader uniform to set.
     * 
     * @param name  The name of the shader uniform to set.
     * @param value The shader uniform's new value.
     */
    template <typename T>
    void setUniform (const String& name, const T value);

    /**
     * @brief Retrieves whether or not this @a `GLShader` has been built and linked properly.
     * 
     * @return  @a `true` if the shader program is valid; @a `false` otherwise. 
     */
    Boolean isValid () const;

  private:
    /**
     * @brief Attempts to build the shader program.
     * 
     * @return @a `true` if there were no build errors; @a `false` otherwise.
     */
    Boolean build ();

    /**
     * @brief Attempts to resolve the given shader uniform name to an ID on the shader program.
     * 
     * @param name  The name of the shader uniform to resolve.
     * 
     * @return  The resolved integer ID of the shader uniform if found; @a `-1` otherwise. 
     */
    Int32 findUniform (StringView name);

  private:
    /**
     * @brief The unique ID pointing to this shader program on the graphics card.
     */
    Uint32 m_handle = 0;

    /**
     * @brief The source code of the vertex shader.
     */
    String m_vertexCode = "";

    /**
     * @brief The source code of the fragment shader.
     */
    String m_fragmentCode = "";

  };

  /**
   * @brief The @a `ShaderManager` class is a static helper class used for keeping track of
   *        and easily accessing currently-loaded @a `Shader` assets.
   */
  class ShaderManager
  {
  public:
    /**
     * @brief Retrieves a loaded @a `Shader` asset which is mapped to the given relative filename
     *        string. If no such asset is found, then a new @a `Shader` asset is created, loaded
     *        from that file, then mapped to the given filename string.
     * 
     * @param filename  The relative filename of the asset to get or emplace.
     * 
     * @return  A shared pointer to the mapped (or newly-created) @a `Shader` asset. 
     */
    static Ref<Shader> getOrEmplace (const String& filename);

    /**
     * @brief Checks to see if a loaded @a `Shader` asset has been mapped to the given relative
     *        filename string.
     * 
     * @param filename  The relative filename of the asset to check for.
     * 
     * @return  @a `true` if a @a `Shader` asset is mapped to the given filename string; @a `false`
     *          otherwise. 
     */
    static Boolean contains (const String& filename);

    /**
     * @brief Retrieves a loaded @a `Shader` asset which is mapped to the given relative filename
     *        string.
     * 
     * @param filename  The relative filename of the asset to retrieve.
     *  
     * @return  A shared pointer to the mapped @a `Shader` asset. 
     */
    static Ref<Shader> get (const String& filename);

    /**
     * @brief Un-maps the loaded @a `Shader` asset which is mapped to the given relative filename 
     *        string.
     * 
     * @param filename  The relative filename of the asset to remove.
     *  
     * @return  A shared pointer to the removed @a `Shader` asset. 
     */
    static Ref<Shader> remove (const String& filename);

    /**
     * @brief Un-maps all loaded, currently-mapped @a `Shader` assets.
     */
    static void clear ();
    
  private:
    /**
     * @brief The collection of currently loaded @a `Shader` assets.
     */
    static Dictionary<Ref<Shader>> s_assets;

  };

}
