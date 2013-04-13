/**
 * Unix Tools For Windows
 *  - cmp: compare two files
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
  namespace po = boost::program_options;

// C++ includes
#include <cstddef>
  using std::size_t;
#include <stdexcept>
  using std::exception;
#include <string>
  using std::string;

int main(int argc, char* argv[])
try
{
  argv = support::commandline_arguments(argc, argv);

  string file1;
  string file2;
  bool byte_info = false;
  bool exit_code_only = false;

  po::options_description options("Options");
  options.add_options()("help", "Show this help output.")
                       (",l", "(Lowercase ell.) Write the byte number (decimal) and the differing bytes (octal) for each difference.")
                       (",s", "Write nothing for differing files; return exit status only.");

  po::positional_options_description operands;
  operands.add("file1", 1);//, "A pathname of the first file to be compared. If file1 is '-', the standard input shall be used.")
  operands.add("file2", 1);//, "A pathname of the second file to be compared. If file2 is '-', the standard input shall be used.");

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(options).positional(operands).run(), vm);
  po::notify(vm);

  if(vm.count("help"))
  {
    std::cout << "cmp: compare two files\n"
                 "Usage: cmp [ -l | -s ] file1 file2\n"
              << options
              << "Operands:\n"
                 "  file1\t\tA pathname of the first file to be compared. If file1 is \'-\', the standard input shall be used.\n"
                 "  file2\t\tA pathname of the second file to be compared. If file2 is \'-\', the standard input shall be used.\n"
                 " If both file1 and file2 refer to standard input or refer to the same FIFO special, block special, or character special file, the results are undefined.\n";
    return 0;
  }
}
catch(const exception& e)
{
  std::cerr << e.what();
  return 1; //TODO
}
