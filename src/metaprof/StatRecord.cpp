/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * StatRecord member definition
 *
 * @copyright BSD
 * @section LICENSE
 *
 * *Copyright (c) 2014, ParaTools, Inc.*
 * *All rights reserved.*
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. Neither the name of ParaTools, Inc. nor the names of its contributors may
 *     be used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * **THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.**
 */
#include <iostream>
#include <sstream>
#include <cstdio>
#include <stdexcept>

#include "StatRecord.hpp"

using namespace std;



void StatRecord::Read(int pid)
{
  // The format string must match the order and number of fields in /proc/[pid]/stat
  static char const * const FORMAT = "%d %s %c %d %d %d %d %d %lu %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %lu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %lu %lu %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %d";
  char fname[256];
  sprintf(fname, "/proc/%d/stat", pid);

  FILE * fptr = fopen(fname, "r");
  if (!fptr) {
    ostringstream buff;
    buff << "Failed to open '" << fname << "' for read";
    throw runtime_error(buff.str());
  }

  fscanf(fptr, FORMAT,
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

