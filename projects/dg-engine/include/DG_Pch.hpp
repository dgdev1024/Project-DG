/** @file DG_Pch.hpp */

#ifndef DG_PCH_HPP
#define DG_PCH_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <DG/Common.hpp>
#include <DG/Core/Logging.hpp>

namespace dg
{

  // Since GLM is included in this precompiled header file, we will add the GLM typedefs here.

  using Vector2f = glm::vec2;
  using Vector2d = glm::dvec2;
  using Vector2i = glm::ivec2;
  using Vector2u = glm::uvec2;
  using Vector2b = glm::bvec2;

  using Vector3f = glm::vec3;
  using Vector3d = glm::dvec3;
  using Vector3i = glm::ivec3;
  using Vector3u = glm::uvec3;
  using Vector3b = glm::bvec3;

  using Vector4f = glm::vec4;
  using Vector4d = glm::dvec4;
  using Vector4i = glm::ivec4;
  using Vector4u = glm::uvec4;
  using Vector4b = glm::bvec4;

  using Matrix2f = glm::mat2;
  using Matrix2d = glm::dmat2;

  using Matrix3f = glm::mat3;
  using Matrix3d = glm::dmat3;

  using Matrix4f = glm::mat4;
  using Matrix4d = glm::dmat4;

}

#endif
