/** @file DG/Core/Clock.cpp */

#include <DG/Core/Clock.hpp>

namespace dg
{

  Clock::Clock () :
    m_startPoint { std::chrono::high_resolution_clock::now() }
  {

  }

  Float32 Clock::getElapsed () const
  {

    // The time elapsed is calculated by subtracting the clock's start point from the current
    // point in time.
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<Float32> elapsed = now - m_startPoint;

    // Return the time elapsed.
    return elapsed.count();

  }

  Float32 Clock::restart ()
  {

    // Before resetting the clock's start point, find out how much time has elapsed since the last
    // restart or construction.
    Float32 elapsed = getElapsed();

    // Restart the clock by resetting its start point to the current point in time.
    m_startPoint = std::chrono::high_resolution_clock::now();

    // Return the time elapsed.
    return elapsed;

  }

}
