/** @file DG/Graphics/Color.cpp */

#include <DG/Graphics/Color.hpp>

namespace dg
{

  const Color Color::Black = Color::fromInteger(0x000000FF);
  const Color Color::White = Color::fromInteger(0xFFFFFFFF);
  const Color Color::Red   = Color::fromInteger(0xFF0000FF);
  const Color Color::Green = Color::fromInteger(0x00FF00FF);
  const Color Color::Blue  = Color::fromInteger(0x0000FFFF);

  Color Color::fromIntegers (Int32 red, Int32 green, Int32 blue, Int32 alpha)
  {
    return {
      static_cast<Float32>(glm::clamp(red, 0, 255)) / 255.0f,
      static_cast<Float32>(glm::clamp(green, 0, 255)) / 255.0f,
      static_cast<Float32>(glm::clamp(blue, 0, 255)) / 255.0f,
      static_cast<Float32>(glm::clamp(alpha, 0, 255)) / 255.0f
    };
  }

  Color Color::fromInteger (Uint32 color)
  {
    return Color::fromIntegers(
      static_cast<Int32>((color >> 24) & 0xFF),
      static_cast<Int32>((color >> 16) & 0xFF),
      static_cast<Int32>((color >>  8) & 0xFF),
      static_cast<Int32>((color      ) & 0xFF)
    );
  }

}
