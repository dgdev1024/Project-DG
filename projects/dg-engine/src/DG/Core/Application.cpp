/** @file DG/Core/Application.cpp */

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

    } catch (std::exception& ex) {
      std::cerr << "Exception running application: " << ex.what() << std::endl;
      return 2;
    }

    return 0;
  }

}
