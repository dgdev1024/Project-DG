/** @file DG/Core/Window.cpp */

#include <DG/Events/EventListener.hpp>
#include <DG/Core/Window.hpp>

namespace dg
{

  namespace Private
  {
    
    /** Helper macro to quickly retrieve the GLFW window structure's user pointer. */
    #define DG_GET_WIN reinterpret_cast<Window*>(glfwGetWindowUserPointer(winptr))

    /**
     * @brief Indicates how many GLFW windows are currently active. This is used to indicate whether
     *        we need to initialize or shut down the GLFW subsystem.
     */
    static Count s_windowCount = 0;

    /**
     * @brief GLFW callback function called in the event of an error within the GLFW subsystem.
     * 
     * @param error       The error code provided by GLFW.
     * @param description A string describing what went wrong.
     */
    static void onError (int error, const char* description)
    {
      DG_ENGINE_ERROR("GLFW Error {}: {}", error, description);
    }

    /**
     * @brief GLFW callback function called when the window is closed.
     * 
     * @param winptr  Points to the @a `GLFWwindow` structure receiving the event. 
     */
    static void onWindowClose (GLFWwindow* winptr)
    {
      DG_GET_WIN->emitEvent<WindowCloseEvent>();
    }

    /**
     * @brief GLFW callback function called when a keyboard key is pressed or released.
     * 
     * @param winptr      Points to the @a `GLFWwindow` structure receiving the event.
     * @param key         An integer describing the key that was interacted with.
     * @param scan        The physical scancode of the key that was interacted with.
     * @param act         Indicates whether the key was pressed, released, or is being held down.
     * @param mods        A bitfield containing any modifier keys that were being held down.
     */
    static void onKey (GLFWwindow* winptr, int key, int scan, int act, int mods)
    {
      auto win = DG_GET_WIN;
      if (act == GLFW_PRESS)        { win->emitEvent<KeyPressEvent>(key, mods); }
      else if (act == GLFW_RELEASE) { win->emitEvent<KeyReleaseEvent>(key, mods); }
    }

    /**
     * @brief GLFW callback function called when Unicode text is input.
     * 
     * @param winptr    Points to the @a `GLFWwindow` structure receiving the event.
     * @param codepoint The codepoint of the Unicode character that was input.
     */
    static void onChar (GLFWwindow* winptr, unsigned int codepoint)
    {
      DG_GET_WIN->emitEvent<TextInputEvent>(codepoint);
    }

    /**
     * @brief GLFW callback function called when the mouse cursor is moved within the bounds of the
     *        given GLFW window.
     * 
     * @param winptrc
     * @param x       The mouse cursor's new X position.
     * @param y       The mouse cursor's new Y position.
     */
    static void onCursorPos (GLFWwindow* winptr, double x, double y)
    {
      DG_GET_WIN->emitEvent<MouseMoveEvent>(x, y);
    }

    /**
     * @brief GLFW callback function called when the mouse cursor either enters or leaves the bounds
     *        of the given GLFW window.
     * 
     * @param winptr  Points to the @a `GLFWwindow` structure receiving the event.  
     * @param entered Will be non-zero if the cursor entered the window, or zero if it leaves. 
     */
    static void onCursorEnter (GLFWwindow* winptr, int entered)
    {
      auto win = DG_GET_WIN;

      if (entered)  { win->emitEvent<MouseEnterEvent>(); }
      else          { win->emitEvent<MouseLeaveEvent>(); }
    }

    /**
     * @brief GLFW callback function called when a mouse button is either pressed or released.
     * 
     * @param winptr  Points to the @a `GLFWwindow` structure receiving the event.  
     * @param button  An integer describing the mouse button that was interacted with.
     * @param act     Indicates whether the mouse button was pressed or released.
     * @param mods    A bitfield containing any modifier keys that were held down.
     */
    static void onMouseButton (GLFWwindow* winptr, int button, int act, int mods)
    {
      auto win = DG_GET_WIN;

      if (act == GLFW_PRESS)  { win->emitEvent<MouseButtonPressEvent>(button, mods); }
      else                    { win->emitEvent<MouseButtonReleaseEvent>(button, mods); }
    }

    /**
     * @brief GLFW callback function called when a scrolling input device, like the mouse wheel, is
     *        scrolled.
     * 
     * @param winptr  Points to the @a `GLFWwindow` structure receiving the event.  
     * @param xoffset The horizontal scrolling offset. 
     * @param yoffset The vertical scrolling offset.
     */
    static void onScroll (GLFWwindow* winptr, double xoffset, double yoffset)
    {
      DG_GET_WIN->emitEvent<ScrollInputEvent>(yoffset, xoffset);
    }

  }

  Window::Window (const WindowSpecification& spec) :
    m_title         { spec.title },
    m_size          { spec.size },
    m_vertical_sync { spec.vertical_sync }
  {

    // If there are no windows currently open, then we can assume that GLFW has not been
    // initialized.
    if (Private::s_windowCount == 0) {

      // Initialize GLFW, then set the error callback.
      if (glfwInit() != GLFW_TRUE) {
        const char* description = nullptr;
        auto errorCode = glfwGetError(&description);

        DG_ENGINE_CRIT("Error initializing GLFW - {}: {}!", errorCode, description);
        throw std::runtime_error { "Error initializing GLFW!" };
      } else {
        glfwSetErrorCallback(Private::onError);
      }

      // Set the window hints.
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    }

    // Create and set up the window.
    m_glfwWindow = glfwCreateWindow(static_cast<int>(m_size.x), static_cast<int>(m_size.y),
      m_title.c_str(), nullptr, nullptr);
    if (m_glfwWindow == nullptr) {
      throw std::runtime_error { "Error creating GLFW window!" };
    } else {
      glfwMakeContextCurrent(m_glfwWindow);
      glfwSetWindowUserPointer(m_glfwWindow, this);
      glfwSwapInterval(m_vertical_sync == true ? 1 : 0);
    }

    // Set the window's input callbacks.
    glfwSetWindowCloseCallback(m_glfwWindow, Private::onWindowClose);
    glfwSetKeyCallback(m_glfwWindow, Private::onKey);
    glfwSetCharCallback(m_glfwWindow, Private::onChar);
    glfwSetCursorPosCallback(m_glfwWindow, Private::onCursorPos);
    glfwSetCursorEnterCallback(m_glfwWindow, Private::onCursorEnter);
    glfwSetMouseButtonCallback(m_glfwWindow, Private::onMouseButton);
    glfwSetScrollCallback(m_glfwWindow, Private::onScroll);

    // Increment the number of windows currently active.
    Private::s_windowCount++;
  }

  Window::~Window ()
  {
    // Destroy the window.
    if (m_glfwWindow != nullptr) {
      glfwSetWindowUserPointer(m_glfwWindow, nullptr);
      glfwDestroyWindow(m_glfwWindow);
      
      // Decrement the window count, then, if the new count is zero, shut down GLFW.
      if (--Private::s_windowCount == 0) {
        glfwTerminate();
      }
    }
  }

  Scope<Window> Window::make (const WindowSpecification& spec)
  {
    return makeScope<Window>(spec);
  }

  void Window::update () const
  {
    glfwPollEvents();
    glfwSwapBuffers(m_glfwWindow);
  }

  GLFWwindow* Window::getPointer () const
  {
    return m_glfwWindow;
  }

}
