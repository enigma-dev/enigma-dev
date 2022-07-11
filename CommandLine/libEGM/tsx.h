#ifndef EGM_TSX_H
#define EGM_TSX_H

#include "file-format.h"
#include "pugixml.hpp"

namespace egm {

class TSXFileFormat : public FileFormat {
public:
  TSXFileFormat(const EventData* event_data) : FileFormat(event_data) {}
  virtual std::unique_ptr<Project> LoadProject(const fs::path& fPath) const override;
};

class TSXTilesetLoader : public pugi::xml_tree_walker {
public:
  TSXTilesetLoader(buffers::TreeNode *root, const fs::path &fPath);
  TSXTilesetLoader(const fs::path &fPath,
                   std::vector<buffers::TreeNode *> &existingTreeNode,
                   buffers::TreeNode * existingBgFolderRef,
                   const std::string &backgroundName,
                   std::unordered_map<std::string, buffers::TreeNode *> *backgroundNamePtrMap);

private:
  std::vector<buffers::TreeNode *> nodes;
  fs::path tsxPath;
  buffers::TreeNode *backgroundFolderRef;
  std::string backgroundResName;
  std::unordered_map<std::string, buffers::TreeNode *> *backgroundNamePtrMapRef;
  std::unordered_map<std::string, int> resourceTypeIdCountMap;

  virtual bool for_each(pugi::xml_node& xmlNode) override;
  void UpdateImageHelper(const pugi::xml_node &innerImgNode, buffers::TreeNode *protoNode);
};

} // namespace egm

#endif
