/**
 * @file
 * @author John C. Linford (jlinford@paratools.com)
 * @date 27 August 2014
 *
 * @brief
 *
 * StatRecord declaration
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
#ifndef _STATRECORD_HPP_
#define _STATRECORD_HPP_

///
/// All the data recorded in /proc/[pid]/stat
/// see man(5) proc for details and scanf format descriptors
///
struct StatRecord
{
  void Read(int pid);

  int pid;                    ///< The process ID
  char comm[256];             ///< The filename of the executable in parentheses
  char state;                 ///< Process state
  int ppid;                   ///< The PID of the parent of this process
  int pgrp;                   ///< The process group ID of the process
  int session;                ///< The session ID of the process
  int tty_nr;                 ///< The controlling terminal of the process
  int tpgid;                  ///< The ID of the foreground process group of the controlling terminal of the process
  unsigned long flags;        ///< The kernel flags word of the process
  unsigned long minflt;       ///< Minor faults not requiring page load from disk
  unsigned long cminflt;      ///< Minor faults made by process's children
  unsigned long majflt;       ///< Major faults requiring page load from disk
  unsigned long cmajflt;      ///< Major faults made by process's children
  unsigned long utime;        ///< Clock ticks spent scheduled in user mode
  unsigned long stime;        ///< Clock ticks spent scheduled in kernel mode
  long cutime;                ///< Clock ticks process's children spent in user mode
  long cstime;                ///< Clock ticks process's children spent in kernel mode
  long priority;              ///< Process priority (man(2) setpriority)
  long nice;                  ///< Process nice value (man(2) setpriority)
  long num_threads;           ///< Number of threads in the process
  long itrealvalue;           ///< Unmaintained since 2.6.17
  unsigned long starttime;    ///< Clock ticks after system boot when the process started
  unsigned long vsize;        ///< Virtual memory size in bytes
  long rss;                   ///< Number of pages process has in real memory
  unsigned long rsslim;       ///< Soft limit in bytes on the RSS of the process
  unsigned long startcode;    ///< The address above which program text can run
  unsigned long endcode;      ///< The address below which program text can run
  unsigned long startstack;   ///< The address of the start (bottom) of the stack
  unsigned long kstkesp;      ///< Current stack pointer value
  unsigned long kstkeip;      ///< Current instruction pointer value
  unsigned long signal;       ///< (Obsolete) Bitmap of pending signals as a decimal number
  unsigned long blocked;      ///< (Obsolete) Bitmap of blocked signals as a decimal number
  unsigned long sigignore;    ///< (Obsolete) Bitmap of ignored signals as a decimal number
  unsigned long sigcatch;     ///< (Obsolete) Bitmap of caught signals as a decimal number
  unsigned long wchan;        ///< Channel in which the process is waiting
  unsigned long nswap;        ///< (Not maintained) Number of pages swapped
  unsigned long cnswap;       ///< (Not maintained) Cumulative nswap for child processes
  int exit_signal;            ///< Signal to be sent to parent when we die
  int processor;              ///< CPU number last executed on
  unsigned long rt_priority;  ///< Real-time scheduling priority
  unsigned long policy;       ///< Scheduling policy
  unsigned long long delayacct_blkio_ticks;   // Aggregated block I/O delays in clock ticks
  unsigned long guest_time;   ///< Clock ticks spent running a virtual CPU for a guest operating system
  long cguest_time;           ///< Process's children's guest_time
  unsigned long start_data;   ///< Address above which BSS data are placed
  unsigned long end_data;     ///< Address below which BSS data are placed
  unsigned long start_brk;    ///< Address above which program heap can be expanded with brk
  unsigned long arg_start;    ///< Address above which program command-line arguments are placed
  unsigned long arg_end;      ///< Address below which program command-line arguments are placed
  unsigned long env_start;    ///< Address above which program environment is placed
  unsigned long env_end;      ///< Address below which program environment is placed
  int exit_code;              ///< The thread's exit status in the form reported by waitpid
};

#endif /* _STATRECORD_HPP_ */
