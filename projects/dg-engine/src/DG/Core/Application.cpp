/** @file DG/Core/Application.cpp */

#include <DG/Events/EventBus.hpp>
#include <DG/Core/Clock.hpp>
#include <DG/Core/Application.hpp>

namespace dg
{

  Application* Application::s_instance = nullptr;

  Application::Application (const ApplicationSpecification& spec) :
    m_running { true },
    m_timestep { 1.0f / spec.framerate }
  {
    try {

      if (s_instance != nullptr) {
        throw std::runtime_error { "Client application instance already exists!" };
      }

      EventBus::initialize(*this);            // Initialize the event bus.
      m_layerStack = makeScope<LayerStack>(); // Initialize the layer stack.

      s_instance = this;

    } catch (std::exception& ex) {
      DG_ENGINE_CRIT("Exception creating application instance: {}!", ex.what());
      m_running = false;
    }
  }

  Application::~Application ()
  {
    m_layerStack.reset();
    s_instance = nullptr;
  }

  Application& Application::get ()
  {
    if (s_instance == nullptr) {
      throw std::runtime_error { "Client application instance does not exist!" };
    }

    return *s_instance;
  }

  Result Application::start ()
  {
    // Early out with an error if something went wrong constructing the application.
    if (m_running == false) { return 1; }

    try {

      // Create the clock to be used for lag time accumulation.
      Clock lagClock;

      // Keep track of amount of lag time accumulated.
      Float32 lagTime = 0.0f;

      // Loop as long as the application continues running.
      while (m_running == true) {

        // Retrieve the amount of time elapsed since the previous frame and accumulate the lag time.
        Float32 elapsedTime = lagClock.restart();
        lagTime += elapsedTime;

        // Poll the event bus and handle any events which have been emitted.
        EventBus::poll();

        // Determine if enough lag time has elapsed to perform a fixed update.
        while (lagTime >= m_timestep) {
          fixedUpdate();
          lagTime -= m_timestep;
        }

        // Perform the general application update.
        update();

      }

    } catch (std::exception& ex) {
      DG_ENGINE_CRIT("Exception running application loop: {}!", ex.what());
      return 2;
    }

    return 0;
  }

  void Application::attachLayer (Layer& layer)
  {
    m_layerStack->attachLayer(layer);
  }

  void Application::detachLayer (Layer& layer)
  {
    m_layerStack->detachLayer(layer);
  }

  void Application::processEvent (Event& ev)
  {

    // Iterate backwards over the layer stack and have each layer handle the event.
    for (auto it = m_layerStack->rbegin(); it != m_layerStack->rend(); ++it) {

      (*it)->processEvent(ev);

    }

    // Respond to a window close event by quitting the application.
    handleEvent<WindowCloseEvent>(ev, [&] (WindowCloseEvent& ev) {
      m_running = false;
      return true;
    });

  }

  void Application::fixedUpdate ()
  {

    // Iterate over the layer stack and run each layer's fixed update routine.
    for (auto layer : *m_layerStack) {
      layer->fixedUpdate(m_timestep);
    }

  }

  void Application::update ()
  {

    // Iterate over the layer stack and run each layer's update routine.
    for (auto layer : *m_layerStack) {
      layer->update();
    }

  }

}
