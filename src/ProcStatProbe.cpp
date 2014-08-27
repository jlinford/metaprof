#include <iostream>
#include <sstream>
#include <cstdio>

#include "IChildProcess.hpp"
#include "ProcStatProbe.hpp"
#include "ProcStatSample.hpp"

using namespace std;

static char const * SAMPLE_FIELD_NAMES[] = 
{
  "Timestamp (s)",
  "VMemory Size (kb)",
  "RSS (kb)",
  "Minor Page Faults",
  "Major Page Faults",
  "User Time (s)",
  "System Time (s)",
  "Aggregated I/O Delay Time (s)",
  "Code Size (b)",
  "Data Size (b)",
  "Threads",
  "Processor",
  "State",
  "Child Minor Faults",
  "Child Major Faults",
  "Child User Time (s)",
  "Child System Time (s)",
  NULL
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
  static long const PAGE_SIZE = sysconf(_SC_PAGESIZE);
  static long const TICKS_PER_SECOND = sysconf(_SC_CLK_TCK);

  unsigned long code_size = initial_stat_.endcode - initial_stat_.startcode;
  unsigned long data_size = initial_stat_.end_data - initial_stat_.start_data;

  // Write column headers
  for (char const ** p=SAMPLE_FIELD_NAMES; *p; ++p) {
    os << *p << d;
  }
  os << endl;

  // Write column data
  // NOTE: Must match order of SAMPLE_FIELD_NAMES above!
  for (SampleVector::const_iterator it=samples_.begin(); it != samples_.end(); it++) {
    ProcStatSample const & s = *(ProcStatSample*)(*it);

    // Timestamp in seconds since first measurement
    timeval diff;
    timersub(&s.timestamp, &t0_, &diff);
    double ts = (diff.tv_sec * 1e6 + diff.tv_usec) / 1e6;

    os << ts << d;
    os << (s.vsize / 1024) << d;
    os << (s.rss * PAGE_SIZE / 1024) << d;
    os << s.minflt << d;
    os << s.majflt << d;
    os << (s.utime * TICKS_PER_SECOND) << d;
    os << (s.stime * TICKS_PER_SECOND) << d;
    os << (s.delayacct_blkio_ticks * TICKS_PER_SECOND) << d;
    os << code_size << d;
    os << data_size << d;
    os << s.num_threads << d;
    os << s.processor << d;
    os << s.state << d;
    os << s.cminflt << d;
    os << s.cmajflt << d;
    os << (s.cutime * TICKS_PER_SECOND) << d;
    os << (s.cstime * TICKS_PER_SECOND) << d;
    os << endl;
  }
    
  return os;
}

