/** @file DG/Core/Logging.cpp */

#include <DG/Core/Logging.hpp>

namespace dg
{

  Ref<Logger> Logging::s_engineLogger = nullptr;
  Ref<Logger> Logging::s_clientLogger = nullptr;

  void Logging::initialize ()
  {
    s_engineLogger = makeRef<Logger>("ENGINE");
    s_clientLogger = makeRef<Logger>("CLIENT");
  }

  Ref<Logger>& Logging::getEngineLogger ()
  {
    return s_engineLogger;
  }

  Ref<Logger>& Logging::getClientLogger ()
  {
    return s_clientLogger;
  }

}