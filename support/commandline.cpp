/**
 * Unix Tools For Windows
 *  - support: library providing common functionality including.
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

// Support include
#include "support.h"

// C++ includes
#include <string>
  using std::string;

// Platform includes
#ifdef _WIN32
#include <windows.h>
#endif

namespace support
{
  namespace commandline
  {
    const string arguments()
    {
#ifdef _WIN32
      int argc;
      wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
      if(argv == nullptr)
        throw; //TODO

      string arguments;
      for(int i=0; i<argc-1; ++i)
      {
        arguments.append(convert_to_utf8(argv[i]));
        arguments.append(' ', 1);
      }
#else
#error unimplemented
#endif
    }
  }
}
