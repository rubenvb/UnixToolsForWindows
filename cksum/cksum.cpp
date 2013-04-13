/**
 * Unix Tools For Windows
 *  - cksum: write file checksums and sizes
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
#include <boost/crc.hpp>
  typedef boost::crc_optimal<32, 0x04C11DB7, 0, 0xFFFFFFFF, false, false> cksum_crc_type;
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
  namespace po = boost::program_options;

// C++ includes
#include <array>
  using std::array;
#include <cstddef>
  using std::size_t;
#include <iostream>
#include <string>
  using std::string;

int main(int argc, char* argv[])
try
{
  argv = support::commandline_arguments(argc, argv);

  std::vector<std::string> input;
  po::options_description options("Options");
  options.add_options()("help", "Show this help output.");

  po::options_description operands("Operands");
  operands.add_options()("files", po::value(&input), "input");

  po::options_description options_operands;
  options_operands.add(options).add(operands);

  po::positional_options_description file_options;
  file_options.add("files", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(options_operands).positional(file_options).run(), vm);
  po::notify(vm);

  if(vm.count("help"))
  {
    std::cout << "cksum: write file checksums and sizes\n"
                 "Usage: cksum [file...]\n"
              << options
              << "Operands:\n"
                 "  file...\t\tA pathname of a file to be checked. If no file operands are specified, the standard input shall be used.";
    return 0;
  }

  const std::size_t buffer_size = 1024;
  array<char, buffer_size> buffer;
  if(argc == 1)
  {
    size_t octets = 0;
    cksum_crc_type crc;
    while(true)
    {
      size_t bytes_read = support::standard_input.read_some(buffer_size, buffer.data());
      octets += bytes_read;
      crc.process_bytes(buffer.data(), bytes_read);
      if(bytes_read < buffer_size)
        break;
    }
    support::print(boost::lexical_cast<string>(crc.checksum()) + " " + boost::lexical_cast<string>(octets));
    return 0;
  }
  for(int i = 1; i<argc; ++i)
  {
    support::file current(argv[i], support::file::access::read);
    size_t octets = 0;
    cksum_crc_type crc;
    while(true)
    {
      size_t bytes_read = current.read_some(buffer_size, buffer.data());
      octets += bytes_read;
      crc.process_bytes(&buffer[0], bytes_read);
      if(bytes_read < buffer_size)
        break;
    }
    if(i>1)
      support::print("\n");

    support::print(boost::lexical_cast<string>(crc.checksum()) + " " + boost::lexical_cast<string>(octets) + " " + argv[i]);
  }
}
catch(const std::exception& e)
{
  std::cerr << e.what();
  return 1; //TODO
}
