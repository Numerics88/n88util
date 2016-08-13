// Copyright (c) Eric Nodwell
// See LICENSE for details.

#ifndef N88UTIL_TimeStamp_hpp_INCLUDED
#define N88UTIL_TimeStamp_hpp_INCLUDED

#include <boost/format.hpp>
#include <boost/timer/timer.hpp>
#include <ostream>

namespace n88
{

class TimeStamp
{
  public:

  	TimeStamp()
      :
      m_format (boost::format("%7.2f "))
      {}

  	void SetFormat(std::string f)
    {
      this->m_format = boost::format(f);
    }

    std::string Print() const
    {
      boost::timer::cpu_times const elapsed_times (this->m_timer.elapsed());
      boost::timer::nanosecond_type const elapsed_ns (
                             elapsed_times.system + elapsed_times.user);
      double elapsed = elapsed_ns / 1000000000.0;
      boost::format f = this->m_format;  // copy to avoid discarding const
      return (f % elapsed).str();
    }

  protected:
    boost::timer::cpu_timer m_timer;
    boost::format m_format;
};

}  // namespace

template <class T>
T& operator<<(T& os, const n88::TimeStamp& ts)
{
  os << ts.Print();
  return os;
}

#endif
