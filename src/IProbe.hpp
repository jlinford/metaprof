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
#include <vector>

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

  /// A collection of samples
  typedef std::vector<ISample*> SampleVector;

  ///
  /// Minimal constructor
  ///
  IProbe(IChildProcess * const proc) :
    proc_(proc)
  { }

  ///
  /// Empty destructor
  ///
  virtual ~IProbe() { }

  ///
  /// samples_ accessor
  ///
  SampleVector const & samples() const {
    return samples_;
  }

  ///
  /// Initializes and enables the probe
  /// In current design this should only happen once
  ///
  virtual void Activate() = 0;

  ///
  /// Takes a sample of the probed metrics
  ///
  virtual void Measure() = 0;

  ///
  /// Disables the probe
  /// In current design this should only happen once
  ///
  virtual void Deactivate() = 0;

  ///
  /// Writes character-deliminated data to the specified stream
  /// @param os The stream to write to
  /// @param d  The deliminator
  /// @return The stream 'os' after writing
  ///
  virtual std::ostream & WriteDeliminated(std::ostream & os, char const d=',') const = 0;

  ///
  /// Writes a short summary of data gathered so far
  /// @param os The stream to write to
  /// @return The stream 'os' after writing
  ///
  virtual std::ostream & WriteSummary(std::ostream & os) const = 0;

protected:

  /// The process being probed
  IChildProcess * const proc_;

  /// Samples of probed metric
  SampleVector samples_;

};

#endif /* _IPROBE_HPP_ */
