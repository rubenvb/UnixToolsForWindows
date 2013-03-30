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

#ifndef UNIXTOOLSFORWINDOWS_SUPPORT
#define UNIXTOOLSFORWINDOWS_SUPPORT

// C++ includes
#include <iostream>
#include <string>

namespace support
{
  const std::string commandline_arguments(int argc, char* argv[]);

  template<typename T>
  void print(const T& stuff)
  {
    std::cout << stuff;
  }
#ifdef _WIN32
  const std::string convert_to_utf8(const std::wstring& utf16_string);
  const std::wstring convert_to_utf16(const std::string& utf8_string);

  template<>
  void print(const std::string& stuff);
#endif
} // namespace support

#endif
