/** @file DG/Common.hpp */

#pragma once

// C++ Includes
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <random>
#include <chrono>
#include <filesystem>
#include <functional>

// C Includes
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstdint>
#include <cctype>

// Filesystem Namespace
namespace fs = std::filesystem;

namespace dg
{

  // Typedefs
  using Sint8       = std::int8_t;
  using Sint16      = std::int16_t;
  using Sint32      = std::int32_t;
  using Sint64      = std::int64_t;
  using Uint8       = std::uint8_t;
  using Uint16      = std::uint16_t;
  using Uint32      = std::uint32_t;
  using Uint64      = std::uint64_t;
  using Float32     = float;
  using Float64     = double;
  using String      = std::string;
  using StringView  = std::string_view;
  using Path        = fs::path;
  using Char        = char;
  using Bool        = bool;
  using Result      = int;

  // Templated Typedefs
  template <typename T>                 using Scope       = std::unique_ptr<T>;
  template <typename T>                 using Ref         = std::shared_ptr<T>;
  template <typename T>                 using Collection  = std::vector<T>;
  template <typename T, typename U>     using Map         = std::unordered_map<T, U>;
  template <typename T>                 using Dictionary  = Map<String, T>;
  template <typename T, typename... Us> using Function    = std::function<T(Us&&...)>;
  template <typename T, typename... Us> using LFunction   = std::function<T(Us...)>;
  template <typename T>                 using InitList    = std::initializer_list<T>;

  // Helper Functions

  /**
   * @brief Creates a unique smart pointer to an object of type @a `T` with the given arguments.
   * 
   * @tparam T      The type of the object being pointed to.
   * @tparam ...Us  The type(s) of the argument(s) passed into @a `T`'s constructor.
   * 
   * @param ...args The argument(s) passed into @a `T`'s constructor.
   * 
   * @return  The newly-created unique smart pointer.
   */
  template <typename T, typename... Us>
  inline Scope<T> makeScope (Us&&... args)
  {
    return std::make_unique<T>(std::forward<Us>(args)...);
  }

  /**
   * @brief Creates a shared smart pointer to an object of type @a `T` with the given arguments.
   * 
   * @tparam T      The type of the object being pointed to.
   * @tparam ...Us  The type(s) of the argument(s) passed into @a `T`'s constructor.
   * 
   * @param ...args The argument(s) passed into @a `T`'s constructor.
   * 
   * @return  The newly-created shared smart pointer.
   */
  template <typename T, typename... Us>
  inline Ref<T> makeRef (Us&&... args)
  {
    return std::make_shared<T>(std::forward<Us>(args)...);
  }

}
