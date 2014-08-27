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
  exe_name_ = argv[1];

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
    // Parent process waits for child to complete
    while (true) {
      Measure();

      int status;
      int code = waitpid(pid_, &status, WNOHANG);
      if (code == pid_) {
        if (WIFEXITED(status)) {
          // Child exited normally, return child process exit code
          return (retval_ = WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
          // Child terminated by signal, return signal number negated
          return (retval_ = -(WTERMSIG(status)));
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

  ofstream os(buff.str().c_str());
  for(ProbeVector::const_iterator it=probes_.begin(); it!=probes_.end(); it++) {
    (*it)->WriteDeliminated(os);
    os << endl;
  }
  os.close();
}

