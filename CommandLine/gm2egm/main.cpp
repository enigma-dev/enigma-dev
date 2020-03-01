#include "gmx.h"
#include "gmk.h"
#include "yyp.h"
#include "egm.h"

#include "strings_util.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
  if (argc != 3) {
    std::cerr << "Usage: gm2egm <input> <output>" << std::endl;
    return -1;
  }

  std::string input_file = argv[1];
  std::string ext;
  size_t dot = input_file.find_last_of('.');
  if (dot != std::string::npos) ext = tolower(input_file.substr(dot + 1));

  buffers::Project* project = nullptr;

  if (ext == "gm81" || ext == "gmk" || ext == "gm6" || ext == "gmd") {
    project = gmk::LoadGMK(input_file);
  } else if (ext == "gmx") {
    project = gmx::LoadGMX(input_file);
  } else if (ext == "yyp") {
    project = yyp::LoadYYP(input_file);
  } else {
    std::cerr << "Error: Unkown extenstion \"" << ext << "\"." << std::endl; 
    return -2;
  }

  if (project == nullptr) {
    std::cerr << "Error: Failure opening file \"" << input_file << "\"" << std::endl;
    return -3;
  }

  if (!egm::WriteEGM(argv[2], project)) {
    std::cerr << "Error: Failure writting \"" << argv[2] << std::endl;
    return -4;
  }

  return 0;
}
