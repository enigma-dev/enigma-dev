/* Copyright (C) 2011 Josh Ventura
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
  #define start_time(ts) time_t ts = clock()
  #define end_time(te,tel) time_t te = clock(); double tel = (((te-ts) * 1000000.0) / CLOCKS_PER_SEC)
#endif

using namespace jdi;
using namespace jdip;

void test_expression_evaluator();

static void putcap(string cap) {
  cout << endl << endl << endl << endl;
  cout << "============================================================================" << endl;
  cout << "=: " << cap << " :="; for (int i = 70 - cap.length(); i > 0; i--) fputc('=',stdout); cout << endl;
  cout << "============================================================================" << endl << endl;
}

int main() {
  add_gnu_declarators();
  builtin.load_standard_builtins();
  builtin.output_types();
  cout << endl << endl;
  
  putcap("Test simple macros");
  builtin.add_macro("scalar_macro","simple value");
  builtin.add_macro_func("simple_function","Takes no parameters");
  builtin.add_macro_func("one_arg_function","x","(1/(1-(x)))",false);
  builtin.add_macro_func("two_arg_function","a","b","(-(b)/(2*(a)))",false);
  builtin.add_macro_func("variadic_three_arg_function","a","b","c","printf(a,b,c)",true);
  builtin.output_macros();
  
  putcap("Metrics");
  cout << "sizeof(jdip::macro_type):       " << sizeof(jdip::macro_type) << endl
       << "sizeof(jdip::macro_function):   " << sizeof(jdip::macro_function) << endl
       << "sizeof(jdip::macro_scalar):     " << sizeof(jdip::macro_scalar) << endl;
  
  test_expression_evaluator();
  
  builtin.add_search_directory("/usr/include/c++/4.6");
  builtin.add_search_directory("/usr/include/c++/4.6/x86_64-linux-gnu");
  builtin.add_search_directory("/usr/include/c++/4.6/backward");
  builtin.add_search_directory("/usr/lib/gcc/x86_64-linux-gnu/4.6.1/include");
  builtin.add_search_directory("/usr/local/include");
  builtin.add_search_directory("/usr/lib/gcc/x86_64-linux-gnu/4.6.1/include-fixed");
  builtin.add_search_directory("/usr/include/x86_64-linux-gnu");
  builtin.add_search_directory("/usr/include");
  
  llreader macro_reader("test/defines.txt");
  if (macro_reader.is_open())
    builtin.parse_C_stream(macro_reader, "defines.txt");
  else
    cout << "ERROR: Could not open GCC macro file for parse!" << endl;
  
  putcap("Test parser");
  llreader f("test/test.cc");
  if (f.is_open())
  {
    context enigma;
    start_time(ts);
    int res = enigma.parse_C_stream(f, "test.cc");
    end_time(te,tel);
    cout << "Parse finished in " << tel << " microseconds." << endl;
    if (res)
      cout << "ERROR: " << enigma.get_last_error() << endl;
    enigma.output_definitions();
    if (res)
      cout << endl << "====[------------------------------ /FAILURE ------------------------------]====" << endl << endl;
    else
      cout << endl << "====[++++++++++++++++++++++++++++++ SUCCESS! ++++++++++++++++++++++++++++++]====" << endl << endl;
    
    cout << "Macro defs: " << endl;
    enigma.output_macro("__asm");
    enigma.output_macro("__asm__");
    enigma.output_macro("__REDIRECT_NTH");
    enigma.output_macro("__BEGIN_DECLS");
    enigma.output_macro("__nonnull");
    enigma.output_macro("__THROW");
    enigma.output_macro("__MATHDECL");
  }
  else
    cout << "Failed to open file for parsing!" << endl;
  
  cleanup_declarators();
  return 0;
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
