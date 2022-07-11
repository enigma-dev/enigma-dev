#include "tmx.h"
#include "tsx.h"
#include "General/zlib_util.h"
#include "strings_util.h"
#include "libbase64_util/libbase64_util.h"

#include "pugixml.hpp"

#include <google/protobuf/text_format.h>

#include <cmath>

using CppType = google::protobuf::FieldDescriptor::CppType;

namespace egm {
namespace {

class TMXMapLoader {
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
  std::map<int, std::string> tilesetIdNameMap;
  // for fast access of backgrounds (based only on tileset) by name
  std::unordered_map<std::string, buffers::TreeNode *> tilesetBgNamePtrMap;
  // TODO: Remove this hack and use "resource name generator"
  int idx=0;

  // same as of saying LoadBackgrounds
  bool LoadTilesets(pugi::xml_node& mapNode, std::string resType) {
    pugi::xml_object_range<pugi::xml_named_node_iterator> tilesets = mapNode.children("tileset");
    for(const pugi::xml_node &tileset : tilesets) {

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
        errStream << "Folder with name \""<< resType <<"\" not found" << std::endl;
        return false;
      }

      fs::path tilesetSrcPath(tileset.attribute("source").value());
      int tilesetFirstGid = tileset.attribute("firstgid").as_int(-1);
      if(!tilesetSrcPath.empty() && tilesetFirstGid != -1) {
        std::string parentDirPath = tmxPath.parent_path().string()+"/";
        std::string tsxPath = parentDirPath + tilesetSrcPath.string();

        // load tsx xml
        pugi::xml_document tilesetDoc;
        if(!tilesetDoc.load_file(tsxPath.c_str())) {
          errStream << "Could not found tsx file with path: " << tsxPath << std::endl;
          return false;
        }

        std::string backgroundName = tilesetSrcPath.stem().string();
        TSXTilesetLoader background_walker(tsxPath, nodes, folderNode, backgroundName, &tilesetBgNamePtrMap);
        tilesetDoc.traverse(background_walker);

        // todo change the name!!!
        tilesetIdNameMap[tilesetFirstGid] = backgroundName;
      }
      else {
        errStream << "Fatal error: Tileset source or firstgid is invalid." << std::endl;
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
      errStream << "Folder with name \""<< resType <<"\" not found" << std::endl;
      return false;
    }

    buffers::TreeNode *resNode = folderNode->mutable_folder()->add_children();
    resNode->set_name(name);
    AddResource(resNode, resType, mapNode);

    // correct width and height, convert from no. of tiles to no. of pixels
    resNode->mutable_room()->set_width(resNode->room().width() * resNode->room().hsnap());
    resNode->mutable_room()->set_height(resNode->room().height() * resNode->room().vsnap());

    bool room_tilesFromObjectsOk = LoadObjects(mapNode, resNode);
    if(!room_tilesFromObjectsOk) {
      errStream << "Something went wrong while loading Room.Tiles from Objects" << std::endl;
      return false;
    }

    bool room_tiledFromLayerDataOk = LoadLayerData(mapNode, resNode, resNode->room().hsnap(), resNode->room().vsnap());
    if(!room_tiledFromLayerDataOk) {
      errStream << "Something went wrong while laoding Room.Tiles from Layer Data" << std::endl;
      return false;
    }

    return true;
  }

  // same as of saying LoadRoom.Tiles
  bool LoadObjects(pugi::xml_node& mapNode, buffers::TreeNode *resNode) {
    pugi::xml_object_range<pugi::xml_named_node_iterator> objectGroups = mapNode.children("objectgroup");
    // iterate over all objectGroups
    for(const pugi::xml_node &objectGroupChild : objectGroups) {
      bool hasOpacity = false;
      pugi::xml_attribute opacityAttr = objectGroupChild.attribute("opacity");
      if(!opacityAttr.empty())
        hasOpacity = true;

      // iterate over all children, which is most probably objects, create new tiles out of them
      pugi::xml_object_range<pugi::xml_named_node_iterator> objects = objectGroupChild.children("object");
      for(const pugi::xml_node &objectChild : objects) {
        unsigned int globalTileId = objectChild.attribute("gid").as_uint();

        bool hasHorizontalFlip=false, hasVerticalFlip=false;
        std::string tilesetName = "";
        int localTileId = GetLocalTileIdInfo(globalTileId, hasHorizontalFlip, hasVerticalFlip, tilesetName);

        if(localTileId == 0){
          errStream << "localId attribute not present(templates are not yet supported) or its value is zero" << std::endl;
          continue;
        }

        std::string backgroundName = tilesetName + "_" + std::to_string(localTileId);

        buffers::resources::Room::Tile* tile = resNode->mutable_room()->add_tiles();

        // if xmlNode is empty then we are dealing with compressed tiled data case
        PackRes(objectChild, tile, "tiles/tile");

        tile->set_background_name(backgroundName);
        tile->set_name(backgroundName+"_"+std::to_string(idx++));

        // convert tiled origin from bottom-left to top-left
        tile->set_y(tile->y()-tile->height());

        tile->set_xoffset(0);
        tile->set_yoffset(0);
        tile->set_depth(0);

        if(hasHorizontalFlip)
          tile->set_xscale(-1.0);
        else
          tile->set_xscale(1.0);

        if(hasVerticalFlip)
          tile->set_yscale(-1.0);
        else
          tile->set_yscale(1.0);

        if(tile && hasOpacity)
          tile->set_alpha(opacityAttr.as_double());
      }
    }

    return true;
  }

  // same as of saying LoadRoom.Tiles
  bool LoadLayerData(pugi::xml_node& mapNode, buffers::TreeNode *resNode, int tileWidth, int tileHeight) {
    pugi::xml_object_range<pugi::xml_named_node_iterator> layers = mapNode.children("layer");
    for(const pugi::xml_node &layer : layers) {
      const pugi::xml_node &dataNode = layer.child("data");
      std::string dataStr = dataNode.first_child().value();

      dataStr = StrTrim(dataStr);

      if(dataStr.empty()) {
        errStream << "Error while loading tiles, Layer Data contains empty string" << std::endl;
        return false;
      }

      // decode base64 data
      std::string decodedStr = base64_decode(dataStr);

      // decompress zlib compressed data
      std::istringstream istr(decodedStr);
      Decoder decoder(istr);
      size_t dataSize = decodedStr.size();
      std::unique_ptr<char[]> layerDataCharPtr = decoder.decompress(dataSize, 0);

      int layerWidth = layer.attribute("width").as_int();
      int layerHeight = layer.attribute("height").as_int();
      size_t expectedSize = layerWidth * layerHeight * 4;
      if(dataSize != expectedSize) {
        errStream << "I/O error, not able to load complete layer data" << std::endl;
        return false;
      }

      unsigned int tileIndex = 0;
      for (int y=0; y < layerHeight; ++y) {
        for (int x=0; x < layerWidth; ++x) {
          unsigned int globalTileId = layerDataCharPtr[tileIndex] |
                                      layerDataCharPtr[tileIndex + 1] << 8 |
                                      layerDataCharPtr[tileIndex + 2] << 16 |
                                      layerDataCharPtr[tileIndex + 3] << 24;
          tileIndex += 4;

          bool hasHorizontalFlip=false, hasVerticalFlip=false;
          std::string tilesetName = "";
          int localTileId = GetLocalTileIdInfo(globalTileId, hasHorizontalFlip, hasVerticalFlip, tilesetName);

          std::string backgroundName = tilesetName;

          // get pointer to the background to be used in setting Room.tile properties
          buffers::resources::Background* bgPtr = tilesetBgNamePtrMap[backgroundName]->mutable_background();

          if(bgPtr == NULL)
            continue;

          buffers::resources::Room::Tile* tile = resNode->mutable_room()->add_tiles();

          tile->set_background_name(backgroundName);
          tile->set_name(backgroundName+"_"+std::to_string(idx++));

          tile->set_x(x*tileWidth);
          tile->set_y(y*tileHeight);

          tile->set_width(tileWidth);
          tile->set_height(tileHeight);

          int numColumns = bgPtr->columns();
          int xOffset = localTileId % numColumns;
          xOffset = xOffset * (tileWidth + bgPtr->horizontal_spacing()) + bgPtr->horizontal_offset();

          int yOffset = floor(localTileId / (1.0f * numColumns));
          yOffset = yOffset * (tileHeight + bgPtr->vertical_spacing()) + bgPtr->vertical_offset();

          tile->set_xoffset(xOffset);
          tile->set_yoffset(yOffset);

          if(hasHorizontalFlip)
            tile->set_xscale(-1.0);
          else
            tile->set_xscale(1.0);

          if(hasVerticalFlip)
            tile->set_yscale(-1.0);
          else
            tile->set_yscale(1.0);

          // TODO: Find a good implementation to set depth and id
          tile->set_depth(0);
          tile->set_id(10000001 + resourceTypeIdCountMap["buffers.resources.Room.Tile"]++);
        }
      }
    }

    return true;
  }

  int GetLocalTileIdInfo(const unsigned int &globalTileId, bool &hasHorizontalFlip, bool &hasVerticalFlip,
                         std::string& tilesetName) {
    int localId = ConvertGlobalTileIdToLocal(globalTileId, hasHorizontalFlip, hasVerticalFlip);

    // find the tileset which this tile belongs to
    // We use lower_bound( O(log(n)) ) to find iterator to first equal to or greater matching firstgid of a tileset,
    // then decrement the iterator by one to get the first tileset with firstgid less than or equal to given
    // local id of tile.
    // The purpose of storing tileset id and name in a map is to reduce search time. Note: Binary search
    // could also be used with a vector of pairs
    std::map<int, std::string>::iterator itr = tilesetIdNameMap.lower_bound(localId);
    if(itr != tilesetIdNameMap.begin())
      itr--;
    localId = localId - itr->first;
    tilesetName = itr->second;

    return localId;
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

  void PackRes(const pugi::xml_node &xmlNode, google::protobuf::Message *m, const std::string& resType) {
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

  int ConvertGlobalTileIdToLocal(const unsigned int& globalTileId, bool& hasHorizontalFlip, bool& hasVerticalFlip) {
    const unsigned int FLIPPED_HORIZONTALLY_FLAG =  0X80000000; // tile is horizontally flipped or not

    const unsigned int FLIPPED_VERTICALLY_FLAG =    0X40000000; // tile is vertically flipped or not

    const unsigned int FLIPPED_DIAGONALLY_FLAG =    0X20000000; // indicates diogonal flip (both left <-> top right)
                                                                // of tile in ortho/iso maps or
                                                                // 60 deg clock rot in case of hexagonal maps

    const unsigned int ROTATED_HEXAGONAL_120_FLAG = 0X10000000; // ignored in ortho or iso maps, indicates
                                                                // 120 deg clock rot in hexagonal maps

    unsigned int tileId = globalTileId;

    hasHorizontalFlip = tileId & FLIPPED_HORIZONTALLY_FLAG;
    hasVerticalFlip = tileId & FLIPPED_VERTICALLY_FLAG;
    // TODO: Figure out implementation for flippedDiagonally and rotateHex120
    // bool flippedDiagonally = tileId & FLIPPED_DIAGONALLY_FLAG;
    // bool rotatedHex120 = tileId & ROTATED_HEXAGONAL_120_FLAG;

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

  std::string str;
  google::protobuf::TextFormat::PrintToString(proj->game(), &str);
  std::ofstream out("/home/kash/github/radialgm-stuff/textProtos/test1tmx.txt");
  out<<str<<std::endl;
  out.close();

  std::cout<<"END LOADING TMX"<<std::endl;
  return proj;
}

} // namespace egm
