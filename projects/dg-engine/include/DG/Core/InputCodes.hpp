/** @file DG/Core/InputCodes.hpp */

#pragma once

#include <DG_Pch.hpp>

namespace dg
{

  /**
   * @brief The @a `Key` enum enumerates the different keys that can be recorded by the
   *        @a `KeyPressEvent` and @a `KeyReleaseEvent` event classes.
   */
  enum class Key
  {
    Unknown = GLFW_KEY_UNKNOWN,
    Space = GLFW_KEY_SPACE,
    Apostrophe = GLFW_KEY_APOSTROPHE,
    Comma = GLFW_KEY_COMMA,
    Minus = GLFW_KEY_MINUS,
    Period = GLFW_KEY_PERIOD,
    Slash = GLFW_KEY_SLASH,
    Number0 = GLFW_KEY_0,
    Number1 = GLFW_KEY_1,
    Number2 = GLFW_KEY_2,
    Number3 = GLFW_KEY_3,
    Number4 = GLFW_KEY_4,
    Number5 = GLFW_KEY_5,
    Number6 = GLFW_KEY_6,
    Number7 = GLFW_KEY_7,
    Number8 = GLFW_KEY_8,
    Number9 = GLFW_KEY_9,
    Semicolon = GLFW_KEY_SEMICOLON,
    Equal = GLFW_KEY_EQUAL,
    A = GLFW_KEY_A,
    B = GLFW_KEY_B,
    C = GLFW_KEY_C,
    D = GLFW_KEY_D,
    E = GLFW_KEY_E,
    F = GLFW_KEY_F,
    G = GLFW_KEY_G,
    H = GLFW_KEY_H,
    I = GLFW_KEY_I,
    J = GLFW_KEY_J,
    K = GLFW_KEY_K,
    L = GLFW_KEY_L,
    M = GLFW_KEY_M,
    N = GLFW_KEY_N,
    O = GLFW_KEY_O,
    P = GLFW_KEY_P,
    Q = GLFW_KEY_Q,
    R = GLFW_KEY_R,
    S = GLFW_KEY_S,
    T = GLFW_KEY_T,
    U = GLFW_KEY_U,
    V = GLFW_KEY_V,
    W = GLFW_KEY_W,
    X = GLFW_KEY_X,
    Y = GLFW_KEY_Y,
    Z = GLFW_KEY_Z,
    Left_Bracket = GLFW_KEY_LEFT_BRACKET,
    Backslash = GLFW_KEY_BACKSLASH,
    Right_Bracket = GLFW_KEY_RIGHT_BRACKET,
    Grave_Accent = GLFW_KEY_GRAVE_ACCENT,
    World_1 = GLFW_KEY_WORLD_1,
    World_2 = GLFW_KEY_WORLD_2,
    Escape = GLFW_KEY_ESCAPE,
    Enter = GLFW_KEY_ENTER,
    Tab = GLFW_KEY_TAB,
    Backspace = GLFW_KEY_BACKSPACE,
    Insert = GLFW_KEY_INSERT,
    Delete = GLFW_KEY_DELETE,
    Right = GLFW_KEY_RIGHT,
    Left = GLFW_KEY_LEFT,
    Down = GLFW_KEY_DOWN,
    Up = GLFW_KEY_UP,
    Page_Up = GLFW_KEY_PAGE_UP,
    Page_Down = GLFW_KEY_PAGE_DOWN,
    Home = GLFW_KEY_HOME,
    End = GLFW_KEY_END,
    Caps_Lock = GLFW_KEY_CAPS_LOCK,
    Scroll_Lock = GLFW_KEY_SCROLL_LOCK,
    Num_Lock = GLFW_KEY_NUM_LOCK,
    Print_Screen = GLFW_KEY_PRINT_SCREEN,
    Pause = GLFW_KEY_PAUSE,
    F1 = GLFW_KEY_F1,
    F2 = GLFW_KEY_F2,
    F3 = GLFW_KEY_F3,
    F4 = GLFW_KEY_F4,
    F5 = GLFW_KEY_F5,
    F6 = GLFW_KEY_F6,
    F7 = GLFW_KEY_F7,
    F8 = GLFW_KEY_F8,
    F9 = GLFW_KEY_F9,
    F10 = GLFW_KEY_F10,
    F11 = GLFW_KEY_F11,
    F12 = GLFW_KEY_F12,
    F13 = GLFW_KEY_F13,
    F14 = GLFW_KEY_F14,
    F15 = GLFW_KEY_F15,
    F16 = GLFW_KEY_F16,
    F17 = GLFW_KEY_F17,
    F18 = GLFW_KEY_F18,
    F19 = GLFW_KEY_F19,
    F20 = GLFW_KEY_F20,
    F21 = GLFW_KEY_F21,
    F22 = GLFW_KEY_F22,
    F23 = GLFW_KEY_F23,
    F24 = GLFW_KEY_F24,
    F25 = GLFW_KEY_F25,
    Keypad0 = GLFW_KEY_KP_0,
    Keypad1 = GLFW_KEY_KP_1,
    Keypad2 = GLFW_KEY_KP_2,
    Keypad3 = GLFW_KEY_KP_3,
    Keypad4 = GLFW_KEY_KP_4,
    Keypad5 = GLFW_KEY_KP_5,
    Keypad6 = GLFW_KEY_KP_6,
    Keypad7 = GLFW_KEY_KP_7,
    Keypad8 = GLFW_KEY_KP_8,
    Keypad9 = GLFW_KEY_KP_9,
    KeypadDecimal = GLFW_KEY_KP_DECIMAL,
    KeypadDivide = GLFW_KEY_KP_DIVIDE,
    KeypadMultiply = GLFW_KEY_KP_MULTIPLY,
    KeypadSubtract = GLFW_KEY_KP_SUBTRACT,
    KeypadAdd = GLFW_KEY_KP_ADD,
    KeypadEnter = GLFW_KEY_KP_ENTER,
    KeypadEqual = GLFW_KEY_KP_EQUAL,
    LeftShift = GLFW_KEY_LEFT_SHIFT,
    LeftControl = GLFW_KEY_LEFT_CONTROL,
    LeftAlt = GLFW_KEY_LEFT_ALT,
    LeftSuper = GLFW_KEY_LEFT_SUPER,
    RightShift = GLFW_KEY_RIGHT_SHIFT,
    RightControl = GLFW_KEY_RIGHT_CONTROL,
    RightAlt = GLFW_KEY_RIGHT_ALT,
    RightSuper = GLFW_KEY_RIGHT_SUPER,
    Menu = GLFW_KEY_MENU
  };

  /**
   * @brief The @a `KeyModifiers` enum enumerates the modifier keys which can be held down to modify
   *        the response to interacting with the keys and mouse buttons.
   */
  enum class KeyModifier
  {
    None = 0,
    Control = GLFW_MOD_CONTROL,
    Alt = GLFW_MOD_ALT,
    Shift = GLFW_MOD_SHIFT,
    Super = GLFW_MOD_SUPER,
    CapsLock = GLFW_MOD_CAPS_LOCK,
    NumLock = GLFW_MOD_NUM_LOCK,

    Ctrl = Control,
    Windows = Super
  };

  /**
   * @brief The @a `MouseButton` enum enumerates the different mouse buttons that can be recorded
   *        by the @a `MouseButtonEvent` event classes.
   */
  enum class MouseButton
  {
    Button1 = GLFW_MOUSE_BUTTON_1,
    Button2,
    Button3,
    Button4,
    Button5,
    Button6,
    Button7,
    Button8,

    Left = Button1,
    Right = Button2,
    Middle = Button3
  };

  /**
   * @brief The @a `GamepadButton` enum enumerates the different buttons which can be interacted
   *        with on a standard gamepad joystick.
   */
  enum class GamepadButton
  {
    A = GLFW_GAMEPAD_BUTTON_A,
    B,
    X,
    Y,
    LeftBumper,
    RightBumper,
    Back,
    Start,
    Guide,
    LeftThumbstick,
    RightThumbstick,
    Up,
    Right,
    Down,
    Left,

    Cross = A,
    Circle = B,
    Square = X,
    Triangle = Y
  };

  /**
   * @brief The @a `GamepadAxis` enum enumerates each of the axes and directions which can be
   *        affected by the sticks and triggers on a standard gamepad joystick.
   */
  enum class GamepadAxis
  {
    LeftX = GLFW_GAMEPAD_AXIS_LEFT_X,
    LeftY,
    RightX,
    RightY,
    LeftTrigger,
    RightTrigger
  };

}
