/**
 * Unix Tools For Windows
 *  - cat: concatenate and print files
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

// Boost includes
#include <boost/program_options.hpp>
  namespace po =  boost::program_options;

// C++ includes
#include <array>
  using std::array;
#include <iostream>
  using std::cout;
#include <string>
  using std::string;
#include <vector>
  using std::vector;

// Windows include
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

int main(int argc, char* argv[])
try
{
// Set binary I/O
  // calling POSIX freopen seems to have strange side effects...
  // http://pubs.opengroup.org/onlinepubs/9699919799/functions/freopen.html
#ifdef _WIN32
  setmode(fileno(stdout), _O_BINARY);
  setmode(fileno(stdin), _O_BINARY);
#endif

  argv = support::commandline_arguments(argc, argv);

  std::vector<std::string> input;
  po::options_description options("cat [-u] [file...]\nArguments");
  options.add_options()("help", "Show help output.")
                       (",u", "Write bytes from the input file to the standard output without delay as each is read.");
  po::options_description hidden("Hidden options");
  hidden.add_options()("file", po::value(&input), "input");
  po::options_description all("All options");
  all.add(options).add(hidden);

  po::positional_options_description file_options;
  file_options.add("files", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(all).positional(file_options).run(), vm);
  po::notify(vm);

  if(vm.count("help"))
  {
    std::cout << "cat: concatenate and print files\n"
                 "Usage: cat [file...]\n"
              << options
              << "Operands:\n"
                 "  file...\t\tA pathname of an input file. If no file operands are specified, the standard input shall be used. If a file is \'-\', the cat utility shall read from the standard input at that point in the sequence. The cat utility shall not close and reopen standard input when it is referenced in this way, but shall accept multiple occurrences of \'-\' as a file operand.";
    return 0;
  }

  size_t buffer_size = 1024;
  if(vm.count("u"))
    buffer_size = 1;

  vector<char> buffer;
  buffer.resize(buffer_size);
  for(auto&& filename : input)
  {
    if(filename == "-")
    {
      while(true)
      {
        size_t bytes_read = support::standard_input.read_some(buffer_size, &buffer[0]);
        support::standard_output.write(bytes_read, buffer.data());
        if(bytes_read < buffer_size)
          break;
      }
    }
    else
    {
      support::file current(filename, support::file::access::read);

      while(true)
      {
        size_t bytes_read = current.read_some(buffer_size, &buffer[0]);
        support::standard_output.write(bytes_read, buffer.data());
        if(bytes_read < buffer_size)
          break;
      }
    }
  }
}
catch(std::exception& e)
{
  std::cerr << e.what();
  return 1; //TODO
}
