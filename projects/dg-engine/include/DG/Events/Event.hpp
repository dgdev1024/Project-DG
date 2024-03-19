/** @file DG/Events/Event.hpp */

#pragma once

#include <DG_Pch.hpp>

namespace dg
{

  /** Event Type Enumeration **********************************************************************/

  /**
   * @brief The @a `EventType` enum enumerates the different types of events which can be emitted,
   *        listened for and handled.
   */
  enum class EventType
  {
    None,
    ApplicationFixedUpdate,
    ApplicationUpdate,
    WindowClose,
    WindowResize,
    KeyPress,
    KeyRelease,
    TextInput,
    MouseMove,
    MouseEnter,
    MouseLeave,
    ScrollInput,
    MouseButtonPress,
    MouseButtonRelease
  };

  /** Event Base Class ****************************************************************************/

  /**
   * @brief The @a `Event` class is the base class for all events emitted, listened for and handled
   *        by this engine.
   */
  class Event
  {
    friend class EventListener;

  protected:
    Event () = default;

  public:
    /**
     * @brief Retrieves the @a `EventType` enumeration of this @a `Event`.
     * 
     * @return  The @a `Event`'s @a `EventType` enumeration. 
     */
    virtual EventType getType () const = 0;

    /**
     * @brief Retrieves the @a `EventType` enumeration of this @a `Event`, expressed as an integer.
     * 
     * @return  The @a `Event`'s @a `EventType` enumeration, expressed as an integer.
     */
    virtual Int32 getTypeId () const = 0;

    /**
     * @brief Retrieves the @a `EventType` enumeration of this @a `Event`, expressed as a string.
     * 
     * @return  The @a `Event`'s @a `EventType` enumeration, expressed as a string.
     */
    virtual const char* getTypeName () const = 0;

    /**
     * @brief Retrieves whether or not an @a `EventListener` class has appropriately handled this
     *        @a `Event`.
     * 
     * @return  @a `true` if this @a `Event` has been handled appropriately; @a `false` otherwise. 
     */
    inline Bool hasBeenHandled () const
    {
      return m_handled;
    }

  private:
    /**
     * @brief Indicates whether or not this event has been handled appropriately by an
     *        @a `EventListener` class. If this is set to @a `true`, then other @a `EventListener`
     *        classes will ignore this event.
     */
    Bool m_handled = false;

  };

  /** Event Implementation Macro ******************************************************************/

  #define DG_EVENT_IMPL(eventType) \
    public: \
      inline static EventType getStaticType () { return eventType; } \
      inline EventType getType () const { return eventType; } \
      inline Int32 getTypeId () const { return static_cast<Int32>(eventType); } \
      inline const char* getTypeName () const { return #eventType; }

}
