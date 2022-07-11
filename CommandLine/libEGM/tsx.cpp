#include "tsx.h"
#include "action.h"
#include "strings_util.h"
#include "General/tiled_util.h"

#include <functional>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using CppType = google::protobuf::FieldDescriptor::CppType;

namespace egm{

// creates a new background resource only project
TSXTilesetLoader::TSXTilesetLoader(buffers::TreeNode *root, const fs::path &fPath) : tsxPath(fPath) {
  root->set_name("/");
  nodes.push_back(root);
  backgroundNamePtrMapRef = NULL;
}

// constructor which fills up the background folder of an existing project
TSXTilesetLoader::TSXTilesetLoader(const fs::path &fPath,
                                   std::vector<buffers::TreeNode *> &existingTreeNode,
                                   buffers::TreeNode *existingBgFolderRef,
                                   const std::string &backgroundName,
                                   std::unordered_map<std::string, buffers::TreeNode *> *backgroundNamePtrMap) : tsxPath(fPath) {
  nodes = existingTreeNode;
  backgroundFolderRef = existingBgFolderRef;
  backgroundResName = backgroundName;
  backgroundNamePtrMapRef = backgroundNamePtrMap;
}

bool TSXTilesetLoader::for_each(pugi::xml_node& xmlNode) {
  // this importer only imports tileset xml nodes
  if(xmlNode.name() != std::string("tileset")) {
    errStream << "Unsupported resource type: " << xmlNode.name() << std::endl;
    return false;
  }

  std::string resType = "background";

  // add new resource according to resType
  if(backgroundFolderRef == NULL) {
    backgroundFolderRef = nodes.back()->mutable_folder()->add_children();
    std::string fixName = resType + 's'; // add an 's' at the end
    fixName[0] -= 32; // make first character capital
    backgroundFolderRef->set_name(fixName);
  }

  // presence of image child node is used to distinguish between loading a tileset based background and an image based background
  pugi::xml_node imgNode = xmlNode.child("image");
  if(imgNode.empty()) {
    pugi::xml_object_range<pugi::xml_named_node_iterator> tileChildrenItr = xmlNode.children("tile");

    outStream << "Adding tile as background" << std::endl;

    for(const pugi::xml_node &tileChild : tileChildrenItr) {
      // TODO: Needs improvement: All compatible* individual tiles can be combined into a single tileset, just like in Tiled

      buffers::TreeNode *protoNode = backgroundFolderRef->mutable_folder()->add_children();
      AddTiledResource(protoNode, resType, tileChild, resourceTypeIdCountMap, tsxPath);

      // set indirectly linked properties
      pugi::xml_node innerImgNode = tileChild.child("image");
      UpdateImageHelper(innerImgNode, protoNode);
      std::string tileId = tileChild.attribute("id").value();
      std::string protoNodeName = backgroundResName + "_" + tileId;
      protoNode->set_name(protoNodeName);

      // use_as_tileset should be false for stanalone tile converted to background
      protoNode->mutable_background()->set_use_as_tileset(false);
    }
  }
  else {
    buffers::TreeNode *protoNode = backgroundFolderRef->mutable_folder()->add_children();
    AddTiledResource(protoNode, resType, xmlNode, resourceTypeIdCountMap, tsxPath);

    // set indirectly linked properties
    UpdateImageHelper(imgNode, protoNode);
    protoNode->set_name(backgroundResName);
    // if single image is holding all the tiles then set use_as_tileset as true
    protoNode->mutable_background()->set_use_as_tileset(true);

    // we only store the backgrounds based on tilesets. This later on helps in loading Room.tiles from TMX files.
    // It distinguishes whether a background is based on tileset(if there is an entry present in map)
    // or not(if no such entry with given background name is in map)
    if(backgroundNamePtrMapRef != NULL)
      backgroundNamePtrMapRef->insert({backgroundResName, protoNode});
  }

  return true;
}

void TSXTilesetLoader::UpdateImageHelper(const pugi::xml_node &innerImgNode, buffers::TreeNode *protoNode) {
  if(!innerImgNode.empty()){
    std::string parentDirPath = tsxPath.parent_path().string()+"/";
    std::string imageSource = parentDirPath + innerImgNode.attribute("source").as_string();
    unsigned int imageWidth = innerImgNode.attribute("width").as_uint();
    unsigned int imageHeight = innerImgNode.attribute("height").as_uint();
    protoNode->mutable_background()->set_image(imageSource);
    protoNode->mutable_background()->set_width(imageWidth);
    protoNode->mutable_background()->set_height(imageHeight);
  }
}

std::unique_ptr<buffers::Project> TSXFileFormat::LoadProject(const fs::path& fPath) const {

  pugi::xml_document doc;
  if(!doc.load_file(fPath.c_str())) return nullptr;

  std::unique_ptr<buffers::Project> proj = std::make_unique<buffers::Project>();
  buffers::Game* game = proj->mutable_game();

  TSXTilesetLoader background_walker(game->mutable_root(), fPath);
  doc.traverse(background_walker);

  return proj;
}

} // namespace egm
