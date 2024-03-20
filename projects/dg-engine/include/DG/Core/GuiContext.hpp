/** @file DG/Core/GuiContext.hpp */

#pragma once

#include <DG_Pch.hpp>

namespace dg
{

  /**
   * @brief The @a `GuiContextSpecification` struct describes attributes defining the application's
   *        graphical-user-interface features.
   */
  struct GuiContextSpecification
  {

    /**
     * @brief Indicates whether the client application should have GUI features.
     */
    Bool enabled = false;

    /**
     * @brief Indicates whether or not the client application should have GUI docking features.
     */
    Bool docking = false;

    /**
     * @brief Indicates whether or not the client application should have GUI viewport features.
     */
    Bool viewport = false;

  };

  /**
   * @brief The @a `GuiContext` class allows the client application to interface with a graphical
   *        user interface context powered by Dear ImGui.
   */
  class GuiContext
  {
  public:
    GuiContext (const GuiContextSpecification& spec);
    ~GuiContext ();

    /**
     * @brief Creates a new GUI context for the client application, if requested.
     *
     * @param spec  The GUI context's specification.
     *
     * @return  A smart pointer to the newly-created @a `GuiContext` if requested;
     *          @a `nullptr` otherwise.
     */
    static Scope<GuiContext> make (const GuiContextSpecification& spec = {});

    /**
     * @brief Begins rendering and updating the GUI context.
     */
    void begin () const;

    /**
     * @brief Ends rendering and updating the GUI context.
     */
    void end () const;

  private:
    Bool m_docking = false;
    Bool m_viewport = false;

  };

}
