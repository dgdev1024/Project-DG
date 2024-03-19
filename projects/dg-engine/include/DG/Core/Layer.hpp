/** @file DG/Core/Layer.hpp */

#pragma once

#include <DG/Events/EventListener.hpp>

namespace dg
{

  /**
   * @brief The @a `Layer` class is the base class for the client application's
   * layers.
   */
  class Layer : public EventListener
  {
  protected:
    /**
     * @brief Constructs a new @a `Layer` with the given settings.
     * 
     * @param name    The name of the layer. It cannot be blank.
     * @param overlay Is this layer an overlay?
     *
     * @throw @a `std::invalid_argument` if the @a `name` string is blank.
     */
    Layer (const String& name, Bool overlay = false) :
      m_name { name },
      m_overlay { overlay }
    {
      if (m_name.empty()) {
        throw std::invalid_argument { "Attempted construction of 'Layer' with blank name string!" };
      }
    }

  public:
    virtual ~Layer () = default;

    /**
     * @brief This function is called when the @a `Layer` is attached to the @a `LayerStack`.
     */
    virtual void onAttach () {}

    /**
     * @brief This function is called when the @a `Layer` is detached from the @a `LayerStack`,
     *        or if the @a `LayerStack` is destroyed at the end of the client application's 
     *        runtime.
     */
    virtual void onDetach () {}

    /**
     * @brief Performs fixed-timestep update operations on this @a `Layer`.
     * 
     * @param timestep  The application's fixed timestep.
     */
    virtual void fixedUpdate (const Float32) {}

    /**
     * @brief Performs general update operations on this @a `Layer`.
     */
    virtual void update () {}

    /**
     * @brief Performs graphical user interaface updates on this @a `Layer`.
     */
    virtual void guiUpdate () {}

    /**
     * @brief Retrieves the name of this @a `Layer`, for debugging purposes.
     * 
     * @return The name of this layer.
     */
    inline const String& getName () const { return m_name; }

    /**
     * @brief Retrieves whether or not this @a `Layer` is an overlay layer. Overlay layers take
     *        priority over other layers in the @a `LayerStack`.
     * 
     * @return @a `true` if this is an overlay layer; @a `false` otherwise.
     */
    inline Bool isOverlay () const { return m_overlay; }

  private:
    /**
     * @brief The name of the layer, for debugging purposes.
     */
    String m_name = "Layer";

    /**
     * @brief Indicates whether this @a `Layer` is an "overlay layer". Overlay layers take
     *        priority over other layers in the @a `LayerStack`.
     */
    Bool m_overlay = false;

  };

}
