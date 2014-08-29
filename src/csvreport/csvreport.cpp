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

#include <iostream>

using namespace std;

#if 0
///
/// Creates a new comma-separated values (CSV) file containing
/// all data from all probes attached to the process.
/// The file is named {child_exe}[.tag].csv the the current directory.
/// @param tag An optional identifier for the CSV file name.
///
virtual void ReportToCSVFile(char const * tag=NULL);

///
/// Creates a new GNUPlot data file containing
/// all data from all probes attached to the process.
/// The file is named {child_exe}[.tag].dat the the current directory.
/// @param tag An optional identifier for the dat file name.
///
virtual void ReportToGnuplot(char const * tag=NULL);


ostream & ProcStatProbe::WriteDeliminatedHeaders(std::ostream & os, char const d) const
{
  // Write column headers
  for (char const ** p=SAMPLE_FIELD_NAMES; *p; ++p) {
    os << *p << d;
  }
  os << endl;
  return os;
}

ostream & ProcStatProbe::WriteDeliminatedData(std::ostream & os, char const d) const
{
  unsigned long code_size = initial_stat_.endcode - initial_stat_.startcode;
  unsigned long data_size = initial_stat_.end_data - initial_stat_.start_data;

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
    os << (s.rss * PAGE_SIZE_ / 1024) << d;
    os << s.minflt << d;
    os << s.majflt << d;
    os << (s.utime * TICKS_PER_SECOND_) << d;
    os << (s.stime * TICKS_PER_SECOND_) << d;
    os << (s.delayacct_blkio_ticks * TICKS_PER_SECOND_) << d;
    os << code_size << d;
    os << data_size << d;
    os << s.num_threads << d;
    os << s.processor << d;
    os << s.state << d;
    os << s.cminflt << d;
    os << s.cmajflt << d;
    os << (s.cutime * TICKS_PER_SECOND_) << d;
    os << (s.cstime * TICKS_PER_SECOND_) << d;
    os << endl;
  }

  return os;
}

ostream & ProcStatProbe::WriteGnuplot(ostream & os) const
{
  static const size_t ROWS = 2;
  static const size_t COLS = 2;

  // Write gnuplot script
  os << "#!/usr/bin/gnuplot -persist\n";
  os << "\n";
  os << "set multiplot layout " << ROWS << ", " << COLS << " title '" << proc_->exe_name() << "'\n";
  os << "set tmargin 3\n";
  os << "set xtics font 'Verdana,6'\n";
  os << "set ytics font 'Verdana,6'\n";
  os << "\n";

  os << "set title 'VMemory Size (kb)'\n";
  os << "unset key\n";
  os << "plot '-' using 1:2 with line\n";
  for (SampleVector::const_iterator it=samples_.begin(); it != samples_.end(); it++) {
    ProcStatSample const & s = *(ProcStatSample*)(*it);

    // Timestamp in seconds since first measurement
    timeval diff;
    timersub(&s.timestamp, &t0_, &diff);
    double ts = (diff.tv_sec * 1e6 + diff.tv_usec) / 1e6;

    os << ts << " " << (s.vsize / 1024) << "\n";
  }
  os << "EOF\n\n";

  os << "set title 'RSS (kb)'\n";
  os << "unset key\n";
  os << "plot '-' using 1:2 with line\n";
  for (SampleVector::const_iterator it=samples_.begin(); it != samples_.end(); it++) {
    ProcStatSample const & s = *(ProcStatSample*)(*it);

    // Timestamp in seconds since first measurement
    timeval diff;
    timersub(&s.timestamp, &t0_, &diff);
    double ts = (diff.tv_sec * 1e6 + diff.tv_usec) / 1e6;

    os << ts << " " << (s.rss * PAGE_SIZE_ / 1024) << "\n";
  }
  os << "EOF\n\n";

  os << "set title 'Minor Page Faults'\n";
  os << "unset key\n";
  os << "plot '-' using 1:2 with line\n";
  for (SampleVector::const_iterator it=samples_.begin(); it != samples_.end(); it++) {
    ProcStatSample const & s = *(ProcStatSample*)(*it);

    // Timestamp in seconds since first measurement
    timeval diff;
    timersub(&s.timestamp, &t0_, &diff);
    double ts = (diff.tv_sec * 1e6 + diff.tv_usec) / 1e6;

    os << ts << " " << s.minflt << "\n";
  }
  os << "EOF\n\n";

  os << "set title 'Major Page Faults'\n";
  os << "unset key\n";
  os << "plot '-' using 1:2 with line\n";
  for (SampleVector::const_iterator it=samples_.begin(); it != samples_.end(); it++) {
    ProcStatSample const & s = *(ProcStatSample*)(*it);

    // Timestamp in seconds since first measurement
    timeval diff;
    timersub(&s.timestamp, &t0_, &diff);
    double ts = (diff.tv_sec * 1e6 + diff.tv_usec) / 1e6;

    os << ts << " " << s.majflt << "\n";
  }
  os << "EOF\n\n";

  return os;
}

///
/// Creates a new comma-separated values (CSV) file containing
/// all data from all probes attached to the process.
/// The file is named {child_exe}[.tag].csv the the current directory.
/// @param tag An optional identifier for the CSV file name.
///
virtual void ReportToCSVFile(char const * tag=NULL) = 0;

///
/// Creates a new GNUPlot data file containing
/// all data from all probes attached to the process.
/// The file is named {child_exe}[.tag].dat the the current directory.
/// @param tag An optional identifier for the dat file name.
///
virtual void ReportToGnuplot(char const * tag=NULL) = 0;

///
/// Writes character-deliminated data header to the specified stream
/// @param os The stream to write to
/// @param d  The deliminator
/// @return The stream 'os' after writing
///
virtual std::ostream & WriteDeliminatedHeaders(std::ostream & os, char const d=',') const = 0;

///
/// Writes character-deliminated data to the specified stream
/// @param os The stream to write to
/// @param d  The deliminator
/// @return The stream 'os' after writing
///
virtual std::ostream & WriteDeliminatedData(std::ostream & os, char const d=',') const = 0;

///
/// Writes a gnuplot script with data to the specified stream
/// @param os The stream to write to
/// @return The stream 'os' after writing
///
virtual std::ostream & WriteGnuplot(std::ostream & os) const = 0;



void ForkExecChild::ReportToCSVFile(char const * tag)
{
  // Open CSV file for write
  ofstream os(BuildFilename("csv", tag).c_str());

  // Write summary
  os << "Executable,Runtime (s)," << endl;
  os << exe_name_ << ',';
  os << runtime() << ',';
  os << endl;

  // Separate summary from probe table by an empty field
  os << ',' << endl;

  // Write probe tables
  for(ProbeVector::const_iterator it=probes_.begin(); it!=probes_.end(); it++) {
    (*it)->WriteDeliminatedHeaders(os);
    (*it)->WriteDeliminatedData(os);
    os << endl;
  }

  // Flush and close file
  os.close();
}

void ForkExecChild::ReportToGnuplot(char const * tag)
{
  // Write gnuplot script
  for(ProbeVector::const_iterator it=probes_.begin(); it!=probes_.end(); it++) {
    IProbe const & probe = **it;
    ofstream script(BuildFilename("gnuplot", probe.name().c_str()).c_str());
    probe.WriteGnuplot(script);
    script.close();
  }
}

#endif

int main(int argc, char ** argv)
{

  return 0;
}

