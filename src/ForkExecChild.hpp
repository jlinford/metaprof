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
};


#endif /* _FORKEXECHILD_HPP_ */
