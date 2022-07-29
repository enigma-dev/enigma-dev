#ifndef EGM_TSX_H
#define EGM_TSX_H

#include "file-format.h"
#include "pugixml.hpp"

namespace egm {


/**
 * @brief The TSXFileFormat class provides functionality to load Tiled .tsx files independently
 */
class TSXFileFormat : public FileFormat {
public:
  /**
   * @brief TSXFileFormat constructor
   * @param event_data
   */
  TSXFileFormat(const EventData* event_data) : FileFormat(event_data) {}

  /**
   * @brief Loads .tsx file to EGM Project
   * @param fPath .tsx file path
   * @return Pointer to EGM Project
   */
  virtual std::unique_ptr<Project> LoadProject(const fs::path& fPath) const override;
};


/**
 * @brief The TSXTilesetLoader class manages parsing of Tiled .tsx file format
 */
class TSXTilesetLoader : public pugi::xml_tree_walker {
public:
  /**
   * @brief Constructor to init variables required to load standalone Tiled .tsx file(s)
   * @param root Pointer to TreeNode proto message
   * @param fPath .tsx file path
   */
  TSXTilesetLoader(buffers::TreeNode *root, const fs::path &fPath);

  /**
   * @brief Constructor to init variables to load Tiled tileset into a new Background resource for an existing EGM project
   * @param fPath .tsx file path
   * @param existingTreeNode Reference to existing EGM project TreeNode heirarchy
   * @param existingBgFolderRef Pointer to existing Background resource folder(if already exist)
   * @param backgroundName Reference to background resource name to be created
   * @param backgroundNamePtrMap Pointer to hash map of Background res name and its corresponding Background res pointer
   */
  TSXTilesetLoader(const fs::path &fPath,
                   std::vector<buffers::TreeNode *> &existingTreeNode,
                   buffers::TreeNode *existingBgFolderRef,
                   const std::string &backgroundName,
                   std::unordered_map<std::string, buffers::TreeNode *> *backgroundNamePtrMap);

private:
  /**
   * @brief Stores path to the .tsx file
   */
  fs::path tsxPath;

  /**
   * @brief Stores TreeNode heirarchy of an existing or a new EGM project
   */
  std::vector<buffers::TreeNode *> nodes;

  /**
   * @brief Stores pointer to Background resource folder
   */
  buffers::TreeNode *backgroundFolderRef;

  /**
   * @brief Name of the Background resource to be created
   */
  std::string backgroundResName;

  /**
   * @brief Pointer to hash map of Background resource name and its corresponding Background resource pointer
   */
  std::unordered_map<std::string, buffers::TreeNode *> *backgroundNamePtrMapRef;

  /**
   * @brief Pointer to hash map of unique resource name and their total count
   */
  std::unordered_map<std::string, int> resourceTypeIdCountMap;

  /**
   * @brief Traverses pugixml node, handled by parent pugixml class
   * @param xmlNode Current xml node to be traversed
   * @return A boolean to denote whether traversal should end or proceed further
   */
  virtual bool for_each(pugi::xml_node& xmlNode) override;

  /**
   * @brief Helper method to update image specific details of Background resource from given xml node
   * @param innerImgNode Reference to image node which contains relevant information
   * @param protoNode Pointer to Background resource which has to be updated
   */
  void UpdateImageHelper(const pugi::xml_node &innerImgNode, buffers::TreeNode *protoNode);

  /**
   * @brief Creates new Background resources for each <tile> node present in parent <tileset> node in xml hierarchy
   * @param xmlNode Reference to <tile> node to be replicated
   * @param resType "background" resource type in strings
   */
  void LoadTilesetAsIndividualBackgrounds(const pugi::xml_node& xmlNode, const std::string& resType);

  /**
   * @brief Creates a Background resource for given <tileset> representing atlas of images
   * @param xmlNode Reference to <tileset> node to be replicated
   * @param resType "background" resource type in strings
   * @param childXmlNode Reference to child <image> xml node
   */
  void LoadTilesetAsSingleBackground(const pugi::xml_node& xmlNode, const std::string& resType,
                                     const pugi::xml_node& childXmlNode);
};

} // namespace egm

#endif
