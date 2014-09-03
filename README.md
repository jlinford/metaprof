metaprof
========

Who profiles the profiler?
--------------------------

This tool will help quantify TAU overhead and guide efforts to improve TAU's 
internals. It's also useful for viewing the effects of memory leaks and locking.

Requirements
-----------

 * CMake
 * Doxygen, but only if you want documentation

Installation
------------

 1. mkdir build
 2. cd build
 3. cmake ../src
 4. make -j
 5. make DESTDIR=/some/path install


27 August 2014
John C. Linford (jlinford@paratools.com)

