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
#include <vector>
#include <string>

#include "IProbe.hpp"

///
/// Base class for child process launchers.
/// Provides common functionality, i.e. adding and triggering probes
///
class IChildProcess
{
public:

  /// A collection of probes
  typedef std::vector<IProbe*> ProbeVector;

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
  /// Creates a new probe of the specified type and adds it to the list
  /// of probes on this process.
  /// @param ProbeType The type of probe to add, e.g. ProcStatProbe
  ///
  template < typename ProbeType >
  void AddProbe() {
    IProbe * probe = new ProbeType(this);
    probes_.push_back(probe);
  }

  ///
  /// probes_ accessor
  ///
  ProbeVector const & probes() const {
    return probes_;
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
  /// Calls Activate() on all probes attached to the process
  ///
  virtual void ActivateProbes() {
    for(ProbeVector::iterator it = probes_.begin(); it != probes_.end(); it++) {
      (*it)->Activate();
    }
  }

  ///
  /// Calls Measure() on all probes attached to the process
  ///
  virtual void Measure() {
    for(ProbeVector::iterator it = probes_.begin(); it != probes_.end(); it++) {
      (*it)->Measure();
    }
  }

  ///
  /// Calls Deactivate() on all probes attached to the process
  ///
  virtual void DeactivateProbes() {
    for(ProbeVector::iterator it = probes_.begin(); it != probes_.end(); it++) {
      (*it)->Deactivate();
    }
  }

  ///
  /// Creates the child process using the specified command line arguments
  /// @param argc Argument count
  /// @param argv Command line arguments
  ///
  virtual int Create(int argc, char ** argv) = 0;

  ///
  /// Creates a new comma-separated values (CSV) file containing
  /// all data from all probes attached to the process.
  /// The file is named {child_exe}[.tag].csv the the current directory.
  /// @param tag An optional identifier for the CSV file name.
  ///
  virtual void ReportToCSVFile(char const * tag=NULL) = 0;

protected:

  /// Resource usage probes
  ProbeVector probes_;

  /// Child process executable name
  std::string exe_name_;

  /// Child process return value
  int retval_;

  /// Child process identifier
  int pid_;

  /// Microseconds between samples for all probes
  useconds_t freq_;

};


#endif /* _ICHILDPROCESS_HPP_ */

