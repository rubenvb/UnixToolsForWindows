/**
 * Unix Tools For Windows
 *  - echo: write arguments to standard output
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

int main(int argc, char* argv[])
{
  argv = support::commandline_arguments(argc, argv);

  if(argc >= 2)
  {
    support::print(argv[1]);
    for(int i = 2; i<argc; ++i)
      support::print(" " + string(argv[i]));

    support::print("\n");
  }
}
