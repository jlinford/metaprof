/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * Base class for probes.
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
#ifndef _IPROBE_HPP_
#define _IPROBE_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "ISample.hpp"


// Forward declaration
class IChildProcess;

///
/// Base class for probes.
/// Provides common interface for activation, measurement, deactivation, etc.
///
class IProbe
{
public:

  ///
  /// Minimal constructor
  ///
  IProbe(IChildProcess * const proc, std::string const & name) :
    proc_(proc),
    name_(name),
    sample_count_(0)
  { }

  ///
  /// Empty destructor
  ///
  virtual ~IProbe() { }

  ///
  /// name_ accessor
  ///
  std::string const & name() const {
    return name_;
  }

  ///
  /// Initializes and enables the probe
  /// In current design this should only happen once
  ///
  virtual void Activate();

  ///
  /// Disables the probe
  /// In current design this should only happen once
  ///
  virtual void Deactivate();

  ///
  /// Takes a sample of the probed metrics
  ///
  virtual void Measure() = 0;

  ///
  /// Writes a short summary of data gathered so far
  /// @param os The stream to write to
  /// @return The stream 'os' after writing
  ///
  virtual std::ostream & WriteSummary(std::ostream & os) const = 0;

protected:

  ///
  /// Records a sample, possibly flushing to disk
  /// @param sample sample to record
  ///
  virtual void RecordSample(ISample const & sample) {
    if (!sample.Write(sample_stream_)) {
      std::ostringstream buff;
      buff << "Failed to write to " << sample_stream_fname_;
      throw std::runtime_error(buff.str());
    }
  }

  /// The process being probed
  IChildProcess * const proc_;

  /// Probe name, e.g. "ProcStat"
  std::string const name_;

  /// Number of samples recorded so far
  size_t sample_count_;

  /// Name of samples file
  std::string sample_stream_fname_;

  /// Store samples in a file buffer
  std::ofstream sample_stream_;
};

#endif /* _IPROBE_HPP_ */
