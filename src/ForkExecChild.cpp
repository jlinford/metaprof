/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * ForkExecChild member definitions.
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
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#include "ForkExecChild.hpp"
#include "IProbe.hpp"

using namespace std;

int ForkExecChild::Create(int argc, char ** argv)
{
  // Record name of child executable as seen on command line
  exe_name_ = argv[1];

  // Mark time of child process creation
  runtime_.Start();

  pid_ = fork();
  if (pid_ == -1) {
    throw runtime_error("Failed to fork child process");
  } else if (pid_ == 0) {
    // Child process executes normally
    retval_ = execvp(argv[1], argv+1);
    if (retval_ == -1) {
      retval_ = errno;
      cerr << "execvp failed: " << strerror(retval_) << endl;
    }
    return retval_;
  } else {
    // Nasty hack: Skip the first sample
    usleep(freq_);

    // Parent process waits for child to complete
    ActivateProbes();
    while (true) {
      Measure();
      int status;
      int code = waitpid(pid_, &status, WNOHANG);
      if (code == pid_) {
        if (WIFEXITED(status)) {
          // Child exited normally, return child process exit code
          retval_ = WEXITSTATUS(status);
          break;
        } else if (WIFSIGNALED(status)) {
          // Child terminated by signal, return signal number negated
          retval_ = -(WTERMSIG(status));
          break;
        }
      } else if (code == -1) {
        // waitpid failed
        retval_ = errno;
        throw runtime_error(strerror(retval_));
      } else { 
        // Child still executing, wait then check again
        usleep(freq_);
      }
    }
    runtime_.Stop();
    DeactivateProbes();
    return retval_;
  }
  // Something unexpected happened
  retval_ = -1;
  throw runtime_error("Unexpected exit path from ForkExecChild::Create");
  // Keep the compiler happy
  return retval_;
}

void ForkExecChild::PrintSummary()
{
  // Print probe summaries
  for(ProbeVector::const_iterator it=probes_.begin(); it!=probes_.end(); it++) {
    (*it)->WriteSummary(cout);
    cout << endl;
  }
  cout << "Runtime (s): " << runtime().Seconds() << endl;
}
