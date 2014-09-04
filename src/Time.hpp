/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * Timer and Timestamp declaration
 *
 * @copyright BSD
 * @section LICENSE
 *
 * *Copyright (c) 2014, ParaTools, Inc.*
 * *All rights reserved.*
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. Neither the name of ParaTools, Inc. nor the names of its contributors may
 *     be used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * **THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.**
 */
#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "config.h"

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#include <string.h>

#include <iostream>
#include <stdexcept>


template < typename T, typename V >
class ITime
{
public:

  typedef T derived_type;
  typedef V value_type;
  typedef ITime<T, V> base_type;

  static derived_type Now() {
    return T::__Now();
  }

  ITime()
  { }

  ITime(value_type const & value) :
    value_(value)
  { }

  double Seconds() const {
    return static_cast<derived_type const*>(this)->__Seconds();
  }

  derived_type & operator+=(derived_type const & rhs) {
    return static_cast<derived_type*>(this)->__AddAssign(rhs);
  }

  derived_type & operator-=(derived_type const & rhs) {
    return static_cast<derived_type*>(this)->__SubtractAssign(rhs);
  }

  friend derived_type operator+(derived_type lhs, derived_type const & rhs) {
    return lhs += rhs;
  }

  friend derived_type operator-(derived_type lhs, derived_type const & rhs) {
    return lhs -= rhs;
  }

protected:

  value_type value_;
};


#if defined(HAVE_CLOCK_GETTIME)

typedef struct TimespecTime : public ITime<TimespecTime, struct timespec>
{
  typedef TimespecTime my_type;

  static my_type __Now() {
    my_type t;
    clock_gettime(CLOCK_REALTIME, &t.value_);
    return t;
  }

  double __Seconds() const {
    return (double)value_.tv_sec + (value_.tv_nsec / 1e9);
  }

  my_type & __AddAssign(my_type const & rhs) {
    value_.tv_sec += rhs.value_.tv_sec;
    long long nsec = value_.tv_nsec + rhs.value_.tv_nsec;
    if (nsec >= 1000000000LL) {
      value_.tv_sec++;
      value_.tv_nsec = nsec - 1000000000LL;
    }
    return *this;
  }

  my_type & __SubtractAssign(my_type const & rhs) {
    value_.tv_sec -= rhs.value_.tv_sec;
    long long nsec = value_.tv_nsec - rhs.value_.tv_nsec;
    if (nsec < 0) {
      value_.tv_sec--;
      value_.tv_nsec = nsec + 1000000000LL;
    }
    return *this;
  }
} Time;

#elif defined(HAVE_GETTIMEOFDAY)

typedef struct TimevalTime : public ITime<TimevalTime, struct timeval>
{
  typedef TimevalTime my_type;

  static my_type __Now() {
    my_type t;
    gettimeofday(&t.value_, NULL);
    return t;
  }

  double __Seconds() const {
    return (double)value_.tv_sec + (value_.tv_usec / 1e6);
  }

  my_type & __AddAssign(my_type const & rhs) {
    timeradd(&value_, &rhs.value_, &value_);
    return *this;
  }

  my_type & __SubtractAssign(my_type const & rhs) {
    timersub(&value_, &rhs.value_, &value_);
    return *this;
  }
} Time;

#else
#error No valid time implementation for this platform!
#endif


struct TimeStamp : public Time
{
  TimeStamp() : Time(Time::Now()) { }
};


class Timer
{
public:

  void Start() {
    start_ = Time::Now();
  }

  void Stop() {
    elapsed_ = Time::Now() - start_;
  }

  double Seconds() const {
    return elapsed_.Seconds();
  }

  Time const & elapsed() const {
    return elapsed_;
  }

private:

  Time start_;
  Time elapsed_;
};

#endif /* TIMER_HPP_ */
