#ifndef _FORKEXECHILD_HPP_
#define _FORKEXECHILD_HPP_

#include "IChildProcess.hpp"

class ForkExecChild : public IChildProcess
{
public:

  virtual ~ForkExecChild()
  { }
  
  int Create(int argc, char ** argv);

  virtual void ReportToCSVFile(char const * tag=NULL);

private:

  // Time of child process creation
  timeval start_time_;

  // Time parent process saw child process termination
  timeval end_time_;
};


#endif /* _FORKEXECHILD_HPP_ */
