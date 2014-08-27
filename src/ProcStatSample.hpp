#ifndef _PROCSTATSAMPLE_HPP_
#define _PROCSTATSAMPLE_HPP_

#include "ISample.hpp"

//
// Data measured from /proc/[pid]/stat
// See man(5) proc for details
// 
struct ProcStatSample : public ISample
{
  char state;                 // Process state
  unsigned long minflt;       // Minor faults not requiring page load from disk
  unsigned long cminflt;      // Minor faults made by process's children
  unsigned long majflt;       // Major faults requiring page load from disk
  unsigned long cmajflt;      // Major faults made by process's children
  unsigned long utime;        // Clock ticks spent scheduled in user mode
  unsigned long stime;        // Clock ticks spent scheduled in kernel mode
  long cutime;                // Clock ticks process's children spent in user mode
  long cstime;                // Clock ticks process's children spent in kernel mode
  long num_threads;           // Number of threads in the process
  unsigned long vsize;        // Virtual memory size in bytes
  long rss;                   // Number of pages process has in real memory
  int processor;              // CPU number last executed on
  unsigned long long delayacct_blkio_ticks;   // Aggregated block I/O delays in clock ticks
}; 

#endif /* _PROCSTATSAMPLE_HPP_ */
