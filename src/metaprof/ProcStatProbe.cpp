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


void ProcStatProbe::Measure()
{
  StatRecord stat;
  stat.Read(proc_->pid());

  ProcStatSample sample(stat);

  max_minflt_ = max(sample.minflt, max_minflt_);
  max_majflt_ = max(sample.majflt, max_majflt_);
  max_num_threads_ = max(sample.num_threads, max_num_threads_);
  max_vsize_ = max(sample.vsize, max_vsize_);
  max_rss_ = max(sample.rss, max_rss_);

  RecordSample(sample);
}


ostream & ProcStatProbe::WriteSummary(ostream & os) const
{
  /// Page size in bytes
  static long const PAGE_SIZE = sysconf(_SC_PAGESIZE);

  unsigned long code_size = initial_stat_.endcode - initial_stat_.startcode;
  unsigned long data_size = initial_stat_.end_data - initial_stat_.start_data;

  os << "Max Threads:      " << max_num_threads_ << '\n';
  os << "Max Major Faults: " << max_majflt_ << '\n';
  os << "Max Minor Faults: " << max_minflt_ << '\n';
  os << "Code Size (b):    " << code_size << '\n';
  os << "Data Size (b):    " << data_size << '\n';
  os << "Max RSS (kb):     " << (max_rss_ * PAGE_SIZE / 1024) << '\n';
  os << "Max Vmem (kb):    " << (max_vsize_ / 1024);

  return os;
}
