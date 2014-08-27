#ifndef _PROCSTATPROBE_HPP_
#define _PROCSTATPROBE_HPP_

#include "IProbe.hpp"

//
// Uses /proc/[pid]/stat to get resource usage
//
class ProcStatProbe : public IProbe
{
public:

  ProcStatProbe(IChildProcess * const proc) :
    IProbe(proc)
  { }

  virtual ~ProcStatProbe()
  { }

  virtual void Measure();

  virtual std::ostream & WriteDeliminated(std::ostream & os, char const d=',') const;

};

#endif /* _PROCSTATPROBE_HPP_ */
