/** @file DG/Events/WindowEvent.hpp */

#pragma once

#include <DG/Events/Event.hpp>

namespace dg
{

  /**
   * @brief The @a `WindowCloseEvent` event class is emitted when the client application's window
   *        is closed.
   */
  class WindowCloseEvent : public Event
  {
    DG_EVENT_IMPL(EventType::WindowClose)

  public:
    WindowCloseEvent () : Event {} {}

  };

  /**
   * @brief The @a `WindowResizeEvent` event class is emitted whenever the client application's
   *        window is resized.
   */
  class WindowResizeEvent : public Event
  {
    DG_EVENT_IMPL(EventType::WindowResize)

  public:
    WindowResizeEvent (
      const Uint32 width, 
      const Uint32 height
    ) :
      Event {},
      m_size { width, height }
    {

    }

    /**
     * @brief Retrieves the new size of the application window.
     *
     * @return  The new size of the application window.
     */
    inline const glm::uvec2& getSize () const { return m_size; }

    /**
     * @brief Retrieves the new width of the application window.
     *
     * @return  The new width of the application window.
     */
    inline Uint32 getWidth () const { return m_size.x; }

    /**
     * @brief Retrieves the new height of the application window.
     *
     * @return  The new height of the application window.
     */
    inline Uint32 getHeight () const { return m_size.y; }

  private:
    /**
     * @brief Contains the client application window's new size.
     */
    glm::uvec2 m_size;

  };

}
