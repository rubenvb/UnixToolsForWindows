/**
 * Unix Tools For Windows
 *  - echo: write the date and time
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

int main()
{
  int argc;
  wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
  HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD cChars;
  if(argc > 0)
  {
    for(int i=1; i<argc; ++i)
    {
      WriteConsoleW(hstdout, argv[i], DWORD(wcslen(argv[i])), &cChars, NULL);
    }
  }
}
