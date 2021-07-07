#include "gmx.h"
#include "gmk.h"
#include "yyp.h"
#include "egm.h"
#include "filesystem.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
  if (argc != 3) {
    std::cerr << "Usage: gm2egm <input> <output>" << std::endl;
    return -1;
  }
  
  const std::filesystem::path outDir = argv[2];
  if (FolderExists(outDir)) {
    std::cerr << '"' << outDir << '"' << " already exists; would you like to overwrite it? (Y/N)" << std::endl;
    char c;
    std::cin >> c;
    if (c == 'y' || c == 'Y')
      DeleteFolder(outDir);
    else {
      std::cerr << "Aborting." << std::endl;
      return -5;
    }
  }

  EventData event_data(ParseEventFile("events.ey"));
  egm::LibEGMInit(&event_data);

  std::unique_ptr<buffers::Project> project = egm::LoadProject(argv[1]);

  if (project == nullptr) {
    std::cerr << "Error: Failure opening file \"" << argv[1] << "\"" << std::endl;
    return -3;
  }
  if (!egm::WriteProject(project.get(), outDir)) {
    std::cerr << "Error: Failure writting \"" << argv[2] << std::endl;
    return -4;
  }
  
  std::cout << "Success" << std::endl;

  return 0;
}
