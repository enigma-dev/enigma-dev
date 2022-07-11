#ifndef XML_UTIL_H
#define XML_UTIL_H

#include "file-format.h"
#include "pugixml.hpp"

#include <string>

void AddTiledResource(buffers::TreeNode *protoNode, std::string resType, const pugi::xml_node &xmlNode,
                 std::unordered_map<std::string, int> &resourceTypeIdCountMap, const std::filesystem::path& tiledFilePath);

void PackTiledRes(const pugi::xml_node &xmlNode, google::protobuf::Message *m,
             std::unordered_map<std::string, int> &resourceTypeIdCountMap, const std::filesystem::path& tiledFilePath);

#endif // XML_UTIL_H
