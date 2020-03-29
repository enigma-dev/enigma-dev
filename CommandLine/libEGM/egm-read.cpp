/** Copyright (C) 2018-2020 Greg Williamson, Robert B. Colton, Josh Ventura
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
#include "svg-d.h"
#include "proto_util.h"

#include <yaml-cpp/yaml.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/util/message_differencer.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <functional>
#include <map>
#include <ctype.h>
#include <set>

namespace proto = google::protobuf;
using CppType = proto::FieldDescriptor::CppType;
using std::string;

namespace egm {

using buffers::TreeNode;
using Type = buffers::TreeNode::TypeCase;
using FactoryFunction = std::function<google::protobuf::Message *(TreeNode*)>;
struct ResourceFactory {
  FactoryFunction func;
  Type type;
  std::string ext = "";
};
using FactoryMap = std::unordered_map<std::string, ResourceFactory>;

static const FactoryMap factoryMap({
  { "sprite",     { &TreeNode::mutable_sprite,     Type::kSprite,     ".spr"  } },
  { "sound",      { &TreeNode::mutable_sound,      Type::kSound,      ".snd"  } },
  { "background", { &TreeNode::mutable_background, Type::kBackground, ".bkg"  } },
  { "path",       { &TreeNode::mutable_path,       Type::kPath,       ".pth"  } },
  { "script",     { &TreeNode::mutable_script,     Type::kScript,     ".edl"  } },
  { "shader",     { &TreeNode::mutable_shader,     Type::kShader,     ".shdr" } },
  { "font",       { &TreeNode::mutable_font,       Type::kFont,       ".fnt"  } },
  { "timeline",   { &TreeNode::mutable_timeline,   Type::kTimeline,   ".tln", } },
  { "object",     { &TreeNode::mutable_object,     Type::kObject,     ".obj", } },
  { "room",       { &TreeNode::mutable_room,       Type::kRoom,       ".rm",  } }
});

static const FactoryMap extFactoryMap({
  { ".spr",  { &TreeNode::mutable_sprite,     Type::kSprite     } },
  { ".snd",  { &TreeNode::mutable_sound,      Type::kSound      } },
  { ".bkg",  { &TreeNode::mutable_background, Type::kBackground } },
  { ".pth",  { &TreeNode::mutable_path,       Type::kPath,      } },
  { ".edl",  { &TreeNode::mutable_script,     Type::kScript     } },
  { ".shdr", { &TreeNode::mutable_shader,     Type::kShader     } },
  { ".fnt",  { &TreeNode::mutable_font,       Type::kFont       } },
  { ".tln",  { &TreeNode::mutable_timeline,   Type::kTimeline   } },
  { ".obj",  { &TreeNode::mutable_object,     Type::kObject     } },
  { ".rm",   { &TreeNode::mutable_room,       Type::kRoom       } }
});

std::map<Type, int> maxID = {
  { Type::kBackground, 0 },
  { Type::kFont,       0 },
  { Type::kObject,     0 },
  { Type::kPath,       0 },
  { Type::kRoom,       0 },
  { Type::kScript,     0 },
  { Type::kShader,     0 },
  { Type::kSound,      0 },
  { Type::kSprite,     0 },
  { Type::kTimeline,   0 }
};

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
  std::cerr << "Warning: Ignoring improperly named file " << fName << " in " << res.stem() << std::endl;

  if (res.extension() == ".tln")
    std::cerr << "Supported naming for timelines is step[moment].edl" << std::endl;

  else if (res.extension() == ".obj")
    std::cerr << "Supported naming for objects is event[subevent].edl" << std::endl;

  else if (res.extension() == ".rm")
    std::cerr << "Supported naming for rooms is create[instance].edl" << std::endl;
}

inline void yamlErrorPosition(const YAML::Mark errPos) {
  std::cerr << "YAML position: line: " << errPos.line << " column: " << errPos.column << std::endl << std::endl;
}

inline void invalidYAMLType(const YAML::Node& yaml, const fs::path& fPath, const google::protobuf::FieldDescriptor *field) {
  const std::map<int, std::string> yamlTypes {
    { YAML::NodeType::Sequence,  "sequence"  },
    { YAML::NodeType::Scalar,    "scalar"    },
    { YAML::NodeType::Map,       "map"       },
    { YAML::NodeType::Undefined, "undefined" },
    { YAML::NodeType::Null,      "null"      }
  };

  std::string expectedType;
  if (field->is_repeated()) expectedType = "sequence";
  else if (field->cpp_type() == CppType::CPPTYPE_MESSAGE) expectedType = "map";
  else expectedType = "scalar";

  std::cerr << "YAML parsing error" << std::endl;
  std::cerr << "Expected a " << expectedType << " but got " << yamlTypes.at(yaml.Type()) << std::endl;
  std::cerr << "File path: " << fPath << std::endl;
  std::cerr << "Protobuf field: " << field->name() << std::endl;
  yamlErrorPosition(yaml[field->name()].Mark());
}

buffers::resources::Timeline* LoadTimeLine(const fs::path& fPath) {
  buffers::resources::Timeline* tln = new buffers::resources::Timeline();

  const std::string delim = "step[";
  for(auto& f : fs::directory_iterator(fPath)) {

    const std::string stem = f.path().stem().string(); // base filename

    // If its not an edl file, the filename is too short to fit the naming scheme or the filename doesn't end with ]
    // exit here before doing any string cutting to prevent segfaults
    if (f.path().extension() != ".edl" || stem.length() < std::string("step[0]").length() || stem.back() != ']') {
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

void RepackLayers(buffers::resources::Room *room, bool tiles, YAML::Node& yaml,
                  const fs::path& fPath) {
  // All layers require a "format" and a "data" key in the YAML
  YAML::Node format = yaml["Format"];
  if (!format) {
    std::cerr << "Error: missing \"Format\" key in " << fPath << std::endl;
    yamlErrorPosition(yaml.Mark());
    return;
  }

  YAML::Node data = yaml["Data"];
  if (!data) {
    std::cerr << "Error: missing \"Data\" key in " << fPath << std::endl;
    yamlErrorPosition(yaml.Mark());
    return;
  }

  // Send our data to the appropriate handler (if one exists)
  const std::string formatStr = format.as<std::string>();
  if (formatStr == "svg-d") {
    std::stringstream details;
    details << fPath.string() << ":" << yaml.Mark().line
                              << ":" << yaml.Mark().column;
    if (tiles)
      svg_d::ParseTiles(data.as<std::string>(), room, details.str());
    else
      svg_d::ParseInstances(data.as<std::string>(), room, details.str());
  } else {
    std::cerr << "Error: unsupported instance layer format \"" << formatStr
              << "\" in " << fPath << std::endl;
  }
}

// Load all edl files in our object dir
inline void loadObjectEvents(const fs::path& fPath, google::protobuf::Message *m, const google::protobuf::FieldDescriptor *field) {
  for(auto& f : fs::directory_iterator(fPath)) {
    if (f.path().extension() == ".edl") {
      const std::string eventName = f.path().stem().string();
      buffers::resources::Object_Event event;
      event.set_name(eventName);
      event.set_code(FileToString(f.path()));

      const google::protobuf::Reflection *refl = m->GetReflection();

      google::protobuf::Message* msg = refl->AddMessage(m, field);
      msg->CopyFrom(event);
    }
  }
}

void RecursivePackBuffer(google::protobuf::Message *m, int id, YAML::Node& yaml,
                         const fs::path& fPath, int depth) {
  const google::protobuf::Descriptor *desc = m->GetDescriptor();
  const google::protobuf::Reflection *refl = m->GetReflection();
  const std::string ext = fPath.extension().string();

  for (int i = 0; i < desc->field_count(); i++) {
    const google::protobuf::FieldDescriptor *field = desc->field(i);
    const google::protobuf::OneofDescriptor *oneof = field->containing_oneof();
    if (oneof && refl->HasOneof(*m, oneof)) continue;
    const google::protobuf::FieldOptions opts = field->options();

    std::string key = field->name();

    if (ext == ".rm" && depth == 0) {
      if (key == "instances") key = "instance-layers";
      if (key == "tiles") key = "tile-layers";
      if (key == "code") continue;
    }

    if (ext == ".obj" && depth == 0) {
       // code is loaded from edl files
      if (key == "events") {
        loadObjectEvents(fPath, m, field);
        continue;
      }
    }

    if (key == "id" && depth == 0) {
      if (id >= 0)
        SetProtoField(m, field, std::to_string(id));
      continue;
    }

    YAML::Node node = yaml[key];

    // YAML field not in properties.yaml
    if (!node) {
      std::cerr << "Warning: could not locate YAML field " << field->name() << " in " << fPath << std::endl;
      continue;
    }

    const bool isFilePath = opts.GetExtension(buffers::file_path);

    if (field->is_repeated()) {

      if (!node.IsSequence()) {
        invalidYAMLType(yaml, fPath, field);
        continue;
      }

      switch (field->cpp_type()) {
        case CppType::CPPTYPE_MESSAGE: {
          for (auto n : node) {
            if (key == "instance-layers")
              RepackLayers((buffers::resources::Room*) m, false, n, fPath);
            else if (key == "tile-layers")
              RepackLayers((buffers::resources::Room*) m, true, n, fPath);
            else {
              google::protobuf::Message* msg = refl->AddMessage(m, field);
              RecursivePackBuffer(msg, id, n, fPath, depth + 1);
            }
          }
          break;
        }

        case CppType::CPPTYPE_STRING: {
          for (auto n : node) {
            refl->AddString(m, field, (isFilePath) ? fPath.string() + "/" + n.as<std::string>() : n.as<std::string>());
          }
          break;
        }

        default: {
          std::cerr << "Error: missing condition for repeated type: " << field->type_name()
                    << ". Instigated by: " << field->type_name() << std::endl;
          break;
        }
      }

    } else {  // Now we parse individual proto fields
      YAML::Node n = node;

      if ((!n.IsScalar() && field->cpp_type() != CppType::CPPTYPE_MESSAGE) || (!n.IsMap() && field->cpp_type() == CppType::CPPTYPE_MESSAGE)) {
        invalidYAMLType(yaml, fPath, field);
        continue;
      }

      switch (field->cpp_type()) {
        // If field is a singular message we need to recurse into this method again
        case CppType::CPPTYPE_MESSAGE: {
          google::protobuf::Message *msg = refl->MutableMessage(m, field);
          RecursivePackBuffer(msg, id, n, fPath, depth + 1);
          break;
        }
        default: {
          SetProtoField(m, field, (isFilePath) ? fPath.string() + "/" + n.as<std::string>() : n.as<std::string>());
          break;
        }
      }
    }
  }
}

inline bool isNumber(const std::string& s) {
  return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

inline void LoadInstanceEDL(const fs::path& fPath, buffers::resources::Room* rm) {
  for(auto& f : fs::directory_iterator(fPath)) {
    if (f.path().extension() == ".edl") {
      const std::string edlFile = f.path().stem().string();

      if (edlFile == "create") {
        rm->set_creation_code(FileToString(f.path()));
        continue;
      }

      const std::string delim = "[";
      size_t pos = edlFile.find_first_of(delim);
      if (pos == std::string::npos || edlFile.back() != ']') {
        invalidEDLNaming(fPath, edlFile);
        continue;
      }

      const std::string instName = edlFile.substr(pos+1, edlFile.length()-pos-2);

      if (instName.empty()) continue;

      // See if we have an instance with matching a name
      auto instances = rm->instances();
      auto instItr = std::find_if(instances.begin(), instances.end(),
        [&instName](const buffers::resources::Room_Instance inst) {
          return (inst.name() == instName);
        });

      if (instItr != instances.end()) {
        instItr->set_creation_code(FileToString(f.path()));
        continue;
      }

      if (!isNumber(instName)) {
        std::cerr << "Error: Failed to set instance code. Could not find instance named: " << instName << " in " << fPath << std::endl;
        continue;
      }

      // If its a number check if we have a matching id
      int id = std::stoi(instName);
      instItr = std::find_if(instances.begin(), instances.end(),
        [&id](const buffers::resources::Room_Instance inst) {
          return (inst.id() == id);
        });

      if (instItr != instances.end()) {
        instItr->set_creation_code(FileToString(f.path()));
      } else {
        std::cerr << "Error: Failed to set instance code. Could not find instance with the ID: " << id << " in " << fPath << std::endl;
      }
    }
  }
}

bool LoadResource(const fs::path& fPath, google::protobuf::Message *m, int id) {

  std::string ext = fPath.extension().string();

  // Scripts and shaders are not folders so we exit here
  if (ext == ".edl") {
    buffers::resources::Script* scr = LoadScript(fPath);
    scr->set_id(id);
    m->CopyFrom(*static_cast<google::protobuf::Message*>(scr));
    return true;
  }

  if (ext == ".shdr") {
    buffers::resources::Shader* shdr = LoadShader(fPath);
    shdr->set_id(id);
    m->CopyFrom(*static_cast<google::protobuf::Message*>(shdr));
    return true;
  }

  if (!FolderExists(fPath)) {
    std::cerr << "Error: the resource folder " << fPath << " referenced in the project tree does not exist" << std::endl;
  }

  // Timelines are folders but do not have a properties.yaml so we exit here
  if (ext == ".tln") {
    buffers::resources::Timeline* tln = LoadTimeLine(fPath);
    tln->set_id(id);
    m->CopyFrom(*static_cast<google::protobuf::Message*>(tln));
    return true;
  }

  const fs::path yamlFile = fPath.string() + "/properties.yaml";
  if (!FileExists(yamlFile)) {
    std::cerr << "Error: missing the resource YAML " << yamlFile << std::endl;
  }

  YAML::Node yaml = YAML::LoadFile(yamlFile.string());

  RecursivePackBuffer(m, id, yaml, fPath, 0);

  if (ext == ".rm") {
    LoadInstanceEDL(fPath, static_cast<buffers::resources::Room*>(m));
  }

  return true;
}

// Load EGM using tree file
bool LoadTree(const fs::path& fPath, YAML::Node yaml, buffers::TreeNode* buffer) {

  if (!FolderExists(fPath)) {
    std::cerr << "Error: the folder " << fPath << " referenced in the project tree does not exist" << std::endl;
  }

  for (auto n : yaml) {
    buffers::TreeNode* b = buffer->add_child();

    if (n["folder"]) {
      const std::string name = n["folder"].as<std::string>();
      b->set_name(name);
      b->set_folder(true);
      LoadTree(fPath.string() + "/" + name, n["contents"], b);
    } else {
      const std::string name = n["name"].as<std::string>();
      b->set_name(name);

      const std::string type = n["type"].as<std::string>();

      int id = -1;
      if (n["id"]) id = n["id"].as<int>();

      auto factory = factoryMap.find(type);
      if (factory != factoryMap.end()) {

        // Update our max id per res
        if (maxID[factory->second.type] < id)
          maxID[factory->second.type] = id;

        LoadResource(fPath.string() + "/" + name + factory->second.ext, factory->second.func(b), id);
      }
      else
        std::cerr << "Warning: Unsupported resource type: " << n["type"] << std::endl;
    }
  }

  return true;
}

auto fsCompare = [](const fs::directory_entry& a, const fs::directory_entry& b) { return a.path().stem().string() < b.path().stem().string(); };

// Load EGM without a tree file
bool LoadDirectory(const fs::path& fPath, buffers::TreeNode* n, int depth) {

  // Sort dirs alphabetical
  std::set<fs::directory_entry, decltype(fsCompare)> files(fsCompare);
  for(auto& p: fs::directory_iterator(fPath)) {
    files.insert(p);
  }

  for(auto& p: files) {
    const std::string ext = p.path().extension().string();
    if (p.is_directory()) {

      buffers::TreeNode* c = n->add_child();

      c->set_name(p.path().stem().string());

      // If directory is resource
      auto factory = extFactoryMap.find(ext);
      if (factory != extFactoryMap.end()) {
        LoadResource(p.path(), factory->second.func(c), maxID.at(factory->second.type)++);
        continue;
      }

      // If directory is just a folder
      c->set_folder(true);
      LoadDirectory(p, c, depth + 1);
    } else { // is a file

      buffers::TreeNode* c = n->add_child();
      c->set_name(p.path().stem().string());
      if (ext == ".edl") { // script
        LoadResource(p.path(), extFactoryMap.at(".edl").func(c), maxID.at(Type::kScript)++);
      } else if (ext == ".vert") { // shader
        LoadResource(p.path().parent_path().string() + ".shdr", extFactoryMap.at(".shdr").func(c), maxID.at(Type::kShader)++);
      }
    }
  }

  return true;
}

bool LoadEGM(const std::string& yaml, buffers::Game* game) {
  YAML::Node project = YAML::LoadFile(yaml);

  const fs::path egm_root = fs::path(yaml).parent_path();
  buffers::TreeNode* game_root = game->mutable_root();
  game_root->set_name("/");

  // Load EGM without a tree file
  if (!project["tree"] || project["tree"].as<std::string>() == "autogen") {
    return LoadDirectory(egm_root, game_root, 0);
  // Load EGM with a tree file
  } else {
    YAML::Node tree = YAML::LoadFile(egm_root.string() + "/tree.yaml");
    return LoadTree(egm_root, tree["contents"], game_root);
  }
}

void RecursiveResourceSanityCheck(buffers::TreeNode* n, std::map<Type, std::map<int, std::string>>& IDmap) {

  for (int i = 0; i < n->child_size(); ++i) {
    buffers::TreeNode* c = n->mutable_child(i);
    RecursiveResourceSanityCheck(c, IDmap);

    google::protobuf::Message* m = nullptr;
    std::string type;

    switch(c->type_case()) {
     case Type::kBackground:
      m = c->mutable_background();
      type = "background";
      break;
     case Type::kFont:
      m = c->mutable_font();
      type = "font";
      break;
     case Type::kObject:
      m = c->mutable_object();
      type = "object";
      break;
     case Type::kPath:
      m = c->mutable_path();
      type = "path";
      break;
     case Type::kRoom:
      m = c->mutable_room();
      type = "room";
      break;
     case Type::kScript:
      m = c->mutable_script();
      type = "script";
      break;
     case Type::kShader:
      m = c->mutable_shader();
      type = "shader";
      break;
     case Type::kSound:
      m = c->mutable_sound();
      type = "sound";
      break;
     case Type::kSprite:
      m = c->mutable_sprite();
      type = "sprite";
      break;
     case Type::kTimeline:
      m = c->mutable_timeline();
      type = "timeline";
      break;
     default:
      break;
    }

    if (m == nullptr) continue;

    const google::protobuf::Descriptor* desc = m->GetDescriptor();
    const google::protobuf::Reflection* refl = m->GetReflection();
    const google::protobuf::FieldDescriptor* field = desc->FindFieldByName("id");

    int id = -1;
    if (refl->HasField(*m, field))
      id = refl->GetInt32(*m, field);
    else if (id == -1) {
      id = ++maxID.at(c->type_case());
      std::cerr << "Warning: the " << type << " \"" << c->name() << "\" has no ID set assigning new ID: " << id << std::endl;
      refl->SetInt32(m, field, id);
      continue;
    }

    auto it = IDmap[c->type_case()].find(id);
    if (it != IDmap[c->type_case()].end()) {
      id = ++maxID.at(c->type_case());
      std::string dupName = (*it).second;
      std::cerr << "Warning: the " << type << "s \"" << dupName << "\" and \"" << c->name() << "\" have the same ID reassigning " << c->name() << "\'s ID to: " << id << std::endl;
      refl->SetInt32(m, field, id);
    } else IDmap[c->type_case()].emplace(id, c->name());

  }
}

// Every resource needs unique id so the engine don't assplode
void ResourceSanityCheck(buffers::TreeNode* n) {
  std::map<Type, std::map<int, std::string>> IDmap;
  RecursiveResourceSanityCheck(n, IDmap);
}

buffers::Project* LoadEGM(std::string fName) {
  buffers::Project* proj = new buffers::Project();

  if (!FileExists(fName)) {
    std::cerr << "Error: " << fName << " does not exist" << std::endl;
    return proj;
  }

  LoadEGM(fName, proj->mutable_game());
  ResourceSanityCheck(proj->mutable_game()->mutable_root());

  return proj;
}

} //namespace egm
