#include "tsx.h"
#include "action.h"
#include "strings_util.h"

#include "pugixml.hpp"

#include <functional>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace buffers::resources;

namespace egm{

namespace {

class TSXTilesetLoader : public pugi::xml_tree_walker {
public:
  TSXTilesetLoader(buffers::resources::Background *bg, const fs::path& fPath) {
    background = bg;
    background->set_use_as_tileset(true);
    parentDirPath = fPath.parent_path().string()+"/";
  }

  buffers::resources::Background *background;
  std::string parentDirPath;

private:
  virtual bool for_each(pugi::xml_node& node) {
    for(pugi::xml_attribute attr : node.attributes()) {
      if(strcmp(node.name(),"tileset") == 0){
        if(strcmp(attr.name(),"tilewidth") == 0)
          background->set_tile_width(std::stoi(attr.value()));
        else if(strcmp(attr.name(),"tileheight") == 0)
          background->set_tile_height(std::stoi(attr.value()));
        else if(strcmp(attr.name(),"spacing") == 0){
          background->set_horizontal_spacing(std::stoi(attr.value()));
          background->set_vertical_spacing(std::stoi(attr.value()));
        }
        else if(strcmp(attr.name(),"margin") == 0){
          background->set_horizontal_offset(std::stoi(attr.value()));
          background->set_vertical_offset(std::stoi(attr.value()));
        }
      }
      else if(strcmp(node.name(),"image") == 0){
        if(strcmp(attr.name(),"source") == 0){
          std::string absoluteImagePath = parentDirPath+attr.value();
          background->set_image(absoluteImagePath);
        }
        else if(strcmp(attr.name(),"width") == 0)
          background->set_width(std::stoi(attr.value()));
        else if(strcmp(attr.name(),"height") == 0)
          background->set_height(std::stoi(attr.value()));
      }
    }

    return true;
  }
};

} // anonymous namespace

std::unique_ptr<buffers::Project> TSXFileFormat::LoadProject(const fs::path& fPath) const {

  pugi::xml_document doc;
  if(!doc.load_file(fPath.c_str())) return nullptr;

  std::unique_ptr<buffers::Project> proj = std::make_unique<buffers::Project>();
  buffers::Game* game = proj->mutable_game();
  buffers::TreeNode* root = game->mutable_root();
  root->set_name("tsxRootTest");

  buffers::TreeNode* background = root->mutable_folder()->add_children();
  background->set_name("tsxBackgroundTest");

  TSXTilesetLoader background_walker(background->mutable_background(), fPath);
  doc.traverse(background_walker);

  return proj;
}

} // namespace egm
