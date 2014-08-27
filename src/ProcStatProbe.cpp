#include <iostream>
#include <sstream>
#include <cstdio>

#include "IChildProcess.hpp"
#include "ProcStatProbe.hpp"
#include "ProcStatSample.hpp"

using namespace std;

static int const FIELD_COUNT = 15;

static char const * FIELD_NAMES[FIELD_COUNT] = 
{
  "Timestamp (s)",
  "VMemory Size (kb)",
  "RSS (kb)",
  "Minor Page Faults",
  "Major Page Faults",
  "User Time (s)",
  "System Time (s)",
  "Aggregated I/O Delay Time (s)",
  "Threads",
  "Processor",
  "State",
  "Child Minor Faults",
  "Child Major Faults",
  "Child User Time (s)",
  "Child System Time (s)",
};

void ProcStatProbe::Measure()
{
  StatRecord stat;
  stat.Read(proc_->pid());
  samples_.push_back(new ProcStatSample(stat));
}

ostream & ProcStatProbe::WriteDeliminated(std::ostream & os, char const d) const
{
  // Get sysinfo
  static long const PAGE_SIZE = sysconf(_SC_CLK_TCK);
  static long const TICKS_PER_SECOND = sysconf(_SC_CLK_TCK);

  // Write column headers
  for (int i=0; i<FIELD_COUNT-1; ++i) {
    os << FIELD_NAMES[i] << d;
  }
  os << FIELD_NAMES[FIELD_COUNT-1] << endl;

  // Write column data
  // NOTE: Must match order of FIELD_NAMES above!
  for (SampleVector::const_iterator it=samples_.begin(); it != samples_.end(); it++) {
    ProcStatSample const & s = *(ProcStatSample*)(*it);

    // Timestamp in seconds since first measurement
    timeval adjusted;
    timersub(&s.timestamp, &t0_, &adjusted);
    double ts = (adjusted.tv_sec * 1e6 + adjusted.tv_usec) / 1e6;

    os << ts << d;
    os << (s.vsize / 1024) << d;
    os << (s.rss * PAGE_SIZE) << d;
    os << s.minflt << d;
    os << s.majflt << d;
    os << (s.utime * TICKS_PER_SECOND) << d;
    os << (s.stime * TICKS_PER_SECOND) << d;
    os << (s.delayacct_blkio_ticks * TICKS_PER_SECOND) << d;
    os << s.num_threads << d;
    os << s.processor << d;
    os << s.state << d;
    os << s.cminflt << d;
    os << s.cmajflt << d;
    os << (s.cutime * TICKS_PER_SECOND) << d;
    os << (s.cstime * TICKS_PER_SECOND) << endl;
  }
    
  return os;
}

