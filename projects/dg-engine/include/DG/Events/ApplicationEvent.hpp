/** @file DG/Events/ApplicationEvent.hpp */

#pragma once

#include <DG/Events/Event.hpp>

namespace dg
{

  /**
   * @brief The @a `ApplicationFixedUpdateEvent` event class is emitted every time a fixed-timestep
   *        client application update is started.
   */
  class ApplicationFixedUpdateEvent : public Event
  {
    DG_EVENT_IMPL(EventType::ApplicationFixedUpdate)

  public:
    ApplicationFixedUpdateEvent (const Float32 timestep) :
      Event {},
      m_timestep { timestep }
    {

    }

    /**
     * @brief Retrieves the application's fixed timestep.
     * 
     * @return The application's fixed timestep. 
     */
    inline Float32 getTimestep () const
    {
      return m_timestep;
    }

  private:
    /**
     * @brief Contains the client application's fixed timestep.
     */
    Float32 m_timestep;

  };

  /**
   * @brief The @a `ApplicationUpdateEvent` event class is emitted every time a general client
   *        application update is started.
   */
  class ApplicationUpdateEvent : public Event
  {
    DG_EVENT_IMPL(EventType::ApplicationUpdate)

  public:
    ApplicationUpdateEvent () : Event {} {}

  };

}
