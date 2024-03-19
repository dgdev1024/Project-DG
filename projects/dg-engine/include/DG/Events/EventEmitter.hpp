/** @file DG/Events/EventEmitter.hpp */

#pragma once

#include <DG/Events/Event.hpp>
#include <DG/Events/EventBus.hpp>

namespace dg
{

  class EventEmitter
  {
  public:
    /**
     * @brief Emits an @a `Event` class of type @a `T`, sending it to the @a `EventBus` to
     *        be constructed.
     *
     * @tparam T     The type of @a `Event` class which has been emitted. The 
     *               event class needs to derive from the @a `Event` base class.
     * @tparam ...As The types of the arguments to be passed into the
     *               @a `Event` class's constructor.
     *
     * @param ...args The arguments, if any, to be passed into the @a `Event` class's
     *                constructor.
     */
    template <typename T, typename... As>
    inline void emitEvent (
      As&&... args
    )
    {
      static_assert(std::is_base_of_v<Event, T>,
        "[EventEmitter::emitEvent] 'T' must derive from 'dg::Event'.");

      EventBus::pushEvent<T>(
        std::forward<As>(args)...
      );
    }

  };

}
