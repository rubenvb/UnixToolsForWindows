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

#ifndef UNIXTOOLSFORWINDOWS_SUPPORT_H
#define UNIXTOOLSFORWINDOWS_SUPPORT_H

// C++ includes
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace support
{
  // because Unicode, and because binary I/O.
  class file
  {
  public:
    enum class access { read, write, readwrite };
    file(const std::string& filename,
         access mode);
    file(FILE* handle);

    std::size_t read_some(const std::size_t bytes_to_read,
                          char* buffer)
    {
      return std::fread(buffer, sizeof(char), bytes_to_read, handle);
    }
    std::size_t write(const std::size_t bytes_to_write,
                      char* buffer)
    {
      return std::fwrite(buffer, sizeof(char), bytes_to_write, handle);
    }

  private:
    FILE* handle; // C file handle
  };

  extern file standard_input;
  extern file standard_output;
  extern file standard_error;

  char** commandline_arguments(int& argc, char* argv[]);

  template<typename T> // backup for laziness
  void print(const T& stuff)
  {
    std::cout << stuff;
  }
  template<>
  void print(const std::vector<std::string>& stuff);
#ifdef _WIN32
  const std::string convert_to_utf8(const std::wstring& utf16_string);
  const std::wstring convert_to_utf16(const std::string& utf8_string);

  template<>
  void print(const std::string& stuff);
#endif
} // namespace support

#endif
