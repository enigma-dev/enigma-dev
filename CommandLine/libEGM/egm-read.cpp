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
#include <functional>

namespace proto = google::protobuf;
using CppType = proto::FieldDescriptor::CppType;
using std::string;

namespace egm {
  
using buffers::TreeNode;
using Type = buffers::TreeNode::TypeCase;
using FactoryFunction = std::function<google::protobuf::Message *(TreeNode*)>;
struct ResourceFactory {
  FactoryFunction func;
  std::string ext;
};
using FactoryMap = std::unordered_map<std::string, ResourceFactory>;

static const FactoryMap factoryMap({
  { "sprite", {&TreeNode::mutable_sprite, ".spr" } },
  { "sound", {&TreeNode::mutable_sound, ".snd" } },
  { "background", {&TreeNode::mutable_background, ".bkg" } },
  { "path", {&TreeNode::mutable_path, ".pth" } },
  { "script", {&TreeNode::mutable_script, ".edl" } },
  { "shader", {&TreeNode::mutable_shader, ".shdr" } },
  { "font", {&TreeNode::mutable_font, ".fnt" } },
  { "timeline", {&TreeNode::mutable_timeline, ".tln" } },
  { "object", {&TreeNode::mutable_object, ".obj" } },
  { "room", {&TreeNode::mutable_room, ".rm" } },
  { "datafile", {&TreeNode::mutable_include, ".dat" } }
});

buffers::resources::Script* LoadScript(const fs::path& fPath) {
  buffers::resources::Script* scr = new buffers::resources::Script();
  
  scr->set_code(FileToString(fPath));
  
  return scr;
}

buffers::resources::Shader* LoadShader(const fs::path& fPath) {
  buffers::resources::Shader* shdr = new buffers::resources::Shader();
  
  const std::string p = fPath.parent_path().string() + "/" + fPath.stem().string();
  shdr->set_vertex_code(FileToString(p + ".vert"));
  shdr->set_fragment_code(p + ".frag");
  
  return shdr;
}

inline void invalidEDLNaming(const fs::path& res, const fs::path& fName) {
  std::cerr << "Ignoring improperly named file " << fName << " in " << res.stem() << std::endl;
  
  if (res.extension() == ".tln")
    std::cerr << "Supported naming for timelines is step[moment].edl" << std::endl;
    
  else if (res.extension() == ".obj")
    std::cerr << "Supported naming for objects is event[subevent].edl" << std::endl;
    
  else if (res.extension() == ".rm")
    std::cerr << "Supported naming for rooms is create[instance].edl" << std::endl;
}

buffers::resources::Timeline* LoadTimeLine(const fs::path& fPath) {
  buffers::resources::Timeline* tln = new buffers::resources::Timeline();
  
  const std::string delim = "step[";
  for(auto& f : fs::directory_iterator(fPath)) {
    
    const std::string stem = f.path().stem(); // base filename
    
    // If its not an edl file, the filename is too short to fit the naming scheme or the filename doesn't end with ]
    // exit here before doing any string cutting to prevent segfaults 
    if (f.path().extension().string() != ".edl" || stem.length() < std::string("step[0]").length() || stem.back() != ']') {
      invalidEDLNaming(stem, fPath);
      continue;
    }
    
    const std::string substr1 = stem.substr(0, delim.length());
    const std::string stepStr = stem.substr(delim.length(), stem.length()-1);
    std::size_t idx;
    const int step = std::stoi(stepStr, &idx);
    // Check the string to int succeeded and the begining of the string is correct.
    if (idx != stepStr.length()-1 || substr1 != delim) {
      invalidEDLNaming(stem, fPath);
      continue;
    }
    
    // If we made it this far we can add event to timeline
    buffers::resources::Timeline_Moment* m = tln->add_moments();
    m->set_step(step);
    m->set_code(FileToString(f.path()));
    
  }
  
  return tln;
}

const std::map<YAML::NodeType, std::string> yamlTypes {
  {YAML::NodeType::Sequence, "sequence"},
  {YAML::NodeType::Scalar, "scalar"},
  {YAML::NodeType::Map, "map"},
  {YAML::NodeType::Undefined, "undefined"},
  {YAML::NodeType::Null, "null"}
};

void RecursivePackBuffer(google::protobuf::Message *m, YAML::Node yaml, const fs::path& fPath, int depth) {
  const google::protobuf::Descriptor *desc = m->GetDescriptor();
  const google::protobuf::Reflection *refl = m->GetReflection();
  
  for (int i = 0; i < desc->field_count(); i++) {
    const google::protobuf::FieldDescriptor *field = desc->field(i);
    const google::protobuf::OneofDescriptor *oneof = field->containing_oneof();
    if (oneof && refl->HasOneof(*m, oneof)) continue;
    const google::protobuf::FieldOptions opts = field->options();
    
    if (field->is_repeated()) {
      
      if (!node.IsSequence()) {
        std::cerr << "Expected a sequence for node but got a " << yamlTypes[yaml.Type()] << std::endl;
        std::cerr << "File path: " << fPath << std::endl;
        std::cerr << "Protobuf field: " << field->name();
        std::cerr << "Yaml field: " << ... ;
        break;
      }

      switch (field->cpp_type()) {
        case CppType::CPPTYPE_MESSAGE: {
          forloop yaml;
          google::protobuf::Message *msg = refl->AddMessage(m, field);
          RecursivePackBuffer(msg, ..., fPath, depth + 1);
          break;
        }

        case CppType::CPPTYPE_STRING: {
          forloopyaml
          refl->AddString(m, field, value);
          break;
        }

        default: {
          std::cerr << "Error: missing condition for repeated type: " << field->type_name()
                    << ". Instigated by: " << field->type_name() << std::endl;
          break;
        }
      }
    
    } else {  // Now we parse individual proto fields
      switch (field->cpp_type()) {
        // If field is a singular message we need to recurse into this method again
        case CppType::CPPTYPE_MESSAGE: {
          google::protobuf::Message *msg = refl->MutableMessage(m, field);
          //PackRes(dir, 0, child, msg, depth + 1);
          RecursivePackBuffer(msg, ..., fPath, depth + 1);
          break;
        }
        case CppType::CPPTYPE_INT32: {
          refl->SetInt32(m, field,);
          break;
        }
        case CppType::CPPTYPE_INT64: {
          refl->SetInt64(m, field,);
          break;
        }
        case CppType::CPPTYPE_UINT32: {
          refl->SetUInt32(m, field, );
          break;
        }
        case CppType::CPPTYPE_UINT64: {
          refl->SetUInt64(m, field, );
          break;
        }
        case CppType::CPPTYPE_DOUBLE: {
          refl->SetDouble(m, field,);
          break;
        }
        case CppType::CPPTYPE_FLOAT: {
          refl->SetFloat(m, field,);
          break;
        }
        case CppType::CPPTYPE_BOOL: {
          refl->SetBool(m, field,);
          break;
        }
        case CppType::CPPTYPE_ENUM: {
          refl->SetEnum(m, field, field->enum_type()->FindValueByNumber());
          break;
        }
        case CppType::CPPTYPE_STRING: {
          refl->SetString(m, field, value);
          break;
        }
      }
    }
  }
}

bool LoadResource(const fs::path& fPath, google::protobuf::Message *m) {
  
  // Scripts and shaders are not folders so we exit here
  if (fPath.extension() == ".edl") {
    m->CopyFrom(*static_cast<google::protobuf::Message*>(LoadScript(fPath)));
    return true;
  }
  
  if (fPath.extension() == ".shdr") {
    m->CopyFrom(*static_cast<google::protobuf::Message*>(LoadShader(fPath)));
    return true;
  }
  
  if (!FolderExists(fPath)) {
    std::cerr << "Error: the resource folder " << fPath << " referenced in the project tree does not exist" << std::endl;
  }
  
  // Timelines are folders but do not have a properties.yaml so we exit here
  if (fPath.extension() == ".tln") {
    m->CopyFrom(*static_cast<google::protobuf::Message*>(LoadTimeLine(fPath)));
    return true;
  }
  
  const fs::path yamlFile = fPath.string() + "/properties.yaml";
  if (!FileExists(yamlFile)) {
    std::cerr << "Error: missing the resource yaml " << yamlFile << std::endl;
  }
}

bool RecursiveLoadTree(const fs::path& fPath, YAML::Node yaml, buffers::TreeNode* buffer) {
  
  if (!FolderExists(fPath)) {
    std::cerr << "Error: the folder " << fPath << " referenced in the project tree does not exist" << std::endl;
  }
  
  for (auto n : yaml) {
    buffers::TreeNode* b = buffer->add_child();
    
    if (n["folder"]) {
      const std::string name = n["folder"].as<std::string>();
      b->set_name(name);
      b->set_folder(true);
      RecursiveLoadTree(fPath.string() + "/" + name, n["contents"], b);
    } else {
      const std::string name = n["name"].as<std::string>();
      const std::string type = n["type"].as<std::string>();
      b->set_name(name);
      auto factory = factoryMap.find(type);
      if (factory != factoryMap.end()) {
        LoadResource(fPath.string() + "/" + name + factory->second.ext, factory->second.func(b));
      }
      else 
        std::cerr << "Unsupported resource type: " << n["type"] << std::endl;
    }
  }
  
  return true;
}

bool LoadTree(const std::string& yaml, buffers::Game* game) {
  YAML::Node tree = YAML::LoadFile(yaml);
  const fs::path egm_root = fs::path(yaml).parent_path();
  buffers::TreeNode* game_root = game->mutable_root();
  game_root->set_name("/");
  return RecursiveLoadTree(egm_root, tree["contents"], game_root);
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
