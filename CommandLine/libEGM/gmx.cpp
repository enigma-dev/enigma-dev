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

#include "gmx.h"
#include "action.h"
#include "strings_util.h"

#include <pugixml.hpp>

#include <functional>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using CppType = google::protobuf::FieldDescriptor::CppType;
using LookupMap = std::unordered_map<std::string, std::unordered_map<int, std::string>>;

using namespace buffers::resources;

namespace gmx {
std::ostream outputStream(nullptr);
std::ostream errorStream(nullptr);

void PackBuffer(const LookupMap& resMap, std::string type, std::string res, int &id, google::protobuf::Message *m, std::string gmxPath);
void PackRes(const LookupMap& resMap, std::string &dir, int id, pugi::xml_node &node, google::protobuf::Message *m, int depth);

namespace {

inline std::string GMXPath2FilePath(std::string dir, std::string value) {
  value = string_replace_all(value, "\\", "/");
  const std::string end = dir.substr(dir.find_last_of("/") + 1);
  if (value.find(end) == 0) return dir.substr(0, dir.find_last_of("/") + 1) + value;
  return dir + "/" + value;
}

class visited_walker : public pugi::xml_tree_walker {
  virtual bool for_each(pugi::xml_node &node) {
    if (node.type() != pugi::node_pcdata && std::string(node.attribute("visited").value()) != "true")
      errorStream << "Error: Node at " << node.path() << " was never visited " << std::endl;
    return true;
  }
};

class gmx_root_walker {
 public:
  gmx_root_walker(buffers::TreeNode *root, std::string &gmxPath) : gmxPath(gmxPath) {
    root->set_name("/");  // root node is called assets in GM but we'll use /
    nodes.push_back(root);
  }

  // this works similar to pugixml's doc.traverse but allows us
  // to return false from for_each when we just want to skip the subtree
  // and not the rest of the document
  // need to parse the xml once to build lookup map
  virtual void traverse(pugi::xml_node &node, size_t depth=-1, bool firstRun = false) {
    for (auto &child : node) {
      bool result = for_each(child, depth + 1);
      if (result) {
        traverse(child, depth + 1);
      }
    }
    
    if (firstRun) {
      treeParsed = true;
      traverse(node, depth);
    }
  }

  bool treeParsed = false;

 private:
  std::vector<buffers::TreeNode *> nodes;
  std::string lastName;
  std::string gmxPath;
  std::unordered_map<std::string, int> idMap;
  LookupMap idLookup;

  void AddResource(buffers::TreeNode *node, std::string resType, pugi::xml_node &xmlNode) {
    using buffers::TreeNode;

    using FactoryFunction = std::function<google::protobuf::Message *(TreeNode*)>;
    using FactoryMap = std::unordered_map<std::string, FactoryFunction>;

    static const FactoryMap factoryMap({
      { "sprite", &TreeNode::mutable_sprite },
      { "sound", &TreeNode::mutable_sound },
      { "background", &TreeNode::mutable_background },
      { "path", &TreeNode::mutable_path },
      { "script", &TreeNode::mutable_script },
      { "shader", &TreeNode::mutable_shader },
      { "font", &TreeNode::mutable_font },
      { "timeline", &TreeNode::mutable_timeline },
      { "object", &TreeNode::mutable_object },
      { "room", &TreeNode::mutable_room },
      { "datafile", &TreeNode::mutable_include },
      { "Config", &TreeNode::mutable_settings },
    });

    auto createFunc = factoryMap.find(resType);
    if (createFunc != factoryMap.end()) {
        auto *res = createFunc->second(node);
        if (resType == "datafile") {
          std::string groupPath = gmxPath;
          for (auto parent = std::next(nodes.begin()); parent != nodes.end(); ++parent) {
            groupPath += (*parent)->name() + "/";
          }
          PackRes(idLookup, groupPath, idMap[resType], xmlNode, res, 0);
        } else {
          PackBuffer(idLookup, resType, xmlNode.value(), idMap[resType], res, gmxPath);
        }
        return;
    }
    errorStream << "Unsupported resource type: " << resType << " " << xmlNode.value() << std::endl;
  }

  std::string fix_folder_name(const std::string &name) {
    std::string fixedName = name;
    fixedName[0] = toupper(fixedName[0]);
    if (fixedName != "Help" && fixedName.back() != 's') fixedName += 's';
    return fixedName;
  }

  virtual bool for_each(pugi::xml_node &node, int depth) {
    if (node.type() != pugi::node_pcdata && node.name() != std::string("datafile")) {
      
      std::string name = node.attribute("name").value();

      // These nodes don't have name attributes but appear in tree
      if (name.empty()) {
        if (node.name() == std::string("help")) name = "help";
        if (node.name() == std::string("constants")) name = "constants";
        if (node.name() == std::string("TutorialState")) name = "TutorialState";
      }

      while (depth > 0 && static_cast<int>(nodes.size()) >= depth + 1) {
        // our xml depth was less than our tree depth need to go back
        nodes.pop_back();
      }

      if (!name.empty() && treeParsed) {
        if (node.name() == std::string("constant")) return true;  //TODO: add constants here

        buffers::TreeNode *n = nodes.back()->add_child();  // adding a folder
        if (depth == 1)                                  // fix root folder names
          name = fix_folder_name(name);

        n->set_name(name);
        n->set_folder(true);
        nodes.push_back(n);
      }
    } else {
      pugi::xml_node xml_parent = node.parent();
      if (xml_parent.name() == std::string("constant")) return true;  //constants are handled above with folders

      if (xml_parent.parent().name() == std::string("TutorialState")) return true;  // TODO: handle tutorial states

      std::string resName;
      std::string resType = node.name();
      if (resType != "datafile") {
        // remove extensions (eg .gml, .shader)
        std::string res = node.value();
        size_t marker = res.find_last_of("\\");  // split sound\song
        if (marker != std::string::npos) {
          resName = res.substr(marker + 1, res.length());
          resType = res.substr(0, (res[marker - 1] == 's') ? marker - 1 : marker);
        } else {  // some things are in the root (eg help.rtf) of the gmx so there is no \. Therfore, we must get the type from the parent tag
          resName = res;
          resType = xml_parent.name();
        }

        size_t dot = resName.find_last_of(".");
        if (dot != std::string::npos) {
          resName = resName.substr(0, dot);
        }
      } else {
        resName = node.child_value("name");
      }
      
      if (!treeParsed) {
        int count = idLookup[resType].size();
        count = idLookup[resType].size();
        idLookup[resType][count] = resName;
      } else {
        // Can't parse resource until lookup map is done
        buffers::TreeNode *n = nodes.back()->add_child();  // adding res here
        n->set_name(resName);
        AddResource(n, resType, node);
      }

      if (resType == "datafile") {
        // we handled the metadata in AddResource
        // so just skip the subtree
        return false;
      }
    }

    return true;
  }
};

std::string FileToString(const std::string &fName) {
  std::ifstream t(fName.c_str());
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

}  // Anonymous namespace

void PackScript(std::string fName, int id, buffers::resources::Script *script) {
  outputStream << "Parsing " << fName << std::endl;
  std::string code = FileToString(fName);

  outputStream << "Setting id as:" << std::endl << id << std::endl;
  script->set_id(id);

  if (code.empty()) errorStream << "Warning: " << fName << " empty." << std::endl;

  outputStream << "Setting code as:" << std::endl << code << std::endl;
  script->set_code(code);
}

void PackShader(std::string fName, int id, buffers::resources::Shader *shader) {
  outputStream << "Parsing " << fName << std::endl;
  std::string code = FileToString(fName);

  outputStream << "Setting id as:" << std::endl << id << std::endl;
  shader->set_id(id);

  if (code.empty()) errorStream << "Warning: " << fName << " empty." << std::endl;

  // GMS 1.4 doesn't care if you have a newline after the marker
  // and before the start of the first line of the fragment shader
  const std::string marker = "//######################_==_YOYO_SHADER_MARKER_==_######################@~";
  size_t markerPos = code.find(marker);

  if (markerPos == std::string::npos)
    errorStream << "Error: missing shader marker." << std::endl;
  else {
    std::string vert = code.substr(0, markerPos);
    std::string frag = code.substr(markerPos + marker.length() - 1, code.length() - 1);
    outputStream << "Setting vertex code as:" << std::endl << vert << std::endl;
    outputStream << "Setting fragment code as:" << std::endl << frag << std::endl;
    shader->set_vertex_code(vert);
    shader->set_fragment_code(frag);
  }
}

void PackRes(const LookupMap& resMap, std::string &dir, int id, pugi::xml_node &node, google::protobuf::Message *m, int depth) {
  const google::protobuf::Descriptor *desc = m->GetDescriptor();
  const google::protobuf::Reflection *refl = m->GetReflection();
  for (int i = 0; i < desc->field_count(); i++) {
    const google::protobuf::FieldDescriptor *field = desc->field(i);
    const google::protobuf::OneofDescriptor *oneof = field->containing_oneof();
    if (oneof && refl->HasOneof(*m, oneof)) continue;
    const google::protobuf::FieldOptions opts = field->options();

    if (field->name() == "id") {
      id += opts.GetExtension(buffers::id_start);
      outputStream << "Setting " << field->name() << " (" << field->type_name() << ") as " << id << std::endl;
      refl->SetInt32(m, field, id);
    } else {
      const std::string gmxName = opts.GetExtension(buffers::gmx);
      const bool isFilePath = opts.GetExtension(buffers::file_path);
      const std::string refType = opts.GetExtension(buffers::resource_ref);
      std::string alias = gmxName;
      bool isSplit = false;
      bool isAttribute = false;
      pugi::xml_node child = node;
      pugi::xml_attribute attr;

      if (gmxName == "GMX_DEPRECATED")
        continue;
        
      // NOTE: GMX typically stores resource refs as strings 
      // but path background rooms seem to be the exception
      if (gmxName == "backroom") {
        child = child.child("backroom");
        child.append_attribute("visited") = "true";
        std::string roomName = "";
        if (resMap.count("room") > 0) {
          if (resMap.at("room").count(child.text().as_int()) > 0) {
            roomName = resMap.at("room").at(child.text().as_int());
          }
        }
        refl->SetString(m, field, roomName);
        outputStream << "Setting " << field->name() << " (" << field->type_name() << ") as " << roomName << std::endl;
        continue;
      } 
        
      if (string_endswith(gmxName, "action")) {
        std::vector<Action> actions;
        int cid = 0;
        for (pugi::xml_node n = child.first_element_by_path(gmxName.c_str()); n != nullptr; n = n.next_sibling()) {
          if (strcmp(n.name(), "action") == 0) {  // skip over any siblings that aren't twins <foo/><bar/><foo/> <- bar would be skipped
            n.append_attribute("visited") = "true";
            Action action;
            PackRes(resMap, dir, cid++, n, &action, depth + 1);
            actions.emplace_back(action);
          }
        }
        refl->SetString(m, field, Actions2Code(actions));
        continue;
      }

      // use the name the protobuf field uses unless there a (gmx) attr
      if (alias.empty()) alias = field->name();

      // this is for <point>0,0</point> crap
      const std::string splitMarker = "GMX_SPLIT/";
      size_t splitPos = gmxName.find(splitMarker);
      isSplit = splitPos != std::string::npos;

      // if it's not a split then we deal with yoyo's useless nesting
      if (!isSplit && alias != "EGM_NESTED") {  // and our useless nesting
        std::vector<std::string> nodes = split_string(alias, '/');

        for (auto n : nodes) {
          child = child.child(n.c_str());
          child.append_attribute("visited") = "true";
          alias = n;
        }
      }

      // We want the data from the node "child" but if its empty what we seek is likely in the attributes
      if (child.empty()) attr = node.attribute(alias.c_str());
      isAttribute = !attr.empty();

      if (child.empty() && !isAttribute && !field->is_repeated() && alias != "EGM_NESTED") {
        // ename only exists if etype = 4. Also, etype and enumb don't exist in timeline events
        pugi::xml_attribute a = node.attribute("eventtype");
        if (alias != "ename" && a.as_int() != 4 && node.path() != "/timeline/entry/event")
          errorStream << "Error: no such element " << node.path() << "/" << alias << std::endl;
      } else {                       // bullshit special cases out of the way, we now look for proto fields in xml
        if (field->is_repeated()) {  // Repeated fields (are usally messages or file_paths(strings)
          outputStream << "Appending (" << field->type_name() << ") to " << field->name() << std::endl;

          switch (field->cpp_type()) {
            case CppType::CPPTYPE_MESSAGE: {
              int cid = 0;
              for (pugi::xml_node n = child; n != nullptr; n = n.next_sibling()) {
                if (n.name() ==
                    alias) {  // skip over any siblings that aren't twins <foo/><bar/><foo/> <- bar would be skipped
                  n.append_attribute("visited") = "true";
                  google::protobuf::Message *msg = refl->AddMessage(m, field);
                  PackRes(resMap, dir, cid++, n, msg, depth + 1);
                }
              }
              break;
            }

            case CppType::CPPTYPE_STRING: {
              for (pugi::xml_node n = child; n != nullptr; n = n.next_sibling()) {
                if (n.name() == alias) {
                  n.append_attribute("visited") = "true";
                  std::string value = n.text().as_string();
                  if (isFilePath) {  // gotta prepend the gmx's path & fix the string to be posix compatible
                    value = GMXPath2FilePath(dir, value);
                  }
                  if (!refType.empty() && value == "<undefined>") break;
                  refl->AddString(m, field, value);
                }
              }
              break;
            }
            /* I don't code options for repeated fields other than messages and strings because we don't need them atm
             * BUT incase someone tries to add one to a proto in the future I added this warning to prevent the reader
             * from exploding and gives them a warning of why their shit don't work and a clue where to implement a fix */
            default: {
              errorStream << "Error: missing condition for repeated type: " << field->type_name()
                            << ". Instigated by: " << field->type_name() << std::endl;
              break;
            }
          }
        } else {  // Now we parse individual proto fields to individual xml fields (and attributes)
          pugi::xml_text xmlValue;
          std::string splitValue;

          if (!isAttribute) {  // if data we want is not in an attribute (eg <bar x="9001">)
            if (isSplit) {     // if data use a comma delimiter (eg (<foo>0,7,9</foo>)
              std::vector<std::string> split = split_string(node.text().as_string(), ',');
              splitValue = split[static_cast<int>(gmxName.back()) - '0'];
            } else  // else data is just in an xml tag (eg <foo>Josh can't read code</foo>)
              xmlValue = child.text();
          }

          std::string value = (isAttribute) ? attr.as_string() : (isSplit) ? splitValue : xmlValue.as_string();
          outputStream << "Setting " << field->name() << " (" << field->type_name() << ") as " << value << std::endl;

          /* Here we finally set the proto values from the xml (unless it's a message). The same logic above is followed,
           * if datas in attribute use that, else if it's in a split grab the text from a vector we previous split up by
           * the delimeter, else if the datas in a element use that */
          switch (field->cpp_type()) {
            // If field is a singular message we need to recurse into this method again
            case CppType::CPPTYPE_MESSAGE: {
              google::protobuf::Message *msg = refl->MutableMessage(m, field);
              PackRes(resMap, dir, 0, child, msg, depth + 1);
              break;
            }
            case CppType::CPPTYPE_INT32: {
              refl->SetInt32(m, field,
                              (isAttribute) ? attr.as_int() : (isSplit) ? std::stoi(splitValue) : xmlValue.as_int());
              break;
            }
            case CppType::CPPTYPE_INT64: {
              refl->SetInt64(m, field,
                              (isAttribute) ? attr.as_int() : (isSplit) ? std::stoi(splitValue) : std::stoll(xmlValue.as_string()));
              break;
            }
            case CppType::CPPTYPE_UINT32: {
              refl->SetUInt32(
                  m, field, (isAttribute) ? attr.as_uint() : (isSplit) ? std::stoi(splitValue) : xmlValue.as_uint());
              break;
            }
            case CppType::CPPTYPE_UINT64: {
              refl->SetUInt64(
                  m, field, (isAttribute) ? attr.as_uint() : (isSplit) ? std::stoi(splitValue) : std::stoull(xmlValue.as_string()));
              break;
            }
            case CppType::CPPTYPE_DOUBLE: {
              refl->SetDouble(
                  m, field,
                  (isAttribute) ? attr.as_double() : (isSplit) ? std::stod(splitValue) : xmlValue.as_double());
              break;
            }
            case CppType::CPPTYPE_FLOAT: {
              refl->SetFloat(
                  m, field,
                  (isAttribute) ? attr.as_float() : (isSplit) ? std::stof(splitValue) : xmlValue.as_float());
              break;
            }
            case CppType::CPPTYPE_BOOL: {
              refl->SetBool(m, field,
                            (isAttribute) ? (attr.as_int() != 0)
                                          : (isSplit) ? (std::stof(splitValue) != 0) : (xmlValue.as_int() != 0));
              break;
            }
            case CppType::CPPTYPE_ENUM: {
              refl->SetEnum(
                  m, field,
                  field->enum_type()->FindValueByNumber(
                      (isAttribute) ? attr.as_int() : (isSplit) ? std::stoi(splitValue) : xmlValue.as_int()));
              break;
            }
            case CppType::CPPTYPE_STRING: {
              std::string value = (isAttribute) ? attr.as_string() : (isSplit) ? splitValue : xmlValue.as_string();
              if (isFilePath) {  // again gotta prepend the gmx's path & fix the string to be posix compatible
                value = GMXPath2FilePath(dir, value);
              }
              if (!refType.empty() && value == "<undefined>") break;
              refl->SetString(m, field, value);
              break;
            }
          }
        }
      }
    }
  }
}

void PackBuffer(const LookupMap& resMap, std::string type, std::string res, int &id, google::protobuf::Message *m, std::string gmxPath) {
  // Scripts and Shaders are plain text not xml
  std::string fName = gmxPath + string_replace_all(res, "\\", "/");
  std::string resName = fName.substr(fName.find_last_of('/') + 1, fName.length() - 1);
  resName = resName.substr(0, resName.find_last_of('.'));

  if (type == "script") {
    buffers::resources::Script script;
    PackScript(fName, id++, &script);
    m->CopyFrom(*static_cast<google::protobuf::Message *>(&script));
  } else if (type == "shader") {
    buffers::resources::Shader shader;
    PackShader(fName, id++, &shader);
    m->CopyFrom(*static_cast<google::protobuf::Message *>(&shader));
  } else {
    std::string fileExt = type;
    fName += "." + fileExt + ".gmx";

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fName.c_str());
    pugi::xml_node root = doc.document_element();
    root.append_attribute("visited") = "true";

    if (!result)
      errorStream << "Error opening: " << fName << " : " << result.description() << std::endl;
    else {
      outputStream << "Parsing " << fName << "..." << std::endl;
      // Start a resource (sprite, object, room)
      std::string dir = fName.substr(0, fName.find_last_of("/"));
      PackRes(resMap, dir, id++, root, m, 0);

      visited_walker walker;
      doc.traverse(walker);
    }
  }
}

buffers::Project *LoadGMX(std::string fName) {
  pugi::xml_document doc;
  if (!doc.load_file(fName.c_str())) return nullptr;

  fName = string_replace_all(fName, "\\", "/");
  std::string gmxPath = fName.substr(0, fName.find_last_of("/") + 1);

  buffers::Project *proj = new buffers::Project();
  buffers::Game *game = proj->mutable_game();
  gmx_root_walker walker(game->mutable_root(), gmxPath);
  // we use our own traverse(...) instead of the pugixml one
  // so that we can skip subtrees for datafiles and such
  walker.traverse(doc, -1, true);

  return proj;
}

template<class T>
T* LoadResource(std::string fName, std::string type) {
  size_t dot = fName.find_last_of(".");
  size_t slash = fName.find_last_of("/");

  if (dot == std::string::npos || slash == std::string::npos)
    return nullptr;

  std::string resType = fName.substr(dot+1, fName.length());
  std::string resName = fName.substr(slash+1, fName.length());

  dot = resName.find_first_of(".");

  if (dot == std::string::npos)
    return nullptr;

  resName = resName.substr(0, dot);
  std::string dir = fName.substr(0, slash+1);

  if (resType == "gmx") {
    pugi::xml_document doc;
    if (!doc.load_file(fName.c_str())) return nullptr;
    resType = doc.document_element().name(); // get type from root xml element
  }  else if (resType == "gml") resType = "script";

  if (resType != type || resName.empty()) // trying to load wrong type (eg a.gmx has <b> instead of <a> as root xml)
    return nullptr;

  int id = 0;
  T* res = new T();
  PackBuffer(LookupMap(), resType, resName, id, res, dir);
  return res;
}

Background* LoadBackground(std::string fName) {
  return LoadResource<Background>(fName, "background");
}

buffers::resources::Sound* LoadSound(std::string fName) {
  return LoadResource<Sound>(fName, "sound");
}

buffers::resources::Sprite* LoadSprite(std::string fName) {
  return LoadResource<Sprite>(fName, "sprite");
}

buffers::resources::Shader* LoadShader(std::string fName) {
  return LoadResource<Shader>(fName, "shader");
}

buffers::resources::Font* LoadFont(std::string fName) {
  return LoadResource<Font>(fName, "font");
}

buffers::resources::Object* LoadObject(std::string fName) {
  return LoadResource<Object>(fName, "object");
}

buffers::resources::Timeline* LoadTimeLine(std::string fName) {
  return LoadResource<Timeline>(fName, "timeline");
}

buffers::resources::Room* LoadRoom(std::string fName) {
  return LoadResource<Room>(fName, "room");
}

buffers::resources::Path* LoadPath(std::string fName) {
  return LoadResource<Path>(fName, "path");
}

buffers::resources::Script* LoadScript(std::string fName) {
  return LoadResource<Script>(fName, "script");
}

}  //namespace gmx
