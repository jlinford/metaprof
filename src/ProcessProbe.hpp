/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * ProcessProbe declaration.
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
#ifndef PROCESSPROBE_HPP_
#define PROCESSPROBE_HPP_

#include "Statistics.hpp"
#include "IProbe.hpp"

///
/// Measures process resource usage
///
class ProcessProbe : public IProbe
{
public:

  ///
  /// Initializes the probe
  /// @param proc The process being probed
  ///
  ProcessProbe(IChildProcess * const proc) :
    IProbe(proc, "process")
  { }

  ///
  /// Empty destructor
  ///
  virtual ~ProcessProbe() { }

  ///
  /// Empty
  ///
  virtual void Initialize() { }

  ///
  /// Take a sample
  ///
  virtual void Measure();

  ///
  /// Empty
  ///
  virtual void Finalize() { }

private:

  Statistics minor_falt_;   ///< Minor fault statistics (no disk access)
  Statistics major_falt_;   ///< Major fault statistics (disk accessed)
  Statistics num_threads_;  ///< Number of threads in process
  Statistics vmem_;         ///< Virtual memory statistics
  Statistics rss_;          ///< Resident set size statistics
};

#endif /* PROCESSPROBE_HPP_ */
