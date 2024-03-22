/** @file DG/Graphics/ColorPalette.hpp */

#pragma once

#include <DG_Pch.hpp>
#include <DG/Graphics/Color.hpp>

namespace dg
{

  /**
   * @brief The @a `ColorPalette` class describes a palette of colors which can be loaded from a
   *        file.
   */
  class ColorPalette
  {
  public:

    /**
     * @brief Constructs a @a `ColorPalette` with no colors.
     */
    ColorPalette ();
    
    /**
     * @brief Creates a new @a `ColorPalette` with no color data
     *  
     * @return  A shared pointer to the newly-created @a `ColorPalette`. 
     */
    static Ref<ColorPalette> make ();
    
    /**
     * @brief Creates a new @a `ColorPalette` with color data loaded from the given file.
     * 
     * @param filepath  The path to the color palette file to load.
     *  
     * @return  A shared pointer to the newly-created @a `ColorPalette`. 
     */
    static Ref<ColorPalette> make (const Path& filepath);

    /**
     * @brief Loads color palette data from a text file at the given path.
     * 
     * @param filepath  The path to the text file to load.
     * 
     * @return  @a `true` if the palette file is valid and loaded successfully;
     *          @a `false` otherwise. 
     */
    Bool loadFromFile (const Path& filepath);

    /**
     * @brief Adds a new @a `Color` structure to the end of this @a `ColorPalette`.
     * 
     * @param color The new color to be added. 
     */
    void pushColor (const Color& color);

    /**
     * @brief Indicates whether or not this @a `ColorPalette` contains any colors.
     * 
     * @return  @a `true` if this palette contains no colors;
     *          @a `false` otherwise. 
     */
    Bool isEmpty () const;

    /**
     * @brief Retrieves the number of @a `Color` structures in this @a `ColorPalette`.
     * 
     * @return  The number of colors in this palette. 
     */
    Count getColorCount () const;

    /**
     * @brief Retrieves a @a `Color` from this @a `ColorPalette` at the given index.
     * 
     * @param index The index of the color to retrieve.
     *  
     * @return  The retrieved @a `Color`. 
     */
    Color& get (const Index index);
    const Color& get (const Index index) const;

    /**
     * @brief Retrieves a @a `Color` from this @a `ColorPalette` at the given index.
     * 
     * @param index The index of the color to retrieve.
     *  
     * @return  The retrieved @a `Color`. 
     */
    Color& operator[] (const Index index);
    const Color& operator[] (const Index index) const;

  public:
    inline Collection<Color>::iterator begin () { return m_colors.begin(); }
    inline Collection<Color>::iterator end () { return m_colors.end(); }
    inline Collection<Color>::const_iterator cbegin () const { return m_colors.cbegin(); }
    inline Collection<Color>::const_iterator cend () const { return m_colors.cend(); }

    inline Collection<Color>::reverse_iterator rbegin () { return m_colors.rbegin(); }
    inline Collection<Color>::reverse_iterator rend () { return m_colors.rend(); }
    inline Collection<Color>::const_reverse_iterator crbegin () const { return m_colors.crbegin(); }
    inline Collection<Color>::const_reverse_iterator crend () const { return m_colors.crend(); }

  private:
    
    /**
     * @brief The collection of @a `Color` structures making up this @a `ColorPalette`.
     */
    Collection<Color> m_colors;

  };

  /**
   * @brief The @a `ColorPaletteManager` class is a static helper class used for keeping track of
   *        and easily accessing currently-loaded @a `ColorPalette` assets.
   */
  class ColorPaletteManager
  {
  public:
    /**
     * @brief Retrieves a loaded @a `ColorPalette` asset which is mapped to the given relative filename
     *        string. If no such asset is found, then a new @a `ColorPalette` asset is created, loaded
     *        from that file, then mapped to the given filename string.
     * 
     * @param filename  The relative filename of the asset to get or emplace.
     * 
     * @return  A shared pointer to the mapped (or newly-created) @a `ColorPalette` asset. 
     */
    static Ref<ColorPalette> getOrEmplace (const String& filename);

    /**
     * @brief Checks to see if a loaded @a `ColorPalette` asset has been mapped to the given relative
     *        filename string.
     * 
     * @param filename  The relative filename of the asset to check for.
     * 
     * @return  @a `true` if a @a `ColorPalette` asset is mapped to the given filename string; @a `false`
     *          otherwise. 
     */
    static Boolean contains (const String& filename);

    /**
     * @brief Retrieves a loaded @a `ColorPalette` asset which is mapped to the given relative filename
     *        string.
     * 
     * @param filename  The relative filename of the asset to retrieve.
     *  
     * @return  A shared pointer to the mapped @a `ColorPalette` asset. 
     */
    static Ref<ColorPalette> get (const String& filename);

    /**
     * @brief Un-maps the loaded @a `ColorPalette` asset which is mapped to the given relative filename 
     *        string.
     * 
     * @param filename  The relative filename of the asset to remove.
     *  
     * @return  A shared pointer to the removed @a `ColorPalette` asset. 
     */
    static Ref<ColorPalette> remove (const String& filename);

    /**
     * @brief Un-maps all loaded, currently-mapped @a `ColorPalette` assets.
     */
    static void clear ();
    
  private:
    /**
     * @brief The collection of currently loaded @a `ColorPalette` assets.
     */
    static Dictionary<Ref<ColorPalette>> s_assets;

  };

}
