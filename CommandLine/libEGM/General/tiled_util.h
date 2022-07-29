#ifndef XML_UTIL_H
#define XML_UTIL_H

#include "file-format.h"
#include "pugixml.hpp"

#include <string>

/**
 * @brief Creates EGM Resource of specified type
 * @param protoNode Pointer to generic EGM Resource
 * @param resType Name of resource type to be created
 * @param xmlNode Reference to xml node to parse data from
 * @param resourceTypeIdCountMap Reference to hash map of resource type name and its current count
 * @param tiledFilePath Reference to path of the current Tiled .tsx or .tmx file
 */
void AddTiledResource(buffers::TreeNode *protoNode, std::string resType, const pugi::xml_node &xmlNode,
                 std::unordered_map<std::string, int> &resourceTypeIdCountMap, const std::filesystem::path& tiledFilePath);

/**
 * @brief Parses data stored in given xml node attribute to the corresponding EGM Resource proto message
 * @param xmlNode Reference to xml node to parse data from
 * @param m Pointer to proto message to parse data to
 * @param resourceTypeIdCountMap Reference to hash map of resource type name and its current count
 * @param tiledFilePath Reference to path of the current Tiled .tsx or .tmx file
 */
void PackTiledRes(const pugi::xml_node &xmlNode, google::protobuf::Message *m,
             std::unordered_map<std::string, int> &resourceTypeIdCountMap, const std::filesystem::path& tiledFilePath);

#endif // XML_UTIL_H
