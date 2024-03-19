/** @file DG/Core/Logger.hpp */

#pragma once

#include <DG/Core/String.hpp>

namespace dg
{

  /**
   * @brief The @a `Logger` is a helper class used for logging output to destination output and
   *        error streams.
   */
  class Logger
  {
  public:
    /**
     * @brief Constructs a new @a `Logger` with the given string name, using the default output
     *        and error streams for streaming output.
     * 
     * @param name  Contains the name of this @a `Logger`. 
     */
    Logger (const String& name) :
      m_name      { name },
      m_cout      { std::cout },
      m_cerr      { std::cerr },
      m_oneStream { false }
    {

    }

    /**
     * @brief Constructs a new @a `Logger` with the given string name, using a single output and
     *        error stream for streaming output.
     * 
     * @param name    Contains the name of this @a `Logger`.
     * @param stream  A handle to the stream to use for both output and error streaming.
     */
    Logger (
      const String& name,
      std::ostream& stream
    ) :
      m_name      { name },
      m_cout      { stream },
      m_cerr      { stream },
      m_oneStream { true }
    {

    }

    /**
     * @brief Constructs a new @a `Logger` with the given string name, using the given dedicated
     *        output and error streams for streaming output.
     * 
     * @param name          Contains the name of this @a `Logger`.
     * @param outputStream  A handle to the stream to use for output streaming.
     * @param errorStream   A handle to the stream to use for error streaming.
     */
    Logger (
      const String& name,
      std::ostream& outputStream,
      std::ostream& errorStream
    ) :
      m_name      { name },
      m_cout      { outputStream },
      m_cerr      { errorStream },
      m_oneStream { false }
    {

    }

    ~Logger ()
    {
      m_cout.flush();

      if (m_oneStream == false) { 
        m_cerr.flush();
      }
    }

    /**
     * @brief Logs general information to the output stream.
     * 
     * @tparam ...Ts  The types of the variadic arguments used to fill placeholders.
     * 
     * @param format  The format string.
     * @param ...args The variadic arguments used to fill placeholders.
     */
    template <typename... Ts>
    inline void info (const char* format, Ts... args)
    {
      streamFormatted(m_cout, "[{} | Info]     ", m_name);
      streamFormatted(m_cout, format, args...);
      m_cout << "\n";
    }

    /**
     * @brief Logs a warning to the output stream.
     * 
     * @tparam ...Ts  The types of the variadic arguments used to fill placeholders.
     * 
     * @param format  The format string.
     * @param ...args The variadic arguments used to fill placeholders.
     */
    template <typename... Ts>
    inline void warning (const char* format, Ts... args)
    {
      streamFormatted(m_cerr, "[{} | Warning]  ", m_name);
      streamFormatted(m_cerr, format, args...);
      m_cerr << "\n";
    }

    /**
     * @brief Logs an error to the output stream.
     * 
     * @tparam ...Ts  The types of the variadic arguments used to fill placeholders.
     * 
     * @param format  The format string.
     * @param ...args The variadic arguments used to fill placeholders.
     */
    template <typename... Ts>
    inline void error (const char* format, Ts... args)
    {
      streamFormatted(m_cerr, "[{} | Error]    ", m_name);
      streamFormatted(m_cerr, format, args...);
      m_cerr << std::endl;
    }

    /**
     * @brief Logs a critical error to the output stream.
     * 
     * @tparam ...Ts  The types of the variadic arguments used to fill placeholders.
     * 
     * @param format  The format string.
     * @param ...args The variadic arguments used to fill placeholders.
     */
    template <typename... Ts>
    inline void critical (const char* format, Ts... args)
    {
      streamFormatted(m_cerr, "[{} | Critical] ", m_name);
      streamFormatted(m_cerr, format, args...);
      m_cerr << std::endl;
    }

  private:
    /**
     * @brief A string containing the name of this @a `Logger`.
     */
    String  m_name = "";
  
    /**
     * @brief A handle to the @a `std::ostream` used for standard logging. By default, this is set 
     *        to @a `std::cout`.
     */
    std::ostream& m_cout;
    
    /**
     * @brief A handle to the @a `std::ostream` used for error logging By default, this is set to 
     *        @a `std::cerr`.
     */
    std::ostream& m_cerr;

    /**
     * @brief Indicates whether or not @a `m_cout` and @a `m_cerr` are handles to the same stream
     *        object.
     */
    bool m_oneStream = false;
    
  };

}