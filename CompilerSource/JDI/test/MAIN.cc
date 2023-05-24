/* Copyright (C) 2011-2013 Josh Ventura
 * This file is part of JustDefineIt.
 *
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 *
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdio>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <unistd.h>

#include <API/AST.h>
#include <API/user_tokens.h>
#include <General/llreader.h>
#include <General/quickstack.h>
#include <System/builtins.h>

#ifdef linux
  #include <sys/time.h>
  #define start_time(ts) timeval ts; usleep(10000); gettimeofday(&ts,nullptr)
  #define end_time(te,tel) timeval te; gettimeofday(&te,nullptr); long unsigned tel = (te.tv_sec*1000000 + te.tv_usec) - (ts.tv_sec*1000000 + ts.tv_usec)
#else
  #include <time.h>
  #define start_time(ts) time_t ts = clock()
  #define end_time(te,tel) time_t te = clock(); double tel = (((te-ts) * 1000000.0) / CLOCKS_PER_SEC)
#endif

#include <System/lex_cpp.h>

using namespace jdi;
using namespace std;
namespace fs = std::filesystem;

void test_expression_evaluator();
void name_type(string type, Context &ct);
std::vector<std::string> get_gcc_include_paths(int standard);

static void putcap(string cap) {
  cout << endl << endl << endl << endl;
  cout << "============================================================================" << endl;
  cout << "=: " << cap << " :="; for (int i = 70 - cap.length(); i > 0; i--) fputc('=',stdout); cout << endl;
  cout << "============================================================================" << endl << endl;
}

void do_cli(Context &ct);

// Returns the number of elements to insert into v1 at i to make it match v2.
// If elements are instead missing from v2, a negative number is returned.
static inline int compute_diff(
    const vector<token_t> &v1, const vector<token_t> &v2,
    size_t ind, size_t window = 16) {
  if (ind + window > v1.size() || ind + window > v2.size()) {
    int candoff = v2.size() - v1.size();
    if (candoff > 0 && v1[ind].type == v2[ind + candoff].type) return candoff;
    if (candoff < 0 && v1[ind - candoff].type == v2[ind].type) return candoff;
    return 0;
  }
  const size_t hwindow = window / 2;
  const size_t qwindow = window / 4;
  for (size_t o = 0; o <= hwindow; ++o) {
    for (size_t i = 0; i < hwindow; ++i) {
      if (v1[ind + o + i].type != v2[ind + qwindow + i].type) goto continue2;
    }
    return qwindow - o;
    continue2: ;
  }
  return window < 512 ? compute_diff(v1, v2, ind, window * 2) : 0;
}

#include <fstream>
const char* tname[TT_INVALID + 1];
/*static void write_tokens() {
  populate_tnames();
  context ct;
  llreader llr("test/test.cc");
  macro_map undamageable = ct.get_macros();
  lexer c_lex(llr, undamageable, "User expression");
  ofstream f("/home/josh/Desktop/tokensnew.txt");
  for (token_t tk = c_lex.get_token(); tk.type != TT_ENDOFCODE; tk = c_lex.get_token())
    f << tname[tk.type] << "(" << tk.linenum << "), ";
  f << "END OF STREAM" << endl;
  f.close();
}*/


/*
This is the command I use to generate the test data for this system.
Tweak it to use your own code input and source locations.

gcc -I/home/josh/Projects/ENIGMA/ENIGMAsystem/SHELL -I/home/josh/.enigma/ -E \
    --std=c++2a /home/josh/Projects/ENIGMA/ENIGMAsystem/SHELL/SHELLmain.cpp \
    -o /home/josh/Projects/JustDefineIt/shellmain-pp.cc && \
    (cpp -dM -x c++ --std=c++2a -E /dev/null \
         > /home/josh/Projects/JustDefineIt/test/defines_linux.txt);

You can run this (with locale=LC_ALL) to generate the include directory list
used below:

gcc -E -x c++ --std=c++2a -v /dev/null 2>&1 | \
    sed -n '/#include ..... search starts here:/,/End of search list[.]/p'
*/

inline std::string get_cpp_standard_str(int standard) {
  std::string std_str;
  switch (standard) {
    case 98: std_str = "c++89"; break;
    case 03: std_str = "c++03"; break;
    case 11: std_str = "c++11"; break;
    case 14: std_str = "c++14"; break;
    case 17: std_str = "c++17"; break;
    case 20: std_str = "c++2a"; break;
    default: std_str = "c++11"; break;
  }
  return std_str;
}


std::vector<std::string> get_gcc_include_paths(int standard) {

  std::string std_str = get_cpp_standard_str(standard);
  std::vector<std::string> include_paths;

  std::filesystem::create_directories("/tmp/jdi");
  if (std::system(("gcc -E -x c++ --std=" + std_str + " -v /dev/null 2>&1 &> /tmp/jdi/search_dirs.txt").c_str()) == 0 ) {
    std::ifstream search_dirs_f("/tmp/jdi/search_dirs.txt");
    std::string line;

    bool search_dirs = false;
    std::cout << "System include paths: " << std::endl;
    while(std::getline(search_dirs_f, line)) {
      if (line == "End of search list.") search_dirs = false;

      if (search_dirs) {
        std::cout << "  " << line << std::endl;
        include_paths.push_back(line.substr(1, line.length()));
      }

      if (line == "#include <...> search starts here:") search_dirs = true;
    }
  } else exit(1);

  return include_paths;
}

inline
const char *first_not_null(const char *a, const char *b) { return a? a : b; }

int main(int argc, char** argv) {
  // For JDI testing, developers use ENIGMA--the game engine JDI was originally
  // built to support. There's nothing special about ENIGMA for the purposes of
  // this project, but it is a large codebase that developers are familiar with
  // and actively seek to support using the JDI project.

  fs::path home = first_not_null(getenv("HOME"), "/tmp/");
  fs::path enigma_path = home/"Projects/ENIGMA";
  fs::path enigma_temp = home/".enigma";

  fs::path gcc_defines = "/tmp/jdi/defines.cc";
  fs::path gcc_preprocessed = "/tmp/jdi/preprocessed.cc";

  bool test_lexer = false;
  int cpp_std = 20;
  if (argc != 4) {
    std::cerr << "Usage: JustDefineIt <c++std> </path/to/enigma> <test_lexer>"
              << std::endl << "Will assume default values." << std::endl;
  } else {
    enigma_path = argv[2];
    test_lexer = stoi(argv[3], nullptr, 10);
    cpp_std = stoi(argv[1], nullptr, 10);
  }

  fs::path enigma_shared = enigma_path/"shared";
  fs::path enigma_shell = enigma_path/"ENIGMAsystem/SHELL";
  fs::path enigma_shellmain = enigma_shell/"SHELLmain.cpp";

  // ensure place to write stuffs exists
  if (!fs::create_directory("/tmp/jdi") && !fs::exists("/tmp/jdi")) {
    std::cerr << "Failed to create working directory: /tmp/jdi" << std::endl;
    return -1;
  }

  if (!fs::exists(enigma_path)) {
    std::cerr << "could not locate enigma at: " << enigma_path << std::endl;
    return -1;
  }
  std::cout << "Attemping parse of the " << get_cpp_standard_str(cpp_std)  << " standard" << std::endl << std::endl;

  putcap("Test simple macros");
  Context &builtin = builtin_context();
  builtin.add_macro("scalar_macro","simple value");
  builtin.add_macro_func("simple_function","Takes no parameters");
  builtin.add_macro_func("one_arg_function","x","(1/(1-(x)))",false);
  builtin.add_macro_func("two_arg_function","a","b","(-(b)/(2*(a)))",false);
  builtin.add_macro_func("variadic_three_arg_function","a","b","c","printf(a,b,c)",true);
  //builtin->output_macros();

  putcap("Metrics");
  cout << "sizeof(jdi::macro_type):         " << sizeof(jdi::macro_type) << endl
       << "sizeof(jdi::definition):         " << sizeof(jdi::definition) << endl
       << "sizeof(jdi::ref_stack):          " << sizeof(jdi::ref_stack) << endl
       << "sizeof(jdi::full_type):          " << sizeof(jdi::full_type) << endl
       << "sizeof(jdi::template::arg_key):  " << sizeof(jdi::arg_key) << endl;

  //test_expression_evaluator();

  cout << endl << "Test arg_key::operator<" << endl;
  {
    arg_key a(2), b(2);
    a.put_type(0, full_type(builtin_type__double));
    a.put_type(1, full_type(builtin_type__int));
    b.put_type(0, full_type(builtin_type__double));
    b.put_type(1, full_type(builtin_type__double));
    const int w = max(a.toString().length(), b.toString().length());
    cout <<     "  [" << setw(w) << a.toString()
         << "]  <  [" << setw(w) << b.toString() << "]: " << (a < b) << endl;
    cout <<     "  [" << setw(w) << b.toString()
         << "]  <  [" << setw(w) << a.toString() << "]: " << (b < a) << endl;
    cout <<     "  [" << setw(w) << a.toString()
         << "]  <  [" << setw(w) << a.toString() << "]: " << (a < a) << endl;
    cout <<     "  [" << setw(w) << b.toString()
         << "]  <  [" << setw(w) << b.toString() << "]: " << (b < b) << endl;
  }

  for (const std::string& p : get_gcc_include_paths(cpp_std)) builtin.add_search_directory(p);

  builtin.add_search_directory(enigma_shell.u8string());
  builtin.add_search_directory(enigma_shared.u8string());
  builtin.add_search_directory(enigma_temp.u8string());

  std::string preprocess_command =
      "cpp -dM -x c++ --std=" + get_cpp_standard_str(cpp_std) + " -E /dev/null"
      " > " + gcc_defines.u8string();
  if (std::system(preprocess_command.c_str()) != 0) {
    std::cerr << "Failed to generate gcc defines. Bye" << std::endl;
    return -1;
  }
  llreader macro_reader(gcc_defines);

  if (macro_reader.is_open())
    builtin.parse_stream(macro_reader);
  else {
    cout << "ERROR: Could not open GCC macro file for parse!" << endl;
    return -1;
  }

  putcap("Test type reading");
  name_type("int", builtin);
  name_type("int*", builtin);
  name_type("int&", builtin);
  name_type("int&()", builtin);
  name_type("int(*)()", builtin);
  name_type("int&(*)()", builtin);

  if (test_lexer) {
    std::string gcc_command =
        "g++ --std=" + get_cpp_standard_str(cpp_std)
         + " -E " + enigma_shellmain.u8string()
         + " -I" + enigma_shell.u8string()
         + " -I" + enigma_shared.u8string()
         + " -I" + enigma_temp.u8string()
         // + " -DJUST_DEFINE_IT_RUN"
         + " > " + gcc_preprocessed.u8string();
    std::cout << "Preprocessing compiler:\n" << gcc_command << "\n\n";
    if (std::system(gcc_command.c_str()) != 0) {
      std::cerr << "Failed to preprocess ENIGMA with GCC. Abort." << std::endl;
      return -1;
    }

    vector<token_t> tokens, tokens2;
    size_t correct = 0, incorrect = 0;
    if (true) {
      Context butts;
      llreader f(gcc_preprocessed);
      macro_map buttMacros = butts.get_macros();
      lexer lex(f, buttMacros, default_error_handler);
      for (token_t token = lex.get_token(); token.type != TT_ENDOFCODE; token = lex.get_token()) {
        tokens2.push_back(token);
      }
    }
    bool had_diff = false;
    if (true) {
      Context butts;
      llreader f(enigma_shellmain);
      macro_map buttMacros = butts.get_macros();
      lexer lex(f, buttMacros, default_error_handler);
      for (token_t token = lex.get_token(); token.type != TT_ENDOFCODE; token = lex.get_token()) {
        size_t p = tokens.size();
        tokens.push_back(token);
        if (!had_diff && p < tokens2.size() && (tokens[p].type != tokens2[p].type || tokens[p].content.view() != tokens2[p].content.view())) {
          cerr << p << endl;
          for (const auto &sl : lex.detailed_position())
            cerr << "In " << sl.filename << ":" << sl.line << ":" << sl.pos << ":\n";
          token.report_errorf(default_error_handler,
                              "Token differs from golden set! Read "
                              + token.to_string() + ", expected "
                              + token_t::get_name((TOKEN_TYPE) tokens2[p].type) + ".");
          tokens2[p].report_error(default_error_handler,"Note: golden token declared here.");
          had_diff = true;
        }
        ++(had_diff? incorrect : correct);
      }
    }
    for (size_t i = 0; i < tokens.size() && i < tokens2.size(); ++i) {
      if (tokens[i].type == tokens2[i].type) continue;
      int off = compute_diff(tokens, tokens2, i);
      size_t ins = 0;
      if (!off) { i += 16; continue; }
      if (off > 0) tokens.insert(tokens.begin() + i, ins = (size_t) off,  token_t((TOKEN_TYPE) -1337, "bullshit token", 0, 0, "bullshit", 8));
      if (off < 0) tokens2.insert(tokens2.begin() + i, ins = (size_t) -off, token_t((TOKEN_TYPE) -1337, "bullshit token", 0, 0, "bullshit", 8));
      i += ins;
    }
    int ndiffs = 0;
    for (size_t i = 0; i < tokens.size() || i < tokens2.size(); ++i) {
      int a = i < tokens.size()?  tokens[i].type  : TT_ENDOFCODE;
      int b = i < tokens2.size()? tokens2[i].type : TT_ENDOFCODE;
      const bool diff = a != b;
      if (diff and not ndiffs)
        printf("%2d : %2d  -  %d\n", a, b, ndiffs);
      ndiffs += diff;
    }
    printf("Final stats: %zu correct, %zu incorrect\n", correct, incorrect);
    return 0;
  }

  putcap("Test parser");
  llreader f(enigma_shellmain);

  if (f.is_open())
  {
    Context enigma;
    // This is the difference between success and failure in 03 mode.
    // enigma.add_macro_func("__attribute__", "", "", true);
    start_time(ts);
    int res = enigma.parse_stream(f);
    end_time(te,tel);
    cout << "Parse finished in " << tel << " microseconds." << endl;

    //enigma.output_definitions();
    if (res)
      cout << endl << "====[------------------------------ FAILURE. ------------------------------]====" << endl << endl;
    else
      cout << endl << "====[++++++++++++++++++++++++++++++ SUCCESS! ++++++++++++++++++++++++++++++]====" << endl << endl;
    cout << "Parse completed with " << default_error_handler->error_count << " errors and " << default_error_handler->warning_count << " warnings." << endl;

    do_cli(enigma);
    /*/
    macro_map m;
    lexer lex(f, m, "test.cc");
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

#include <Parser/context_parser.h>

void name_type(string type, Context &ct) {
  llreader llr("type string", type, type.length());
  macro_map undamageable = ct.get_macros();
  lexer c_lex(llr, undamageable, default_error_handler);
  context_parser cp(&ct, &c_lex);
  token_t tk = c_lex.get_token_in_scope(ct.get_global());
  full_type ft = cp.read_fulltype(tk, ct.get_global());
  cout << ft.toString() << ": " << ft.toEnglish() << endl;
}

static char getch() {
  int c = fgetc(stdin);
  int ignore = c;
  while (ignore != '\n')
    ignore = fgetc(stdin);
  return c;
}

#include <System/lex_cpp.h>
#include <General/parse_basics.h>

void do_cli(Context &ct) {
  putcap("Command Line Interface");
  char c = ' ';
  macro_map undamageable = ct.get_macros();
  while (c != 'q' and c != '\n') {
    switch (c) {
      case 'd': {
        bool justflags, justorder;
        justflags = false;
        justorder = false;
        if (false) { case 'f': justflags = true; justorder = false; }
        if (false) { case 'o': justorder = true; justflags = false; }
        cout << "Enter the item to define:" << endl << ">> " << flush;
        char buf[4096]; cin.getline(buf, 4096);
        llreader llr("user input", buf, true);
        lexer c_lex(llr, undamageable, default_error_handler);
        token_t dummy = c_lex.get_token_in_scope(ct.get_global());
        if (dummy.type != TT_DEFINITION && dummy.type != TT_DECLARATOR && dummy.type != TT_SCOPE) {
          dummy.report_errorf(default_error_handler, "Expected definition; encountered %s. Perhaps your term is a macro?");
          break;
        }
        context_parser cp(&ct, &c_lex);
        definition *def = cp.read_qualified_definition(dummy, ct.get_global());
        if (def) {
          if (justflags) {
            cout << flagnames(def->flags) << endl;
          }
          else if (justorder) {
            if (def->flags & DEF_TEMPLATE)
              def = ((definition_template*)def)->def.get();
            if (def->flags & DEF_SCOPE) {
              definition_scope *sc = (definition_scope*)def;
              for (definition_scope::orditer it = sc->dec_order.begin(); it != sc->dec_order.end(); ++it)
                cout << "- " << ((*it)->second ? (*it)->second->name : "<null>") << endl;
            }
          }
          else
            cout << def->toString() << endl;
        }
        break;
      }
      case 'm': {
        cout << "Enter the macro to define:" << endl << ">> " << flush;
        char buf[4096]; cin.getline(buf, 4096);
        macro_iter_c mi = ct.get_macros().find(buf);
        if (mi != ct.get_macros().end())
          cout << mi->second->toString() << endl;
        else
          cout << "Not found." << endl;
        break;
      }
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
        llreader llr("user input", buf, true);
        lexer c_lex(llr, undamageable, default_error_handler);
        AST a;
        context_parser cparse(&ct, &c_lex);
        token_t dummy = c_lex.get_token_in_scope(ct.get_global());
        if (!cparse.get_AST_builder()->parse_expression(&a, dummy, ct.get_global(), precedence::all)) {
          if (render) {
            cout << "Filename to render to:" << endl;
            cin.getline(buf, 4096);
            a.writeSVG(buf);
          }
          if (eval) {
            value v = a.eval(ErrorContext(default_error_handler, dummy));
            cout << "Value returned: " << v.toString() << endl;
          }
          if (coerce) {
            full_type t = a.coerce(ErrorContext(default_error_handler, dummy));
            cout << "Type of expression: " << t.toString() << endl;
            cout << (t.def? t.def->toString() : "nullptr") << endl;
          }
          if (show) {
            a.writeSVG("/tmp/anus.svg");
            if (system("xdg-open /tmp/anus.svg"))
              cout << "Failed to open." << endl;
          }
        } else cout << "Bailing." << endl;
        break;
      }
      case 'h': {
        cout <<
            "'c' Coerce an expression, printing its type\n"
            "'d' Define a symbol, printing it recursively\n"
            "'e' Evaluate an expression, printing its result\n"
            "'f' Print flags for a given definition\n"
            "'h' Print this help information\n"
            "'m' Define a macro, printing a breakdown of its definition\n"
            "'o' Print the order of declarations in a given scope\n"
            "'r' Render an AST representing an expression\n"
            "'s' Render an AST representing an expression and show it\n"
            "'q' Quit this interface\n";
        break;
      }
      default: {
        cout << "Unrecognized command. Empty command or 'q' to quit." << endl << endl;
        break;
      }
      case ' ': {
        cout << "Commands are single-letter; 'h' for help." << endl
             << "Follow commands with ENTER on non-unix." << endl;
      }
    }
    cout << "> " << flush;
    c = getch();
  }
  cout << endl << "Goodbye" << endl;
}

#if 0
void test_expression_evaluator() {
  putcap("Test expression evaluator");

  lexer dlex;
  context ct;//(&dlex, default_error_handler);
  context_parser cp(&ct, &dlex, default_error_handler);
  AST ast;
  ErrorContext dec(default_error_handler, "Test AST", 0, 0);

  dlex << create_token_dec_literal("10",2);
  cp.get_AST_builder()->parse_expression(&ast);
  value v = ast.eval(dec);
  ast.writeSVG("/tmp/jdi/AST_00.svg");
  cout << v.val.i << endl;

  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("20",2);
  cp.get_AST_builder()->parse_expression(&ast);
  v = ast.eval(dec);
  ast.writeSVG("/tmp/jdi/AST_01.svg");
  cout << v.val.i << endl;

  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("20",2);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_dec_literal("10",2);
  cp.get_AST_builder()->parse_expression(&ast);
  v = ast.eval(dec);
  ast.writeSVG("/tmp/jdi/AST_02.svg");
  cout << v.val.i << endl;

  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("20",2);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_dec_literal("10",2);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_dec_literal("10",2);
  cp.get_AST_builder()->parse_expression(&ast);
  ast.writeSVG("/tmp/jdi/AST_03.svg");
  v = ast.eval(dec);
  cout << v.val.i << endl;

  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("20",2);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_dec_literal("40",2);
  dlex << create_token_operator(jdi::TT_MINUS, "-",1);
  dlex << create_token_dec_literal("10",2);
  cp.get_AST_builder()->parse_expression(&ast);
  ast.writeSVG("/tmp/jdi/AST_04.svg");
  v = ast.eval(dec); dlex.clear();
  cout << v.val.i << endl;

  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("20",2);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_dec_literal("40",2);
  dlex << create_token_operator(jdi::TT_SLASH, "/",1);
  dlex << create_token_dec_literal("4",1);
  dlex << create_token_operator(jdi::TT_LSHIFT, "<<",2);
  dlex << create_token_dec_literal("1",1);
  token_t token;
  cp.get_AST_builder()->parse_expression(&ast);
  ast.writeSVG("/tmp/jdi/AST_05.svg");
  v = ast.eval(dec);
  cout << v.val.i << endl;

  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator(jdi::TT_LSHIFT, "<<",2);
  dlex << create_token_dec_literal("1",1);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_dec_literal("6",1);
  dlex << create_token_operator(jdi::TT_SLASH,"/",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator(jdi::TT_MINUS, "-",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator(jdi::TT_SLASH,"/",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator(jdi::TT_LSHIFT, "<<",2);
  dlex << create_token_dec_literal("8",1);
  dlex << create_token_operator(jdi::TT_MODULO, "%",1);
  dlex << create_token_dec_literal("5",1);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_operator(jdi::TT_MINUS, "-",1);
  dlex << create_token_dec_literal("5",1);
  dlex << create_token_operator(jdi::TT_SLASH, "/",1);
  dlex << create_token_operator(jdi::TT_MINUS, "-",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator(jdi::TT_EQUAL_TO, "==",2);
  dlex << create_token_dec_literal("1",1);
  dlex << create_token_operator(jdi::TT_LSHIFT, "<<",2);
  dlex << create_token_operator(jdi::TT_MINUS, "-",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator(jdi::TT_STAR, "*",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator(jdi::TT_STAR, "*",1);
  dlex << create_token_operator(jdi::TT_MINUS, "-",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_dec_literal("1",1);
  cp.get_AST_builder()->parse_expression(&ast);
  ast.writeSVG("/tmp/jdi/AST_06.svg");
  v = ast.eval(dec);
  cout << v.val.i << endl;

  ast.clear(); dlex.clear();
  dlex << create_token_dec_literal("25",2);
  dlex << create_token_operator(jdi::TT_SLASH, "/",1);
  dlex << create_token_opening_parenth();
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_dec_literal("3",1);
  dlex << create_token_closing_parenth();
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_opening_parenth();
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_dec_literal("1",1);
  dlex << create_token_operator(jdi::TT_STAR, "*",1);
  dlex << create_token_dec_literal("1",1);
  dlex << create_token_closing_parenth();
  dlex << create_token_operator(jdi::TT_STAR, "*",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_dec_literal("1",1);
  cp.get_AST_builder()->parse_expression(&ast);
  ast.writeSVG("/tmp/jdi/AST_07.svg");
  v = ast.eval(dec);
  cout << v.val.i << endl;

  ast.clear();
  dlex << create_token_identifier("a",1);
  dlex << create_token_operator(jdi::TT_EQUAL, "=",1);
  dlex << create_token_dec_literal("2",1);
  dlex << create_token_operator(jdi::TT_EQUAL_TO, "==",2);
  dlex << create_token_dec_literal("1",1);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_dec_literal("1",1);
  dlex << create_token_operator(jdi::TT_QUESTIONMARK, "?",1);
  dlex << create_token_identifier("b",1);
  dlex << create_token_operator(jdi::TT_EQUAL, "=",1);
  dlex << create_token_dec_literal("15",2);
  dlex << create_token_operator(jdi::TT_STAR, "*",1);
  dlex << create_token_dec_literal("8",1);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_dec_literal("3",1);
  dlex << create_token_colon();
  dlex << create_token_identifier("c",1);
  dlex << create_token_operator(jdi::TT_EQUAL, "=",1);
  dlex << create_token_dec_literal("3",1);
  dlex << create_token_operator(jdi::TT_PLUS, "+",1);
  dlex << create_token_dec_literal("4",1);
  cp.get_AST_builder()->parse_expression(&ast);
  ast.writeSVG("/tmp/jdi/AST_08.svg");
  v = ast.eval(dec);
  cout << v.val.i << endl;
}
#endif
