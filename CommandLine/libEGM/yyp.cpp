/** Copyright (C) 2018 Robert B. Colton
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

#include "yyp.h"

#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/reader.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <functional>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

using namespace buffers;
using namespace buffers::resources;

namespace yyp {
std::ostream out(nullptr);
std::ostream err(nullptr);

buffers::Project *LoadYYP(std::string fName) {
  std::ifstream ifs(fName);
  if (!ifs) {
    err << "Could not open YYP for reading: " << fName << std::endl;
    return nullptr;
  }
  std::string yypDir = fName.substr(0, fName.find_last_of("/\\\\") + 1);
  rapidjson::IStreamWrapper isw(ifs);
  rapidjson::Document doc;
  doc.ParseStream(isw);

  const auto &resourcesArray = doc["resources"].GetArray();
  std::unordered_map<std::string, const rapidjson::Value::ConstObject> treeMap;
  for (const auto &res : resourcesArray) {
    const auto &key = res["Key"].GetString();
    const auto &value = res["Value"].GetObject();
    treeMap.insert(std::make_pair(key, value));
  }

  std::vector<TreeNode*> roots;
  std::unordered_map<std::string, TreeNode*> nodes;
  std::vector<std::pair<TreeNode*, std::vector<std::string> > > parents;
  for (const auto &treeEntry : treeMap) {
    const auto &key = treeEntry.first;
    const auto &value = treeEntry.second;
    const auto &resourcePath = value["resourcePath"].GetString();
    const auto &resourceType = value["resourceType"].GetString();

    const std::string nodePath = yypDir + resourcePath;
    std::ifstream ifsNode(nodePath);
    if (!ifsNode) {
      err << "Could not open YYP resource for reading: " << nodePath << std::endl;
      continue;
    }
    rapidjson::IStreamWrapper iswNode(ifsNode);
    rapidjson::Document nodeDoc;
    nodeDoc.ParseStream(iswNode);

    TreeNode* node = new TreeNode();
    nodes[key] = node;
    if (strcmp(resourceType, "GMFolder") == 0) {
      node->set_folder(true);
      node->set_name(nodeDoc["folderName"].GetString());
      const auto filterType = nodeDoc["filterType"].GetString();
      if (strcmp(filterType, "root") == 0)
        roots.push_back(node);
      const auto &children = nodeDoc["children"];
      const auto &childrenArray = children.GetArray();
      std::vector<std::string> childrenVector;
      childrenVector.resize(childrenArray.Size());
      for (const auto &child : childrenArray) {
        childrenVector.emplace_back(child.GetString());
      }
      parents.push_back(std::make_pair(node, childrenVector));
    } else {
      node->set_folder(false);
      node->set_name(nodeDoc["name"].GetString());

      using FactoryFunction = std::function<google::protobuf::Message *(TreeNode*)>;
      using FactoryMap = std::unordered_map<std::string, FactoryFunction>;

      static const FactoryMap factoryMap({
        { "GMSound",      &TreeNode::mutable_sound      },
        { "GMTileSet",    &TreeNode::mutable_background },
        { "GMSprite",     &TreeNode::mutable_sprite     },
        { "GMShader",     &TreeNode::mutable_shader     },
        { "GMScript",     &TreeNode::mutable_script     },
        { "GMFont",       &TreeNode::mutable_font       },
        { "GMObject",     &TreeNode::mutable_object     },
        { "GMTimeline",   &TreeNode::mutable_timeline   },
        { "GMRoom",       &TreeNode::mutable_room       },
        { "GMPath",       &TreeNode::mutable_path       }
      });

      auto createFunc = factoryMap.find(resourceType);
      if (createFunc != factoryMap.end()) {
          auto *res = createFunc->second(node);
          //PackBuffer(resType, resName, idMap[resType], res, gmxPath);
      }
      //err << "Unsupported resource type: " << resourceType << " " << resourceName << std::endl;
    }
  }

  if (roots.empty()) {
    err << "The project contained no root tree nodes and could not finish loading" << std::endl;
    return nullptr;
  }

  for (const auto &parent : parents) {
    TreeNode* node = parent.first;
    const auto &children = parent.second;
    for (const auto &child : children) {
      const auto &key = child;
      const auto &childNodeIt = nodes.find(key);
      if (childNodeIt == nodes.end()) {
        err << "Could not find child '" << key << "'" << std::endl;
        continue;
      }
      TreeNode *childNode = childNodeIt->second;
      node->mutable_child()->AddAllocated(childNode);
    }
  }

  buffers::Project *proj = new buffers::Project();
  buffers::Game *game = proj->mutable_game();
  game->set_allocated_root(roots[0]);

  return proj;
}

}  //namespace yyp
