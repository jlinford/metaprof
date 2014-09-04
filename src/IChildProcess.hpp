/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * Base class for child process launchers.
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
#ifndef _ICHILDPROCESS_HPP_
#define _ICHILDPROCESS_HPP_

#include <unistd.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "IProbe.hpp"
#include "IReport.hpp"
#include "Time.hpp"

///
/// Base class for child process launchers.
/// Provides common functionality, i.e. adding and triggering probes
///
class IChildProcess
{
public:

  /// Friend classes
  friend class IProbe;
  friend class IReport;

  /// A collection of probes
  typedef std::vector<IProbe*> ProbeVector;

  /// A collection of reports
  typedef std::vector<IReport*> ReportVector;

  ///
  /// Returns platform-specific child process executor
  ///
  static IChildProcess * Instance();

  ///
  /// Minimal constructor
  ///
  IChildProcess() : 
    retval_(-1),
    pid_(-1),
    freq_(100000)
  { }

  ///
  /// Empty destructor
  ///
  virtual ~IChildProcess() { }

  ///
  /// probes_ accessor
  ///
  ProbeVector const & probes() const {
    return probes_;
  }

  ///
  /// exe_name accessor
  ///
  std::string const & exe_name() const {
    return exe_name_;
  }

  ///
  /// freq_ accessor
  ///
  useconds_t freq() const {
    return freq_;
  }

  ///
  /// freq_ accessor
  ///
  void set_freq(useconds_t freq) {
    freq_ = freq;
  }

  ///
  /// retval_ accessor
  ///
  int retval() const {
    return retval_;
  }

  ///
  /// pid_ accessor
  ///
  int pid() const {
    return pid_;
  }

  ///
  /// Returns child process runtime
  ///
  Timer const & runtime() const {
    return runtime_;
  }

  ///
  /// Calls Activate() on all probes attached to the process
  ///
  virtual void Initialize() {
    for (ReportVector::iterator it = reports_.begin(); it != reports_.end(); it++) {
      (*it)->Initialize();
    }
    for (ProbeVector::iterator it = probes_.begin(); it != probes_.end(); it++) {
      (*it)->Initialize();
    }
  }

  ///
  /// Calls Measure() on all probes attached to the process and sends
  /// data to reports for further processing
  ///
  virtual void Measure() {
    for (ProbeVector::iterator i = probes_.begin(); i != probes_.end(); i++) {
      IProbe * probe = *i;
      probe->Measure();
      for (ReportVector::iterator j = reports_.begin(); j != reports_.end(); j++) {
        (*j)->Update(probe);
      }
      probe->Flush();
    }
  }

  ///
  /// Calls Deactivate() on all probes attached to the process
  ///
  virtual void Finalize() {
    for (ProbeVector::iterator it = probes_.begin(); it != probes_.end(); it++) {
      (*it)->Finalize();
    }
    for (ReportVector::iterator it = reports_.begin(); it != reports_.end(); it++) {
      (*it)->Finalize();
    }
  }

  ///
  /// Return a filename based on the name of the child process executable
  /// Filename is exe_name.pid.[tag.]ext
  /// @param ext File extension
  /// @param tag Optional tag
  /// @return The filename string
  virtual std::string BuildFilename(char const * ext, char const * tag=NULL) {
    std::ostringstream buff;
    buff << exe_name_.substr(exe_name_.find_last_of("/\\")+1) << ".";
    buff << pid_ << ".";
    if (tag) {
      buff << tag << ".";
    }
    buff << ext;
    return buff.str();
  }

  ///
  /// Runs the child process using the specified command line arguments
  /// @param argc Argument count
  /// @param argv Command line arguments
  ///
  virtual int Run(int argc, char ** argv) = 0;

protected:

  /// Resource usage probes
  ProbeVector probes_;

  /// Resource usage reports
  ReportVector reports_;

  /// Child process executable name
  std::string exe_name_;

  /// Child process runtime
  Timer runtime_;

  /// Child process return value
  int retval_;

  /// Child process identifier
  int pid_;

  /// Microseconds between samples for all probes
  useconds_t freq_;
};


#endif /* _ICHILDPROCESS_HPP_ */

