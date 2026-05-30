/* Copyright (C) 2018-2020 Greg Williamson, Robert B. Colton
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
*/

#ifndef EGM_H
#define EGM_H

#include "file-format.h"

#include <yaml-cpp/yaml.h>

#include <map>

namespace egm {

// Reads and writes EGM files
class EGMFileFormat : public FileFormat {
 public:
  EGMFileFormat(const EventData* event_data) : FileFormat(event_data) {}
  
  // Read
  virtual std::unique_ptr<Project> LoadProject(const fs::path& fName) const override;

  // Write
  virtual bool WriteProject(Project* project, const fs::path& fName) const override;

 private:
  // Reading ===================================================================
  bool LoadEGM(const fs::path& yamlFile, buffers::Game* game) const;

  bool LoadTree(const fs::path& fPath, YAML::Node yaml,
                buffers::TreeNode* buffer) const;
  bool LoadDirectory(const fs::path& fPath, buffers::TreeNode* n,
                     int depth) const;
  bool LoadResource(const fs::path& fPath, google::protobuf::Message *m,
                    int id) const;
  void RecursivePackBuffer(google::protobuf::Message *m, int id,
                           YAML::Node& yaml, const fs::path& fPath,
                           int depth) const;
  virtual bool PackResource(const fs::path& fPath, google::protobuf::Message *m) const override;

  // Writing ===================================================================
  bool WriteNode(buffers::TreeNode* folder, std::string dir,
                 const fs::path &egm_root, YAML::Emitter& tree) const;
  bool WriteRes(buffers::TreeNode* res, const fs::path &dir,
                const fs::path &egm_root) const;
  bool WriteObject(const fs::path &egm_root, const fs::path &dir,
                   const buffers::resources::Object& object) const;
  virtual bool DumpResource(TreeNode* res, const fs::path& fName) const override;

  // 'Rithmatic ================================================================
  std::map<std::string, const buffers::TreeNode*> FlattenTree(
      const buffers::TreeNode &tree);
};

} //namespace egm

#endif
