/** @file DG/Core/String.hpp */

#pragma once

#include <DG/Common.hpp>

namespace dg
{

  /**
   * @brief Streams a formatted string to the given output stream. This overload is used if no 
   *        variadic arguments are provided.
   *
   * @param stream  A handle to the output stream to stream the formatted string to.
   * @param format  The format string.
   */
  inline void streamFormatted (
    std::ostream& stream,
    const char* format
  )
  {
    stream << format;
  }

  /**
   * @brief Streams a formatted string to the given output stream.
   *
   * @tparam T The type of the first provided variadic argument.
   * @tparam ...Ts The types of the remaining variadic arguments provided.
   *
   * @param stream    A handle to the output stream to stream the formatted string to.
   * @param format    The format string to use. Instances of '{}' will be replaced with the given, 
   *                  subsequent variadic arguments.
   * @param first     The first variadic argument.
   * @param ...other  The remaining variadic arguments, if any.
   */
  template <typename T, typename... Ts>
  inline void streamFormatted (
    std::ostream& stream,
    const char* format,
    T first,
    Ts... other
  )
  {
    // Iterate until a null terminator is found.
    for ( ; *format != '\0'; ++format) {
      if (*format == '{' && *(format + 1) == '}') {
        stream << first;
        streamFormatted(stream, format + 2, other...);
        return;
      }

      stream << *format;
    }
  }

  /**
   * @brief Builds a formatted string using the given format string and variadic arguments.
   *
   * @tparam ...Ts The types of the variadic arguments provided.
   *
   * @param format  The format string.
   * @param ...args The variadic arguments, if any are provided.
   *
   * @return  The formatted string.
   */
  template <typename... Ts>
  inline String formatString (
    const char* format, 
    Ts... args
  )
  {
    std::stringstream stream;
    streamFormatted(stream, format, args...);
    return stream.str();
  }

}
