#ifndef _ICHILDPROCESS_HPP_
#define _ICHILDPROCESS_HPP_

#include <unistd.h>

#include <iostream>
#include <vector>
#include <string>

#include "IProbe.hpp"


class IChildProcess
{
public:

  typedef std::vector<IProbe*> ProbeVector;

  IChildProcess() : 
    retval_(-1),
    pid_(-1),
    freq_(100000)
  { }

  virtual ~IChildProcess()
  { }

  template < typename ProbeType >
  void AddProbe() {
    IProbe * probe = new ProbeType(this);
    probes_.push_back(probe);
  }

  ProbeVector const & probes() const {
    return probes_;
  }

  useconds_t freq() const {
    return freq_;
  }

  void set_freq(useconds_t freq) {
    freq_ = freq;
  }

  int retval() const {
    return retval_;
  }

  int pid() const {
    return pid_;
  }

  virtual void Measure() {
    for(ProbeVector::iterator it = probes_.begin(); it != probes_.end(); it++) {
      (*it)->Measure();
    }
  }

  virtual int Create(int argc, char ** argv) = 0;

  virtual void ReportToCSVFile(char const * tag=NULL) = 0;

protected:

  // Resource usage probes
  ProbeVector probes_;

  // Child process executable name
  std::string exe_name_;

  // Child process return value
  int retval_;

  // Child process identifier
  int pid_;

  // Microseconds between samples for all probes
  useconds_t freq_;

};


#endif /* _ICHILDPROCESS_HPP_ */

