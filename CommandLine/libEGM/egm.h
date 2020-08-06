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

#include "project.pb.h"
#include "event_reader/event_parser.h"

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <map>
#include <memory>
#include <string>

namespace egm {
namespace fs = std::filesystem;

// Reads and writes EGM files
class EGM {
 public:
  bool WriteEGM(std::string fName, buffers::Project* project) const;
  std::unique_ptr<buffers::Project> LoadEGM(std::string fName) const;

  EGM(const EventData* events): events_(events) {}

 private:
  // Reading ===================================================================
  bool LoadEGM(const std::string& yaml, buffers::Game* game) const;

  bool LoadTree(const fs::path& fPath, YAML::Node yaml,
                buffers::TreeNode* buffer) const;
  bool LoadDirectory(const fs::path& fPath, buffers::TreeNode* n,
                     int depth) const;
  bool LoadResource(const fs::path& fPath, google::protobuf::Message *m,
                    int id) const;
  void LoadObjectEvents(const fs::path& fPath, google::protobuf::Message *m,
                        const google::protobuf::FieldDescriptor *field) const;
  void RecursivePackBuffer(google::protobuf::Message *m, int id,
                           YAML::Node& yaml, const fs::path& fPath,
                           int depth) const;

  // Writing ===================================================================
  bool WriteNode(buffers::TreeNode* folder, std::string dir,
                 const fs::path &egm_root, YAML::Emitter& tree) const;
  bool WriteRes(buffers::TreeNode* res, const fs::path &dir,
                const fs::path &egm_root) const;
  bool WriteObject(const fs::path &egm_root, const fs::path &dir,
                   const buffers::resources::Object& object) const;

  // 'Rithmatic ================================================================
  std::map<std::string, const buffers::TreeNode*> FlattenTree(
      const buffers::TreeNode &tree);
  const EventData* events_;
};

} //namespace egm
