/** @file DG/Events/KeyboardEvents.hpp */

#pragma once

#include <DG/Events/Event.hpp>

namespace dg
{

  /**
   * @brief The @a `KeyEvent` class is the base class for events emitted by interacting with the
   *        keyboard.
   */
  class KeyEvent : public Event
  {
  protected:
    KeyEvent (
      const Int32 keycode,
      const Int32 modifiers
    ) :
      Event {},
      m_keycode { keycode },
      m_modifiers { modifiers }
    {

    }

  public:
    /**
     * @brief Retrieves the integer code that describes the key that was
     *        interacted with.
     *
     * @return The interacted key's integer code.
     */
    inline Int32 getKeyCode () const { return m_keycode; }

  protected:
    /**
     * @brief The integer code describing the key that was interacted with.
     */
    Int32 m_keycode;

    /**
     * @brief A bitfield containing information on any modifier keys that were
     *        held down when the key was interacted with.
     */
    Int32 m_modifiers;

  };

  /**
   * @brief The @a `KeyPressEvent` event class is emitted whenever a key is pressed on the keyboard.
   */
  class KeyPressEvent : public KeyEvent
  {
    DG_EVENT_IMPL(EventType::KeyPress)

  public:
    KeyPressEvent (
      const Int32 keycode,
      const Int32 modifiers
    ) :
      KeyEvent { keycode, modifiers }
    {

    }

  };

  /**
   * @brief The @a `KeyReleaseEvent` event class is emitted whenever a key has been released on the
   *        keyboard.
   */
  class KeyReleaseEvent : public KeyEvent
  {
    DG_EVENT_IMPL(EventType::KeyRelease)

  public:
    KeyReleaseEvent (
      const Int32 keycode,
      const Int32 modifiers
    ) :
      KeyEvent { keycode, modifiers }
    {

    }

  };

  /**
   * @brief Event emitted when a Unicode character is input.
   */
  class TextInputEvent : public Event
  {
    DG_EVENT_IMPL(EventType::TextInput)

  public:
    TextInputEvent (
      Uint32 codepoint
    ) :
      Event {},
      m_codepoint { codepoint }
    {

    }

    /**
     * @brief Retrieves the codepoint of the Unicode character that was input.
     * 
     * @return  The Unicode codepoint. 
     */
    inline Uint32 getCodepoint () const { return m_codepoint; }

  private:
    /**
     * @brief The codepoint of the Unicode character that was input.
     */
    Uint32 m_codepoint = 0;

  };

}
