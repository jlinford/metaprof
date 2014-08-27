#ifndef _ISAMPLE_HPP_
#define _ISAMPLE_HPP_

#include <sys/time.h>
#include <errno.h>
#include <string.h>

#include <stdexcept>

struct ISample
{ 
  ISample() {
    if (gettimeofday(&timestamp, NULL) == -1) {
      throw std::runtime_error(strerror(errno));
    }
  }

  timeval timestamp;
};


#endif /* _ISAMPLE_HPP_ */
