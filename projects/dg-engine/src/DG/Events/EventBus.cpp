/** @file DG/Events/EventBus.cpp */

#include <DG/Events/EventBus.hpp>

namespace dg
{

  EventListener* EventBus::s_topLevelListener = nullptr;
  Collection<Scope<Event>> EventBus::s_events;

}
