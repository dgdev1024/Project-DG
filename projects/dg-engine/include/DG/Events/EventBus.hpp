/** @file DG/Events/EventBus.hpp */

#pragma once

#include <DG/Events/EventListener.hpp>

namespace dg
{

  class EventBus
  {
  public:
    /**
     * @brief Initializes the @a `EventBus` by supplying an @a `EventListener` class to
     *        act as the top-level event listener.
     *
     * @param topLevelListener  A handle to an @a `EventListener` which shall act
     *                          as the top-level event listener.
     */
    inline static void initialize (
      EventListener& topLevelListener
    )
    {
      s_topLevelListener = &topLevelListener;
    }

    /**
     * @brief Polls the @a `EventBus`, send all emitted events to the top-level
     *        event listener.
     */
    inline static void poll ()
    {
      // Iterate over each event and propogate them downward.
      for (auto& ev : s_events) {
        s_topLevelListener->processEvent(*ev);
      }

      // Clear the event pool.
      s_events.clear();
    }

    /**
     * @brief Pushes a newly-emitted @a `Event` class of type @a `T` into the event
     *        bus, storing it until the next call to @a `poll`.
     *
     * @tparam T     The type of event which has been emitted. The 
     *               event needs to derive from the @a `Event` base
     *               class.
     * @tparam ...As The types of the arguments to be passed into the
     *               event class's constructor.
     *
     * @param ...args The arguments, if any, to be passed into the event class's
     *                constructor.
     */
    template <typename T, typename... As>
    inline static void pushEvent (As&&... args)
    {
      static_assert(std::is_base_of_v<Event, T>,
        "[EventBus::pushEvent] 'T' must derive from 'dg::Event'.");

      s_events.push_back(
        makeScope<T>(
          std::forward<As>(args)...
        )
      );
    }

  private:
    /**
     * @brief Points to an event listener class which acts as the top-level
     *        event listener. This will be the first event listener to receive
     *        events from the event bus.
     */
    static EventListener* s_topLevelListener;

    /**
     * @brief A collection of @a `Event` classes which have been emitted and need to be
     *        handled.
     */
    static Collection<Scope<Event>> s_events;

  };

}
