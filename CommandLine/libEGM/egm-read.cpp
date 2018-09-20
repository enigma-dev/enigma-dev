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

inline void tooManyArgsGiven(char cmd, unsigned maxArgs, const std::vector<std::string>& args, unsigned argNum, const YAML::Mark& errPos, const fs::path& fPath) {
  std::cerr << std::endl << "Error: too many arguments given to the \"" << cmd << "\" command" << std::endl;
  std::cerr << "maximum supported arguments is: " << maxArgs << std::endl;
  std::cerr << "argument " << argNum << ": " << args[argNum] << " will be ignored" << std::endl;
  std::cerr << fPath << " line: " << errPos.line << " column: " << errPos.column << std::endl << std::endl;
}

inline void tooFewArgsGiven(char cmd, unsigned minArgs, const YAML::Mark& errPos, const fs::path& fPath) {
  std::cerr << std::endl << "Error: too few arguments given to the \"" << cmd << "\" command" << std::endl;
  std::cerr << "minimum required arguments is: " << minArgs << std::endl;
  std::cerr << "the command will be ignored" << std::endl;
  std::cerr << fPath << " line: " << errPos.line << " column: " << errPos.column << std::endl << std::endl;
}

inline std::string unquote(const std::string& quotedStr) {
  std::string str = quotedStr;
  if (str.length() >= 2 && str.front() == '"' && str.back() == '"')
    str = str.substr(1, str.length()-2);

  return str;
}

inline std::string escape_hex(const std::string &hex) {
  return string_replace_all(hex, "#", "0x");
}

struct Command {
  size_t minArgs = 0;
  std::vector<std::string> field_names;
};

const std::map<char, Command> instanceParameters = {
  { 'i', { 0, {"id"}                         } },
  { 'n', { 1, {"name"}                       } },
  { 'o', { 1, {"object_type"}                } },
  { 'p', { 1, {"x", "y", "z"}                } },
  { 'x', { 1, {"x"}                          } },
  { 'y', { 1, {"y"}                          } },
  { 'z', { 1, {"z"}                          } },
  { 's', { 1, {"xscale", "yscale", "zscale"} } },
  { 'w', { 1, {"xscale"}                     } },
  { 'h', { 1, {"yscale"}                     } },
  { 'd', { 1, {"zscale"}                     } },
  { 'r', { 1, {"rotation"}                   } },
  { 'c', { 1, {"color"}                      } },
};

const std::map<char, Command> tileParameters = {
  { 't', { 0, {"id"}                 } },
  { 'n', { 1, {"name"}               } },
  { 'b', { 1, {"background_name"}    } },
  { 'd', { 1, {"depth"}              } },
  { 'p', { 1, {"x", "y"}             } },
  { 'x', { 1, {"x"}                  } },
  { 'y', { 1, {"y"}                  } },
  { 'o', { 1, {"xoffset", "yoffset"} } },
  { 'u', { 1, {"xoffset"}            } },
  { 'v', { 1, {"yoffset"}            } },
  { 'd', { 1, {"width", "height"}    } },
  { 's', { 1, {"xscale", "yscale"}   } },
  { 'c', { 1, {"color"}              } },
};

void RepackSVGDLayer(google::protobuf::Message *m, const google::protobuf::FieldDescriptor *f, char createCMD, 
  const std::map<char, Command>& parameters, YAML::Node& yaml, const fs::path& fPath) {
  
  std::cout << fPath << std::endl;
  const google::protobuf::Reflection *refl = m->GetReflection();
  
  // split at spaces or comma etc
  std::vector<std::string> tokens = Tokenize(yaml.as<std::string>(), " ,\n\t");
  
  google::protobuf::Message* currInstance = nullptr;
  std::vector<google::protobuf::Message*> instances;

  char currentCmd = '\0';
  std::vector<std::pair<char, std::vector<std::string> > > arguments;
  
  // Group all our commands into <char, vector<string>> (cmd, args)
  auto cmdIt = arguments.end();
  for (unsigned i = 0; i < tokens.size(); ++i) {
    std::string token = std::string(tokens[i]);
    if (isCommandToken(token)) {
      currentCmd = token[0];
      arguments.emplace_back(currentCmd, std::vector<std::string>());
      cmdIt = arguments.end() - 1;
    } else if (cmdIt != arguments.end()) {
      (*cmdIt).second.emplace_back(token);
    }
  }

  // Proccess commands here
  for (auto cmdPair : arguments) {
    char cmd = cmdPair.first;
    std::vector<std::string> args = cmdPair.second;

    auto pit = parameters.find(tolower(cmd));
    if (pit == parameters.end()) {
      std::cerr << "Error: unsupported command \"" << cmd << "\"" << std::endl;
      continue;
    }
    auto sig = (*pit).second;
    auto pars = sig.field_names;

    // Too few args, exit here
    if (args.size() < sig.minArgs) {
      tooFewArgsGiven(cmd, sig.minArgs, yaml.Mark(), fPath);
      continue;
    }
    
    // Special case for create cmds
    if (tolower(cmd) == tolower(createCMD)) {
        currInstance = refl->AddMessage(m, f);
        instances.push_back(currInstance);
        
        if (cmd == tolower(createCMD)) {
          if (!instances.empty()) {
            currInstance->CopyFrom(*instances.back());
            const google::protobuf::Descriptor *instDesc = currInstance->GetDescriptor();
            const google::protobuf::Reflection *instRefl = currInstance->GetReflection();
            
            // Don't copy this crap
            for (const std::string& field_name : {"id", "name", "code"}) {
              const google::protobuf::FieldDescriptor *instField = instDesc->FindFieldByName(field_name);
              if (instField != nullptr)
                instRefl->ClearField(currInstance, instField); 
            }
          }
          else
            std::cerr << "Error: \"" << cmd << "\" called but there exists no previous instance to copy the attributes from" << std::endl;
        }
    }

    cmd = tolower(cmd); // NOTE: for now, all cmds are relative

    // General case for rest of command args
    for (size_t i = 0; i < args.size(); ++i) {
      if (i < pars.size()) {
        std::string field_name = pars[i];
        std::string arg = args[i];

        const google::protobuf::Descriptor *desc = currInstance->GetDescriptor();
        const google::protobuf::Reflection *refl = currInstance->GetReflection();
        const google::protobuf::FieldDescriptor *field = desc->FindFieldByName(field_name);

        // allow all integer parameters to have hexadecimal formatted arguments
        switch (field->cpp_type()) {
          case CppType::CPPTYPE_INT32:
          case CppType::CPPTYPE_INT64:
          case CppType::CPPTYPE_UINT32:
          case CppType::CPPTYPE_UINT64:
            arg = escape_hex(arg);
            break;
          default:
            break;
        };

        SetProtoField(refl, currInstance, field, arg, true);
      } else {
        tooManyArgsGiven(cmd, pars.size(), args, i, yaml.Mark(), fPath);
      }
    }
  }
}

void RepackInstanceLayers(google::protobuf::Message *m, const google::protobuf::FieldDescriptor *f, char createCMD, 
  const std::map<char, Command>& parameters, YAML::Node& yaml, const fs::path& fPath) {
    
  // All layers require a "format" and a "data" key in the YAML
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
  
  // Send our data to the appropriate handler (if one exists)
  const std::string formatStr = format.as<std::string>();
  if (formatStr == "svg-d") RepackSVGDLayer(m, f, createCMD, parameters, data, fPath.string());
  else std::cerr << "Error: unsupported instance layer format \"" << formatStr << "\" in " << fPath.string() << std::endl;
}

inline void loadObjectEvents(const fs::path& fPath) {
  for(auto& f : fs::directory_iterator(fPath)) {
    
  }
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
      if (key == "code") {
        const fs::path edlFile = fPath.string() + "/create[room].edl";
        if (FileExists(edlFile))
          SetProtoField(refl, m, field, FileToString(edlFile));
        continue;
      }
    }
    
    if (ext == ".obj" && depth == 0) {
      if (key == "events") continue; // code is loaded from edl files
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
            if (key == "instance-layers")
              RepackInstanceLayers(m, field, 'I', instanceParameters, n, fPath.string());
            else {
              google::protobuf::Message* msg = refl->AddMessage(m, field);
              RecursivePackBuffer(msg, n, fPath.string(), depth + 1);
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
        default: {
          SetProtoField(refl, m, field, (isFilePath) ? fPath.string() + "/" + n.as<std::string>() : n.as<std::string>());
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

  RecursivePackBuffer(m, yaml, fPath.string(), 0);
  
  if (ext == ".rm") {
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
