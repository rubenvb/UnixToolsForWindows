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
#include <iostream>
  using std::cout;
#include <string>
  using std::string;
#include <vector>
  using std::vector;

int main(int argc, char* argv[])
try
{
  //argv = support::commandline_arguments(argc, argv);


  std::vector<std::string> input;
  po::options_description options("Options");
  options.add_options()("-u", po::value<bool>(), "Write bytes from the input file to the standard output without delay as each is read.")
                       ("file", po::value(&input), "input");

  po::positional_options_description file_options;
  file_options.add("file", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(options).positional(file_options).run(), vm);
  po::notify(vm);

  size_t buffer_size = 1024;
  if(vm.count("-u"))
    buffer_size = 1;

  vector<char> buffer;
  buffer.reserve(buffer_size);
  for(auto&& filename : input)
  {
    if(filename == "-")
    {
      size_t bytes_read = support::standard_input.read_some(buffer_size, buffer);
      support::standard_output.write(bytes_read, buffer);
    }
    else
    {
      support::file current(filename, support::file::access::read);

      while(true)
      {
        size_t bytes_read = current.read_some(buffer_size, buffer);
        support::standard_output.write(bytes_read, buffer);
        if(bytes_read < buffer_size)
          break;
      }
    }
  }
}
catch(std::exception& e)
{
  std::cerr <<  e.what();
}
