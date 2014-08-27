#ifndef __RESOURCEUSAGE_H__
#define __RESOURCEUSAGE_H__

#include <iostream>
#include <vector>
#include <stdexcept>
#include <sys/resource.h>

/*
 * Records resource usage over time
 */
class ResourceUsage 
{
public:

  ResourceUsage(int target) : who(target)
  { 
    switch(target) {
      case RUSAGE_SELF: // fall through
      case RUSAGE_CHILDREN: // fall through
      case RUSAGE_THREAD: // fall through
        break;
      default:
        throw std::invalid_argument("Invalid 'who' parameter for rusage");
    }
  }

  typedef std::vector<rusage*> usage_vector_t;

  void measure();

private:

  friend std::ostream & operator<<(std::ostream & os, const ResourceUsage & x);
  std::string report_str() const;

  int who;
  usage_vector_t samples;
};

#endif /* __RESOURCEUSAGE_H__ */
