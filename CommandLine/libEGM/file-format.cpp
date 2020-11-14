#include "file-format.h"
#include "egm.h"
#include "gmx.h"
#include "gmk.h"
#include "yyp.h"
#include "strings_util.h"

namespace egm {

// Access file formats based on extensions
std::unordered_map<std::string, std::unique_ptr<FileFormat>> fileFormats;
void LibEGMInit(const EventData* event_data) {
  fileFormats[".egm"] = std::make_unique<EGMFileFormat>(event_data);
  fileFormats[".gmx"] = std::make_unique<GMXFileFormat>(event_data);
  fileFormats[".gmk"] = std::make_unique<GMKFileFormat>(event_data);
  fileFormats[".yyp"] = std::make_unique<YYPFileFormat>(event_data);
}

std::unique_ptr<Project> LoadProject(const fs::path& fName) {
  if (!fs::exists(fName)) {
    errStream << "File: " << fName.u8string() << " does not exists" << std::endl;
    return nullptr;
  }

  std::string ext;
  std::string inputFile = fName.u8string();

  // if is directory remove ending /
  if (fs::is_directory(fName) && inputFile.back() == '/') {
    inputFile.pop_back();
  }
  
  // find last . 
  size_t dot = inputFile.find_last_of('.');
  if (dot != std::string::npos) ext = ToLower(inputFile.substr(dot));

  // gmk has multiple extensions
  if (ext == ".gm81"  || ext == ".gm6" || ext == ".gmd") {
    ext = ".gmk";
  // add project file to path if egm or gmx directory
  } else if (fs::is_directory(fName) && ext == ".gmx") {
    fs::path p = inputFile;
    inputFile += "/" + p.filename().stem().u8string() + ".project.gmx";
    if (!fs::exists(inputFile)) {
      errStream << "Error: gmx missing project file" << std::endl;
      return nullptr;
    }
  } else if (fs::is_directory(fName) && ext == ".egm") {
    fs::path p = inputFile;
    inputFile += "/" + p.filename().stem().u8string() + ".egm";
    if (!fs::exists(inputFile)) {
      errStream << "Error: egm missing project file" << std::endl;
      return nullptr;
    }
  }

  if (fileFormats.count(ext) > 0) {
    return fileFormats[ext]->LoadProject(inputFile);
  }
   
  errStream << "Error: Unknown file type: \"" << ext << "\" cannot determine type of file" << std::endl;
  return nullptr;

}

// Debugging output streams for file formats
std::ostream outStream(nullptr);
std::ostream errStream(nullptr);

void BindOutputStreams(std::ostream &out, std::ostream &err) { outStream.rdbuf(out.rdbuf()); errStream.rdbuf(err.rdbuf()); }

} //namespace egm
