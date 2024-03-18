/** @file DG/Core/Application.hpp */

#pragma once

#include <DG_Pch.hpp>

namespace dg
{

  /**
   * @brief The @a `ApplicationSpecification` structure describes the attributes defining the
   *        client application and its components.
   */
  struct ApplicationSpecification
  {
    /**
     * @brief The application's framerate. This dictates how often the application's fixed update
     *        is to be run.
     */
    Float32 framerate = 60.0f;
  };

  /**
   * @brief The @a `Application` class is the base class for the engine's client application.
   */
  class Application
  {
  protected:

    /**
     * @brief Constructs the @a `Application` with the given specification.
     * 
     * @param spec  The application's specification.
     */
    Application (const ApplicationSpecification& spec);

  public:
    virtual ~Application ();

    /**
     * @brief Retrieves the singleton @a `Application` instance.
     * 
     * @return  A handle to the singleton application instance.
     * 
     * @throw   @a `std::runtime_error` if the application instance was not yet created.
     */
    static Application& get ();

    /**
     * @brief Starts the client application's loop.
     * 
     * @return  @a `0` if the application loop exits with no errors;
     *          A non-zero value if the application loop exits due to an error. 
     */
    Result start ();

  protected:

    /**
     * @brief A pointer to the singleton @a `Application` instance.
     */
    static Application* s_instance;

    /**
     * @brief Indicates whether or not the application should continue running.
     */
    Bool m_running;

    /**
     * @brief The application's fixed timestep, which controls how often the @a `fixedUpdate`
     *        method is run.
     */
    Float32 m_timestep;

  };

}