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
                   buffers::TreeNode * existingBgFolderRef);

private:
  std::vector<buffers::TreeNode *> nodes;
  fs::path tsxPath;
  std::unordered_map<std::string, std::string> tiledEnigmaResourceNameMap;
  buffers::TreeNode *backgroundFolderRef;

  virtual bool for_each(pugi::xml_node& xmlNode) override;
  void AddResource(buffers::TreeNode *protoNode, std::string resType, const pugi::xml_node &xmlNode);
  void PackRes(const pugi::xml_node &xmlNode, google::protobuf::Message *m, const std::string& resType, std::string fieldPrefix = "");
};

} // namespace egm

#endif
