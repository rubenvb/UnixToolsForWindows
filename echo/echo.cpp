/**
 * Unix Tools For Windows
 *  - echo: write arguments to standard output
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

// Windows includes
#include <windows.h>

// C++ includes
#include <iostream>
#include <string>
  using std::wstring;

int main()
{
  int argc;
  wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
  HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);

  for(int i=1; i<argc; ++i)
  {
    std::wstring arg(argv[i]);
    // Warning: failure is not detected.
    WriteConsoleW(std_out, arg.c_str(), static_cast<DWORD>(arg.size()), NULL, NULL);
    WriteConsoleW(std_out, L" ", 1, NULL, NULL);
  }
  WriteConsoleW(std_out, L"\n", 1, NULL, NULL);
}
