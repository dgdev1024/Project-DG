/** @file DG/Core/Application.hpp */

#pragma once

#include <DG_Pch.hpp>
#include <DG/Core/LayerStack.hpp>
#include <DG/Core/Window.hpp>
#include <DG/Events/EventListener.hpp>

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

    /**
     * @brief The application window's specification.
     */
    WindowSpecification windowSpec;

  };

  /**
   * @brief The @a `Application` class is the base class for the engine's client application.
   */
  class Application : public EventListener
  {
  protected:

    /**
     * @brief Constructs the @a `Application` with the given specification.
     * 
     * @param spec  The application's specification.
     * 
     * @throw   @a `std::runtime_error` if the application instance was already created.
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

    /**
     * @brief Attaches the given @a `Layer` to the application's @a `LayerStack`.
     * 
     * @param layer   A handle to the @a `Layer` to attach.
     */
    void attachLayer (Layer& layer);

    /**
     * @brief Detaches the given @a `Layer` from the application's @a `LayerStack`, if it is 
     *        attached.
     * 
     * @param layer   A handle to the @a `Layer` to detach.
     */
    void detachLayer (Layer& layer);

  protected:

    /**
     * @brief Processes and handles events which need to be handled by the application.
     * 
     * @param ev  Refers to an @a `Event` instance which was emitted. 
     */
    void processEvent (Event& ev);

    /**
     * @brief Performs application updates which rely on the application's fixed timestep.
     */
    void fixedUpdate ();

    /**
     * @brief Performs application updates which do not rely on a fixed timestep.
     */
    void update ();

  protected:

    /**
     * @brief A pointer to the singleton @a `Application` instance.
     */
    static Application* s_instance;

    /**
     * @brief Points to the client application's @a `LayerStack`.
     */
    Scope<LayerStack> m_layerStack = nullptr;

    /**
     * @brief Points to the client application's @a `Window`.
     */
    Scope<Window> m_window = nullptr;

    /**
     * @brief Indicates whether or not the application should continue running.
     */
    Bool m_running = true;

    /**
     * @brief The application's fixed timestep, which controls how often the @a `fixedUpdate`
     *        method is run.
     */
    Float32 m_timestep = 1.0f / 60.0f;

  };

}
