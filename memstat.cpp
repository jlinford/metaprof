#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#include "ResourceUsage.hpp"

// Timer frequency
#define TIMER_US 100000

using namespace std;

static int child_main(char const * cmd, char ** argv)
{
  int retval = execvp(cmd, argv);
  if (retval = -1) {
    retval = errno;
    cerr << strerror(retval) << endl;
  } else {
    retval = 0;
  }
  return retval;
}

static int parent_main(int child_pid)
{
  int status;
  int retval;
  ResourceUsage usage(RUSAGE_SELF);

  // Wait for child to complete
  while (true) {
    usage.measure();
    retval = waitpid(child_pid, &status, WNOHANG);
    if (retval == child_pid) {
      // Child has exited.  Get exit status.
      if (WIFEXITED(status)) {
        retval = WEXITSTATUS(status);
        cout << "Child exited with status " << retval << endl;
        break;
      } else if (WIFSIGNALED(status)) {
        int signum = WTERMSIG(status);
        cerr << "Child process terminated by signal " << signum << endl;
        retval = -signum;
        break;
      }
    } else if (retval == -1) {
      // waitpid has failed
      retval = errno;
      cerr << strerror(retval) << endl;
    } 
    // Child still executing, wait then check again
    usleep(TIMER_US);
  }

  cout << usage << endl;
  return retval;
}

int main(int argc, char ** argv)
{
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " cmd [args...]" << endl;
    return 0;
  }

  int retval = -1;
  int pid = fork();
  if (pid == -1) {
    cerr << "Failed to fork child process" << endl;
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    retval = child_main(argv[1], argv+2);
  } else {
    retval = parent_main(pid);
  }

  return retval;
}
