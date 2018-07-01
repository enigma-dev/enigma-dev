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

#include "egm.h"

#include "yaml-cpp/yaml.h"
#include <iostream>
#include <fstream>

static_assert(__cplusplus >= 201103L, "C++11 is required");
#if __cplusplus >= 201703L
  #include <filesystem>
  namespace fs = std::filesystem;
#else
  #include <experimental/filesystem>
  namespace fs = std::experimental::filesystem;
#endif

namespace proto = google::protobuf;
using CppType = proto::FieldDescriptor::CppType;
using std::string;

namespace {

bool CreateDirectory(const fs::path &directory) {
  std::error_code ec;
  if (fs::create_directory(directory, ec) || !ec) return true;
  std::cerr << "Failed to create directory " << directory << std::endl;
  return false;
}

bool StartsWith(const string &str, const string &prefix) {
  if (prefix.length() > str.length()) return false;
  return str.substr(0, prefix.length()) == prefix;
}

string StripPath(string fName) {
  size_t pos = fName.find_last_of('/');
  if (pos == string::npos)
    return fName;

  return fName.substr(pos+1, fName.length());
}

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

fs::path InternalizeFile(const fs::path &file,
                         const fs::path &directory, const fs::path &egm_root) {
  const fs::path data = "data";
  fs::path demistified = fs::canonical(fs::absolute(file));
  if (StartsWith(demistified, egm_root)) {
    return fs::relative(demistified, directory);
  }
  if (!CreateDirectory(directory/data)) {
    std::cerr << "Failed to copy \"" << file
              << "\" into EGM: could not create output directory." << std::endl;
    return "";
  }
  fs::path relative = data/StripPath(file);
  if (!fs::copy_file(file, directory/relative)) {
    std::cerr << "Failed to copy \"" << file << "\" into EGM." << std::endl;
    return "";
  }
  return relative;
}

bool WriteYaml(const fs::path &egm_root, const fs::path &dir,
               YAML::Emitter &yaml, proto::Message *m) {
  if (!CreateDirectory(dir))
    return false;

  const proto::Descriptor *desc = m->GetDescriptor();
  const proto::Reflection *refl = m->GetReflection();
  for (int i = 0; i < desc->field_count(); i++) {

    const proto::FieldDescriptor *field = desc->field(i);
    const proto::OneofDescriptor *oneof = field->containing_oneof();
    if (oneof && refl->HasOneof(*m, oneof)) continue;
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
            refl->SetRepeatedString(m, field, i, internalized);
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
          refl->SetString(m, field, internalized);
        }
        yaml << YAML::Value << Proto2String(*m, field);
      }
    }
  }

  return true;
}

bool WriteYaml(const fs::path &egm_root, const fs::path &dir, proto::Message *m) {
  YAML::Emitter yaml;
  yaml << YAML::BeginMap;

  if (!WriteYaml(egm_root, dir, yaml, m))
    return false;

  yaml << YAML::EndMap;
  if (std::ofstream out{dir/"properties.yaml"}) {
    out << yaml.c_str();
  } else {
    std::cerr << "Failed to open resource properties file "
              << dir/"properties.yaml" << " for write!" << std::endl;
    return false;
  }

  return true;
}

bool WriteScript(string fName, buffers::resources::Script* scr) {
  if (std::ofstream fout{fName}) {
    fout << scr->code();
    return true;
  }
  std::cerr << "Failed to open script " << fName << " for write" << std::endl;
  return false;
}

bool WriteShader(string fName, buffers::resources::Shader* shdr) {
  if (std::ofstream vout{fName + ".vert"}) {
    vout << shdr->vertex_code();

    if (std::ofstream fout{fName + ".frag"}) {
      fout << shdr->fragment_code();
      return true;
    }
  }

  std::cerr << "Failed to open shader " << fName << " for write" << std::endl;
  return false;
}

bool WriteRes(buffers::TreeNode* res, const fs::path &dir, const fs::path &egm_root) {
  string newDir = dir/res->name();
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
    return WriteYaml(egm_root, newDir + ".rm", res->mutable_room());
   case Type::kScript:
    return WriteScript(newDir + ".edl", res->mutable_script());
   case Type::kShader:
    return WriteShader(newDir, res->mutable_shader());
   case Type::kSound:
    return WriteYaml(egm_root, newDir + ".snd", res->mutable_sound());
   case Type::kSprite:
    return WriteYaml(egm_root, newDir + ".spr", res->mutable_sprite());
   case Type::kTimeline:
    return WriteYaml(egm_root, newDir + ".tml", res->mutable_timeline());
   default:
    std::cerr << "Error: Unsupported Resource Type" << std::endl;
    return false;
  }
}

bool WriteNode(buffers::TreeNode* folder, string dir, string egm_root) {
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
