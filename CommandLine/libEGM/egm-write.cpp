/** Copyright (C) 2018 Greg Williamson, Robert B. Colton, Josh Ventura
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
#include "egm-events.h"
#include "filesystem.h"

#include <yaml-cpp/yaml.h>
#include <event_reader/event_parser.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/util/message_differencer.h>
#include <iostream>
#include <fstream>

namespace proto = google::protobuf;
using Type = buffers::TreeNode::TypeCase;
using CppType = proto::FieldDescriptor::CppType;
using std::string;
using egm::errStream;

namespace egm_internal {

string Proto2String(const proto::Message &m, const proto::FieldDescriptor *field) {
  const proto::Reflection *refl = m.GetReflection();
  switch (field->cpp_type()) {
    case CppType::CPPTYPE_INT32:
      return std::to_string(refl->GetInt32(m, field));
    case CppType::CPPTYPE_INT64:
      return std::to_string(refl->GetInt64(m, field));
    case CppType::CPPTYPE_UINT32:
      return std::to_string(refl->GetUInt32(m, field));
    case CppType::CPPTYPE_UINT64:
      return std::to_string(refl->GetUInt64(m, field));
    case CppType::CPPTYPE_DOUBLE:
      return std::to_string(refl->GetDouble(m, field));
    case CppType::CPPTYPE_FLOAT:
      return std::to_string(refl->GetFloat(m, field));
    case CppType::CPPTYPE_BOOL:
      return std::to_string(refl->GetBool(m, field));
    case CppType::CPPTYPE_ENUM:
      return std::to_string(refl->GetEnumValue(m, field));
    case CppType::CPPTYPE_STRING:
      return refl->GetString(m, field);
    case CppType::CPPTYPE_MESSAGE: // Handled by caller.
      break;
  }

  return "";
}

bool FieldIsPresent(proto::Message *m,
    const proto::Reflection *refl, const proto::FieldDescriptor *field) {
  if (field->is_repeated()) return refl->FieldSize(*m, field);
  return refl->HasField(*m, field);
}

bool WriteYaml(const fs::path &egm_root, const fs::path &dir,
               YAML::Emitter &yaml, proto::Message *m, int depth) {
  const proto::Descriptor *desc = m->GetDescriptor();
  const proto::Reflection *refl = m->GetReflection();
  for (int i = 0; i < desc->field_count(); i++) {

    const proto::FieldDescriptor *field = desc->field(i);
    const proto::OneofDescriptor *oneof = field->containing_oneof();
    if (oneof && refl->HasOneof(*m, oneof)) continue;
    if (!FieldIsPresent(m, refl, field)) continue;
    if (depth == 0 && field->name() == "id") continue;
    const proto::FieldOptions opts = field->options();
    const bool isFilePath = opts.GetExtension(buffers::file_path);

    yaml << YAML::Key << field->name();
    if (field->is_repeated()) {
      if (field->cpp_type() == CppType::CPPTYPE_MESSAGE) {
        yaml << YAML::BeginSeq;
        for (int i = 0; i < refl->FieldSize(*m, field); i++) {
          yaml << YAML::BeginMap;
          WriteYaml(egm_root, dir, yaml,
                    refl->MutableRepeatedMessage(m, field, i), depth+1);
          yaml << YAML::EndMap;
        }
        yaml << YAML::EndSeq;
      }
      else if (field->cpp_type() == CppType::CPPTYPE_STRING) {
        yaml << YAML::BeginSeq;
        for (int i = 0; i < refl->FieldSize(*m, field); i++) {
          const string str = refl->GetRepeatedString(*m, field, i);
          if (isFilePath) {
            const fs::path internalized = InternalizeFile(str, dir, egm_root);
            if (internalized.empty()) return false;
            refl->SetRepeatedString(m, field, i, internalized.string());
          }
          yaml << refl->GetRepeatedString(*m, field, i);
        }
        yaml << YAML::EndSeq;
      }
    } else {
      if (field->cpp_type() == CppType::CPPTYPE_MESSAGE) {
        yaml << YAML::BeginMap;
        WriteYaml(egm_root, dir, yaml, refl->MutableMessage(m, field), depth+1);
        yaml << YAML::EndMap;
      } else {
        if (isFilePath) {
          const fs::path src = Proto2String(*m, field);
          const fs::path internalized = InternalizeFile(src, dir, egm_root);
          if (internalized.empty()) return false;
          refl->SetString(m, field, internalized.string());
        }
        yaml << YAML::Value << Proto2String(*m, field);
      }
    }
  }

  return true;
}

bool WriteYaml(const fs::path &egm_root, const fs::path &dir, proto::Message *m) {
  if (!CreateDirectoryRegular(dir))
    return false;

  YAML::Emitter yaml;
  yaml << YAML::BeginMap;

  if (!WriteYaml(egm_root, dir, yaml, m, 0))
    return false;

  yaml << YAML::EndMap;
  if (std::ofstream out{(dir/"properties.yaml").string()}) {
    out << yaml.c_str();
  } else {
    errStream << "Failed to open resource properties file "
              << dir/"properties.yaml" << " for write!" << std::endl;
    return false;
  }

  return true;
}

bool WriteScript(string fName, const buffers::resources::Script &scr) {

  if (std::ofstream fout{fName}) {
    fout << scr.code();
    return true;
  }
  errStream << "Failed to open script " << fName << " for write" << std::endl;
  return false;
}

bool WriteShader(string fName, const buffers::resources::Shader &shdr) {

  if (std::ofstream vout{fName + ".vert"}) {
    vout << shdr.vertex_code();

    if (std::ofstream fout{fName + ".frag"}) {
      fout << shdr.fragment_code();
      return true;
    }
  }

  errStream << "Failed to open shader " << fName << " for write" << std::endl;
  return false;
}

template<typename T>
bool WriteRoomSnowflakes(const fs::path &egm_root, const fs::path &dir,
                         YAML::Emitter &yaml, T *layer) {
  if (layer->snowflakes.size()) {
    yaml << YAML::BeginMap;
    yaml << YAML::Key << "Format" << "yaml-proto";
    yaml << YAML::Key << "Data";
    yaml << YAML::BeginSeq;
    for (auto &inst : layer->snowflakes) {
      if (!WriteYaml(egm_root, dir, yaml, &inst, 0)) return false;
    }
    yaml << YAML::EndSeq;
    yaml << YAML::EndMap;
  }
  return true;
}

bool WriteRoom(const fs::path &egm_root, const fs::path &dir,
               buffers::resources::Room *room) {
  if (!CreateDirectoryRegular(dir))
    return false;

  buffers::resources::Room cleaned = *room;
  cleaned.clear_instances();
  cleaned.clear_tiles();
  cleaned.clear_creation_code();

  // Build tile and instance layers.
  auto tile_layers = egm::util::BuildTileLayers(*room);

  YAML::Emitter yaml;
  yaml << YAML::BeginMap;

  if (!WriteYaml(egm_root, dir, yaml, &cleaned, 0))
    return false;

  *cleaned.mutable_instances() = room->instances();
  *cleaned.mutable_tiles() = room->tiles();
  cleaned.set_creation_code(room->creation_code());

  if (!proto::util::MessageDifferencer::Equivalent(*room, cleaned)) {
    errStream << "WARNING: Room " << dir << " contained external file "
                 "references or has otherwise been modified for no reason. "
                 "Neither possibility is expected. Diff:" << std::endl;
    /* Destruct reporters before modifying protos. */ {
      proto::io::OstreamOutputStream  gcout(&std::cout);
      proto::util::MessageDifferencer::StreamReporter reporter(&gcout);
      proto::util::MessageDifferencer differ;
      differ.ReportDifferencesTo(&reporter);
      differ.Compare(*room, cleaned);
    }
    *room = cleaned;  // Propagate the subroutine's changes...
  }

  // Write the code to edl
  if (std::ofstream fout{(dir/"roomcreate.edl").string()}) {
    fout << room->creation_code();
  } else return false;

  for (auto &inst : room->instances()) {
  if (!inst.creation_code().empty()) {
    string name = inst.name();
    if (name.empty()) name = std::to_string(inst.id());
    string edlFile = dir.string() + "/create[" + name + "].edl";

    if (std::ofstream fout{edlFile}) {
        fout << inst.creation_code();
      } else return false;
    }
  }

  // Build and append instance layers.
  auto inst_layers = egm::util::BuildInstanceLayers(*room);
  yaml << YAML::Key << "instance-layers";
  yaml << YAML::BeginSeq;
  for (auto &layer : inst_layers) {
    yaml << YAML::BeginMap;
    yaml << YAML::Key << "Format" << layer.format;
    yaml << YAML::Key << "Data" << YAML::Literal << layer.data;
    yaml << YAML::EndMap;
    WriteRoomSnowflakes(egm_root, dir, yaml, &layer);
  }
  yaml << YAML::EndSeq;

  // Append tile layers.
  yaml << YAML::Key << "tile-layers" << YAML::BeginSeq;
  for (auto &layer : tile_layers) {
    yaml << YAML::BeginMap;
    yaml << YAML::Key << "Format" << layer.format;
    yaml << YAML::Key << "Data" << YAML::Literal << layer.data;
    yaml << YAML::EndMap;
    WriteRoomSnowflakes(egm_root, dir, yaml, &layer);
  }
  yaml << YAML::EndSeq;

  yaml << YAML::EndMap;
  if (std::ofstream out{(dir/"properties.yaml").string()}) {
    out << yaml.c_str();
  } else {
    errStream << "Failed to open resource properties file "
              << dir/"properties.yaml" << " for write!" << std::endl;
    return false;
  }

  return true;
}

bool WriteTimeline(const fs::path &/*egm_root*/, const fs::path &dir,
                   const buffers::resources::Timeline& timeline) {
  if (!CreateDirectoryRegular(dir))
    return false;

  for (auto &m : timeline.moments()) {
    string edlFile = dir.string() + "/step[" + std::to_string(m.step()) + "].edl";
    std::ofstream fout{edlFile};
    fout << m.code();
  }

  return true;
}

}  // namespace
namespace egm {

bool EGMFileFormat::WriteObject(const fs::path &egm_root, const fs::path &dir,
                      const buffers::resources::Object& object) const {
  buffers::resources::Object cleaned = object;
  auto events = object.egm_events();
  cleaned.clear_egm_events();

  if (!egm_internal::WriteYaml(egm_root, dir, &cleaned))
    return false;

  WriteObjectEvents(dir, events, _event_data);

  return true;
}

bool EGMFileFormat::WriteRes(buffers::TreeNode* res, const fs::path &dir,
                   const fs::path &egm_root) const {
  string newDir = (dir/res->name()).string();
  switch (res->type_case()) {
   case Type::kBackground:
    return egm_internal::WriteYaml(egm_root, newDir + ".bkg", res->mutable_background());
   case Type::kFont:
    return egm_internal::WriteYaml(egm_root, newDir + ".fnt", res->mutable_font());
   case Type::kObject:
    return WriteObject(egm_root, newDir + ".obj", res->object());
   case Type::kPath:
    return egm_internal::WriteYaml(egm_root, newDir + ".pth", res->mutable_path());
   case Type::kRoom:
    return egm_internal::WriteRoom(egm_root, newDir + ".rm", res->mutable_room());
   case Type::kScript:
    return egm_internal::WriteScript(newDir + ".edl", res->script());
   case Type::kShader:
    return egm_internal::WriteShader(newDir, res->shader());
   case Type::kSound:
    return egm_internal::WriteYaml(egm_root, newDir + ".snd", res->mutable_sound());
   case Type::kSprite:
    return egm_internal::WriteYaml(egm_root, newDir + ".spr", res->mutable_sprite());
   case Type::kTimeline:
    return egm_internal::WriteTimeline(egm_root, newDir + ".tln", res->timeline());
   default:
    errStream << "Error: Unsupported Resource Type: " << dir << std::endl;
  }

  return true;
}

namespace {

inline const std::string type2name(int type) {
  switch (type) {
   case Type::kFolder:
     return "folder";
   case Type::kBackground:
    return "background";
   case Type::kFont:
    return "font";
   case Type::kObject:
    return "object";
   case Type::kPath:
    return "path";
   case Type::kRoom:
    return "room";
   case Type::kScript:
    return "script";
   case Type::kShader:
    return "shader";
   case Type::kSound:
    return "sound";
   case Type::kSprite:
    return "sprite";
   case Type::kTimeline:
    return "timeline";
   default:
    return "unknown";
  }
}

inline int getResID(buffers::TreeNode* res) {
  switch (res->type_case()) {
   case Type::kBackground:
    return res->background().id();
   case Type::kFont:
    return res->font().id();
   case Type::kObject:
    return res->object().id();
   case Type::kPath:
    return res->path().id();
   case Type::kRoom:
    return res->room().id();
   case Type::kScript:
    return res->script().id();
   case Type::kShader:
    return res->shader().id();
   case Type::kSound:
    return res->sound().id();
   case Type::kSprite:
    return res->sprite().id();
   case Type::kTimeline:
    return res->timeline().id();
   default:
    return -1;
  }
}

}  // namespace

bool EGMFileFormat::WriteNode(buffers::TreeNode* folder, string dir,
                    const fs::path &egm_root, YAML::Emitter& tree) const {
  tree << YAML::BeginMap << YAML::Key << "folder" << YAML::Value << folder->name();

  if (folder->child_size() > 0) {
    tree << YAML::Key << "contents" << YAML::Value << YAML::BeginSeq;
    for (int i = 0; i < folder->child_size(); i++) {
      auto child = folder->mutable_child(i);

      std::string type = type2name(child->type_case());

      if (type != "folder") {
        tree << YAML::BeginMap;
        tree << YAML::Key << "name" << child->name();
        tree << YAML::Key << "type" << type;
        tree << YAML::Key << "id" << getResID(child);
        tree << YAML::EndMap;
      }

      if (child->has_folder()) {
        if (!CreateDirectoryRegular(dir + "/" + child->name()))
          return false;

        string lastDir = dir;
        string newDir = dir + child->name() + "/";

        if (!WriteNode(child, newDir, egm_root, tree))
          return false;
      }
      else if (!WriteRes(child, dir, egm_root))
        return false;
    }

    tree << YAML::EndSeq;
  }

  tree << YAML::EndMap;

  return true;
}

static const std::string EGM_VERSION = "2.0.0";

bool EGMFileFormat::WriteProject(Project* project, const fs::path& fPath) const {
  std::string fName = fPath.u8string();

  if (fName.back() != '/')
    fName += '/';

  if (!CreateDirectoryRegular(fName))
    return false;

  // need to remove the '/' here
  const fs::path projectFile = fName + "/" + fs::path(fName.substr(0, fName.length()-1)).filename().string();

  if (std::ofstream out{projectFile.string()}) { // egm settings
    YAML::Emitter projYAML;
    projYAML << YAML::BeginMap;
    projYAML << YAML::Key << "version" << EGM_VERSION;
    projYAML << YAML::Key << "tree" << "tree.yaml";
    projYAML << YAML::Key << "backup" << "protobuf.bin";
    projYAML << YAML::EndMap;
    out << projYAML.c_str();
  } else {
    errStream << "Failed to open project file "
              << projectFile << " for write!" << std::endl;
    return false;
  }

  std::fstream bin(fName + "/protobuf.bin", std::ios::out | std::ios::trunc | std::ios::binary);
  project->SerializeToOstream(&bin);

  YAML::Emitter tree;

  fs::path abs_root = fs::canonical(fs::absolute(fName));
  bool success = WriteNode(project->mutable_game()->mutable_root(), fName, abs_root, tree);

  if (!success) return false;

  if (std::ofstream out{(abs_root/"tree.yaml").string()}) {
    out << tree.c_str();
  } else {
    errStream << "Failed to open resource tree file "
              << abs_root/"tree.yaml" << " for write!" << std::endl;
    return false;
  }

  return true;
}

bool EGMFileFormat::DumpResource(TreeNode* res, const fs::path& fName) const {
  return WriteRes(res, fName, fName);
}

} //namespace egm
