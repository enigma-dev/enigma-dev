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

#include <sys/types.h> //mkdir
#include <sys/stat.h>
#include <unistd.h> //chdir
#include <iostream>
#include <fstream>

using CppType = google::protobuf::FieldDescriptor::CppType;

namespace egm {

bool CreateDirectory(std::string dir) {
  /* MSYS2/MINGW64 error:
  egm.cpp:33:39: error: too many arguments to function 'int mkdir(const char*)'
   int result = mkdir(dir.c_str(), 0777);
                                       ^
  */
  /*
  int result = mkdir(dir.c_str(), 0777);
  if (result != 0 && errno != EEXIST) {
    std::cerr << "Error: Failed to Create Directory: " << dir << " " << errno << std::endl;
    return false;
  }
  */

  return true;
}

bool ChangeDirectory(std::string dir) {
  if (chdir(dir.c_str()) != 0) {
    std::cerr << "Error: Failed to Change Directory to: " << dir << std::endl;
    return false;
  }

  return true;
}

bool CopyFile(std::string src, std::string dst) {
  //std::cout << src << " > " << dst << std::endl;

  if (src == dst)
    return true;

  std::ifstream s(src, std::ios::binary);
  std::ofstream d(dst, std::ios::binary);

  d << s.rdbuf();

  return true; // how 2 check for errors?
}

std::string RemovePath(std::string fName) {
  size_t pos = fName.find_last_of('/');
  if (pos == std::string::npos)
    return fName;

  return fName.substr(pos+1, fName.length());
}

std::string Proto2String(google::protobuf::Message *m, const google::protobuf::FieldDescriptor *field) {
  const google::protobuf::Descriptor *desc = m->GetDescriptor();
  const google::protobuf::Reflection *refl = m->GetReflection();
  switch (field->cpp_type()) {
    case CppType::CPPTYPE_INT32:
      return std::to_string(refl->GetInt32(*m, field));
    case CppType::CPPTYPE_INT64:
      return std::to_string(refl->GetInt64(*m, field));
    case CppType::CPPTYPE_UINT32:
      return std::to_string(refl->GetUInt32(*m, field));
    case CppType::CPPTYPE_UINT64:
      return std::to_string(refl->GetUInt64(*m, field));
    case CppType::CPPTYPE_DOUBLE:
      return std::to_string(refl->GetDouble(*m, field));
    case CppType::CPPTYPE_FLOAT:
      return std::to_string(refl->GetFloat(*m, field));
    case CppType::CPPTYPE_BOOL:
      return std::to_string(refl->GetBool(*m, field));
    case CppType::CPPTYPE_ENUM:
      return std::to_string(refl->GetEnumValue(*m, field));
    case CppType::CPPTYPE_STRING:
      return refl->GetString(*m, field);
  }

  return "";
}

bool WriteYaml(std::string& dir, std::string& lastDir, std::string& ext, YAML::Emitter& yaml, google::protobuf::Message *m) {
  std::string newDir = dir + ext;
  std::vector<std::string> files;

  const google::protobuf::Descriptor *desc = m->GetDescriptor();
  const google::protobuf::Reflection *refl = m->GetReflection();
  for (int i = 0; i < desc->field_count(); i++) {

    const google::protobuf::FieldDescriptor *field = desc->field(i);
    const google::protobuf::OneofDescriptor *oneof = field->containing_oneof();
    if (oneof && refl->HasOneof(*m, oneof)) continue;
    const google::protobuf::FieldOptions opts = field->options();
    const bool isFilePath = opts.GetExtension(buffers::file_path);

    if (field->is_repeated()) {
      if (field->cpp_type() == CppType::CPPTYPE_MESSAGE) {
        yaml << YAML::BeginSeq;
        for (int i = 0; i < refl->FieldSize(*m, field); i++) {
          yaml << YAML::BeginMap;
          WriteYaml(dir,lastDir, ext, yaml, refl->MutableRepeatedMessage(m, field, i));
          yaml << YAML::EndMap;
        }
        yaml << YAML::EndSeq;
      }
      else if (field->cpp_type() == CppType::CPPTYPE_STRING) {
        yaml << YAML::Key << field->name();
        yaml << YAML::BeginSeq;
        for (int i = 0; i < refl->FieldSize(*m, field); i++) {
          std::string str = refl->GetRepeatedString(*m, field, i);
          if (isFilePath) {
            files.push_back(str);
            refl->SetRepeatedString(m, field, i, newDir + "/data/" + RemovePath(str));
          }
          yaml << refl->GetRepeatedString(*m, field, i);
        }
        yaml << YAML::EndSeq;
      }
    } else {
      if (field->cpp_type() == CppType::CPPTYPE_MESSAGE) {
        yaml << YAML::Key << field->name();
        yaml << YAML::BeginMap;
        WriteYaml(dir, lastDir, ext, yaml, refl->MutableMessage(m, field));
        yaml << YAML::EndMap;
      } else {
        if (isFilePath) {
          files.push_back(Proto2String(m, field));
          refl->SetString(m, field, newDir + "/data/" + RemovePath(Proto2String(m, field)));
        }
        yaml << YAML::Key << field->name();
        yaml << YAML::Value << Proto2String(m, field);
      }
    }
  }

  if (!CreateDirectory(newDir) || !ChangeDirectory(newDir))
      return false;

  yaml << YAML::EndMap;
  std::ofstream fout("properties.yaml");
  fout << yaml.c_str();

  if (files.size() > 0) {
    if (!CreateDirectory(newDir + "/data") || !ChangeDirectory(newDir + "/data"))
      return false;

    for (std::string& f : files) {
      if (!CopyFile(f, RemovePath(f))) return false;
    }
  }

  if (!ChangeDirectory(lastDir))
    return false;
}

void WriteScript(std::string fName, buffers::resources::Script* scr) {
  std::ofstream fout(fName);
  fout << scr->code();
}

void WriteShader(std::string fName, buffers::resources::Shader* shdr) {
  std::ofstream vout(fName + ".vert");
  vout << shdr->vertex_code();

  std::ofstream fout(fName + ".frag");
  fout << shdr->fragment_code();
}

bool WriteRes(buffers::TreeNode* res, std::string& dir) {
  std::string ext;
  std::string newDir = dir + res->name();
  YAML::Emitter yaml;
  yaml << YAML::BeginMap;

  if (res->has_background()) {
    ext = ".bkg";
    WriteYaml(newDir, dir, ext, yaml, res->mutable_background());
  }
  else if (res->has_font()) {
    ext = ".fnt";
    WriteYaml(newDir, dir, ext, yaml, res->mutable_font());
  }
  //if (res->has_include)
  else if (res->has_object()) {
    ext = ".obj";
    WriteYaml(newDir, dir, ext, yaml, res->mutable_object());
  }
  else if (res->has_path()) {
    ext = ".pth";
    WriteYaml(newDir, dir, ext, yaml, res->mutable_path());
  }
  else if (res->has_room()) {
    ext = ".rm";
    WriteYaml(newDir, dir, ext, yaml, res->mutable_room());
  }
  else if (res->has_script()) {
    ext = ".edl";
    WriteScript(newDir + ext, res->mutable_script());
  }
  else if (res->has_shader()) {
    WriteShader(newDir, res->mutable_shader());
  }
  else if (res->has_sound()) {
    ext = ".snd";
    WriteYaml(newDir, dir, ext, yaml, res->mutable_sound());
  }
  else if (res->has_sprite()) {
    ext = ".spr";
    WriteYaml(newDir, dir, ext, yaml, res->mutable_sprite());
  }
  else if (res->has_timeline()) {
    ext = ".tml";
    WriteYaml(newDir, dir, ext, yaml, res->mutable_timeline());
  }
  else {
    std::cerr << "Error: Unsupported Resource Type" << std::endl;
    //return false;
  }

  return true;
}


bool WriteNode(buffers::TreeNode* folder, std::string dir) {
  for (unsigned i = 0; i < folder->child_size(); i++) {
    auto child = folder->mutable_child(i);
    if (child->has_folder()) {
      if (!CreateDirectory(child->name()))
        return false;

      std::string lastDir = dir;
      std::string newDir = dir + child->name() + "/";

      if (!ChangeDirectory(newDir))
        return false;

      if (!WriteNode(child, newDir))
        return false;

      if (!ChangeDirectory(lastDir))
        return false;
    }
    else if (!WriteRes(child, dir))
      return false;
  }

  return true;
}

bool WriteEGM(std::string fName, buffers::Project* project) {

  if (fName.back() != '/')
    fName += '/';

  if (!CreateDirectory(fName))
    return false;

  if (!ChangeDirectory(fName))
    return false;

  return WriteNode(project->mutable_game()->mutable_root(), fName);
}

} //namespace egm
