/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * ProcStatProbe declaration.
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
#ifndef _PROCSTATPROBE_HPP_
#define _PROCSTATPROBE_HPP_

#include "StatRecord.hpp"
#include "IProbe.hpp"

///
/// A probe that uses /proc/[pid]/stat to get resource usage
///
class ProcStatProbe : public IProbe
{
public:

  ///
  /// Initializes the probe and marks creation time
  /// @param proc The process being probed
  ///
  ProcStatProbe(IChildProcess * const proc) :
    IProbe(proc)
  {
    if (gettimeofday(&t0_, NULL) == -1) {
      throw std::runtime_error(strerror(errno));
    }
  }

  ///
  /// Empty destructor
  ///
  virtual ~ProcStatProbe() { }

  ///
  /// Reads /proc/[pid]/stat into initial_stat_ for future use
  ///
  virtual void Activate() {
    initial_stat_.Read(proc_->pid());
  }

  ///
  /// Empty
  ///
  virtual void Deactivate() {
    // Nothing to do
  }

  ///
  /// Samples /proc/[pid]/stat
  ///
  virtual void Measure();

  ///
  /// Writes character-deliminated data to the specified stream
  /// @param os The stream to write to
  /// @param d  The deliminator
  /// @return The stream 'os' after writing
  ///
  virtual std::ostream & WriteDeliminated(std::ostream & os, char const d=',') const;

private:

  /// First known time for timestamp adjustment
  timeval t0_;

  /// Contents of /proc/[pid]/stat when probe was activated
  StatRecord initial_stat_;

};

#endif /* _PROCSTATPROBE_HPP_ */
