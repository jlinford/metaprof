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


//
// Use fork/exec to launch the child process
// 
int ForkExecChild::Create(int argc, char ** argv)
{
  // Record name of child executable as seen on command line
  exe_name_ = argv[1];

  // Mark time of child process creation
  gettimeofday(&start_time_, NULL);

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
    // Mark time of child process completion
    gettimeofday(&end_time_, NULL);
    DeactivateProbes();
    return retval_;
  }
  // Something unexpected happened
  retval_ = -1;
  throw runtime_error("Unexpected exit path from ForkExecChild::Create");
  // Keep the compiler happy
  return retval_;
}

void ForkExecChild::ReportToCSVFile(char const * tag)
{
  ostringstream buff;
  buff << exe_name_.substr(exe_name_.find_last_of("/\\")+1) << ".";
  if (tag) {
    buff << tag << ".";
  }
  buff << "csv";

  timeval diff;
  timersub(&end_time_, &start_time_, &diff);
  double runtime = (diff.tv_sec * 1e6 + diff.tv_usec) / 1e6;

  // Open CSV file for write
  ofstream os(buff.str().c_str());

  // Write summary
  os << "Executable,Runtime (s)," << endl;
  os << exe_name_ << ',';
  os << runtime << ',';
  os << endl;

  // Separate summary from probe table by an empty field
  os << ',' << endl;

  // Write probe tables
  for(ProbeVector::const_iterator it=probes_.begin(); it!=probes_.end(); it++) {
    (*it)->WriteDeliminated(os);
    os << endl;
  }

  // Flush and close file
  os.close();
}

