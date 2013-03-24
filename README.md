UnixToolsForWindows
===================

Native Windows implementations of common Unix utilities.

The interfaces implemented here are documented in the Single Unix Specification available online here:
http://pubs.opengroup.org/onlinepubs/9699919799/

See the toplevel CMakeLists.txt file for the proposed split of utilities:
 - DEVELOPMENT/FORTRAN extensions are currently out of the scope of this project
 - utilities better implemented in the shell interpreter sh, which will be part of this project when I know enough Boost to do something useful with it.
 - utilities that in themselves are quite extensive. Ports of the GNU utilities might already exist. Anyone willing to write some of these is welcome to do so.
 - utilities to be written. Most are commented out, some are not. The uncommented ones are planned for the near future or immediate development.
NOTE: I would like to start with "echo" and "date", as these seem nice starting points to get to know Boost.ProgramOptions and Boost.DateTime myself.

Any help is much appreciated.

Simple code guidelines:
 - Avoid using the raw Win32 API where possible. How awesome would it be to have a full set of POSIX utilities implemented on top of Boost?
 - look at already written code to figure out the style. Basically:
   - no hungarian notation,
   - opening and closing braces at matching columns, no Egyptian style braces.
   - single statement after if/else does not need braces.
   - loops always require braces
   - snake_case,
   - tab size of 2 spaces,
   - functions with more than one argument will have the arguments listed vertically as is common in Win32 API.
   - Includes ordered: local, Boost, C++
   - use C++11 where applicable. Acceptable level is MSVS2012 November CTP+ (this includes initializer_list support, although the MSVS STL is missing every single constructor for them)

DISCLAIMER: this project's source code is placed under the CC0 public domain dedication.

A Bash 2.0 BNF grammar which will be used in the Boost.Spirit sh implementation is the following:
(from http://books.google.be/books?id=cEtmZlLzlwsC&pg=PA307&lpg=PA307&dq=posix+sh+%22bnf%22+grammar&source=bl&ots=HQohV1rg0V&sig=cL8Iym1FRdle75du9j8n8QJXBlQ&hl=nl&sa=X&ei=6wlPUczGKIenO7S4gMgE&ved=0CCsQ6AEwADgK#v=onepage&q=posix%20sh%20%22bnf%22%20grammar&f=false)
Reserved words:
if then else elig fi case esac for while until do done function in select ! { } time
The BNF grammar:
D.2 BNF for bash
The following is the syntax of bash 2.0 in Backus−Naur Form (BNF):
<letter> ::= a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|
             A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z
<digit> ::= 0|1|2|3|4|5|6|7|8|9
<number> ::= <digit>
           | <number> <digit>
<word> ::= <letter>
         | <word> <letter>
         | <word> '_'
<word_list> ::= <word>
              | <word_list> <word>
<assignment_word> ::= <word> '=' <word>
<redirection> ::= '>' <word>
                | '<' <word>
                | <number> '>' <word>
                | <number> '<' <word>
                | '>>' <word>
                | <number> '>>' <word>
                | '<<' <word>
                | <number> '<<' <word>
                | '<&' <number>
                | <number> '<&' <number>
                | '>&' <number>
                | <number> '>&' <number>
                | '<&' <word>
                | <number> '<&' <word>
                | '>&' <word>
                | <number> '>&' <word>
                | '<<−' <word>
                | <number> '<<−' <word>
                | '>&' '−'
                | <number> '>&' '−'
                | '<&' '−'
                | <number> '<&' '−'
                | '&>' <word>
                | <number> '<>' <word>
                | '<>' <word>
                | '>|' <word>
                | <number> '>|' <word>
<simple_command_element> ::= <word>
                           | <assignment_word>
                           | <redirection>
<redirection_list> ::= <redirection>
                     | <redirection_list> <redirection>
<simple_command> ::= <simple_command_element>
                   | <simple_command> <simple_command_element>
<command> ::= <simple_command>
            | <shell_command>
            | <shell_command> <redirection_list>
<shell_command> ::= <for_command>
                  | <case_command>
                  | while <compound_list> do <compound_list> done
                  | until <compound_list> do <compound_list> done
                  | <select_command>
                  | <if_command>
                  | <subshell>
                  | <group_command>
                  | <function_def>
<for_command> ::= for <word> <newline_list> do <compound_list> done
                | for <word> <newline_list> '{' <compound_list> '}'
                | for <word> ';' <newline_list> do <compound_list> done
                | for <word> ';' <newline_list> '{' <compound_list> '}'
                | for <word> <newline_list> in <word_list> <list_terminator>
                      <newline_list> do <compound_list> done
                | for <word> <newline_list> in <word_list> <list_terminator>
                      <newline_list> '{' <compound_list> '}'
<select_command> ::= select <word> <newline_list> do <list> done
                   | select <word> <newline_list> '{' <list> '}'
                   | select <word> ';' <newline_list> do <list> done
                   | select <word> ';' <newline_list> '{' list '}'
                   | select <word> <newline_list> in <word_list>
                            <list_terminator> <newline_list> do <list> done
                   | select <word> <newline_list> in <word_list>
                            <list_terminator> <newline_list> '{' <list> '}'
<case_command> ::= case <word> <newline_list> in <newline_list> esac
                 | case <word> <newline_list> in <case_clause_sequence>
                        <newline_list> esac
                 | case <word> <newline_list> in <case_clause> esac
<function_def> ::= <word> '(' ')' <newline_list> <group_command>
                 | function <word> '(' ')' <newline_list> <group_command>
                 | function <word> <newline_list> <group_command>
<subshell> ::= '(' <compound_list> ')'
<if_command> ::= if <compound_list> then <compound_list> fi
               | if <compound_list> then <compound_list> else <compound_list> fi
               | if <compound_list> then <compound_list> <elif_clause> fi
<group_command> ::= '{' <list> '}'
<elif_clause> ::= elif <compound_list> then <compound_list>
                | elif <compound_list> then <compound_list> else <compound_list>
                | elif <compound_list> then <compound_list> <elif_clause>
<case_clause> ::= <pattern_list>
                | <case_clause_sequence> <pattern_list>
<pattern_list> ::= <newline_list> <pattern> ')' <compound_list>
                 | <newline_list> <pattern> ')' <newline_list>
                 | <newline_list> '(' <pattern> ')' <compound_list>
                 | <newline_list> '(' <pattern> ')' <newline_list>
<case_clause_sequence> ::= <pattern_list> ';;'
                         | <case_clause_sequence> <pattern_list> ';;'
<pattern> ::= <word>
            | <pattern> '|' <word>
<list> ::= <newline_list> <list0>
<compound_list> ::= <list>
                  | <newline_list> <list1>
<list0> ::= <list1> '\n' <newline_list>
          | <list1> '&' <newline_list>
          | <list1> ';' <newline_list>
<list1> ::= <list1> '&&' <newline_list> <list1>
          | <list1> '||' <newline_list> <list1>
          | <list1> '&' <newline_list> <list1>
          | <list1> ';' <newline_list> <list1>
          | <list1> '\n' <newline_list> <list1>
          | <pipeline_command>
<list_terminator> ::= '\n'
                    | ';'
<newline_list> ::=
                 | <newline_list> '\n'
<simple_list> ::= <simple_list1>
                | <simple_list1> '&'
                | <simple_list1> ';'
<simple_list1> ::= <simple_list1> '&&' <newline_list> <simple_list1>
                 | <simple_list1> '||' <newline_list> <simple_list1>
                 | <simple_list1> '&' <simple_list1>
                 | <simple_list1> ';' <simple_list1>
                 | <pipeline_command>
<pipeline_command> ::= <pipeline>
                     | '!' <pipeline>
                     | <timespec> <pipeline>
                     | <timespec> '!' <pipeline>
                     | '!' <timespec> <pipeline>
<pipeline> ::=
          <pipeline> '|' <newline_list> <pipeline>
        | <command>
<time_opt> ::= '−p'
<timespec> ::= time
             | time <time_opt>
