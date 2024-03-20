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
     * @brief Constructs a @a `ColorPalette` with color data loaded from the given file.
     * 
     * @param filepath  The path to the palette file to load. 
     */
    ColorPalette (const Path& filepath);

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

}
