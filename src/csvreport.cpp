/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * 'quantify' program entry point.
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

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include "ProcStatSample.hpp"

using namespace std;

///
/// Ordered, NULL-terminated list of field names
///
static char const * PROCSTAT_FIELDS[] =
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


static void WriteProcStatHeaders(ostream & os)
{
  for (char const ** p=PROCSTAT_FIELDS; *p; ++p) {
    os << *p << ',';
  }
  os << endl;
}

static void WriteProcStatRecord(ostream & os, Time const & t0,
    StatRecord const & initial_stat, ProcStatSample const & s)
{
  static long PAGE_SIZE = sysconf(_SC_PAGESIZE);
  static long TICKS_PER_SECOND = sysconf(_SC_CLK_TCK);
  static char const d = ',';

  unsigned long code_size = initial_stat.endcode - initial_stat.startcode;
  unsigned long data_size = initial_stat.end_data - initial_stat.start_data;

  // NOTE: Must match order of PROCSTAT_FIELD_NAMES above
  os << (s.timestamp - t0).Seconds() << d;
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


static ofstream & procstat_report(string const & fname)
{
  static map<string,ofstream*> file_map;

  ofstream * osptr = file_map[fname];
  if (!osptr) {
    string const stem = fname.substr(fname.find_last_of("/\\")+1);
    string const fname = stem + ".csv";
    osptr = new ofstream(fname.c_str());
    file_map[fname] = osptr;
    if (!*osptr) {
      cerr << "Failed to open '" << fname << "' for write" << endl;
      exit(EXIT_FAILURE);
    }
  }
  return *osptr;
}


static void UpdateProcStatReport(string const & fname)
{
  ofstream & os = procstat_report(fname);

  ifstream is(fname.c_str());
  if (!is) {
    cerr << "Failed to open '" << fname << "' for read" << endl;
  }

  // Get executable name length
  size_t exe_name_len;
  is.read((char *)&exe_name_len, sizeof(size_t));

  // Get executable name
  char buff[exe_name_len+1];
  is.read(buff, exe_name_len);
  buff[exe_name_len] = '\0';
  string exe_name(buff);

  // Get initial timestamp and stat record
  Time t0;
  is.read((char*)&t0, sizeof(Time));

  // Get initial stat record
  StatRecord initial_stat;
  is.read((char*)&initial_stat, sizeof(StatRecord));

  // Write title
  os << exe_name << ',' << endl;

  // Separate title from data by an empty field
  os << ',' << endl;

  // Write column headers
  WriteProcStatHeaders(os);

  // Read samples into records
  ProcStatSample sample;
  while (is) {
    is.read((char*)&sample, sizeof(ProcStatSample));
    WriteProcStatRecord(os, t0, initial_stat, sample);
  }
}


static void UpdateReport(string const & fname)
{
  string const ext = fname.substr(fname.find_last_of('.')+1);
  if (ext == "ProcStat") {
    UpdateProcStatReport(fname);
  } else {
    cerr << "Unknown file format: " << fname << endl;
  }
}

int main(int argc, char ** argv)
{
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " file0 [file1 ...]" << endl;
    return 0;
  }

  for (int i=1; i<argc; ++i) {
    char const * fname = argv[i];
    UpdateReport(fname);
  }

  return 0;
}

