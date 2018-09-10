/** Copyright (C) 2018 Greg Williamson
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

#include "egm.h"
#include "egm-rooms.h"
#include "filesystem.h"

#include <yaml-cpp/yaml.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/util/message_differencer.h>
#include <iostream>
#include <fstream>

namespace proto = google::protobuf;
using CppType = proto::FieldDescriptor::CppType;
using std::string;

namespace egm {
  
bool LoadResource(google::protobuf::Message *m) {
}

using Type = buffers::TreeNode::TypeCase;
//using FactoryFunction = std::function<google::protobuf::Message *(TreeNode*)>;
const std::map<std::string, int> resTypes = {
  {"folder", Type::kFolder},
  /*{"background", Type::kBackground},
  {"font", Type::kBackground},
  {"object", Type::kBackground},
  {"path", Type::kBackground},
  {"room", Type::kBackground},
  {"script", Type::kBackground},
  {"shader", Type::kBackground},
  {"sound", Type::kBackground},
  {"sprite", Type::kBackground},
  {"timeline", Type::kBackground}*/
};
  
bool RecursiveLoadTree(const fs::path& fPath, YAML::Node yaml, buffers::TreeNode* buffer) {
  for (auto n : yaml["contents"]) {
    buffers::TreeNode* b = buffer->add_child();
    
    if (n.size() > 1) {
      b->set_name(n["name"].as<std::string>());
      b->set_folder(true);
      std::cout << n["name"].as<std::string>() << std::endl;
    }
    
    RecursiveLoadTree(fPath, n, b);
  }
  
  return true;
}

bool LoadTree(const std::string& yaml, buffers::Game* game) {
  YAML::Node tree = YAML::LoadFile(yaml);
  const fs::path egm_root = fs::path(yaml).parent_path();
  buffers::TreeNode* game_root = game->mutable_root();
  game_root->set_name("/");
  return RecursiveLoadTree(egm_root, tree, game_root);
}

buffers::Project* LoadEGM(std::string fName) {
  buffers::Project* proj = new buffers::Project();
  
  if (!FileExists(fName)) {
    std::cerr << "Error: " << fName << " does not exist" << std::endl;
  }
  
  LoadTree(fName, proj->mutable_game());
  
  return proj;
}

} //namespace egm
