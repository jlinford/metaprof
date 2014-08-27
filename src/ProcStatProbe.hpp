#ifndef _PROCSTATPROBE_HPP_
#define _PROCSTATPROBE_HPP_

#include "StatRecord.hpp"
#include "IProbe.hpp"

//
// Uses /proc/[pid]/stat to get resource usage
//
class ProcStatProbe : public IProbe
{
public:

  ProcStatProbe(IChildProcess * const proc) :
    IProbe(proc)
  {
    if (gettimeofday(&t0_, NULL) == -1) {
      throw std::runtime_error(strerror(errno));
    }
  }

  virtual ~ProcStatProbe()
  { }

  virtual void Activate() {
    initial_stat_.Read(proc_->pid());
  }

  virtual void Deactivate() {
    // Nothing to do
  }

  virtual void Measure();

  virtual std::ostream & WriteDeliminated(std::ostream & os, char const d=',') const;

private:

  // First known time for timestamp adjustment
  timeval t0_;
  // First recorded stat record
  StatRecord initial_stat_;

};

#endif /* _PROCSTATPROBE_HPP_ */
