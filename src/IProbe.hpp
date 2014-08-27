#ifndef _IPROBE_HPP_
#define _IPROBE_HPP_

#include <iostream>
#include <vector>

#include "ISample.hpp"


class IChildProcess;


class IProbe
{
public:

  typedef std::vector<ISample*> SampleVector;

  IProbe(IChildProcess * const proc) : proc_(proc)
  { }

  virtual ~IProbe()
  { }

  SampleVector const & samples() const {
    return samples_;
  }

  virtual void Measure() = 0;

  virtual std::ostream & WriteDeliminated(std::ostream & os, char const d=',') const = 0;

protected:

  // The process being probed
  IChildProcess * const proc_;

  // Samples of probed process
  SampleVector samples_;

};


#endif /* _IPROBE_HPP_ */
