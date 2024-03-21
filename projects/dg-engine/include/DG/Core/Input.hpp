/** @file DG/Core/Input.hpp */

#pragma once

#include <DG/Core/InputCodes.hpp>

namespace dg
{

  /**
   * @brief The @a `Input` class is a static helper class used for dynamically detecting user input.
   */
  class Input
  {
  public:
    /**
     * @brief Checks to see if the given @a `Key` is currently being held down.
     * 
     * @param key The @a `Key` to check for.
     *  
     * @return  @a `true` if the given @a `Key` is currently down; @a `false` otherwise. 
     */
    static Bool isKeyDown (const Key key);

    /**
     * @brief Retrieves the current position of the mouse, relative to the corner of the application
     *        window.
     * 
     * @return The current, relative mouse position. 
     */
    static Vector2f getCursorPos ();

    /**
     * @brief Retrieves the current integer position of the mouse, relative to the corner of the
     *        application window.
     * 
     * @return The current, relative mouse position, as an integer vector. 
     */
    static Vector2i getCursorIntegerPos ();

    /**
     * @brief Retrieves whether or not the given @a `MouseButton` is currently being held down.
     * 
     * @param button  The @a `MouseButton` to check for.
     *  
     * @return  @a `true` if the given @a `MouseButton` is currently down; @a `false` otherwise. 
     */
    static Bool isMouseButtonDown (const MouseButton button);

    /**
     * @brief Checks to see if a gamepad is connected at the given index.
     * 
     * @param index The index to check.
     *  
     * @return  @a `true` if @a `index` is valid and a gamepad is connected at that index;
     *          @a `false` otherwise. 
     */
    static Bool isGamepadConnected (Index index = 0);

    /**
     * @brief Checks to see if a @a `GamepadButton` is down on a gamepad connected at the given
     *        index.
     * 
     * @param button  The @a `GamepadButton` to check.
     * @param index   The index of the connected gamepad to check.
     * 
     * @return  @a `true` if @a `index` is valid, a gamepad is connected at that index, and the
     *          given @a `GamepadButton` on that gamepad is currently down; @a `false` otherwise. 
     */
    static Bool isGamepadButtonDown (const GamepadButton button, Index index = 0);

    /**
     * @brief Retrieves the value of the given @a `GamepadAxis` on a gamepad connected at the given
     *        index.
     * 
     * @param axis  The @a `GamepadAxis` to check. 
     * @param index The index of the connected gamepad to check.
     * 
     * @return  The axis value (a value between @a `-1.0f` and @a `1.0f`) if @a `index` is valid and
     *          a gamepad is connected at that index; @a `0.0f` otherwise. 
     */
    static Float32 getGamepadAxis (const GamepadAxis axis, Index index = 0);

  };

}
