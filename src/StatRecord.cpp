#include <iostream>
#include <sstream>
#include <cstdio>
#include <stdexcept>

#include "StatRecord.hpp"

using namespace std;



void StatRecord::Read(int pid)
{
  char const * const format = "%d %s %c %d %d %d %d %d %lu %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %lu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %lu %lu %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %d"; 
  char fname[256];
  sprintf(fname, "/proc/%d/stat", pid);

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
}

