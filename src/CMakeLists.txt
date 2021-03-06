#
# @file
# @author John C. Linford (jlinford@paratools.com)
# @date 27 August 2014
# 
# @brief
# 
# CMake file
# 
# @copyright BSD
# @section LICENSE
# 
# *Copyright (c) 2014, ParaTools, Inc.*
# *All rights reserved.*
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
#  1. Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#  3. Neither the name of ParaTools, Inc. nor the names of its contributors may
#     be used to endorse or promote products derived from this software without
#     specific prior written permission.
# 
# **THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.**
# 

cmake_minimum_required (VERSION 2.6)

project (metaprof)
set (metaprof_VERSION_MAJOR 0)
set (metaprof_VERSION_MINOR 1)


#
# metaprof
#
add_executable (metaprof 
                metaprof.cpp
                DeliminatedReport.cpp
                ForkExecChild.cpp
                GnuplotReport.cpp
                IChildProcess.cpp
                IProbe.cpp
                IReport.cpp
                ProcessProbe.cpp
                ProcessSample.cpp
                StatRecord.cpp)
install (TARGETS metaprof DESTINATION bin)

#
# Configure a header file to pass some of the CMake settings to the source code
#
include (CheckFunctionExists)
include (CheckIncludeFile)

if (EXISTS "/proc" AND IS_DIRECTORY "/proc")
set (HAVE_PROCFS 1)
else()
set (HAVE_PROCFS 0)
endif()

if (EXISTS "/proc/self/statm")
set (HAVE_PROCFS_STATM 1)
else()
set (HAVE_PROCFS_STATM 0)
endif()

check_include_file ("unistd.h" HAVE_UNISTD_H)
check_include_file ("errno.h"  HAVE_ERRNO_H)
check_include_file ("sys/wait.h" HAVE_SYS_WAIT_H)
check_include_file ("signal.h"   HAVE_SIGNAL_H)
check_include_file ("time.h"     HAVE_TIME_H)

check_function_exists (fork   HAVE_FORK)
check_function_exists (execvp HAVE_EXECVP)
check_function_exists (usleep HAVE_USLEEP)
check_function_exists (gettimeofday HAVE_GETTIMEOFDAY)
check_function_exists (clock_gettime HAVE_CLOCK_GETTIME)

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h")
include_directories ("${PROJECT_BINARY_DIR}")

