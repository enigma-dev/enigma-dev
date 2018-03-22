/** Copyright (C) 2018 Greg Williamson, Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "codegen/project.pb.h"

#include <pugixml.hpp>

#include <string>

#include <iostream>

namespace gmx {
buffers::Project* LoadGMX(std::string fName, bool verbose = false);
void PackBuffer(std::string type, std::string res, int &id, google::protobuf::Message *m, std::string gmxPath);
template<class T>
T* LoadResource(std::string fName, std::string type, bool verbose) {
  size_t dot = fName.find_last_of(".");
  size_t slash = fName.find_last_of("/");
  
  if (dot == std::string::npos || slash == dot == std::string::npos)
    return nullptr;
  
  std::string resType = fName.substr(dot+1, fName.length());
  std::string resName = fName.substr(slash+1, fName.length());
  
  dot = resName.find_first_of(".");
  
  if (dot == std::string::npos)
    return nullptr;
  
  resName = resName.substr(0, dot);
  std::string dir = fName.substr(0, slash+1);

  if (resType == "gmx") {
    pugi::xml_document doc;
    if (!doc.load_file(fName.c_str())) return nullptr;
    resType = doc.document_element().name(); // get type from root xml element
  }  else if (resType == "gml") resType = "script";
  
  if (resType != type || resName.empty()) // trying to load wrong type (eg a.gmx has <b> instead of <a> as root xml)
    return nullptr;
  
  int id = 0;
  T* res = new T();
  PackBuffer(resType, resName, id, res, dir);
  return res;
}

}  //namespace gmx
