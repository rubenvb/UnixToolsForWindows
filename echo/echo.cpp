/**
 * Unix Tools For Windows
 *  - date: write the date and time
 *
 * Written in 2013 by Ruben Van Boxem <vanboxem.ruben@gmail.com>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright and related
 * and neighboring rights to this software to the public domain worldwide. This software is
 * distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along with this software.
 * If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 *
 */

// Boost includes
#include <boost/program_options.hpp>
  namespace po = boost::program_options;

// C++ includes
#include <iostream>

//Platform includes
#include <io.h>
#include <fcntl.h>
#include <windows.h>

int main()
{
  int argc;
  wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
  HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD cChars;
  if(argc > 0)
  {
    for(int i=1; i<argc-1; ++i)
    {
      WriteConsoleW(hstdout, argv[i], DWORD(wcslen(argv[i])), &cChars, NULL);
    }
    WriteConsoleW(hstdout, argv[argc-1], DWORD(wcslen(argv[argc-1])), &cChars, NULL);
  }
}
