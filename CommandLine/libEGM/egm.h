/** Copyright (C) 2018-2020 Greg Williamson, Robert B. Colton
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
  virtual std::optional<Background> LoadBackground(const fs::path& fName) const override;
  virtual std::optional<Sound> LoadSound(const fs::path& fName) const override;
  virtual std::optional<Sprite> LoadSprite(const fs::path& fName) const override;
  virtual std::optional<Shader> LoadShader(const fs::path& fName) const override;
  virtual std::optional<Font> LoadFont(const fs::path& fName) const override;
  virtual std::optional<Object> LoadObject(const fs::path& fName) const override;
  virtual std::optional<Timeline> LoadTimeLine(const fs::path& fName) const override;
  virtual std::optional<Room> LoadRoom(const fs::path& fName) const override;
  virtual std::optional<Path> LoadPath(const fs::path& fName) const override;
  virtual std::optional<Script> LoadScript(const fs::path& fName) const override;
  // Write
  bool WriteEGM(std::string fName, buffers::Project* project) const;

 private:
  // Reading ===================================================================
  template<class T>
  std::optional<T> LoadRes(const fs::path& fName) const {
    T res;
    if (!LoadResource(fName, &res, 0)) return {};
    return res;
  }

  bool LoadEGM(const fs::path& yamlFile, buffers::Game* game) const;

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

#endif