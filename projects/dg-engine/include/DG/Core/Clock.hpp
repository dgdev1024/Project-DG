/** @file DG/Core/Clock.hpp */

#pragma once

#include <DG_Pch.hpp>

namespace dg
{

  /**
   * @brief The @a `Clock` class is a utility class used for timekeeping operations, such as lag
   *        time accumulation.
   */
  class Clock
  {
  public:

    /**
     * @brief Constructs the @a `Clock`, setting its start point to the time of construction.
     */
    Clock ();

    /**
     * @brief Retrieves the amount of time which has passed since this @a `Clock` was either first
     *        created or last restarted.
     * 
     * @return  The amount of time elapsed, in seconds. 
     */
    Float32 getElapsed () const;

    /**
     * @brief Restarts the @a `Clock`, moving its starting point to the current point in time, then
     *        retrieves the amount of time which has passed since it was either first created or
     *        last restarted before this call.
     * 
     * @return  The amount of time elapsed, in seconds. 
     */
    Float32 restart ();

  private:

    /**
     * @brief Indicates the @a `Clock`'s current starting point, which is set when it is first
     *        created, and when it is reset with the @a `restart` method.
     */
    std::chrono::high_resolution_clock::time_point m_startPoint;

  };

}
