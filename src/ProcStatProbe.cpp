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
  "Timestamp (s.us)",
  "VMemory Size (kb)",
  "RSS (kb)",
  "User Time (s)",
  "System Time (s)",
  "I/O Delay (s)",
  "Major Faults",
  "Minor Faults",
  "Threads",
  "Processor",
  "State",
  "Child Major Faults",
  "Child Minor Faults",
  "Child User Time",
  "Child System Time",
};

void ProcStatProbe::Measure()
{
  int pid;                    // The process ID
  char comm[256];             // The filename of the executable in parentheses
  char state;                 // Process state
  int ppid;                   // The PID of the parent of this process
  int pgrp;                   // The process group ID of the process
  int session;                // The session ID of the process
  int tty_nr;                 // The controlling terminal of the process
  int tpgid;                  // The ID of the foreground process group of the controlling terminal of the process
  unsigned long flags;        // The kernel flags word of the process
  unsigned long minflt;       // Minor faults not requiring page load from disk
  unsigned long cminflt;      // Minor faults made by process's children
  unsigned long majflt;       // Major faults requiring page load from disk
  unsigned long cmajflt;      // Major faults made by process's children
  unsigned long utime;        // Clock ticks spent scheduled in user mode
  unsigned long stime;        // Clock ticks spent scheduled in kernel mode
  long cutime;                // Clock ticks process's children spent in user mode
  long cstime;                // Clock ticks process's children spent in kernel mode
  long priority;              // Process priority (man(2) setpriority)
  long nice;                  // Process nice value (man(2) setpriority)
  long num_threads;           // Number of threads in the process
  long itrealvalue;           // Unmaintained since 2.6.17
  unsigned long starttime;    // Clock ticks after system boot when the process started
  unsigned long vsize;        // Virtual memory size in bytes
  long rss;                   // Number of pages process has in real memory
  unsigned long rsslim;       // Soft limit in bytes on the RSS of the process
  unsigned long startcode;    // The address above which program text can run
  unsigned long endcode;      // The address below which program text can run
  unsigned long startstack;   // The address of the start (bottom) of the stack
  unsigned long kstkesp;      // Current stack pointer value
  unsigned long kstkeip;      // Current instruction pointer value
  unsigned long signal;       // (Obsolete) Bitmap of pending signals as a decimal number
  unsigned long blocked;      // (Obsolete) Bitmap of blocked signals as a decimal number
  unsigned long sigignore;    // (Obsolete) Bitmap of ignored signals as a decimal number
  unsigned long sigcatch;     // (Obsolete) Bitmap of caught signals as a decimal number
  unsigned long wchan;        // Channel in which the process is waiting
  unsigned long nswap;        // (Not maintained) Number of pages swapped
  unsigned long cnswap;       // (Not maintained) Cumulative nswap for child processes
  int exit_signal;            // Signal to be sent to parent when we die
  int processor;              // CPU number last executed on
  unsigned long rt_priority;  // Real-time scheduling priority
  unsigned long policy;       // Scheduling policy
  unsigned long long delayacct_blkio_ticks;   // Aggregated block I/O delays in clock ticks
  unsigned long guest_time;   // Clock ticks spent running a virtual CPU for a guest operating system
  long cguest_time;           // Process's children's guest_time
  unsigned long start_data;   // Address above which BSS data are placed
  unsigned long end_data;     // Address below which BSS data are placed
  unsigned long start_brk;    // Address above which program heap can be expanded with brk
  unsigned long arg_start;    // Address above which program command-line arguments are placed
  unsigned long arg_end;      // Address below which program command-line arguments are placed
  unsigned long env_start;    // Address above which program environment is placed
  unsigned long env_end;      // Address below which program environment is placed
  int exit_code;              // The thread's exit status in the form reported by waitpid

  char const * const format = "%d %s %c %d %d %d %d %d %lu %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %lu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %lu %lu %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %d"; 
  char fname[256];
  sprintf(fname, "/proc/%d/stat", proc_->pid());

  FILE * fptr = fopen(fname, "r");
  if (!fptr) {
    ostringstream buff;
    buff << "Failed to open '" << fname << "' for read";
    throw runtime_error(buff.str());
  }

  fscanf(fptr, format, 
            &pid,
            comm,
            &state,
            &ppid,
            &pgrp,
            &session,
            &tty_nr,
            &tpgid,
            &flags,
            &minflt,
            &cminflt,
            &majflt,
            &cmajflt,
            &utime,
            &stime,
            &cutime,
            &cstime,
            &priority,
            &nice,
            &num_threads,
            &itrealvalue,
            &starttime,
            &vsize,
            &rss,
            &rsslim,
            &startcode,
            &endcode,
            &startstack,
            &kstkesp,
            &kstkeip,
            &signal,
            &blocked,
            &sigignore,
            &sigcatch,
            &wchan,
            &nswap,
            &cnswap,
            &exit_signal,
            &processor,
            &rt_priority,
            &policy,
            &delayacct_blkio_ticks,
            &guest_time,
            &cguest_time,
            &start_data,
            &end_data,
            &start_brk,
            &arg_start,
            &arg_end,
            &env_start,
            &env_end,
            &exit_code);

  fclose(fptr);

  ProcStatSample * sample = new ProcStatSample;
  sample->state = state;
  sample->minflt = minflt;
  sample->cminflt = cminflt;
  sample->majflt = majflt;
  sample->cmajflt = cmajflt;
  sample->utime = utime;
  sample->stime = stime;
  sample->cutime = cutime;
  sample->cstime = cstime;
  sample->num_threads = num_threads;
  sample->vsize = vsize;
  sample->rss = rss;
  sample->processor = processor;
  sample->delayacct_blkio_ticks = delayacct_blkio_ticks;

  samples_.push_back(sample);
}

ostream & ProcStatProbe::WriteDeliminated(std::ostream & os, char const d) const
{
  // Write column headers
  for (int i=0; i<FIELD_COUNT-1; ++i) {
    os << FIELD_NAMES[i] << d;
  }
  os << FIELD_NAMES[FIELD_COUNT-1] << endl;

  // Write column data
  // NOTE: Must match order of FIELD_NAMES above!
  for (SampleVector::const_iterator it=samples_.begin(); it != samples_.end(); it++) {
    ProcStatSample const & s = *(ProcStatSample*)(*it);
    os << s.timestamp.tv_sec << "." << s.timestamp.tv_usec << d;
    os << s.vsize << d;
    os << s.rss << d;
    os << s.utime << d;
    os << s.stime << d;
    os << s.delayacct_blkio_ticks;
    os << s.majflt << d;
    os << s.minflt << d;
    os << s.num_threads << d;
    os << s.processor << d;
    os << s.state << d;
    os << s.cmajflt << d;
    os << s.cminflt << d;
    os << s.cutime << d;
    os << s.cstime << endl;
  }
    
  return os;
}

