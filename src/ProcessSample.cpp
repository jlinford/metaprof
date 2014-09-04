/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * ProcessSample definition
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
#include <unistd.h>
#include "ProcessSample.hpp"

using namespace std;

/// First known time for timestamp adjustment
static TimeStamp t0;

// System page size in bytes
static long PAGE_SIZE = sysconf(_SC_PAGESIZE);

/// Sytem ticks per second
static long TICKS_PER_SECOND = sysconf(_SC_CLK_TCK);


ISample::FieldVector ProcessSample::PackageFields()
{
  FieldVector fields;
  fields.push_back(PackageField("Timestamp (s)", (timestamp_ - t0).Seconds()));
  fields.push_back(PackageField("VMemory Size (kb)", vsize / 1024));
  fields.push_back(PackageField("RSS (kb)", (rss * PAGE_SIZE / 1024)));
  fields.push_back(PackageField("Minor Page Faults", minflt));
  fields.push_back(PackageField("Major Page Faults", majflt));
  fields.push_back(PackageField("User Time (s)", utime * TICKS_PER_SECOND));
  fields.push_back(PackageField("System Time (s)", stime * TICKS_PER_SECOND));
  fields.push_back(PackageField("Aggregated I/O Delay Time (s)", delayacct_blkio_ticks * TICKS_PER_SECOND));
  fields.push_back(PackageField("Code Size (b)", code_size));
  fields.push_back(PackageField("Data Size (b)", data_size));
  fields.push_back(PackageField("Threads", num_threads));
  fields.push_back(PackageField("Processor", processor));
  fields.push_back(PackageField("State", state));
  fields.push_back(PackageField("Child Minor Faults", cminflt));
  fields.push_back(PackageField("Child Major Faults", cmajflt));
  fields.push_back(PackageField("Child User Time (s)", cutime * TICKS_PER_SECOND));
  fields.push_back(PackageField("Child System Time (s)", cstime * TICKS_PER_SECOND));
  return fields;
}

