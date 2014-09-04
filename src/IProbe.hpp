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
  /// A collection of samples
  ///
  typedef std::vector<class ISample*> SampleBuffer;

  ///
  /// Constructor
  ///
  IProbe(IChildProcess * const proc, std::string const & name);

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
  /// samples_ accessor
  ///
  SampleBuffer const & samples() const {
    return samples_;
  }

  ///
  /// Initializes and enables the probe
  /// In current design this should only happen once
  ///
  virtual void Initialize() = 0;

  ///
  /// Takes a sample of the probed metrics
  ///
  virtual void Measure() = 0;

  ///
  /// Flushes buffered samples from memory
  ///
  virtual void Flush() {
    samples_.clear();
  }

  ///
  /// Disables the probe
  /// In current design this should only happen once
  ///
  virtual void Finalize() = 0;

protected:

  /// The process being probed
  IChildProcess * const proc_;

  /// Probe name, e.g. "ProcStat"
  std::string const name_;

  /// In-memory sample buffer
  SampleBuffer samples_;
};

#endif /* _IPROBE_HPP_ */
