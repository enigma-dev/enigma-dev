/* Copyright (C) 2011-2012 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdio>
#include <iostream>
#include <unistd.h>
using namespace std;
#include <API/jdi.h>
#include <API/AST.h>
#include <API/user_tokens.h>
#include <General/llreader.h>
#include <General/quickstack.h>
#include "debug_lexer.h"

#ifdef linux
  #include <sys/time.h>
  #define start_time(ts) timeval ts; usleep(10000); gettimeofday(&ts,NULL)
  #define end_time(te,tel) timeval te; gettimeofday(&te,NULL); long unsigned tel = (te.tv_sec*1000000 + te.tv_usec) - (ts.tv_sec*1000000 + ts.tv_usec)
#else
  #include <time.h>
  #define start_time(ts) time_t ts = clock()
  #define end_time(te,tel) time_t te = clock(); double tel = (((te-ts) * 1000000.0) / CLOCKS_PER_SEC)
#endif

#include <System/lex_cpp.h>

using namespace jdi;
using namespace jdip;

void test_expression_evaluator();

static void putcap(string cap) {
  cout << endl << endl << endl << endl;
  cout << "============================================================================" << endl;
  cout << "=: " << cap << " :="; for (int i = 70 - cap.length(); i > 0; i--) fputc('=',stdout); cout << endl;
  cout << "============================================================================" << endl << endl;
}

void do_cli(context &ct);

int main() {
  initialize();
  cout << endl << endl;
  
  putcap("Test simple macros");
  builtin->add_macro("scalar_macro","simple value");
  builtin->add_macro_func("simple_function","Takes no parameters");
  builtin->add_macro_func("one_arg_function","x","(1/(1-(x)))",false);
  builtin->add_macro_func("two_arg_function","a","b","(-(b)/(2*(a)))",false);
  builtin->add_macro_func("variadic_three_arg_function","a","b","c","printf(a,b,c)",true);
  //builtin->output_macros();
  
  putcap("Metrics");
  cout << "sizeof(jdip::macro_type):        " << sizeof(jdip::macro_type) << endl
       << "sizeof(jdip::macro_function):    " << sizeof(jdip::macro_function) << endl
       << "sizeof(jdip::macro_scalar):      " << sizeof(jdip::macro_scalar) << endl
       << "sizeof(jdi::definition):         " << sizeof(jdi::definition) << endl
       << "sizeof(jdi::ref_stack):          " << sizeof(jdi::ref_stack) << endl
       << "sizeof(jdi::full_type):          " << sizeof(jdi::full_type) << endl
       << "sizeof(jdi::template::arg_key):  " << sizeof(jdi::arg_key) << endl;
  
  test_expression_evaluator();
  
  #if 0
    builtin->add_search_directory("c:\\mingw/lib/gcc/mingw32/4.6.1/include/c++");
    builtin->add_search_directory("c:\\mingw/lib/gcc/mingw32/4.6.1/include/c++/mingw32");
    builtin->add_search_directory("c:\\mingw/lib/gcc/mingw32/4.6.1/include/c++/backward");
    builtin->add_search_directory("c:\\mingw/lib/gcc/mingw32/4.6.1/include");
    builtin->add_search_directory("c:\\mingw\\include");
    builtin->add_search_directory("c:\\mingw/lib/gcc/mingw32/4.6.1/include-fixed");
    
    llreader macro_reader("test/defines_custom.txt");
  #else
    #ifdef __WIN32__
      builtin->add_search_directory("c:\\mingw/lib/gcc/mingw32/4.6.1/include/c++");
      builtin->add_search_directory("c:\\mingw/lib/gcc/mingw32/4.6.1/include/c++/mingw32");
      builtin->add_search_directory("c:\\mingw/lib/gcc/mingw32/4.6.1/include/c++/backward");
      builtin->add_search_directory("c:\\mingw/lib/gcc/mingw32/4.6.1/include");
      builtin->add_search_directory("c:\\mingw\\include");
      builtin->add_search_directory("c:\\mingw/lib/gcc/mingw32/4.6.1/include-fixed");
      
      llreader macro_reader("test/defines_mingw.txt");
    #else
      builtin->add_search_directory("/home/josh/Projects/ENIGMA/ENIGMAsystem/SHELL");
      builtin->add_search_directory("/usr/include/c++/4.6");
      builtin->add_search_directory("/usr/include/c++/4.6/x86_64-linux-gnu");
      builtin->add_search_directory("/usr/include/c++/4.6/backward");
      builtin->add_search_directory("/usr/lib/gcc/x86_64-linux-gnu/4.6/include");
      builtin->add_search_directory("/usr/local/include");
      builtin->add_search_directory("/usr/lib/gcc/x86_64-linux-gnu/4.6/include-fixed");
      builtin->add_search_directory("/usr/include/x86_64-linux-gnu");
      builtin->add_search_directory("/usr/include");
      
      llreader macro_reader("test/defines_linux.txt");
    #endif
  #endif
  
  if (macro_reader.is_open())
    builtin->parse_C_stream(macro_reader, "defines.txt");
  else
    cout << "ERROR: Could not open GCC macro file for parse!" << endl;
  
  putcap("Test parser");
  llreader f("test/test.cc");
  if (f.is_open())
  {
    /* */
    context enigma;
    start_time(ts);
    int res = enigma.parse_C_stream(f, "test.cc");
    end_time(te,tel);
    cout << "Parse finished in " << tel << " microseconds." << endl;
    
    //enigma.output_definitions();
    if (res)
      cout << endl << "====[------------------------------ FAILURE. ------------------------------]====" << endl << endl;
    else
      cout << endl << "====[++++++++++++++++++++++++++++++ SUCCESS! ++++++++++++++++++++++++++++++]====" << endl << endl;
    
    do_cli(enigma);
    /*/
    macro_map m;
    lexer_cpp lex(f, m, "test.cc");
    token_t t = lex.get_token();
    while (t.type != TT_ENDOFCODE) {
      t = lex.get_token();
    }
    cout << "Done..." << endl;
    // */
  }
  else
    cout << "Failed to open file for parsing!" << endl;
  
  clean_up();
  return 0;
}

#ifdef __linux__d
#include <ncurses>
#else
static char getch() {
  int c = fgetc(stdin);
  int ignore = c;
  while (ignore != '\n')
    ignore = fgetc(stdin);
  return c;
}
#endif

#include <cstring>
#include <System/lex_cpp.h>
#include <General/parse_basics.h>

void do_cli(context &ct) {
  putcap("Command Line Interface");
  char c = ' ';
  macro_map undamageable = ct.get_macros();
  while (c != 'q' and c != '\n') { switch (c) {
    case 'd': {
        bool justflags; justflags = false;
        if (false) { case 'f': justflags = true; }
        cout << "Enter the item to define:" << endl << ">> " << flush;
        char buf[4096]; cin.getline(buf, 4096);
        size_t start, e = 0;
        while (is_useless(buf[e]) or buf[e] == ':') ++e;
        definition* def = ct.get_global();
        while (buf[e] and def) {
          if (!is_letter(buf[e])) { cout << "Unexpected symbol '" << buf[e] << "'" << endl; break; }
          start = e;
          while (is_letterd(buf[++e]));
          if (def->flags & DEF_SCOPE) {
            string name(buf+start, e-start);
            definition_scope::defiter it = ((definition_scope*)def)->members.find(name);
            if (it == ((definition_scope*)def)->members.end()) {
              cout << "No `" << name << "' found in scope `" << def->name << "'" << endl;
              def = NULL;
              break;
            }
            def = it->second;
          }
          else {
            cout << "Definition `" << def->name << "' is not a scope" << endl;
            def = NULL;
            break;
          }
          while (is_useless(buf[e]) or buf[e] == ':') ++e;
        }
        if (def and e) {
          if (justflags) {
            map<int, string> flagnames;
            DEF_FLAGS d = DEF_TYPENAME;
            switch (d) {
              case DEF_TYPENAME: flagnames[DEF_TYPENAME] = "DEF_TYPENAME";
              case DEF_NAMESPACE: flagnames[DEF_NAMESPACE] = "DEF_NAMESPACE";
              case DEF_CLASS: flagnames[DEF_CLASS] = "DEF_CLASS";
              case DEF_ENUM: flagnames[DEF_ENUM] = "DEF_ENUM";
              case DEF_UNION: flagnames[DEF_UNION] = "DEF_UNION";
              case DEF_SCOPE: flagnames[DEF_SCOPE] = "DEF_SCOPE";
              case DEF_TYPED: flagnames[DEF_TYPED] = "DEF_TYPED";
              case DEF_FUNCTION: flagnames[DEF_FUNCTION] = "DEF_FUNCTION";
              case DEF_VALUED: flagnames[DEF_VALUED] = "DEF_VALUED";
              case DEF_DEFAULTED: flagnames[DEF_DEFAULTED] = "DEF_DEFAULTED";
              case DEF_EXTERN: flagnames[DEF_EXTERN] = "DEF_EXTERN";
              case DEF_TEMPLATE: flagnames[DEF_TEMPLATE] = "DEF_TEMPLATE";
              case DEF_TEMPPARAM: flagnames[DEF_TEMPPARAM] = "DEF_TEMPPARAM";
              case DEF_HYPOTHETICAL: flagnames[DEF_HYPOTHETICAL] = "DEF_HYPOTHETICAL";
              case DEF_TEMPSCOPE: flagnames[DEF_TEMPSCOPE] = "DEF_TEMPSCOPE";
              case DEF_PRIVATE: flagnames[DEF_PRIVATE] = "DEF_PRIVATE";
              case DEF_PROTECTED: flagnames[DEF_PROTECTED] = "DEF_PROTECTED";
              case DEF_INCOMPLETE: flagnames[DEF_INCOMPLETE] = "DEF_INCOMPLETE";
              case DEF_ATOMIC: flagnames[DEF_ATOMIC] = "DEF_ATOMIC";
              default: ;
            }
            bool hadone = false;
            for (int i = 1; i < (1 << 30); i <<= 1)
              if (def->flags & i)
                cout << (hadone? " | " : "  ") << flagnames[i], hadone = true;
            cout << endl;
          }
          else
            cout << def->toString() << endl;
        }
      } break;
    
    case 'm': {
        cout << "Enter the macro to define:" << endl << ">> " << flush;
        char buf[4096]; cin.getline(buf, 4096);
        macro_iter_c mi = ct.get_macros().find(buf);
        if (mi != ct.get_macros().end())
          cout << mi->second->toString();
        else
          cout << "Not found." << endl;
      } break;
    
    case 'e': {
        bool eval, coerce, render, show;
        eval = true,
        coerce = false;
        render = false;
        show = false;
        if (false) { case 'c': eval = false; coerce = true;  render = false; show = false;
        if (false) { case 'r': eval = false; coerce = false; render = true;  show = false;
        if (false) { case 's': eval = false; coerce = false; render = false; show = true;
        } } }
        cout << "Enter the expression to evaluate:" << endl << ">> " << flush;
        char buf[4096]; cin.getline(buf, 4096);
        llreader llr(buf, strlen(buf));
        AST a;
        lexer_cpp c_lex(llr, undamageable, "User expression");
        token_t dummy = c_lex.get_token_in_scope(ct.get_global());
        if (!a.parse_expression(dummy, &c_lex, ct.get_global(), precedence::all, def_error_handler)) {
          if (render) {
            cout << "Filename to render to:" << endl;
            cin.getline(buf, 4096);
            a.writeSVG(buf);
          }
          if (eval) {
            value v = a.eval();
            cout << "Value returned: " << v.toString() << endl;
          }
          if (coerce) {
            full_type t = a.coerce();
            cout << "Type of expression: " << t.toString() << endl;
          }
          if (show) {
            a.writeSVG("/tmp/anus.svg");
            if (system("xdg-open /tmp/anus.svg"))
              cout << "Failed to open." << endl;
          }
        } else cout << "Bailing." << endl;
      } break;
    
    case 'h':
      cout <<
      "'c' Coerce an expression, printing its type\n"
      "'d' Define a symbol, printing it recursively\n"
      "'e' Evaluate an expression, printing its result\n"
      "'f' Print flags for a given definition\n"
      "'h' Print this help information\n"
      "'m' Define a macro, printing a breakdown of its definition\n"
      "'r' Render an AST representing an expression\n"
      "'s' Render an AST representing an expression and show it\n"
      "'q' Quit this interface\n";
    break;
      
    
    default: cout << "Unrecognized command. Empty command or 'q' to quit." << endl << endl; break;
    case ' ': cout << "Commands are single-letter; 'h' for help." << endl << "Follow commands with ENTER on non-unix." << endl;
  } 
  cout << "> " << flush;
  c = getch();
  }
  cout << endl << "Goodbye" << endl;
}

void test_expression_evaluator() {
  putcap("Test expression evaluator");
  
  AST ast;
  debug_lexer dlex;
  dlex << create_token_dec_literal("10",2);
  ast.parse_expression(&dlex);
  value v = ast.eval();
  ast.writeSVG("/home/josh/Desktop/RecursiveAST/AST_00.svg");
  cout << v.val.i << endl;
  
  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("20",2);
  ast.parse_expression(&dlex);
  v = ast.eval();
  ast.writeSVG("/home/josh/Desktop/RecursiveAST/AST_01.svg");
  cout << v.val.i << endl;
  
  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("20",2);
  dlex << create_token_operator("+",1);
  dlex << create_token_dec_literal("10",2);
  ast.parse_expression(&dlex);
  v = ast.eval();
  ast.writeSVG("/home/josh/Desktop/RecursiveAST/AST_02.svg");
  cout << v.val.i << endl;
  
  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("20",2);
  dlex << create_token_operator("+",1);
  dlex << create_token_dec_literal("10",2);
  dlex << create_token_operator("+",1);
  dlex << create_token_dec_literal("10",2);
  ast.parse_expression(&dlex);
  ast.writeSVG("/home/josh/Desktop/RecursiveAST/AST_03.svg");
  v = ast.eval();
  cout << v.val.i << endl;
  
  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("20",2);
  dlex << create_token_operator("+",1);
  dlex << create_token_dec_literal("40",2);
  dlex << create_token_operator("-",1);
  dlex << create_token_dec_literal("10",2);
  ast.parse_expression(&dlex);
  ast.writeSVG("/home/josh/Desktop/RecursiveAST/AST_04.svg");
  v = ast.eval(); dlex.clear();
  cout << v.val.i << endl;
  
  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("20",2);
  dlex << create_token_operator("+",1);
  dlex << create_token_dec_literal("40",2);
  dlex << create_token_operator("/",1);
  dlex << create_token_dec_literal("4",1);
  dlex << create_token_operator("<<",2);
  dlex << create_token_dec_literal("1",1);
  token_t token;
  ast.parse_expression(&dlex);
  ast.writeSVG("/home/josh/Desktop/RecursiveAST/AST_05.svg");
  v = ast.eval();
  cout << v.val.i << endl;
  
  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator("<<",2);
  dlex << create_token_dec_literal("1",1);
  dlex << create_token_operator("+",1);
  dlex << create_token_dec_literal("6",1);
  dlex << create_token_operator("/",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator("-",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator("/",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator("<<",2);
  dlex << create_token_dec_literal("8",1);
  dlex << create_token_operator("%",1);
  dlex << create_token_dec_literal("5",1);
  dlex << create_token_operator("+",1);
  dlex << create_token_operator("-",1);
  dlex << create_token_dec_literal("5",1);
  dlex << create_token_operator("/",1);
  dlex << create_token_operator("-",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator("==",2);
  dlex << create_token_dec_literal("1",1);
  dlex << create_token_operator("<<",2);
  dlex << create_token_operator("-",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator("*",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator("*",1);
  dlex << create_token_operator("-",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator("+",1);
  dlex << create_token_dec_literal("1",1);
  ast.parse_expression(&dlex);
  ast.writeSVG("/home/josh/Desktop/RecursiveAST/AST_06.svg");
  v = ast.eval();
  cout << v.val.i << endl;
  
  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("25",2);
  dlex << create_token_operator("/",1);
  dlex << create_token_opening_parenth();
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator("+",1);
  dlex << create_token_dec_literal("3",1);
  dlex << create_token_closing_parenth();
  dlex << create_token_operator("+",1);
  dlex << create_token_opening_parenth();
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator("+",1);
  dlex << create_token_dec_literal("1",1);
  dlex << create_token_operator("*",1);
  dlex << create_token_dec_literal("1",1);
  dlex << create_token_closing_parenth();
  dlex << create_token_operator("*",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator("+",1);
  dlex << create_token_dec_literal("1",1);
  ast.parse_expression(&dlex);
  ast.writeSVG("/home/josh/Desktop/RecursiveAST/AST_07.svg");
  v = ast.eval();
  cout << v.val.i << endl;
  
  ast.clear();
  dlex << create_token_identifier("a",1);
  dlex << create_token_operator("=",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator("==",2);
  dlex << create_token_dec_literal("1",1);
  dlex << create_token_operator("+",1);
  dlex << create_token_dec_literal("1",1);
  dlex << create_token_operator("?",1);
  dlex << create_token_identifier("b",1);
  dlex << create_token_operator("=",1);
  dlex << create_token_dec_literal("15",2);
  dlex << create_token_operator("*",1);
  dlex << create_token_dec_literal("8",1);
  dlex << create_token_operator("+",1);
  dlex << create_token_dec_literal("3",1);
  dlex << create_token_colon();
  dlex << create_token_identifier("c",1);
  dlex << create_token_operator("=",1);
  dlex << create_token_dec_literal("3",1);
  dlex << create_token_operator("+",1);
  dlex << create_token_dec_literal("4",1);
  ast.parse_expression(&dlex);
  ast.writeSVG("/home/josh/Desktop/RecursiveAST/AST_08.svg");
  v = ast.eval();
  cout << v.val.i << endl;
}
