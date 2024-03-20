/** @file DG/Core/Window.hpp */

#pragma once

#include <DG_Pch.hpp>
#include <DG/Events/EventEmitter.hpp>

namespace dg
{

  /**
   * @brief The @a `WindowSpecification` structure describes attributes which define the client
   *        application's @a `Window`.
   */
  struct WindowSpecification
  {

    /**
     * @brief Contains the text which appears in the @a `Window`'s title bar.
     */
    String title = "DG Engine Application";

    /**
     * @brief Contains the width and height of the @a `Window`.
     */
    Vector2u size = { 1280, 720 };

    /**
     * @brief Indicates whether or not the application's framerate should be locked to the monitor's
     *        refresh rate.
     */
    Bool vertical_sync = true;

  };

  /**
   * @brief The @a `Window` class is responsible for presenting the client application's window and
   *        emitting input events raised by the keyboard and mouse.
   */
  class Window : public EventEmitter
  {
  public:
    /**
     * @brief Constructs a @a `Window` with the given specification.
     * 
     * @param spec  A handle to the window's specification. 
     */
    Window (const WindowSpecification& spec);
    ~Window ();

    /**
     * @brief Makes a new @a `Window` with the given specification.
     * 
     * @param spec  A handle to the window's specification.
     *  
     * @return  A unique pointer to the newly-created @a `Window`. 
     */
    static Scope<Window> make (const WindowSpecification& spec = {});

    /**
     * @brief Updates the current state of the @a `Window` and polls events from it.
     */
    void update () const;

    /**
     * @brief Retrieves the @a `Window`'s underlying @a `GLFWwindow` structure.
     * 
     * @return  A pointer to the underlying @a `GLFWwindow` structure. 
     */
    GLFWwindow* getPointer () const;

  private:

    /**
     * @brief A pointer to the @a `Window`'s underlying @a `GLFWwindow` structure.
     */
    GLFWwindow* m_glfwWindow = nullptr;

    /**
     * @brief Contains the text which appears in the @a `Window`'s title bar.
     */
    String m_title = "";

    /**
     * @brief Contains the width and height of the @a `Window`, in pixels.
     */
    Vector2u m_size;

    /**
     * @brief Indicates whether or not the application's framerate should be locked to the monitor's
     *        refresh rate.
     */
    Bool m_vertical_sync;

  };

}
