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
#include "filesystem.h"

#include <yaml-cpp/yaml.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/util/message_differencer.h>
#include <iostream>
#include <fstream>

namespace proto = google::protobuf;
using CppType = proto::FieldDescriptor::CppType;
using std::string;

namespace {

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
               YAML::Emitter &yaml, proto::Message *m) {
  const proto::Descriptor *desc = m->GetDescriptor();
  const proto::Reflection *refl = m->GetReflection();
  for (int i = 0; i < desc->field_count(); i++) {

    const proto::FieldDescriptor *field = desc->field(i);
    const proto::OneofDescriptor *oneof = field->containing_oneof();
    if (oneof && refl->HasOneof(*m, oneof)) continue;
    if (!FieldIsPresent(m, refl, field)) continue;
    const proto::FieldOptions opts = field->options();
    const bool isFilePath = opts.GetExtension(buffers::file_path);

    yaml << YAML::Key << field->name();
    if (field->is_repeated()) {
      if (field->cpp_type() == CppType::CPPTYPE_MESSAGE) {
        yaml << YAML::BeginSeq;
        for (int i = 0; i < refl->FieldSize(*m, field); i++) {
          yaml << YAML::BeginMap;
          WriteYaml(egm_root, dir, yaml,
                    refl->MutableRepeatedMessage(m, field, i));
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
        WriteYaml(egm_root, dir, yaml, refl->MutableMessage(m, field));
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
  if (!CreateDirectory(dir))
    return false;

  YAML::Emitter yaml;
  yaml << YAML::BeginMap;

  if (!WriteYaml(egm_root, dir, yaml, m))
    return false;

  yaml << YAML::EndMap;
  if (std::ofstream out{(dir/"properties.yaml").string()}) {
    out << yaml.c_str();
  } else {
    std::cerr << "Failed to open resource properties file "
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
  std::cerr << "Failed to open script " << fName << " for write" << std::endl;
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

  std::cerr << "Failed to open shader " << fName << " for write" << std::endl;
  return false;
}

template<typename T>
bool WriteRoomSnowflakes(const fs::path &egm_root, const fs::path &dir,
                         YAML::Emitter &yaml, T *layers) {
  if (layers->snowflakes.size()) {
    yaml << YAML::BeginMap;
    yaml << YAML::Key << "Format" << "yaml-proto";
    yaml << YAML::Key << "Data";
    yaml << YAML::BeginSeq;
    for (auto &inst : layers->snowflakes) {
      if (!WriteYaml(egm_root, dir, yaml, &inst)) return false;
    }
    yaml << YAML::EndSeq;
    yaml << YAML::EndMap;
  }
  return true;
}

bool WriteRoom(const fs::path &egm_root, const fs::path &dir,
               buffers::resources::Room *room) {
  if (!CreateDirectory(dir))
    return false;

  buffers::resources::Room cleaned = *room;
  cleaned.clear_instances();
  cleaned.clear_tiles();

  // Build tile and instance layers.
  auto tile_layers = egm::util::BuildTileLayers(*room);

  YAML::Emitter yaml;
  yaml << YAML::BeginMap;

  if (!WriteYaml(egm_root, dir, yaml, &cleaned))
    return false;

  *cleaned.mutable_instances() = room->instances();
  *cleaned.mutable_tiles() = room->tiles();

  if (!proto::util::MessageDifferencer::Equivalent(*room, cleaned)) {
    std::cerr << "WARNING: Room " << dir << " contained external file "
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

  // Build and append instance layers.
  auto inst_layers = egm::util::BuildInstanceLayers(*room);
  yaml << YAML::Key << "instance-layers";
  yaml << YAML::BeginSeq;
  for (const auto &layer : inst_layers.layers) {
    yaml << YAML::BeginMap;
    yaml << YAML::Key << "Format" << layer.format;
    yaml << YAML::Key << "Data" << YAML::Literal << layer.data;
    yaml << YAML::EndMap;
  }
  WriteRoomSnowflakes(egm_root, dir, yaml, &inst_layers);
  yaml << YAML::EndSeq;

  // Append tile layers.
  yaml << YAML::Key << "tile-layers" << YAML::BeginSeq;
  for (const auto &layer : tile_layers.layers) {
    yaml << YAML::BeginMap;
    yaml << YAML::Key << "Format" << layer.format;
    yaml << YAML::Key << "Data" << layer.data;
    yaml << YAML::EndMap;
  }
  WriteRoomSnowflakes(egm_root, dir, yaml, &tile_layers);
  yaml << YAML::EndSeq;

  yaml << YAML::EndMap;
  if (std::ofstream out{(dir/"properties.yaml").string()}) {
    out << yaml.c_str();
  } else {
    std::cerr << "Failed to open resource properties file "
              << dir/"properties.yaml" << " for write!" << std::endl;
    return false;
  }

  return true;
}

bool WriteRes(buffers::TreeNode* res, const fs::path &dir, const fs::path &egm_root) {
  string newDir = (dir/res->name()).string();
  using Type = buffers::TreeNode::TypeCase;
  switch (res->type_case()) {
   case Type::kBackground:
    return WriteYaml(egm_root, newDir + ".bkg", res->mutable_background());
   case Type::kFont:
    return WriteYaml(egm_root, newDir + ".fnt", res->mutable_font());
   case Type::kObject:
    return WriteYaml(egm_root, newDir + ".obj", res->mutable_object());
   case Type::kPath:
    return WriteYaml(egm_root, newDir + ".pth", res->mutable_path());
   case Type::kRoom:
    return WriteRoom(egm_root, newDir + ".rm", res->mutable_room());
   case Type::kScript:
    return WriteScript(newDir + ".edl", res->script());
   case Type::kShader:
    return WriteShader(newDir, res->shader());
   case Type::kSound:
    return WriteYaml(egm_root, newDir + ".snd", res->mutable_sound());
   case Type::kSprite:
    return WriteYaml(egm_root, newDir + ".spr", res->mutable_sprite());
   case Type::kTimeline:
    return WriteYaml(egm_root, newDir + ".tln", res->mutable_timeline());
   default:
    std::cerr << "Error: Unsupported Resource Type" << std::endl;
    return false;
  }
}

bool WriteNode(buffers::TreeNode* folder, string dir, const fs::path &egm_root) {
  for (int i = 0; i < folder->child_size(); i++) {
    auto child = folder->mutable_child(i);
    if (child->has_folder()) {
      if (!CreateDirectory(dir + "/" + child->name()))
        return false;

      string lastDir = dir;
      string newDir = dir + child->name() + "/";

      if (!WriteNode(child, newDir, egm_root))
        return false;
    }
    else if (!WriteRes(child, dir, egm_root))
      return false;
  }

  return true;
}

}  // namespace

namespace egm {

bool WriteEGM(string fName, buffers::Project* project) {

  if (fName.back() != '/')
    fName += '/';

  if (!CreateDirectory(fName))
    return false;

  fs::path abs_root = fs::canonical(fs::absolute(fName));
  return WriteNode(project->mutable_game()->mutable_root(), fName, abs_root);
}

} //namespace egm
