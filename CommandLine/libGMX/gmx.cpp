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

#include <pugixml.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

namespace gmx {

void PackBuffer(std::string type, std::string res, int &id, google::protobuf::Message *m, std::string gmxPath);

namespace {

inline std::string string_replace_all(std::string str, std::string substr, std::string nstr) {
  size_t pos = 0;
  while ((pos = str.find(substr, pos)) != std::string::npos) {
    str.replace(pos, substr.length(), nstr);
    pos += nstr.length();
  }
  return str;
}

inline std::string GMXPath2FilePath(std::string dir, std::string value) {
  value = string_replace_all(value, "\\", "/");
  const std::string end = dir.substr(dir.find_last_of("/") + 1);
  if (value.find(end) == 0) return dir.substr(0, dir.find_last_of("/") + 1) + value;
  return dir + "/" + value;
}

static std::ostream outputStream(nullptr);

class visited_walker : public pugi::xml_tree_walker {
  virtual bool for_each(pugi::xml_node &node) {
    if (node.type() != pugi::node_pcdata && std::string(node.attribute("visited").value()) != "true")
      outputStream << "Error: Node at " << node.path() << " was never visited " << std::endl;
    return true;
  }
};

class gmx_root_walker : public pugi::xml_tree_walker {
 public:
  gmx_root_walker(buffers::TreeNode *root, std::string &gmxPath) : gmxPath(gmxPath) {
    root->set_name("/");  // root node is called assets in GM but we'll use /
    nodes.push_back(root);
  }

  unsigned include_count = 0;
  unsigned sound_count = 0;
  unsigned background_count = 0;
  unsigned sprite_count = 0;
  unsigned shader_count = 0;
  unsigned font_count = 0;
  unsigned object_count = 0;
  unsigned timeline_count = 0;
  unsigned room_count = 0;
  unsigned help_count = 0;
  unsigned config_count = 0;
  unsigned path_count = 0;
  unsigned script_count = 0;
  unsigned constant_count = 0;

 private:
  std::vector<buffers::TreeNode *> nodes;
  std::string lastName;
  std::string gmxPath;
  int id = 0;

  void AddResource(buffers::TreeNode *node, std::string resType, std::string resName) {
    if (resType == "sound") {
      buffers::resources::Sound *snd = node->mutable_sound();
      PackBuffer(resType, resName, id, snd, gmxPath);
      sound_count++;
      return;
    }
    if (resType == "background") {
      buffers::resources::Background *bkg = node->mutable_background();
      PackBuffer(resType, resName, id, bkg, gmxPath);
      background_count++;
      return;
    }
    if (resType == "sprite") {
      buffers::resources::Sprite *spr = node->mutable_sprite();
      PackBuffer(resType, resName, id, spr, gmxPath);
      sprite_count++;
      return;
    }
    if (resType == "shader") {
      buffers::resources::Shader *shdr = node->mutable_shader();
      PackBuffer(resType, resName, id, shdr, gmxPath);
      shader_count++;
      return;
    }
    if (resType == "script") {
      buffers::resources::Script *scr = node->mutable_script();
      PackBuffer(resType, resName, id, scr, gmxPath);
      script_count++;
      return;
    }
    if (resType == "font") {
      buffers::resources::Font *fnt = node->mutable_font();
      PackBuffer(resType, resName, id, fnt, gmxPath);
      font_count++;
      return;
    }
    if (resType == "object") {
      buffers::resources::Object *obj = node->mutable_object();
      PackBuffer(resType, resName, id, obj, gmxPath);
      object_count++;
      return;
    }
    if (resType == "timeline") {
      buffers::resources::Timeline *tml = node->mutable_timeline();
      PackBuffer(resType, resName, id, tml, gmxPath);
      timeline_count++;
      return;
    }
    if (resType == "room") {
      buffers::resources::Room *rm = node->mutable_room();
      PackBuffer(resType, resName, id, rm, gmxPath);
      room_count++;
      return;
    }
    if (resType == "path") {
      buffers::resources::Path *pth = node->mutable_path();
      PackBuffer(resType, resName, id, pth, gmxPath);
      path_count++;
      return;
    }

    outputStream << "Unsuported resource type: " << resType << " " << resName << std::endl;
    /*if (resType == "config") return;    //not supported yet
    if (resType == "datafile") return;  //not supported yet
    if (resType == "constant") return;  //not supported yet
    if (resType == "help") return;      //not supported yet*/
  }
  
  std::string fix_folder_name(const std::string& name) {
    std::string fixedName = name;
    fixedName[0] = toupper(fixedName[0]);
    if (fixedName != "Help" && fixedName.back() != 's') fixedName += 's';
    return fixedName;
  }
  
  virtual bool for_each(pugi::xml_node &node) {
        
    if (node.type() != pugi::node_pcdata) {
      
      std::string name = node.attribute("name").value();
      
      // These nodes don't have name attributes but appear in tree
      if (name.empty()) {
        if (node.name() == std::string("help")) name = "help";
        if (node.name() == std::string("constants")) name = "constants";
        if (node.name() == std::string("TutorialState")) name = "TutorialState";
      }
      
      while (depth() > 0 && static_cast<int>(nodes.size()) >= depth() + 1) {  // our xml depth was less than our tree depth need to go back
        nodes.pop_back();
      }
      
      if (!name.empty()) {
        if (node.name() == std::string("constant"))
          return true; //TODO: add constants here
        
        buffers::TreeNode *parent = nodes.back();
        buffers::TreeNode *n = nodes.back()->add_child(); // adding a folder
        if (depth() == 1) // fix root folder names
          name = fix_folder_name(name);
        
        n->set_name(name);
        n->set_folder(true);
        n->set_allocated_parent(parent);
        nodes.push_back(n);
      }
    } else {
      pugi::xml_node xml_parent = node.parent();
      if (xml_parent.name() == std::string("constant"))
        return true; //constants are handled above with folders
        
      if (xml_parent.parent().name() == std::string("TutorialState"))
        return true; // TODO: handle tutorial states
        
      std::string res = node.value();
      std::string resName;
      std::string resType;
      size_t marker = res.find_last_of("\\");  // split sound\song
      if (marker != std::string::npos) {
        resName = res.substr(marker + 1, res.length());
        resType = res.substr(0, (res[marker - 1] == 's') ? marker - 1 : marker);
      } else {  // some things are in the root (eg help.rtf) of the gmx so there is no \. Therfore, we must get the type from the parent tag
        resName = res;
        resType = xml_parent.name();
      }
      
      if (resType != "datafile") {  // remove extensions (eg .gml, .shader)
        size_t dot = resName.find_last_of(".");
        if (dot != std::string::npos) {
          resName = resName.substr(0, dot);
        }
      }

      buffers::TreeNode *parent = nodes.back();
      buffers::TreeNode *n = nodes.back()->add_child();  // adding res here
      n->set_allocated_parent(parent);
      n->set_name(resName);

      AddResource(n, resType, node.value());
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

std::vector<std::string> SplitString(const std::string &str, char delimiter) {
  std::vector<std::string> vec;
  std::stringstream sstr(str);
  std::string tmp;
  while (std::getline(sstr, tmp, delimiter)) vec.push_back(tmp);

  return vec;
}

}  //namespace

void PackScript(std::string fName, int id, buffers::resources::Script *script) {
  outputStream << "Parsing " << fName << std::endl;
  std::string code = FileToString(fName);

  outputStream << "Setting id as:" << std::endl << id << std::endl;
  script->set_id(id);

  if (code.empty()) outputStream << "Warning: " << fName << " empty." << std::endl;

  outputStream << "Setting code as:" << std::endl << code << std::endl;
  script->set_code(code);
}

void PackShader(std::string fName, int id, buffers::resources::Shader *shader) {
  outputStream << "Parsing " << fName << std::endl;
  std::string code = FileToString(fName);

  outputStream << "Setting id as:" << std::endl << id << std::endl;
  shader->set_id(id);

  if (code.empty()) outputStream << "Warning: " << fName << " empty." << std::endl;

  // GMS 1.4 doesn't care if you have a newline after the marker
  // and before the start of the first line of the fragment shader
  const std::string marker = "//######################_==_YOYO_SHADER_MARKER_==_######################@~";
  size_t markerPos = code.find(marker);

  if (markerPos == std::string::npos)
    outputStream << "Error: missing shader marker." << std::endl;
  else {
    std::string vert = code.substr(0, markerPos);
    std::string frag = code.substr(markerPos + marker.length() - 1, code.length() - 1);
    outputStream << "Setting vertex code as:" << std::endl << vert << std::endl;
    outputStream << "Setting fragment code as:" << std::endl << frag << std::endl;
    shader->set_vertex_code(vert);
    shader->set_fragment_code(frag);
  }
}

void PackRes(std::string &dir, std::string &name, int id, pugi::xml_node &node, google::protobuf::Message *m,
             int depth) {
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
    } else if (depth == 0 && field->name() == "name") {
      outputStream << "Setting " << field->name() << " (" << field->type_name() << ") as " << name.c_str() << std::endl;
      refl->SetString(m, field, name);
    } else {
      const std::string gmxName = opts.GetExtension(buffers::gmx);
      const bool isFilePath = opts.GetExtension(buffers::file_path);
      std::string xmlElement = gmxName;
      bool isSplit = false;
      bool isAttribute = false;
      pugi::xml_node child = node;
      pugi::xml_attribute attr;

      // if deprecated we'll just try gmx name anyway and not whine when it fails
      if (gmxName == "GMX_DEPRECATED")
        xmlElement = gmxName;
      else {
        // use the name the protobuf field uses unless there a (gmx) attr
        if (xmlElement.empty()) xmlElement = field->name();

        // this is for <point>0,0</point> crap
        const std::string splitMarker = "GMX_SPLIT/";
        size_t splitPos = gmxName.find(splitMarker);
        isSplit = splitPos != std::string::npos;

        // if it's not a split then we deal with yoyo's useless nesting
        if (!isSplit && xmlElement != "EGM_NESTED") {  // and our useless nesting
          std::vector<std::string> nodes = SplitString(xmlElement, '/');

          for (auto n : nodes) {
            child = child.child(n.c_str());
            child.append_attribute("visited") = "true";
            xmlElement = n;
          }
        }

        // We want the data from the node "child" but if its empty what we seek is likely in the attributes
        if (child.empty()) attr = node.attribute(xmlElement.c_str());
        isAttribute = !attr.empty();

        if (child.empty() && !isAttribute && !field->is_repeated() && xmlElement != "EGM_NESTED") {
          // ename only exists if etype = 4. Also, etype and enumb don't exist in timeline events
          pugi::xml_attribute a = node.attribute("eventtype");
          if (xmlElement != "ename" && a.as_int() != 4 && node.path() != "/timeline/entry/event")
            outputStream << "Error: no such element " << node.path() << "/" << xmlElement << std::endl;
        } else {                       // bullshit special cases out of the way, we now look for proto feilds in xml
          if (field->is_repeated()) {  // Repeated feilds (are usally messages or file_paths(strings)
            outputStream << "Appending (" << field->type_name() << ") to " << field->name() << std::endl;

            switch (field->cpp_type()) {
              case google::protobuf::FieldDescriptor::FieldDescriptor::CppType::CPPTYPE_MESSAGE: {  // repeated message
                int cid = 0;
                for (pugi::xml_node n = child; n != nullptr; n = n.next_sibling()) {
                  if (n.name() ==
                      xmlElement) {  // skip over any siblings that aren't twins <foo/><bar/><foo/> <- bar would be skipped
                    n.append_attribute("visited") = "true";
                    google::protobuf::Message *msg = refl->AddMessage(m, field);
                    PackRes(dir, name, cid++, n, msg, depth + 1);
                  }
                }
                break;
              }

              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING: {  // repeated string
                for (pugi::xml_node n = child; n != nullptr; n = n.next_sibling()) {
                  if (n.name() == xmlElement) {
                    n.append_attribute("visited") = "true";
                    std::string value = n.text().as_string();
                    if (isFilePath) {  // gotta prepend the gmx's path & fix the string to be posix compatible
                      value = GMXPath2FilePath(dir, value);
                    }
                    refl->AddString(m, field, value);
                  }
                }
                break;
              }
              /* I don't code options for repeated fields other than messages and strings because we don't need them atm
               * BUT incase someone tries to add one to a proto in the future I added this warning to prevent the reader
               * from exploding and gives them a warning of why their shit don't work and a clue where to implement a fix */
              default: {
                outputStream << "Error: missing condition for repeated type: " << field->type_name()
                             << ". Instigated by: " << field->type_name() << std::endl;
                break;
              }
            }
          } else {  // Now we parse individual proto fields to individual xml fields (and attributes)
            pugi::xml_text xmlValue;
            std::string splitValue;

            if (!isAttribute) {  // if data we want is not in an attribute (eg <bar x="9001">)
              if (isSplit) {     // if data use a comma delimiter (eg (<foo>0,7,9</foo>)
                std::vector<std::string> split = SplitString(node.text().as_string(), ',');
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
              case google::protobuf::FieldDescriptor::CppType::
                  CPPTYPE_MESSAGE: {  // If field is a singular message we need to recurse into this method again
                google::protobuf::Message *msg = refl->MutableMessage(m, field);
                PackRes(dir, name, 0, child, msg, depth + 1);
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32: {  // if int32
                refl->SetInt32(m, field,
                               (isAttribute) ? attr.as_int() : (isSplit) ? std::stoi(splitValue) : xmlValue.as_int());
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64: {  // if int64
                refl->SetInt64(m, field,
                               (isAttribute) ? attr.as_int() : (isSplit) ? std::stoi(splitValue) : xmlValue.as_int());
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT32: {  // if uint32
                refl->SetUInt32(
                    m, field, (isAttribute) ? attr.as_uint() : (isSplit) ? std::stoi(splitValue) : xmlValue.as_uint());
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT64: {  // if uint64
                refl->SetUInt64(
                    m, field, (isAttribute) ? attr.as_uint() : (isSplit) ? std::stoi(splitValue) : xmlValue.as_uint());
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE: {  // if double
                refl->SetDouble(
                    m, field,
                    (isAttribute) ? attr.as_double() : (isSplit) ? std::stod(splitValue) : xmlValue.as_double());
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_FLOAT: {  // if float
                refl->SetFloat(
                    m, field,
                    (isAttribute) ? attr.as_float() : (isSplit) ? std::stof(splitValue) : xmlValue.as_float());
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_BOOL: {  // if bool
                refl->SetBool(m, field,
                              (isAttribute) ? (attr.as_int() != 0)
                                            : (isSplit) ? (std::stof(splitValue) != 0) : (xmlValue.as_int() != 0));
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM: {  // if enum
                refl->SetEnum(
                    m, field,
                    field->enum_type()->FindValueByNumber(
                        (isAttribute) ? attr.as_int() : (isSplit) ? std::stoi(splitValue) : xmlValue.as_int()));
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING: {  // if singular string
                std::string value = (isAttribute) ? attr.as_string() : (isSplit) ? splitValue : xmlValue.as_string();
                if (isFilePath) {  // again gotta prepend the gmx's path & fix the string to be posix compatible
                  value = GMXPath2FilePath(dir, value);
                }
                refl->SetString(m, field, value);
                break;
              }
            }
          }
        }
      }
    }
  }
}

void PackBuffer(std::string type, std::string res, int &id, google::protobuf::Message *m, std::string gmxPath) {
  // Scripts and Shaders are plain text not xml
  std::string fName = gmxPath + string_replace_all(res, "\\", "/");
  std::string resName = fName.substr(fName.find_last_of('/') + 1, fName.length() - 1);
  resName = resName.substr(0, resName.find_last_of('.'));

  if (type == "script") {
    buffers::resources::Script *script = new buffers::resources::Script();
    PackScript(fName, id++, script);
    m->CopyFrom(*static_cast<google::protobuf::Message *>(script));
  } else if (type == "shader") {
    buffers::resources::Shader *shader = new buffers::resources::Shader();
    PackShader(fName, id++, shader);
    m->CopyFrom(*static_cast<google::protobuf::Message *>(shader));
  } else {
    std::string fileExt = type;
    fName += "." + fileExt + ".gmx";

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fName.c_str());
    pugi::xml_node root = doc.document_element();
    root.append_attribute("visited") = "true";

    if (!result)
      outputStream << "Error opening: " << fName << " : " << result.description() << std::endl;
    else {
      outputStream << "Parsing " << fName << "..." << std::endl;
      // Start a resource (sprite, object, room)
      std::string dir = fName.substr(0, fName.find_last_of("/"));
      PackRes(dir, resName, id++, root, m, 0);

      visited_walker walker;
      doc.traverse(walker);
    }
  }
}

buffers::Project *LoadGMX(std::string fName, bool verbose) {
  if (verbose) outputStream.rdbuf(std::cerr.rdbuf());

  buffers::Project *proj = new buffers::Project();
  pugi::xml_document doc;
  if (!doc.load_file(fName.c_str())) return nullptr;

  fName = string_replace_all(fName, "\\", "/");
  std::string gmxPath = fName.substr(0, fName.find_last_of("/") + 1);

  buffers::Game *game = proj->mutable_game();
  gmx_root_walker walker(game->mutable_root(), gmxPath);
  game->set_include_count(walker.include_count);
  game->set_sound_count(walker.sound_count);
  game->set_background_count(walker.background_count);
  game->set_sprite_count(walker.sprite_count);
  game->set_shader_count(walker.shader_count);
  game->set_font_count(walker.font_count);
  game->set_object_count(walker.object_count);
  game->set_timeline_count(walker.timeline_count);
  game->set_room_count(walker.room_count);
  game->set_help_count(walker.help_count);
  game->set_config_count(walker.config_count);
  game->set_path_count(walker.path_count);
  game->set_script_count(walker.script_count);
  game->set_constant_count(walker.constant_count);
  doc.traverse(walker);

  return proj;
}

}  //namespace gmx
