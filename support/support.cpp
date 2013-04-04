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

#define _CRT_SECURE_NO_WARNINGS

// Support include
#include "support.h"

// C++ includes
#include <cstdio>
#include <iostream>
  using std::istream;
  using std::ostream;
#include <memory>
  using std::unique_ptr;
#include <string>
  using std::string;
  using std::wstring;
#include <vector>
  using std::vector;

// Platform includes
#ifdef _WIN32
#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#ifdef __GLIBCXX__
#include <ext/stdio_filebuf.h>
#endif
#endif

namespace support
{
  file::file(const string& filename, access mode)
#ifdef _WIN32
  : handle(_wfopen(convert_to_utf16(filename).c_str(), mode==access::read ? L"rb" : mode==access::write ? L"wb" : L"rwb"))
#else
  : handle(fopen(filename.c_str(), mode==access::read ? "rb" : mode==access::write ? "wb" : "rwb"))
#endif
  {
    if(handle == nullptr)
      throw std::runtime_error("Failed to open file: " + filename);
  }
  file::file(FILE* handle)
  : handle(handle)
  {   }

  file standard_input = file(stdin);
  file standard_output = file(stdout);
  file standard_error = file(stderr);

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
  char** commandline_arguments(int& argc, char*[])
  {
    // storage
    static vector<string> arguments;
    arguments.reserve(argc);
    static vector<char*> argv;
    argv.reserve(argc);

    wchar_t** wargv = CommandLineToArgvW(GetCommandLineW(), &argc);

    if(wargv == nullptr)
      throw; //TODO

    for(int i=0; i<argc; ++i)
    {
      arguments.push_back(convert_to_utf8(wargv[i]));
      argv.push_back(const_cast<char*>(arguments.back().c_str()));
    }
    return argv.data();
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
  char** commandline_arguments(int& argc, char* argv[])
  {
    return argv
  }
#endif
  template<>
  void print<vector<string>>(const vector<string>& stuff)
  {
  for(auto&& item : stuff)
    print(item);
  }
} // namespace support
