/**
 * Simple test program for clang adapter
 * Just parses a file and prints all functions found
 */

#include "clang_adapter.h"
#include "clang_definitions.h"
#include <iostream>
#include <string>
#include <clang-c/Index.h>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <file_to_parse> [options] [include_dir1] [include_dir2] ..." << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  -n <namespace>  Only print functions in the specified namespace" << std::endl;
    std::cerr << "Example: " << argv[0] << " file.cpp -n enigma -I/path/to/headers" << std::endl;
    return 1;
  }
  
  std::string filepath = argv[1];
  std::string namespace_filter;
  
  // Collect include directories and options from command line arguments
  std::vector<std::string> include_dirs;
  for (int i = 2; i < argc; ++i) {
    std::string arg = argv[i];
    // Check for namespace filter option
    if (arg == "-n" && i + 1 < argc) {
      namespace_filter = argv[++i];
      std::cout << "Setting namespace filter: " << namespace_filter << std::endl;
      continue;
    }
    // Support both -I/path and /path formats
    if (arg.substr(0, 2) == "-I") {
      if (arg.length() > 2) {
        std::string path = arg.substr(2);
        include_dirs.push_back(path);
        std::cout << "Adding include directory: " << path << std::endl;
      } else if (i + 1 < argc) {
        // -I /path format
        std::string path = argv[++i];
        include_dirs.push_back(path);
        std::cout << "Adding include directory: " << path << std::endl;
      }
    } else {
      // Just a path without -I prefix
      include_dirs.push_back(arg);
      std::cout << "Adding include directory: " << arg << std::endl;
    }
  }
  
  std::cout << "Creating ClangContext..." << std::endl;
  clang_adapter::ClangContext ctx;
  
  // Set namespace filter if specified
  if (!namespace_filter.empty()) {
    ctx.set_namespace_filter(namespace_filter);
  }
  
  std::cout << "Parsing file: " << filepath << std::endl;
  // Pass include directories to parse_file, which will add them
  int result = ctx.parse_file(filepath, include_dirs, {});
  
  if (result != 0) {
    std::cerr << "Failed to parse file (exit code: " << result << ")" << std::endl;
    return result;
  }
  
  std::cout << "Parse successful! Functions should have been printed above." << std::endl;
  
  // Also try to look up some common functions
  clang_adapter::ClangDefinitionScope* global = ctx.get_global();
  if (global) {
    std::cout << "\nGlobal scope has " << global->members.size() << " members" << std::endl;
    
    // Try to find a few common functions
    const char* test_names[] = {"main", "printf", "malloc", "free", nullptr};
    for (const char** name = test_names; *name; ++name) {
      auto it = global->members.find(*name);
      if (it != global->members.end()) {
        std::cout << "Found: " << *name << std::endl;
      }
    }
  }
  
  return 0;
}
