/**
 * Unix Tools For Windows
 *  support: library providing common support functionality.
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
  using std::wstring;

// Platform includes
#ifdef _WIN32
#include <windows.h>
#endif

namespace support
{
#ifdef _WIN32
  const string convert_to_utf8(const wstring& utf16_string)
  {
    // get length
    int length = WideCharToMultiByte(CP_UTF8, 0, utf16_string.c_str(), static_cast<int>(utf16_string.size()),
                                     nullptr, 0, nullptr, nullptr);
    if(!(length > 0))
      return "";
    else
    {
      string result;
      result.resize(static_cast<string::size_type>(length));

      if(WideCharToMultiByte(CP_UTF8, 0, utf16_string.c_str(), static_cast<int>(utf16_string.size()),
                             &result[0], static_cast<int>(result.size()), nullptr, nullptr) == 0 )
        throw; //TODO
      else
        return result;
    }
  }
#endif
}
