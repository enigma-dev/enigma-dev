#include <API/AST.h>
#include <API/user_tokens.h>
#include <General/llreader.h>
#include <General/quickstack.h>
#include <System/builtins.h>
#include <Parser/context_parser.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Retrieves a list of include directories from GCC. Note that you can hard-code
// these values instead, but such a list would need to be maintained for each
// supported version of GCC.
// Arguments:
//   gcc_std:     C++ standard version, in GCC terms. Example, "c++03".
//   blank_file:  Path to an existing empty file on disk.
//                Can be /dev/null on Linux.
//   paths_file:  Temporary path to which GCC output will be written.
static std::vector<std::string> get_gcc_include_paths(
    const std::string &gcc_std,
    std::filesystem::path blank_file, std::filesystem::path paths_file) {
  std::vector<std::string> include_paths;

  std::cout << "Polling GCC for include directories for " << gcc_std << "..."
            << std::endl;

  const std::string command =
      "gcc -E -x c++ --std=" + gcc_std + " -v "
      + blank_file.u8string().c_str()
      + " > " + paths_file.u8string().c_str() + " 2>&1";

  std::cout << " - Command: " << command << std::endl;
  if (std::system(command.c_str()) == 0) {
    std::ifstream search_dirs_f(paths_file);
    std::string line;

    bool search_dirs = false;
    std::cout << " - Command succeeded. System include paths:" << std::endl;
    while(std::getline(search_dirs_f, line)) {
      if (line == "End of search list.") search_dirs = false;

      if (search_dirs) {
        std::cout << "    -" << line << std::endl;
        include_paths.push_back(line.substr(1, line.length()));
      }

      if (line == "#include <...> search starts here:") search_dirs = true;
    }
  }

  return include_paths;
}

static bool prepare_builtin_context() {
  const auto jdi_tmp = std::filesystem::temp_directory_path() / "jdi";
  std::filesystem::create_directories(jdi_tmp);

  std::string cpp_std = "c++03";
  std::filesystem::path blank_file = jdi_tmp/"blank";
  std::filesystem::path paths_file = jdi_tmp/"search_dirs.txt";
  std::filesystem::path defines_file = jdi_tmp/"defines.cc";

  if (std::ofstream blank{blank_file}) {
    std::cout << "Created blank file for preprocessing." << std::endl;
  } else {
    std::cerr << "Failed to create blank file for preprocessing." << std::endl;
    std::cerr << "Filename: " << blank_file << std::endl;
    return false;
  }

  jdi::Context &builtin = jdi::builtin_context();
  auto include_paths = get_gcc_include_paths(cpp_std, blank_file, paths_file);
  if (include_paths.empty()) {
    std::cerr << "Cannot continue with empty #include<...> search path list. "
                 "Abort." << std::endl;
    return false;
  }
  for (const std::string& p : include_paths) builtin.add_search_directory(p);

  std::string preprocess_command =
      "cpp -dM -x c++ --std=" + cpp_std + " -E " + blank_file.u8string() +
      " > " + defines_file.u8string();
  std::cout << "Obtaining GCC defines list via command line: "
            << preprocess_command << std::endl;
  if (std::system(preprocess_command.c_str()) != 0) {
    std::cerr << "Failed to generate GCC defines. "
                 "These are required to parse GNU headers. Abort." << std::endl;
    return false;
  } else {
    std::cout << "Command succeeded. Reading in GCC definitions.";
  }
  llreader macro_reader(defines_file);
  return true;
}

static void do_cli(jdi::Context &ct) {
  char c = ' ';
  jdi::macro_map undamageable = ct.get_macros();
  jdi::ErrorHandler *herr = jdi::default_error_handler;
  while (c != 'q' && c != '\n' && !feof(stdin)) {
    switch (c) {
      case 'd': {
        bool justflags, justorder;
        justflags = false;
        justorder = false;
        if (false) { case 'f': justflags = true; justorder = false; }
        if (false) { case 'o': justorder = true; justflags = false; }
        std::cout << "Enter the item to define:" << std::endl
                  << ">> " << std::flush;
        char buf[4096]; std::cin.getline(buf, 4096);
        llreader llr("user input", buf, true);
        jdi::lexer c_lex(llr, undamageable, herr);
        jdi::token_t dummy = c_lex.get_token_in_scope(ct.get_global());
        if (dummy.type != jdi::TT_DEFINITION &&
            dummy.type != jdi::TT_DECLARATOR &&
            dummy.type != jdi::TT_SCOPE) {
          herr->error(dummy) << "Expected definition; encountered " << dummy
                             << ". Perhaps your term is a macro?";
          break;
        }
        jdi::context_parser cp(&ct, &c_lex);
        jdi::definition *def = cp.read_qualified_definition(dummy, ct.get_global());
        if (def) {
          if (justflags) {
            std::cout << jdi::flagnames(def->flags) << std::endl;
          }
          else if (justorder) {
            if (def->flags & jdi::DEF_TEMPLATE)
              def = ((jdi::definition_template*)def)->def.get();
            if (def->flags & jdi::DEF_SCOPE) {
              jdi::definition_scope *sc = (jdi::definition_scope*) def;
              for (const auto &entry : sc->dec_order)
                std::cout << " - " << (entry->second ? entry->second->name
                                                     : "<null>") << std::endl;
            }
          }
          else
            std::cout << def->toString() << std::endl;
        }
        break;
      }
      case 'm': {
        std::cout << "Enter the macro to define: " << std::endl
                  << ">> " << std::flush;
        char buf[4096]; std::cin.getline(buf, 4096);
        auto mi = ct.get_macros().find(buf);
        if (mi != ct.get_macros().end()) {
            std::cout << mi->second->toString() << std::endl;
        } else {
          std::cout << "Not found." << std::endl;
        }
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
        std::cout << "Enter the expression to evaluate:" << std::endl
                  << ">> " << std::flush;
        char buf[4096]; std::cin.getline(buf, 4096);
        llreader llr("user input", buf, true);
        jdi::lexer c_lex(llr, undamageable, herr);
        jdi::AST a;
        jdi::context_parser cparse(&ct, &c_lex);
        jdi::token_t dummy = c_lex.get_token_in_scope(ct.get_global());
        if (!cparse.get_AST_builder()->parse_expression(&a, dummy, ct.get_global(), jdi::precedence::all)) {
          if (render) {
            std::cout << "Filename to render to:" << std::endl;
            std::cin.getline(buf, 4096);
            a.writeSVG(buf);
          }
          if (eval) {
            jdi::value v = a.eval(herr->at(dummy));
            std::cout << "Value returned: " << v.toString() << std::endl;
          }
          if (coerce) {
            jdi::full_type t = a.coerce(herr->at(dummy));
            std::cout << "Type of expression: " << t.toString() << std::endl;
            std::cout << (t.def? t.def->toString() : "nullptr") << std::endl;
          }
          if (show) {
            a.writeSVG("/tmp/anus.svg");
            if (system("xdg-open /tmp/anus.svg"))
              std::cout << "Failed to open." << std::endl;
          }
        } else std::cout << "Bailing." << std::endl;
        break;
      }
      case 'h': {
        std::cout <<
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
        std::cout << "Unrecognized command. Empty command or 'q' to quit."
                  << std::endl << std::endl;
        break;
      }
      case ' ': {
        std::cout << "Commands are single-letter; 'h' for help." << std::endl
                  << "Follow commands with ENTER on non-unix." << std::endl;
      }
    }
    std::cout << "> " << std::flush;
    std::cin >> c;
  }
  std::cout << std::endl << "Goodbye" << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::string progname(argc < 1? "JustDefineIt" : *argv);
    std::cerr << "Usage: " << progname << " <input file>" << std::endl;
    return 1;
  }

  if (!prepare_builtin_context()) return 1;
  std::filesystem::path input_file = argv[1];
  std::cout << "Beginning parse of file `" << input_file << "`..." << std::endl;

  llreader f(input_file);
  if (!f.is_open()) {
    std::cerr << "Failed to open `" << input_file << "` for read; abort."
              << std::endl;
    return 1;
  }

  jdi::Context user_program;
  // HACK: This is the difference between complete success and failure on some
  // GNU headers in 03 mode. GCC adopted __attribute__ long before it was final.
  user_program.add_macro_func("__attribute__", "", "", true);
  int res = user_program.parse_stream(f);

  if (res) std::cout << std::endl << "====[------------------------------ FAILURE. ------------------------------]====" << std::endl << std::endl;
  else     std::cout << std::endl << "====[++++++++++++++++++++++++++++++ SUCCESS! ++++++++++++++++++++++++++++++]====" << std::endl << std::endl;
  std::cout << "Parse completed with "
            << jdi::default_error_handler->error_count << " errors and "
            << jdi::default_error_handler->warning_count << " warnings."
            << std::endl;

  do_cli(user_program);
  return 0;
}
