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

#include <pugixml.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <algorithm>

#include "gmx.h"

namespace gmx {

inline std::string string_replace_all(std::string str, std::string substr, std::string nstr)
{
  size_t pos = 0;
  while ((pos = str.find(substr, pos)) != std::string::npos)
  {
    str.replace(pos, substr.length(), nstr);
    pos += nstr.length();
  }
  return str;
}

inline std::string extract_file_path(std::string dir, std::string value) {
  value = string_replace_all(value, "\\", "/");
  const std::string end = dir.substr(dir.find_last_of("/") + 1);
  if (value.find(end) == 0)
    return dir.substr(0, dir.find_last_of("/") + 1) + value;
  return dir + "/" + value;
}

static std::ostream outputStream(nullptr);
static std::map<std::string, std::vector<std::string>> resMap;

class visited_walker : public pugi::xml_tree_walker {
  virtual bool for_each(pugi::xml_node &node) {
    if (node.type() != pugi::node_pcdata  && std::string(node.attribute("visited").value()) != "true")
      outputStream << "Error: Node at " << node.path() << " was never visited " << std::endl;
    return true;
  }
};

class gmx_root_walker : public pugi::xml_tree_walker {
  std::map<std::string, int> constants;

  virtual bool for_each(pugi::xml_node &node) {
    if (node.type() == pugi::node_pcdata) {
      if (std::string(node.parent().name()) == "constant") {
        constants[node.parent().attribute("name").value()] = node.text().as_int();
      } else {
        resMap[std::string(node.parent().name()) + "s"].push_back(node.value());
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

std::vector<std::string> SplitString(const std::string &str, char delimiter) {
  std::vector<std::string> vec;
  std::stringstream sstr(str);
  std::string tmp;
  while (std::getline(sstr, tmp, delimiter)) vec.push_back(tmp);

  return vec;
}

void PackScript(std::string fName, std::string &name, int id, buffers::resources::Script *script) {
  outputStream << "Parsing " << fName << std::endl;
  std::string code = FileToString(fName);

  outputStream << "Setting name as:" << std::endl << name << std::endl;
  script->set_name(name);
  outputStream << "Setting id as:" << std::endl << id << std::endl;
  script->set_id(id);

  if (code.empty()) outputStream << "Warning: " << fName << " empty." << std::endl;

  outputStream << "Setting code as:" << std::endl << code << std::endl;
  script->set_code(code);
}

void PackShader(std::string fName, std::string &name, int id, buffers::resources::Shader *shader) {
  outputStream << "Parsing " << fName << std::endl;
  std::string code = FileToString(fName);

  outputStream << "Setting name as:" << std::endl << name << std::endl;
  shader->set_name(name);
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

void PackRes(std::string &dir, std::string &name, int id, pugi::xml_node &node, google::protobuf::Message *m, int depth) {
  const google::protobuf::Descriptor *desc = m->GetDescriptor();
  const google::protobuf::Reflection *refl = m->GetReflection();
  for (int i = 0; i < desc->field_count(); i++) {
    const google::protobuf::FieldDescriptor *field = desc->field(i);
    const google::protobuf::OneofDescriptor *oneof = field->containing_oneof();
    if (oneof && refl->HasOneof(*m, oneof))
      continue;
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
        if (!isSplit) {
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

        if (child.empty() && !isAttribute && !field->is_repeated()) {
          // ename only exists if etype = 4. Also, etype and enumb don't exist in timeline events
          pugi::xml_attribute a = node.attribute("eventtype");
          if (xmlElement != "ename" && a.as_int() != 4 && node.path() != "/timeline/entry/event")
            outputStream << "Error: no such element " << node.path() << "/" << xmlElement << std::endl;
        } else {
          if (field->is_repeated()) {
            outputStream << "Appending (" << field->type_name() << ") to " << field->name() << std::endl;

            switch (field->cpp_type()) {
              case google::protobuf::FieldDescriptor::FieldDescriptor::CppType::CPPTYPE_MESSAGE: {
                int cid = 0;
                for (pugi::xml_node n = child; n != nullptr; n = n.next_sibling()) {
                  // skip over any siblings that aren't twins
                  if (n.name() == xmlElement) {
                    n.append_attribute("visited") = "true";
                    google::protobuf::Message *msg = refl->AddMessage(m, field);
                    PackRes(dir, name, cid++, n, msg, depth + 1);
                  }
                }
                break;
              }

              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING: {
                for (pugi::xml_node n = child; n != nullptr; n = n.next_sibling()) {
                  if (n.name() == xmlElement) {
                    n.append_attribute("visited") = "true";
                    std::string value = n.text().as_string();
                    if (isFilePath) {
                      value = extract_file_path(dir, value);
                    }
                    refl->AddString(m, field, value);
                  }
                }
                break;
              }

              default: {
                outputStream << "Error: missing condition for repeated type: " << field->type_name()
                            << ". Instigated by: " << field->type_name() << std::endl;
                // I don't think we repeat anything other than messages and strings
                break;
              }
            }
          } else {
            pugi::xml_text xmlValue;
            std::string splitValue;

            if (!isAttribute) {
              if (isSplit) {
                std::vector<std::string> split = SplitString(node.text().as_string(), ',');
                splitValue = split[static_cast<int>(gmxName.back()) - '0'];
              } else
                xmlValue = child.text();
            }

            std::string value = (isAttribute) ? attr.as_string() : (isSplit) ? splitValue : xmlValue.as_string();
            outputStream << "Setting " << field->name() << " (" << field->type_name() << ") as " << value << std::endl;

            switch (field->cpp_type()) {
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE: {
                google::protobuf::Message *msg = refl->MutableMessage(m, field);
                PackRes(dir, name, 0, child, msg, depth + 1);
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32: {
                refl->SetInt32(m, field,
                              (isAttribute) ? attr.as_int() : (isSplit) ? std::stoi(splitValue) : xmlValue.as_int());
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64: {
                refl->SetInt64(m, field,
                              (isAttribute) ? attr.as_int() : (isSplit) ? std::stoi(splitValue) : xmlValue.as_int());
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT32: {
                refl->SetUInt32(m, field,
                                (isAttribute) ? attr.as_uint() : (isSplit) ? std::stoi(splitValue) : xmlValue.as_uint());
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT64: {
                refl->SetUInt64(m, field,
                                (isAttribute) ? attr.as_uint() : (isSplit) ? std::stoi(splitValue) : xmlValue.as_uint());
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE: {
                refl->SetDouble(m, field,
                                (isAttribute) ? attr.as_double() : (isSplit) ? std::stod(splitValue) : xmlValue.as_double());
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_FLOAT: {
                refl->SetFloat(m, field,
                              (isAttribute) ? attr.as_float() : (isSplit) ? std::stof(splitValue) : xmlValue.as_float());
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_BOOL: {
                refl->SetBool(m, field,
                              (isAttribute) ? (attr.as_int() != 0) : (isSplit) ? (std::stof(splitValue) != 0) : (xmlValue.as_int() != 0));
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM: {
                refl->SetEnum(m, field, field->enum_type()->FindValueByNumber(
                              (isAttribute) ? attr.as_int() : (isSplit) ? std::stoi(splitValue) : xmlValue.as_int()));
                break;
              }
              case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING: {
                std::string value = (isAttribute) ? attr.as_string() : (isSplit) ? splitValue : xmlValue.as_string();
                if (isFilePath) {
                  value = extract_file_path(dir, value);
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

void PackBuffer(google::protobuf::Message *m, std::string gmxPath) {
  const google::protobuf::Descriptor *desc = m->GetDescriptor();
  const google::protobuf::Reflection *refl = m->GetReflection();

  // Iterate over fbs fields
  for (int i = 0; i < desc->field_count(); i++) {
    const google::protobuf::FieldDescriptor *field = desc->field(i);
    std::string name = field->name();

    if (resMap.find(name) != resMap.end()) {
      outputStream << "Found " << resMap.at(name).size() << " " << name << std::endl;

      // Scripts and Shaders are plain text not xml
      int id = 0;
      for (const auto &res : resMap.at(name)) {
        std::string fName = gmxPath + string_replace_all(res, "\\", "/");
        std::string resName = fName.substr(fName.find_last_of('/') + 1, fName.length() - 1);
        resName = resName.substr(0, resName.find_last_of('.'));

        if (name == "scripts") {
          buffers::resources::Script *script = new buffers::resources::Script();
          PackScript(fName, resName, id++, script);
          google::protobuf::Message *msg = refl->AddMessage(m, field);
          msg->CopyFrom(*static_cast<google::protobuf::Message *>(script));
        } else if (name == "shaders") {
          buffers::resources::Shader *shader = new buffers::resources::Shader();
          PackShader(fName, resName, id++, shader);
          google::protobuf::Message *msg = refl->AddMessage(m, field);
          msg->CopyFrom(*static_cast<google::protobuf::Message *>(shader));
        } else {
          std::string type = name.substr(0, name.length() - 1);
          fName += "." + type + ".gmx";

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
            google::protobuf::Message *msg = refl->AddMessage(m, field);

            PackRes(dir, resName, id++, root, msg, 0);

            visited_walker walker;
            doc.traverse(walker);
          }
        }
      }
    }
  }
}

buffers::Project *LoadGMX(std::string fName, bool verbose) {
  if (verbose) outputStream.rdbuf(std::cerr.rdbuf());

  buffers::Project *proj = new buffers::Project();
  pugi::xml_document doc;
  if (!doc.load_file(fName.c_str())) return nullptr;

  gmx_root_walker walker;
  doc.traverse(walker);

  fName = string_replace_all(fName, "\\", "/");
  std::string gmxPath = fName.substr(0, fName.find_last_of("/") + 1);
  PackBuffer(proj, gmxPath);

  return proj;
}

}  //namespace gmx
