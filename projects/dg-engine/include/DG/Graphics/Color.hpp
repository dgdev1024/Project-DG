/** @file DG/Graphics/Color.hpp */

#pragma once

#include <DG_Pch.hpp>

namespace dg
{

  /**
   * @brief The @a `Color` struct describes a color with red, green, blue and alpha components.
   */
  struct Color
  {
    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;

    Float32 red   = 0.0f;
    Float32 green = 0.0f;
    Float32 blue  = 0.0f;
    Float32 alpha = 1.0f;

    /**
     * @brief Creates a new @a `Color` from the given integer color components.
     * 
     * @param red   The new color's red component.
     * @param green The new color's green component.
     * @param blue  The new color's blue component.
     * @param alpha The new color's alpha component.
     * 
     * @return  The newly-created @a `Color`. 
     */
    static Color fromIntegers (Int32 red, Int32 green, Int32 blue, Int32 alpha = 255);

    /**
     * @brief Creates a new @a `Color` from the bytes of the given integer.
     * 
     * @param color The integer containing the color component bytes.
     * 
     * @return  The newly-created @a `Color`. 
     */
    static Color fromInteger (Uint32 color);

  };

}
