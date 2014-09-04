/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * GnuplotReport declaration
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
#ifndef GNUPLOTREPORT_HPP_
#define GNUPLOTREPORT_HPP_

#include "IReport.hpp"

class GnuplotReport : public IReport
{
public:

  ///
  /// Initializes the report
  /// @param proc The process being reported on
  ///
  GnuplotReport(IChildProcess * const proc) :
    IReport(proc, "gnuplot")
  { }

  ///
  /// Destructor
  ///
  virtual ~GnuplotReport() {
    Finalize();
  }

  ///
  /// Opens the output file for writing
  ///
  virtual void Initialize();

  ///
  /// Updates the report with data taken from a probe
  /// @param probe Probe to read data from
  ///
  virtual void Update(IProbe * probe);

  ///
  /// Closes the output file
  ///
  virtual void Finalize();

protected:

  /// Output file stream
  std::ofstream os;

};

#endif /* GNUPLOTREPORT_HPP_ */
