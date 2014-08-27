#include <sstream>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "ResourceUsage.hpp"

using namespace std;

static double useconds(timeval const & t)
{
  return t.tv_sec*1e6 + t.tv_usec;
}

void ResourceUsage::measure()
{
  rusage * sample = new rusage;

  if (getrusage(who, sample)) {
    char const * what = strerror(errno);
    throw runtime_error(what);
  }
  samples.push_back(sample);
}

string ResourceUsage::report_str() const
{
  static char const * const indent = "    ";
  ostringstream buff;

  for (int i=0; i<samples.size(); ++i) {
    rusage const & u = *samples[i];

    double utime = useconds(u.ru_utime);
    double stime = useconds(u.ru_stime);
    size_t rss_kb = (getpagesize() * u.ru_maxrss) / 1024;

    buff << "Sample " << i << '\n';
    buff << indent << "User time used (microseconds)    : " << utime << '\n';
    buff << indent << "System time used (microseconds)  : " << stime << '\n';
    buff << indent << "Max resident set size (KB)       : " << rss_kb << '\n';
    buff << indent << "Integral shared text memory size : " << u.ru_ixrss << '\n';
    buff << indent << "Integral unshared data size      : " << u.ru_idrss << '\n';
    buff << indent << "Integral unshared stack size     : " << u.ru_isrss << '\n';
    buff << indent << "Page reclaims                    : " << u.ru_minflt << '\n';
    buff << indent << "Page faults                      : " << u.ru_majflt << '\n';
    buff << indent << "Process swaps                    : " << u.ru_nswap << '\n';
    buff << indent << "Block input operations           : " << u.ru_inblock << '\n';
    buff << indent << "Block output operations          : " << u.ru_oublock << '\n';
    buff << indent << "Socket messages sent             : " << u.ru_msgsnd << '\n';
    buff << indent << "Socket messages received         : " << u.ru_msgrcv << '\n';
    buff << indent << "Signals received                 : " << u.ru_nsignals << '\n';
    buff << indent << "Voluntary context switches       : " << u.ru_nvcsw << '\n';
    buff << indent << "Involuntary context switches     : " << u.ru_nivcsw << '\n';
  }
  return buff.str();
}

ostream & operator<<(ostream & os, const ResourceUsage & x)
{
      return os << x.report_str();
}

