UnixToolsForWindows
===================

Native Windows implementations of common Unix utilities.

The interfaces implemented here are documented in the Single Unix Specification available online here:
http://pubs.opengroup.org/onlinepubs/9699919799/

See the toplevel CMakeLists.txt file for the proposed split of utilities:
 - DEVELOPMENT/FORTRAN extensions are currently out of the scope of this project
 - utilities better implemented in the shell interpreter sh, which will be part of this project when I know enough Boost to do something useful with it.
 - utilities that in themselves are quite extensive. Ports of the GNU utilities might already exist. Anyone willing to write some of these is welcome to do so.
 - utilities to be written. Most are commented out, some are not. The uncommented ones are planned for the near future or immediate development.
NOTE: I would like to start with "echo" and "date", as these seem nice starting points to get to know Boost.ProgramOptions and Boost.DateTime myself.

Any help is much appreciated.

Simple code guidelines:
 - Avoid using the raw Win32 API where possible. How awesome would it be to have a full set of POSIX utilities implemented on top of Boost?
 - look at already written code to figure out the style. Basically:
   - no hungarian notation,
   - opening and closing braces at matching columns, no Egyptian style braces.
   - single statement after if/else does not need braces.
   - loops always require braces
   - snake_case,
   - tab size of 2 spaces,
   - functions with more than one argument will have the arguments listed vertically as is common in Win32 API.
   - Includes ordered: local, Boost, C++
   - use C++11 where applicable. Acceptable level is MSVS2012 November CTP+ (this includes initializer_list support, although the MSVS STL is missing every single constructor for them)

DISCLAIMER: this project's source code is placed under the CC0 public domain dedication.
