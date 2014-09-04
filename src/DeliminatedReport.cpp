/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * DeliminatedReport definition
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

#include <cstdlib>

#include "IChildProcess.hpp"
#include "ISample.hpp"
#include "DeliminatedReport.hpp"

using namespace std;

void DeliminatedReport::Initialize()
{
  if (!os.is_open()) {
    string const fname = proc_->BuildFilename("csv", name_.c_str());
    os.open(fname.c_str());
    if (!os) {
      cerr << "Failed to open '" << fname << "' for write" << endl;
      exit(EXIT_FAILURE);
    }
  }
}

void DeliminatedReport::Update(IProbe * probe)
{
  static bool first_write = true;

  IProbe::SampleBuffer const & samples = probe->samples();

  for (int i=0; i<samples.size(); ++i) {
    ISample::FieldVector fields = samples[i]->PackageFields();

    if (first_write) {
      // Write headers on first write
      for (ISample::FieldVector::iterator it = fields.begin(); it != fields.end(); it++) {
        string const & field_name = it->first;
        os << field_name << delim_;
      }
      os << endl;
      first_write = false;
    } // first write

    for (ISample::FieldVector::iterator it = fields.begin(); it != fields.end(); it++) {
      string const & value = it->second;
      os << value << delim_;
    }
    os << endl;
  } // has_samples
}

void DeliminatedReport::Finalize()
{
  if (os.is_open()) {
    os.close();
  }
}
