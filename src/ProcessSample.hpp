/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * ProcessSample declaration.
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
#ifndef PROCESSSAMPLE_HPP_
#define PROCESSSAMPLE_HPP_

#include "StatRecord.hpp"
#include "ISample.hpp"

///
/// Data measured from process
///
struct ProcessSample : public ISample
{
  ///
  /// Empty constructor
  ///
  ProcessSample() :
    state(0),
    minflt(0),
    cminflt(0),
    majflt(0),
    cmajflt(0),
    utime(0),
    stime(0),
    cutime(0),
    cstime(0),
    num_threads(0),
    vsize(0),
    rss(0),
    processor(-1),
    delayacct_blkio_ticks(0),
    code_size(0),
    data_size(0)
  { }

  ///
  /// Initializer
  ///
  ProcessSample(StatRecord const & s) :
    state(s.state),
    minflt(s.minflt),
    cminflt(s.cminflt),
    majflt(s.majflt),
    cmajflt(s.cmajflt),
    utime(s.utime),
    stime(s.stime),
    cutime(s.cutime),
    cstime(s.cstime),
    num_threads(s.num_threads),
    vsize(s.vsize),
    rss(s.rss),
    processor(s.processor),
    delayacct_blkio_ticks(s.delayacct_blkio_ticks),
    code_size(s.endcode - s.startcode),
    data_size(s.end_data - s.start_data)
  { }

  FieldVector PackageFields();

  char state;                ///< Process state
  unsigned long minflt;      ///< Minor faults not requiring page load from disk
  unsigned long cminflt;     ///< Minor faults made by process's children
  unsigned long majflt;      ///< Major faults requiring page load from disk
  unsigned long cmajflt;     ///< Major faults made by process's children
  unsigned long utime;       ///< Clock ticks spent scheduled in user mode
  unsigned long stime;       ///< Clock ticks spent scheduled in kernel mode
  long cutime;               ///< Clock ticks process's children spent in user mode
  long cstime;               ///< Clock ticks process's children spent in kernel mode
  long num_threads;          ///< Number of threads in the process
  unsigned long vsize;       ///< Virtual memory size in bytes
  long rss;                  ///< Number of pages process has in real memory
  int processor;             ///< CPU number last executed on
  unsigned long long delayacct_blkio_ticks;  ///< Aggregated block I/O delays in clock ticks
  unsigned long code_size;   ///< Size of program code in bytes
  unsigned long data_size;   ///< Size of program data in bytes
};

#endif /* _PROCSTATSAMPLE_HPP_ */
