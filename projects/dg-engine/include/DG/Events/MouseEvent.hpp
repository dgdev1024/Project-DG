/** @file DG/Events/MouseEvents.hpp */

#pragma once

#include <DG/Events/Event.hpp>

namespace dg
{

  /**
   * @brief The @a `MouseButtonEvent` class is the base class for events emitted by interacting with
   *        the buttons on the mouse.
   */
  class MouseButtonEvent : public Event
  {
  protected:
    MouseButtonEvent (
      const Int32 button,
      const Int32 modifiers
    ) :
      Event {},
      m_button { button },
      m_modifiers { modifiers }
    {

    }

  public:
    /**
     * @brief Retrieves the integer code that describes the mouse button that was
     *        interacted with.
     *
     * @return The interacted mouse button's integer code.
     */
    inline Int32 getMouseButtonCode () const { return m_button; }

  protected:
    /**
     * @brief The integer code describing the mouse button that was interacted with.
     */
    Int32 m_button;

    /**
     * @brief A bitfield containing information on any modifier keys that were
     *        held down when the mouse button was interacted with.
     */
    Int32 m_modifiers;

  };

  /**
   * @brief The @a `MouseButtonPressEvent` event class is emitted whenever a mouse button is pressed 
   *        on the mouse.
   */
  class MouseButtonPressEvent : public MouseButtonEvent
  {
    DG_EVENT_IMPL(EventType::MouseButtonPress)

  public:
    MouseButtonPressEvent (
      const Int32 button,
      const Int32 modifiers
    ) :
      MouseButtonEvent { button, modifiers }
    {

    }

  };

  /**
   * @brief The @a `MouseButtonReleaseEvent` event class is emitted whenever a mouse button has been 
   *        released on the mouse.
   */
  class MouseButtonReleaseEvent : public MouseButtonEvent
  {
    DG_EVENT_IMPL(EventType::MouseButtonRelease)

  public:
    MouseButtonReleaseEvent (
      const Int32 button,
      const Int32 modifiers
    ) :
      MouseButtonEvent { button, modifiers }
    {

    }

  };

  /**
   * @brief The @a `MouseMoveEvent` event class is emitted whenever the mouse is moved within the
   *        bounds of the client application window.
   */
  class MouseMoveEvent : public Event
  {
    DG_EVENT_IMPL(EventType::MouseMove)

  public:
    MouseMoveEvent (
      const Float32 xPosition,
      const Float32 yPosition
    ) :
      Event {},
      m_position { xPosition, yPosition }
    {

    }

    MouseMoveEvent (
      const Float64 xPosition,
      const Float64 yPosition
    ) :
      Event {},
      m_position { 
        static_cast<Float32>(xPosition), 
        static_cast<Float32>(yPosition) 
      }
    {

    }

    /**
     * @brief Retrieves the position of the mouse at the time this event was
     *        emitted.
     *
     * @return The new position of the mouse cursor.
     */
    inline const glm::vec2& getPosition () const
    {
      return m_position;
    }

  private:
    /**
     * @brief Contains the position of the mouse at the time of this event, relative to the corner
     *        of the application window.
     */
    glm::vec2 m_position;

  };

  /**
   * @brief The @a `MouseEnterEvent` event class is emitted whenever the mouse cursor enters the
   *        bounds of the client application window.
   */
  class MouseEnterEvent : public Event
  {
    DG_EVENT_IMPL(EventType::MouseEnter)

  public:
    MouseEnterEvent () : Event {} {}

  };

  /**
   * @brief The @a `MouseLeaveEvent` event class is emitted whenever the mouse cursor leaves the
   *        bounds of the client application window.
   */
  class MouseLeaveEvent : public Event
  {
    DG_EVENT_IMPL(EventType::MouseLeave)

  public:
    MouseLeaveEvent () : Event {} {}
    
  };

  /**
   * @brief The @a `ScrollInputEvent` event class is emitted whenever a scrolling device - most
   *        commonly the mouse wheel - is used.
   */
  class ScrollInputEvent : public Event
  {
    DG_EVENT_IMPL(EventType::ScrollInput)

  public:
    ScrollInputEvent (
      const Float32 verticalOffset,
      const Float32 horizontalOffset
    ) :
      m_offset { horizontalOffset, verticalOffset }
    {

    }

    ScrollInputEvent (
      const Float64 verticalOffset,
      const Float64 horizontalOffset
    ) :
      m_offset { 
        static_cast<Float32>(horizontalOffset), 
        static_cast<Float32>(verticalOffset) 
      }
    {

    }

    /**
     * @brief Retrieves the offset magnitude of the scrolling input.
     * 
     * @return The offset magnitude of the scrolling input. 
     */
    inline const glm::vec2& getOffset () const
    {
      return m_offset;
    }

  private:
    /**
     * @brief The offset magnitude of the scrolling input.
     */
    glm::vec2 m_offset;

  };

}
