/** @file DG/Core/Logging.hpp */

#pragma once

#include <DG/Core/Logger.hpp>

namespace dg
{

  /**
   * @brief The @a `Logging` class is a static helper class used for hosting @a `Logger` classes
   *        to be used by the engine library and by the client application, respectively.
   */
  class Logging
  {
  public:
    /**
     * @brief This is called internally by the @a `Application` class. This method initializes the
     *        engine and client loggers.
     */
    static void initialize ();

    /**
     * @brief Retrieves the @a `Logger` class used by the engine library's code.
     * 
     * @return  A pointer to the engine @a `Logger`. 
     */
    static Ref<Logger>& getEngineLogger ();

    /**
     * @brief Retrieves the @a `Logger` class used by the client application's code.
     * 
     * @return  A pointer to the client @a `Logger`. 
     */
    static Ref<Logger>& getClientLogger ();

  private:
    /**
     * @brief Points to the @a `Logger` class to be used by the engine library's code.
     */
    static Ref<Logger> s_engineLogger;

    /**
     * @brief Points to the @a `Logger` class to be used by the client application's code.
     */
    static Ref<Logger> s_clientLogger;

  };

}

// Engine Logging Macros
#define DG_ENGINE_CRIT(...)   dg::Logging::getEngineLogger()->critical(__VA_ARGS__)
#define DG_ENGINE_ERROR(...)  dg::Logging::getEngineLogger()->error(__VA_ARGS__)
#define DG_ENGINE_WARN(...)   dg::Logging::getEngineLogger()->warning(__VA_ARGS__)
#define DG_ENGINE_INFO(...)   dg::Logging::getEngineLogger()->info(__VA_ARGS__)

// Client Logging Macros
#define DG_CRIT(...)          dg::Logging::getClientLogger()->critical(__VA_ARGS__)
#define DG_ERROR(...)         dg::Logging::getClientLogger()->error(__VA_ARGS__)
#define DG_WARN(...)          dg::Logging::getClientLogger()->warning(__VA_ARGS__)
#define DG_INFO(...)          dg::Logging::getClientLogger()->info(__VA_ARGS__)