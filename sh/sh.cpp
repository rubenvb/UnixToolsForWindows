/**
 * Unix Tools For Windows
 *  - sh: shell, the standard command language interpreter
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

// Support includes
#include "support.h"

/* code taken from http://www.aihara.co.jp/~taiji/tp/?id=26 */
#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
//#define BOOST_SPIRIT_DEBUG
 #define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/spirit/include/support_utree.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace qi = boost::spirit::qi;
namespace spirit = boost::spirit;

template <typename Iterator>
struct sh_parser : qi::grammar<Iterator, spirit::utree(), qi::locals<std::string> > {
  qi::rule<Iterator, spirit::utree(), qi::locals<std::string> > top,
  variable_name,
  single_quoted_string,
  double_quoted_string,
  unquoted_string,
  command_name,
  command_arguments,
  argument,
  arguments,
  assignment,
  assignments,
  pipe_here,
  io_here,
  name,
  word,
  wordlist,
  filename,
  separator_op,
  separator,
  sequential_sep,
  pipeline,
  and_or,
  list,
  term,
  compound_list,
  io_redirect,
  io_file,
  do_group,
  for_clause,
  case_item, case_item_ns, pattern,
  case_clause,
  if_clause,
  compound_command,
  function_definition,
  command,
  nl,
  nl_list,
  comment;
  qi::rule<Iterator, std::string()> here_end, quoted_here_end, unquoted_here_end;
  qi::rule<Iterator, spirit::utree()> IO_NUMBER;
  qi::rule<Iterator, spirit::utree()> AMP, VLINE, SEMI;
  qi::rule<Iterator, spirit::utree()> AND_IF, OR_IF, DSEMI;
  qi::rule<Iterator, spirit::utree()> LESS, GREAT, DLESS, DGREAT, LESSAND, GREATAND, LESSGREAT, CLOBBER, ANDGREAT, MINUS;
  qi::rule<Iterator, spirit::utree()> If, Then, Elif, Else, Fi, For, While, Until, Do, Done, Case, Esac, In, Lparen, Rparen, Lbrace, Rbrace, Parens, Bang;
  sh_parser() : sh_parser::base_type(top)
  {
    IO_NUMBER = +qi::digit;

    AMP = '&';
    VLINE = '|';
    SEMI = ';';

    AND_IF = qi::as<std::string>()["&&"];
    OR_IF = qi::as<std::string>()["||"];
    DSEMI = qi::as<std::string>()[";;"];

    LESS = '<';
    GREAT = '>';
    DLESS = qi::as<std::string>()["<<"];
    DGREAT = qi::as<std::string>()[">>"];
    LESSAND = qi::as<std::string>()["<&"];
    GREATAND = qi::as<std::string>()[">&"];
    LESSGREAT = qi::as<std::string>()["<>"];
    CLOBBER = qi::as<std::string>()[">|"];
    ANDGREAT = qi::as<std::string>()["&>"];
    MINUS = '-';

    If = qi::as<std::string>()["if"];
    Then = qi::as<std::string>()["then"];
    Elif = qi::as<std::string>()["elif"];
    Else = qi::as<std::string>()["else"];
    Fi = qi::as<std::string>()["fi"];
    For = qi::as<std::string>()["for"];
    While = qi::as<std::string>()["while"];
    Until = qi::as<std::string>()["until"];
    Do = qi::as<std::string>()["do"];
    Done = qi::as<std::string>()["done"];
    Case = qi::as<std::string>()["case"];
    Esac = qi::as<std::string>()["esac"];
    In = qi::as<std::string>()["in"];
    Lparen = '(';
    Rparen = ')';
    Lbrace = '{';
    Rbrace = '}';
    Parens = qi::as<std::string>()["()"];
    Bang = '!';

    top = *compound_list;

    variable_name = qi::as_string[(qi::ascii::alpha | qi::char_('_')) >> *(qi::alnum | qi::char_('_'))];
    single_quoted_string = qi::as_string[qi::char_("'") >> *~qi::char_("'") >> qi::char_("'")];
    double_quoted_string = qi::as_string[qi::char_('"') >> *(
    (qi::lit('\\') >> qi::char_) | (!qi::lit('\\') >> ~qi::lit('"'))
    ) >>
    qi::char_('"')];
    unquoted_string = qi::as_string[+~qi::char_("\t\n \"#&'();<=>\\`|")];

    command_name = +(single_quoted_string | double_quoted_string | unquoted_string) -
    (/*Bang | */Lbrace | Rbrace/* | Lparen | Rparen*/ | Case | Do | Done | Elif | Else | Esac | Fi | For | If | In | Then | Until | While);
    command_arguments = command_name >> *(qi::omit[+qi::ascii::blank] >> argument);

    argument = +(single_quoted_string | double_quoted_string | unquoted_string);
    arguments = argument >> *(qi::omit[+qi::ascii::blank] >> argument);

    assignment = variable_name >> '=' >> -argument;
    assignments = assignment >> *(!qi::eps | qi::omit[+qi::ascii::blank] >> assignment);

    pipe_here = qi::skip(qi::ascii::blank)[VLINE] >> *pipeline;

    quoted_here_end = '\'' >> qi::as_string[(+(qi::char_ - '\''))][qi::labels::_val = qi::labels::_1] >> '\'';
    unquoted_here_end %= qi::as_string[(+(qi::char_ - qi::ascii::space))];
    here_end %= quoted_here_end | unquoted_here_end;

    io_here %= qi::skip(qi::ascii::blank)[DLESS] >> qi::matches[MINUS] >>
    here_end[qi::labels::_a = qi::labels::_1] >> -pipe_here >> nl >>
    (*(qi::as_string[+(qi::char_ - (qi::eol >> qi::string(qi::labels::_a) >> qi::eol))]) >>
    qi::omit[qi::eol >> qi::string(qi::labels::_a)] |
    qi::omit[qi::string(qi::labels::_a)]);

    name = qi::omit[+qi::ascii::blank] >> variable_name;
    word = qi::omit[+qi::ascii::blank] >> argument;
    wordlist = qi::omit[+qi::ascii::blank] >> arguments;
    filename = qi::omit[*qi::ascii::blank] >> argument;

    separator_op = qi::skip(qi::ascii::blank)[AMP | !DSEMI >> SEMI];
    separator = (separator_op >> *nl | nl_list);
    sequential_sep = qi::skip(qi::ascii::blank)[/*!DSEMI >> */SEMI] >> *nl | nl_list;

    pipeline = -Bang >> command %
    (qi::skip(qi::ascii::blank)[VLINE] >> *nl);
    and_or = pipeline % (qi::skip(qi::ascii::blank)[AND_IF | OR_IF] >> *nl);
    list = and_or % separator_op;
    term = and_or % separator;
    compound_list = *nl >> term >> -separator;

    io_redirect = -IO_NUMBER >> (io_file | io_here);
    io_file = qi::skip(qi::ascii::blank)[LESS | LESSAND | GREAT | GREATAND | DGREAT | LESSGREAT | CLOBBER] >> filename;

    do_group = qi::skip(qi::ascii::space)[Do] >> compound_list >> qi::skip(qi::ascii::space)[Done];
    for_clause = qi::skip(qi::ascii::blank)[For] >> name >>
    (SEMI >> *nl | *nl >> -(qi::skip(qi::ascii::blank)[In] >> -wordlist >> -sequential_sep)) >> do_group;
    pattern = argument >> *(qi::omit[VLINE] >> argument);

    case_item = !qi::eps | -qi::skip(qi::ascii::blank)['('] >> pattern >> qi::lit(')') >>
    (compound_list | *nl) >> qi::skip(qi::ascii::space)[DSEMI] >> *nl;
    case_item_ns = !qi::eps | -qi::skip(qi::ascii::blank)['('] >> pattern >> qi::lit(')') >>
    -compound_list >> *nl;

    case_clause = qi::skip(qi::ascii::blank)[Case] >> word >> qi::omit[+qi::ascii::blank] >> *nl >>
    qi::skip(qi::ascii::space)[In] >> *nl >>
    *case_item >> -case_item_ns >>
    qi::skip(qi::ascii::space)[Esac];

    if_clause = qi::skip(qi::ascii::blank)[If] >> compound_list >>
    qi::skip(qi::ascii::blank)[Then] >> compound_list >>
    *(
    qi::skip(qi::ascii::blank)[Elif] >> compound_list >>
    qi::skip(qi::ascii::blank)[Then] >> compound_list
    ) >>
    -(
    qi::skip(qi::ascii::blank)[Else] >> compound_list
    ) >>
    qi::skip(qi::ascii::space)[Fi];

    compound_command =
    qi::skip(qi::ascii::space)[Lbrace] >> compound_list >> qi::skip(qi::ascii::space)[Rbrace] |
    qi::skip(qi::ascii::space)[Lparen] >> compound_list >> qi::skip(qi::ascii::space)[Rparen] |
    for_clause |
    case_clause |
    if_clause |
    qi::skip(qi::ascii::blank)[While] >> compound_list >> do_group |
    qi::skip(qi::ascii::blank)[Until] >> compound_list >> do_group;
    function_definition =
    variable_name >> *qi::ascii::blank >> Parens >> *nl >> compound_command;
    command = qi::omit[*qi::ascii::blank] >>
    (
    function_definition |
    (assignments >> command_arguments | assignments | command_arguments) |
    compound_command
    ) >> *io_redirect;

    nl = qi::omit[*comment] >> qi::eol;
    nl_list = +(qi::omit[*comment] >> qi::eol);
    comment = qi::as_string[qi::skip(qi::ascii::blank)['#'] >> *(~qi::char_("\r\n"))];

    BOOST_SPIRIT_DEBUG_NODE(top);
  }
};

int main(int argc, char* argv[])
{
  sh_parser<std::string::const_iterator> sh_parser;
  if(argc < 2)
  {
    std::cerr << "Please call sh.exe with a file to read.\n";
    return -1;
  }
  std::ifstream file(argv[1]);
  if(!file)
  {
    std::cerr << "Could not find file " + std::string(argv[1]) << ".\n";
    return 1;
  }
  std::string str((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
  {
    std::string::const_iterator it = str.begin(), end = str.end();
    spirit::utree ut;
    bool r = phrase_parse(it, end, sh_parser, qi::ascii::space/*, qi::skip_flag::dont_postskip*/, ut);
    std::cout << str << std::endl;
    if (r && it == end) {
      std::cout << "succeeded:\t" << ut << std::endl;
    }
    else {
      std::cout << "failed:\t" << std::string(it, end) << std::endl;
    }
  }
  return 0;
}
