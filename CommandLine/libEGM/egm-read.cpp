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
#include "Util.h"

#include <yaml-cpp/yaml.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/util/message_differencer.h>
#include <iostream>
#include <fstream>
#include <functional>
#include <map>
#include <ctype.h>

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
  scr->set_code(FileToString(fPath.string()));
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
    {YAML::NodeType::Sequence, "sequence"},
    {YAML::NodeType::Scalar, "scalar"},
    {YAML::NodeType::Map, "map"},
    {YAML::NodeType::Undefined, "undefined"},
    {YAML::NodeType::Null, "null"}
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
    if (f.path().extension().string() != ".edl" || stem.length() < std::string("step[0]").length() || stem.back() != ']') {
      invalidEDLNaming(stem, fPath.string());
      continue;
    }

    const std::string substr1 = stem.substr(0, delim.length());
    const std::string stepStr = stem.substr(delim.length(), stem.length()-1);
    std::size_t idx;
    const int step = std::stoi(stepStr, &idx);
    // Check the string to int succeeded and the begining of the string is correct.
    if (idx != stepStr.length()-1 || substr1 != delim) {
      invalidEDLNaming(stem, fPath.string());
      continue;
    }

    // If we made it this far we can add event to timeline
    buffers::resources::Timeline_Moment* m = tln->add_moments();
    m->set_step(step);
    m->set_code(FileToString(f.path().string()));

  }

  return tln;
}

std::vector<std::string> Tokenize(const std::string& str, const std::string& delimiters) {
    std::vector<std::string> tokens;
    std::size_t lastPos = str.find_first_not_of(delimiters, 0);
    std::size_t pos = str.find_first_of(delimiters, lastPos);

    while (pos != std::string::npos || lastPos != std::string::npos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }

    return tokens;
}

inline bool isCommandToken(const std::string& token) {
  return (token.length() == 1 && isalpha(token[0]));
}

inline void tooManyArgsGiven(char cmd, YAML::Mark errPos, const fs::path& fPath) {
  std::cerr << "Error: too many args given to the \"" << cmd << "\" command in "
  << fPath << " at line: " << errPos.line << " column: " << errPos.column << std::endl;
}

inline unsigned hex2int(const std::string& hex) {
  unsigned i;
  std::stringstream sstream;
  sstream << std::hex << hex;
  sstream >> i;
  return i;
}

void RepackSVGDInstanceLayer(google::protobuf::Message *m, YAML::Node& yaml, const fs::path& fPath) {
  std::cout << fPath << std::endl;

  // split at spaces or comma etc
  std::vector<std::string> tokens = Tokenize(yaml.as<std::string>(), " ,\n\t");

  std::vector<buffers::resources::Room_Instance*> instances;
  buffers::resources::Room_Instance* currInstance = nullptr;

  char currentCmd = '\0';
  for (unsigned i = 0; i < tokens.size();) {
    currentCmd = tokens[i][0];
    std::string commandToken = std::string(tokens[i]);
    std::cout << "command token: " << commandToken << std::endl;
    if (!isCommandToken(commandToken)) {
      i++; continue; // can't do anything with arguments by themselves
    }
    i++;
    std::string token = std::string(tokens[i]);
    if (isCommandToken(token)) {
      continue; // the command must not have any arguments
    }
    std::vector<std::string> arguments;
    while (!isCommandToken(token)) {
      std::cout << "argument token: " << token << std::endl;
      arguments.emplace_back(token);
      i++;
      token = std::string(tokens[i]);
    }

    /*switch (currentCmd) {
      case 'I':  // new instance (clear attributes)
      case 'i': { // new instance (keep attributes)
        if (argCount == 0) {
          currInstance = new buffers::resources::Room_Instance();

          if (currentCmd == 'i') {
            if (instances.size() > 0)
              currInstance->CopyFrom(*instances.back());
            else {
              std::cerr << "Error: the \"i\" command requires at least one instance is created using the \"I\" prior to calling it the first time" << std::endl;
              break;
            }
          }

          instances.push_back(currInstance);
        } else if (argCount == 1) { // first arg is id
          currInstance->set_id(std::stoi(tokens[i]));
        } else {
          tooManyArgsGiven(currentCmd, yaml.Mark(), fPath);
        }
        break;
      }

      case 'n': { // instance name
        if (argCount == 1)
          currInstance->set_name(tokens[i]);
        else if (argCount > 1)
          tooManyArgsGiven(currentCmd, yaml.Mark(), fPath);

        break;
      }

      case 'o': { // object type
        if (argCount == 1)
          currInstance->set_object_type(tokens[i]);
        else if (argCount > 1)
          tooManyArgsGiven(currentCmd, yaml.Mark(), fPath);

        break;
      }

      case 'P': // absolute instance position
      case 'p': { // relative instance position
        if (argCount == 1)
          currInstance->set_x(currInstance->x() + std::stod(tokens[i]));
        else if (argCount == 2)
          currInstance->set_y(currInstance->y() + std::stod(tokens[i]));
        else if (argCount == 3)
          currInstance->set_z(currInstance->z() + std::stod(tokens[i]));
        else if (argCount > 3)
          tooManyArgsGiven(currentCmd, yaml.Mark(), fPath);

        break;
      }

      case 'X': // absolute x
      case 'x': { // relative x
        if (argCount == 1)
          currInstance->set_x(currInstance->x() + std::stod(tokens[i]));
        else if (argCount > 1)
          tooManyArgsGiven(currentCmd, yaml.Mark(), fPath);

        break;
      }

      case 'Y': // absolute y
      case 'y': { // relative y
        if (argCount == 1)
          currInstance->set_y(currInstance->y() + std::stod(tokens[i]));
        else if (argCount > 1)
          tooManyArgsGiven(currentCmd, yaml.Mark(), fPath);

        break;
      }

      case 'Z': // absolute z
      case 'z': { // relative z
        if (argCount == 1)
          currInstance->set_z(currInstance->z() + std::stod(tokens[i]));
        else if (argCount > 1)
          tooManyArgsGiven(currentCmd, yaml.Mark(), fPath);

        break;
      }

      case 'S': { // scaling
        if (argCount == 1)
          currInstance->set_xscale(std::stod(tokens[i]));
        else if (argCount == 2)
          currInstance->set_yscale(std::stod(tokens[i]));
        else if (argCount == 3)
          currInstance->set_zscale(std::stod(tokens[i]));
        else if (argCount > 3)
          tooManyArgsGiven(currentCmd, yaml.Mark(), fPath);

        break;
      }

      case 'W': { // x scale
        if (argCount == 1)
          currInstance->set_xscale(std::stod(tokens[i]));
        else if (argCount > 1)
          tooManyArgsGiven(currentCmd, yaml.Mark(), fPath);

        break;
      }

      case 'H': { // y scale
        if (argCount == 1)
          currInstance->set_yscale(std::stod(tokens[i]));
        else if (argCount > 1)
          tooManyArgsGiven(currentCmd, yaml.Mark(), fPath);

        break;
      }

      case 'D': { // z scale
        if (argCount == 1)
          currInstance->set_zscale(std::stod(tokens[i]));
        else if (argCount > 1)
          tooManyArgsGiven(currentCmd, yaml.Mark(), fPath);

        break;
      }

      case 'R': { // rotation
        if (argCount == 1)
          currInstance->set_rotation(std::stod(tokens[i]));
        else if (argCount > 1)
          tooManyArgsGiven(currentCmd, yaml.Mark(), fPath);

        break;
      }

      case 'C': { // color
        if (argCount == 1)
          currInstance->set_color(hex2int(tokens[i]));
        else if (argCount > 1)
          tooManyArgsGiven(currentCmd, yaml.Mark(), fPath);

        break;
      }

      case 'L':
      case 'F':
      case 'g': {
        if (argCount == 0)
          std::cerr << "Error: the \"" << currentCmd << "\" has not been implemented yet" << std::endl;

        break;
      }

      default: {
        if (argCount == 0)
          std::cerr << "Error: unsupported command \"" << currentCmd << "\"" << std::endl;

        break;
      }

    }*/
  }
}

void RepackInstanceLayers(google::protobuf::Message *m, YAML::Node& yaml, const fs::path& fPath) {

  YAML::Node format = yaml["Format"];
  if (!format) {
    std::cerr << "Error: missing \"Format\" key in " << fPath.string() << std::endl;
    yamlErrorPosition(yaml.Mark());
    return;
  }

  YAML::Node data = yaml["Data"];
  if (!data) {
    std::cerr << "Error: missing \"Data\" key in " << fPath.string() << std::endl;
    yamlErrorPosition(yaml.Mark());
    return;
  }

  const std::string formatStr = format.as<std::string>();
  if (formatStr == "svg-d") RepackSVGDInstanceLayer(m, data, fPath.string());
  else std::cerr << "Error: unsupported instance layer format \"" << formatStr << "\" in " << fPath.string() << std::endl;
}

void RecursivePackBuffer(google::protobuf::Message *m, YAML::Node& yaml, const fs::path& fPath, int depth) {
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
      if (key == "tiles") continue;//key = "tile-layers";
      if (key == "code") continue;
    }

    YAML::Node node = yaml[key];

    // YAML field not in properties.yaml
    if (!node) {
      std::cerr << "Warning: could not locate YAML field " << field->name() << " in " << fPath.string() << std::endl;
      continue;
    }

    const bool isFilePath = opts.GetExtension(buffers::file_path);

    if (field->is_repeated()) {

      if (!node.IsSequence()) {
        invalidYAMLType(yaml, fPath.string(), field);
        continue;
      }

      switch (field->cpp_type()) {
        case CppType::CPPTYPE_MESSAGE: {
          for (auto n : node) {
            google::protobuf::Message *msg = refl->AddMessage(m, field);
            if (key == "instance-layers")
              RepackInstanceLayers(msg, n, fPath.string());
            else
              RecursivePackBuffer(msg, n, fPath.string(), depth + 1);
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
        invalidYAMLType(yaml, fPath.string(), field);
        continue;
      }

      switch (field->cpp_type()) {
        // If field is a singular message we need to recurse into this method again
        case CppType::CPPTYPE_MESSAGE: {
          google::protobuf::Message *msg = refl->MutableMessage(m, field);
          RecursivePackBuffer(msg, n, fPath.string(), depth + 1);
          break;
        }
        case CppType::CPPTYPE_INT32: {
          refl->SetInt32(m, field, n.as<int>());
          break;
        }
        case CppType::CPPTYPE_INT64: {
          refl->SetInt64(m, field, n.as<int>());
          break;
        }
        case CppType::CPPTYPE_UINT32: {
          refl->SetUInt32(m, field, n.as<unsigned>());
          break;
        }
        case CppType::CPPTYPE_UINT64: {
          refl->SetUInt64(m, field, n.as<unsigned>());
          break;
        }
        case CppType::CPPTYPE_DOUBLE: {
          refl->SetDouble(m, field, n.as<double>());
          break;
        }
        case CppType::CPPTYPE_FLOAT: {
          refl->SetFloat(m, field, n.as<float>());
          break;
        }
        case CppType::CPPTYPE_BOOL: {
          refl->SetBool(m, field, n.as<int>()); // as<bool> throws exception
          break;
        }
        case CppType::CPPTYPE_ENUM: {
          refl->SetEnum(m, field, field->enum_type()->FindValueByNumber(n.as<int>()));
          break;
        }
        case CppType::CPPTYPE_STRING: {
          refl->SetString(m, field, (isFilePath) ? fPath.string() + "/" + n.as<std::string>() : n.as<std::string>());
          break;
        }
      }
    }
  }
}

bool LoadResource(const fs::path& fPath, google::protobuf::Message *m) {

  std::string ext = fPath.extension().string();

  // Scripts and shaders are not folders so we exit here
  if (ext == ".edl") {
    m->CopyFrom(*static_cast<google::protobuf::Message*>(LoadScript(fPath.string())));
    return true;
  }

  if (ext == ".shdr") {
    m->CopyFrom(*static_cast<google::protobuf::Message*>(LoadShader(fPath)));
    return true;
  }

  if (!FolderExists(fPath.string())) {
    std::cerr << "Error: the resource folder " << fPath << " referenced in the project tree does not exist" << std::endl;
  }

  // Timelines are folders but do not have a properties.yaml so we exit here
  if (ext == ".tln") {
    m->CopyFrom(*static_cast<google::protobuf::Message*>(LoadTimeLine(fPath.string())));
    return true;
  }

  const fs::path yamlFile = fPath.string() + "/properties.yaml";
  if (!FileExists(yamlFile.string())) {
    std::cerr << "Error: missing the resource YAML " << yamlFile << std::endl;
  }

  YAML::Node yaml = YAML::LoadFile(yamlFile.string());

  if (ext == ".rm") {
    RecursivePackBuffer(m, yaml, fPath.string(), 0);
  }

  return true;
}

bool RecursiveLoadTree(const fs::path& fPath, YAML::Node yaml, buffers::TreeNode* buffer) {

  if (!FolderExists(fPath.string())) {
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
        std::cerr << "Warning: Unsupported resource type: " << n["type"] << std::endl;
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
