/** @file DG/Events/EventListener.hpp */

#pragma once

#include <DG/Events/ApplicationEvent.hpp>
#include <DG/Events/WindowEvent.hpp>
#include <DG/Events/KeyboardEvent.hpp>
#include <DG/Events/MouseEvent.hpp>

namespace dg
{

  /**
   * @brief An alias for a @a `Function` that is called in order to handle an emitted event.
   * @tparam T The type of the event being handled.
   */
  template <typename T>
  using EventHandler = Function<Bool, T&>;

  /**
   * @brief The @a `EventListener` class provides an interface for classes which can listen for and
   *        handle events.
   */
  class EventListener
  {
  public:
    /**
     * @brief Processes an @a `Event` polled by the event bus, checking to see if
     *        that @a `Event` needs to be handled by this @a `EventListener`.
     *
     * @param ev  A handle to the @a `Event` which may need to be handled.
     */
    virtual void processEvent (Event&) = 0;

  protected:
    /**
     * @brief Handles an @a `Event` of type @a `T` by calling the given handler
     *        function, provided the @a `Event`'s type matches the given type.
     *
     * @tparam T The type of the event which needs to be handled. The
     *           type needs to derive from the @a `Event` base class.
     *
     * @param ev      A handle to the @a `Event` which needs to be handled.
     * @param handler A function to call in order to handle the @a `Event`.
     */
    template <typename T>
    inline void handleEvent (
      Event& ev,
      const EventHandler<T>& handler
    )
    {
      static_assert(std::is_base_of_v<Event, T>,
        "[EventListener::handleEvent] 'T' must derive from 'dg::Event'.");

      if (
        ev.getType() == T::getStaticType() &&
        ev.m_handled == false &&
        handler != nullptr
      ) {
        ev.m_handled = handler(static_cast<T&>(ev));
      }
    }
  };

}
