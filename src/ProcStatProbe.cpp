/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * ProcStatProbe member definitions.
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

#include "IChildProcess.hpp"
#include "ProcStatProbe.hpp"
#include "ProcStatSample.hpp"

using namespace std;

///
/// NULL_terminated column headers in output data
///
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

