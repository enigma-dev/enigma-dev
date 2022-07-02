#include "tmx.h"
#include "tsx.h"

#include "pugixml.hpp"

#include <google/protobuf/text_format.h>

using CppType = google::protobuf::FieldDescriptor::CppType;

namespace egm {
namespace {

class TMXMapLoader : public pugi::xml_tree_walker {
public:
  TMXMapLoader(buffers::TreeNode *root, const fs::path &fPath) : tmxPath(fPath) {
    root->set_name("/");
    nodes.push_back(root);
  }

  bool Load(pugi::xml_document &xmlDoc) {
    pugi::xml_node mapNode = xmlDoc.child("map");

    // first load all tileset(s) in background(s)
    bool backgroundsOk = LoadTilesets(mapNode, "background");
    if(!backgroundsOk)
      return false;

    // now load map, backgrounds are available for setting room.tile and/or room.background
    bool roomOk = LoadMap(mapNode, "room");
    if(!roomOk)
      return false;

    return true;
  }

private:
  std::vector<buffers::TreeNode *> nodes;
  fs::path tmxPath;
  std::unordered_map<std::string, buffers::TreeNode *> resourceFolderRefs;
  // helps in generating ids for repetable messages present in resources like Room.proto
  std::unordered_map<std::string, int> resourceTypeIdCountMap;
  std::map<int, std::string> tilesetIdSourcePathMap;
  // TODO: Remove this hack and use "resource name generator"
  int idx=0;

  // same as of saying LoadBackgrounds
  bool LoadTilesets(pugi::xml_node& mapNode, std::string resType) {
    pugi::xml_object_range<pugi::xml_named_node_iterator> tilesets = mapNode.children("tileset");
    for(pugi::xml_node tileset : tilesets) {

      buffers::TreeNode *folderNode;
      if(resourceFolderRefs.find(resType) == resourceFolderRefs.end()) {
        folderNode = nodes.back()->mutable_folder()->add_children();
        std::string fixName = resType + 's'; // add an 's' at the end
        fixName[0] -= 32; // make first character capital
        folderNode->set_name(fixName);
        resourceFolderRefs[resType] = folderNode;
      }
      else
        folderNode = resourceFolderRefs[resType];

      if(folderNode == NULL) {
        std::cout << "Folder with name \""<< resType <<"\" not found" << std::endl;
        return false;
      }

      std::string tilesetSrc = tileset.attribute("source").value();
      int tilesetFirstGid = tileset.attribute("firstgid").as_int(-1);
      if(!tilesetSrc.empty() && tilesetFirstGid != -1) {
        std::string parentDirPath = tmxPath.parent_path().string()+"/";
        std::string tsxPath = parentDirPath + tilesetSrc;

        // load tsx xml
        pugi::xml_document tilesetDoc;
        if(!tilesetDoc.load_file(tsxPath.c_str())) {
          std::cout << "Could not found tsx file with path: " << tsxPath << std::endl;
          return false;
        }

        TSXTilesetLoader background_walker(tsxPath, nodes, folderNode);
        tilesetDoc.traverse(background_walker);

        tilesetIdSourcePathMap[tilesetFirstGid] = tilesetSrc;
      }
      else {
        std::cout << "Fatal error: Tileset source or firstgid is invalid." << std::endl;
        return false;
      }
    }

    return true;
  }

  // same as of saying LoadRoom
  bool LoadMap(pugi::xml_node& mapNode, std::string resType) {
    std::string name = mapNode.attribute("name").value();
    // TODO: Remove this hack using "resource name generator"
    if(name.empty())
      name = resType+std::to_string(idx++);

    buffers::TreeNode *folderNode;
    if(resourceFolderRefs.find(resType) == resourceFolderRefs.end()) {
      folderNode = nodes.back()->mutable_folder()->add_children();
      std::string fixName = resType + 's'; // add an 's' at the end
      fixName[0] -= 32; // make first character capital
      folderNode->set_name(fixName);
    }
    else
      folderNode = resourceFolderRefs[resType];

    if(folderNode == NULL) {
      std::cout << "Folder with name \""<< resType <<"\" not found" << std::endl;
      return false;
    }

    buffers::TreeNode *resNode = folderNode->mutable_folder()->add_children();
    resNode->set_name(name);
    AddResource(resNode, resType, mapNode);

    // correct width and height, convert from no. of tiles to no. of pixels
    resNode->mutable_room()->set_width(resNode->room().width() * resNode->room().hsnap());
    resNode->mutable_room()->set_height(resNode->room().height() * resNode->room().vsnap());

    bool room_tilesOk = LoadObjects(mapNode, resNode);
    if(!room_tilesOk){
      std::cout << "Something went wrong while loading Room.Tiles" << std::endl;
      return false;
    }

    return true;
  }

  // same as of saying LoadRoom.Tiles
  bool LoadObjects(pugi::xml_node& mapNode, buffers::TreeNode *resNode){
    pugi::xml_object_range<pugi::xml_named_node_iterator> objectGroups = mapNode.children("objectgroup");
    // iterate over all objectGroups
    for(pugi::xml_node objectGroupChild : objectGroups) {
      // iterate over all children, which is most probably objects, create new tiles out of them
      pugi::xml_object_range<pugi::xml_named_node_iterator> objects = objectGroupChild.children("object");
      for(pugi::xml_node objectChild : objects) {

        // let's update some not so directly corresponding fields
        unsigned int gid = objectChild.attribute("gid").as_uint();
        int localId = ConvertGlobalTileIdToLocal(gid);

        // find the tileset which this tile belongs to
        // using lower_bound( O(log(n)) ) to find first equal to or greater matching firstgid of a tileset
        // then decrementing the iterator by one to get the first tileset with firstgid less than or equal to given
        // local id of tile
        std::map<int, std::string>::iterator itr = tilesetIdSourcePathMap.lower_bound(localId);
        if(itr != tilesetIdSourcePathMap.begin()) {

          buffers::resources::Room::Tile* tile = resNode->mutable_room()->add_tiles();
          PackRes(objectChild, tile, "tiles/tile");

          itr--;
          localId = localId - itr->first;
          std::cout<<"localId: "<<localId<<" and source "<<itr->second<<std::endl;
          fs::path tilesetSourcePath(itr->second);
          std::string backgroundName = tilesetSourcePath.stem().string() + "_" + std::to_string(localId);
          tile->set_background_name(backgroundName);
          tile->set_xoffset(0);
          tile->set_yoffset(0);
          tile->set_name(backgroundName+"_"+std::to_string(idx++));
          tile->set_depth(0);
          tile->set_xscale(1);
          tile->set_yscale(1);
          tile->set_color(2147483647);
        }
        else {
          std::cout<<"localId: "<<localId<<std::endl;
        }
      }
    }

    return true;
  }

  virtual bool for_each(pugi::xml_node &xmlNode){
    return true;
  }

  void AddResource(buffers::TreeNode *protoNode, std::string resType, pugi::xml_node &xmlNode) {
    using buffers::TreeNode;
    using FactoryFunction = std::function<google::protobuf::Message *(TreeNode*)>;
    using FactoryMap = std::unordered_map<std::string, FactoryFunction>;

    static const FactoryMap factoryMap({
      { "sprite", &TreeNode::mutable_sprite },
      { "sound", &TreeNode::mutable_sound },
      { "background", &TreeNode::mutable_background },
      { "path", &TreeNode::mutable_path },
      { "script", &TreeNode::mutable_script },
      { "shader", &TreeNode::mutable_shader },
      { "font", &TreeNode::mutable_font },
      { "timeline", &TreeNode::mutable_timeline },
      { "object", &TreeNode::mutable_object },
      { "room", &TreeNode::mutable_room },
      { "datafile", &TreeNode::mutable_include },
      { "Config", &TreeNode::mutable_settings },
    });

    auto createFunc = factoryMap.find(resType);
    if (createFunc != factoryMap.end()) {
        auto *res = createFunc->second(protoNode);
        PackRes(xmlNode, res, resType);
        return;
    }
    protoNode->mutable_unknown();
    errStream << "Unsupported resource type: " << resType << " " << xmlNode.value() << std::endl;
  }

  void PackRes(pugi::xml_node &xmlNode, google::protobuf::Message *m, const std::string& resType) {
    const google::protobuf::Descriptor *desc = m->GetDescriptor();
    const google::protobuf::Reflection *refl = m->GetReflection();

    for (int i = 0; i < desc->field_count(); i++) {
      const google::protobuf::FieldDescriptor *field = desc->field(i);
      const google::protobuf::OneofDescriptor *oneof = field->containing_oneof();

      if (oneof && refl->HasOneof(*m, oneof)) continue;

      const google::protobuf::FieldOptions opts = field->options();

      // tmx_option_string is used to fetch correct attribute from current xml node
      std::string tsxPropertyName = opts.GetExtension(buffers::tmx);

      pugi::xml_attribute attr;

      attr = xmlNode.attribute(tsxPropertyName.c_str());

      if(field->name() == "id") {
        // not sure what Getextension returns in cases when id_start extension is absent
        int id = opts.GetExtension(buffers::id_start) + resourceTypeIdCountMap[m->GetTypeName()]++;
        outStream << "Setting " << field->name() << " (" << field->type_name() << ") as " << id << std::endl;
        refl->SetInt32(m, field, id);
      }
      else if(!attr.empty()) {
        switch (field->cpp_type()) {
          case CppType::CPPTYPE_MESSAGE: {
            // google::protobuf::Message *msg = refl->MutableMessage(m, field);
            // TODO: handle message case
            // PackRes(resMap, dir, ids, child, msg, depth + 1);
            break;
          }
          case CppType::CPPTYPE_INT32: {
            refl->SetInt32(m, field, attr.as_int());
            break;
          }
          case CppType::CPPTYPE_INT64: {
            refl->SetInt64(m, field, attr.as_int());
            break;
          }
          case CppType::CPPTYPE_UINT32: {
            refl->SetUInt32(m, field, attr.as_uint());
            break;
          }
          case CppType::CPPTYPE_UINT64: {
            refl->SetUInt64(m, field, attr.as_uint());
            break;
          }
          case CppType::CPPTYPE_DOUBLE: {
            refl->SetDouble(m, field, attr.as_double());
            break;
          }
          case CppType::CPPTYPE_FLOAT: {
            refl->SetFloat(m, field, attr.as_float());
            break;
          }
          case CppType::CPPTYPE_BOOL: {
            refl->SetBool(m, field, (attr.as_int() != 0));
            break;
          }
          case CppType::CPPTYPE_ENUM: {
            refl->SetEnum(m, field, field->enum_type()->FindValueByNumber(attr.as_int()));
            break;
          }
          case CppType::CPPTYPE_STRING: {
            const bool isFilePath = opts.GetExtension(buffers::file_path);
            std::string value;
            if(isFilePath) {
              std::string parentDirPath = tmxPath.parent_path().string()+"/";
              value = parentDirPath + attr.as_string();
            }
            else {
              value = attr.as_string();
            }

            refl->SetString(m, field, value);
            break;
          }
        }
      }
    }
  }

  int ConvertGlobalTileIdToLocal(const unsigned int& globalTileId) {
    const unsigned int FLIPPED_HORIZONTALLY_FLAG =  0X80000000; // tile is horizontally flipped or not
    const unsigned int FLIPPED_VERTICALLY_FLAG =    0X40000000; // tile is vertically flipped or not
    const unsigned int FLIPPED_DIAGONALLY_FLAG =    0X20000000; // indicates diogonal flip(bot left <-> top right) of tile in ortho/iso maps or
                                                                // 60 deg clock rot in case of hexagonal maps
    const unsigned int ROTATED_HEXAGONAL_120_FLAG = 0X10000000; // ignored in ortho or iso maps, indicates 120 deg clock rot in hexagonal maps

    unsigned int tileId = globalTileId;
    // uncomment when required
    /*bool flippedHorizontally = tileId & FLIPPED_HORIZONTALLY_FLAG;
    bool flippedVertically = tileId & FLIPPED_VERTICALLY_FLAG;
    bool flippedDiagonally = tileId & FLIPPED_DIAGONALLY_FLAG;
    bool rotatedHex120 = tileId & ROTATED_HEXAGONAL_120_FLAG;*/

    tileId &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG | ROTATED_HEXAGONAL_120_FLAG);

    return tileId;
  }
};

} // anonymous namespace

std::unique_ptr<buffers::Project> TMXFileFormat::LoadProject(const fs::path& fPath) const {
  pugi::xml_document doc;
  if(!doc.load_file(fPath.c_str())) return nullptr;

  std::unique_ptr<buffers::Project> proj = std::make_unique<buffers::Project>();
  buffers::Game* game = proj->mutable_game();

  TMXMapLoader mapWalker(game->mutable_root(), fPath);

  bool success = mapWalker.Load(doc);
  if(!success){
    std::cout<<"Error occured while loading selected Tiled map"<<std::endl;
    return NULL;
  }

  /*std::string str;
  google::protobuf::TextFormat::PrintToString(proj->game(), &str);
  std::ofstream out("/home/kash/github/radialgm-stuff/textProtos/test1tmx.txt");
  out<<str<<std::endl;
  out.close();*/

  std::cout<<"END LOADING TMX"<<std::endl;
  return proj;
}

} // namespace egm
