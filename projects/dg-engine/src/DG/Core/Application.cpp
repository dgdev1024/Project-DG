/** @file DG/Core/Application.cpp */

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

      s_instance = this;

    } catch (std::exception& ex) {
      std::cerr << "Exception constructing application: " << ex.what() << std::endl;
      m_running = false;
    }
  }

  Application::~Application ()
  {
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

        // Determine if enough lag time has elapsed to perform a fixed update.
        while (lagTime >= m_timestep) {
          fixedUpdate();
          lagTime -= m_timestep;
        }

        // Perform the general application update.
        update();

      }

    } catch (std::exception& ex) {
      std::cerr << "Exception running application: " << ex.what() << std::endl;
      return 2;
    }

    return 0;
  }

  void Application::fixedUpdate ()
  {

  }

  void Application::update ()
  {

  }

}
