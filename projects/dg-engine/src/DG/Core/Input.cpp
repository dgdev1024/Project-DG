/** @file DG/Core/Input.cpp */

#include <DG/Core/Application.hpp>
#include <DG/Core/Input.hpp>

namespace dg
{

  #define DG_WINPTR Application::getWindow().getPointer()

  Bool Input::isKeyDown (const Key key)
  {
    return glfwGetKey(DG_WINPTR, static_cast<int>(key)) == GLFW_PRESS;
  }

  Vector2f Input::getCursorPos ()
  {
    double xpos = 0, ypos = 0;
    glfwGetCursorPos(DG_WINPTR, &xpos, &ypos);
    return {
      static_cast<Float32>(xpos),
      static_cast<Float32>(ypos)
    };
  }

  Vector2i Input::getCursorIntegerPos ()
  {
    double xpos = 0, ypos = 0;
    glfwGetCursorPos(DG_WINPTR, &xpos, &ypos);
    return {
      static_cast<Int32>(xpos),
      static_cast<Int32>(ypos)
    };
  }

  Bool Input::isMouseButtonDown (const MouseButton button)
  {
    return glfwGetMouseButton(DG_WINPTR, static_cast<int>(button)) == GLFW_PRESS;
  }

  Bool Input::isGamepadConnected (Index index)
  {
    return index <= GLFW_JOYSTICK_LAST &&
      glfwJoystickIsGamepad(index);
  }

  Bool Input::isGamepadButtonDown (const GamepadButton button, Index index)
  {
    GLFWgamepadstate state;

    if (isGamepadConnected(index) && glfwGetGamepadState(index, &state)) {
      return state.buttons[static_cast<int>(button)] == GLFW_PRESS;
    }

    return false;
  }

  Float32 Input::getGamepadAxis (const GamepadAxis axis, Index index)
  {
    GLFWgamepadstate state;

    if (isGamepadConnected(index) && glfwGetGamepadState(index, &state)) {
      return state.axes[static_cast<int>(axis)];
    }

    return 0.0f;
  }

}