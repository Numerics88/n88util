// Copyright (c) Eric Nodwell
// See LICENSE for details.

#ifndef N88UTIL_logger_hpp_INCLUDED
#define N88UTIL_logger_hpp_INCLUDED

#include "n88util/TimeStamp.hpp"
#include <boost/noncopyable.hpp>
#include <iostream>
#include <sstream>


namespace n88
{

  namespace io
  {

    enum LogLevel_t {
      INFORMATIVE,
      IMPORTANT,
      ERROR
    };

    struct console_only_class { console_only_class () {} };

    struct informative_level_class { informative_level_class () {} };

    struct important_level_class { important_level_class () {} };

    struct error_level_class { error_level_class () {} };

    class logger : private boost::noncopyable
    {
      public:

        logger ()
          :
          m_logLevel (IMPORTANT),
          m_consoleLevel (INFORMATIVE),
          m_suppress (false)
        {}
      
        virtual LogLevel_t GetLogLevel ()
        { return this->m_logLevel; }
      
        virtual void SetConsoleLevel (LogLevel_t level)
        { this->m_consoleLevel = level; }
        virtual LogLevel_t GetConsoleLevel ()
        { return this->m_consoleLevel; }

        virtual std::string GetLog()
        { return this->m_log.str(); }

        template <typename T>
        logger& operator<< (const T& msg)
        {
          if (this->m_suppress)
          {
            this->m_suppress = false;
          }
          else
          {
            this->m_log << msg;
          }
          if (this->m_logLevel >= this->m_consoleLevel)
          {
            std::cout << msg;
            std::cout.flush();
          }
          return *this;
        }

        // No idea why this requires special handling (sometimes),
        // but having this separate solves some compilation errors
        // on some systems.
        logger& operator<<(const n88::TimeStamp& ts)
        {
          this->operator<< (ts.Print());
          return *this;
        }

        logger& operator<< (const console_only_class&)
        {
          this->m_suppress = true;
          return *this;
        }

        logger& operator<< (const informative_level_class&)
        {
          this->m_logLevel = INFORMATIVE;
          return *this;
        }

        logger& operator<< (const important_level_class&)
        {
          this->m_logLevel = IMPORTANT;
          return *this;
        }

        logger& operator<< (const error_level_class&)
        {
          this->m_logLevel = ERROR;
          return *this;
        }

      protected:   

        LogLevel_t m_logLevel;
        LogLevel_t m_consoleLevel;
        std::ostringstream m_log;
        bool m_suppress;

    };

    static const console_only_class console_only;
    static const informative_level_class informative_level;
    static const important_level_class important_level;
    static const error_level_class error_level;

  }  // namespace io

}  // namespace n88

#endif
