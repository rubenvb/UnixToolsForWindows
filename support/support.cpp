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
#include <vector>
  using std::vector;

// Platform includes
#ifdef _WIN32
#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#endif

namespace support
{
#ifdef _WIN32
  const string convert_to_utf8(const wstring& utf16_string)
  {
    int length = WideCharToMultiByte(CP_UTF8, 0, utf16_string.c_str(), static_cast<int>(utf16_string.size()), nullptr, 0, nullptr, nullptr);
    if(!(length > 0))
      return "";
    else
    {
      std::string result;
      result.resize(static_cast<std::string::size_type>(length));

      if(WideCharToMultiByte(CP_UTF8, 0, utf16_string.c_str(), static_cast<int>(utf16_string.size()),
                             &result[0], static_cast<int>(result.size()), nullptr, nullptr) == 0 )
        throw; //TODO
      else
        return result;
    }
  }
  const wstring convert_to_utf16(const string& utf8_string)
  {
    int length = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), static_cast<int>(utf8_string.size()), nullptr, 0);
    if(!(length > 0))
      return L"";
    else
    {
      wstring result;
      result.resize(static_cast<std::wstring::size_type>(length));

      if(MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), static_cast<int>(utf8_string.size()),
                             &result[0], static_cast<int>(result.size())) == 0 )
        throw; //TODO
      else
        return result;
    }
  }
  const string commandline_arguments(int, char*[])
  {
    int argc;
    wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if(argv == nullptr)
      throw; //TODO

    string arguments;
    // skip argv[0]
    for(int i=1; i<argc-1; ++i)
    {
      arguments.append(convert_to_utf8(argv[i]));
      arguments.append(' ', 1);
    }
    arguments.append(convert_to_utf8(argv[argc-1]));
    return arguments;
  }
  template<>
  void print<string>(const string& stuff)
  {
    static HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    wstring wstuff = convert_to_utf16(stuff);
    if(WriteConsoleW(std_out, wstuff.c_str(), static_cast<DWORD>(wstuff.size()), NULL, NULL) == 0)
      throw; //TODO
  }
#else
  const string commandline_arguments(int argc, char* argv[])
  {
    string arguments;
    // skip argv[0]
    for(int i=1; i<argc-1; ++i)
    {
      arguments.append(argv[i]);
      arguments.append(' ', 1);
    }
    arguments.append(argv[argc-1]);
    return arguments;
  }
#endif
} // namespace support
