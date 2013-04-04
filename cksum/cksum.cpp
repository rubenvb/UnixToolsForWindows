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
#include <boost/program_options.hpp>
  namespace po = boost::program_options;

int main(int argc, char* argv[])
{
  argv = support::commandline_arguments(argc, argv);

  std::vector<std::string> input;
  po::options_description options("Options");
  options.add_options()("file", po::value(&input), "input");

  po::positional_options_description file_options;
  file_options.add("file", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(options).positional(file_options).run(), vm);
  po::notify(vm);
}
