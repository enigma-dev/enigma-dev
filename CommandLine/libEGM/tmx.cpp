#include "tmx.h"
#include "tsx.h"
#include "General/zlib_util.h"
#include "General/tiled_util.h"
#include "strings_util.h"
#include "libbase64_util/libbase64_util.h"

#include "pugixml.hpp"

#include <google/protobuf/text_format.h>
#include <zstd.h>

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
  fs::path tmxPath;
  std::vector<buffers::TreeNode *> nodes;
  std::map<int, std::string> tilesetIdNameMap;
  std::unordered_map<std::string, buffers::TreeNode *> resourceFolderRefs;
  // helps in generating ids for repetable messages present in resources like Room.proto
  std::unordered_map<std::string, int> resourceTypeIdCountMap;
  // for fast access of backgrounds (based only on tileset) by name
  std::unordered_map<std::string, buffers::TreeNode *> tilesetBgNamePtrMap;
  // TODO: Remove this hack and use "resource name generator"
  int idx=0;

  // same as of saying LoadBackgrounds
  bool LoadTilesets(pugi::xml_node& mapNode, std::string resType) {
    pugi::xml_object_range<pugi::xml_named_node_iterator> tilesets = mapNode.children("tileset");
    for(pugi::xml_node &tileset : tilesets) {

      buffers::TreeNode *folderNode;
      if(resourceFolderRefs.find(resType) == resourceFolderRefs.end()) {
        folderNode = nodes.back()->mutable_folder()->add_children();
        folderNode->set_name(GetFolderName(resType));
        resourceFolderRefs[resType] = folderNode;
      }
      else
        folderNode = resourceFolderRefs[resType];

      if(folderNode == NULL) {
        errStream << "Folder with name \""<< resType <<"\" not found" << std::endl;
        return false;
      }

      fs::path tilesetSrcPath(tileset.attribute("source").as_string());
      int tilesetFirstGid = tileset.attribute("firstgid").as_int(-1);
      if(tilesetFirstGid == -1) {
        errStream << "Can't load tileset, firstgid is invalid." << std::endl;
        return false;
      }

      // empty source path refers to internal tileset
      if(tilesetSrcPath.empty()) {
        std::string parentDirPath = tmxPath.parent_path().string()+"/";
        std::string tsxPath = parentDirPath + tilesetSrcPath.string();
        std::string backgroundName = tilesetSrcPath.stem().string();

        TSXTilesetLoader background_walker(tsxPath, nodes, folderNode, backgroundName, &tilesetBgNamePtrMap);

        LoadInternalTileset(tileset, background_walker);

        // todo change the name!!!
        tilesetIdNameMap[tilesetFirstGid] = backgroundName;
      }
      else {

        std::string parentDirPath = tmxPath.parent_path().string()+"/";
        std::string tsxPath = parentDirPath + tilesetSrcPath.string();
        std::string backgroundName = tilesetSrcPath.stem().string();

        TSXTilesetLoader background_walker(tsxPath, nodes, folderNode, backgroundName, &tilesetBgNamePtrMap);

        bool ok = LoadExternalTileset(tsxPath, background_walker);
        if(!ok)
          return false;

        // todo change the name!!!
        tilesetIdNameMap[tilesetFirstGid] = backgroundName;
      }
    }

    return true;
  }

  void LoadInternalTileset(pugi::xml_node &tileset, TSXTilesetLoader &background_walker) {
    // NEED IMPROVEMENT: This call of traverse using xml node can cause lot of trouble, obvious one is, in case
    // of TMX file with multiple tilesets, where internal and external tilesets are mixed matched, we will load very
    // first internal tileset total_internal_tilesets-1 times.
    // CURRENT FIX: This is currently being handled by a hack present in tsx walker for_each loop
    tileset.parent().traverse(background_walker);
  }

  bool LoadExternalTileset(const std::string &tsxPath, TSXTilesetLoader &background_walker) {
    // load tsx xml
    pugi::xml_document tilesetDoc;
    if(!tilesetDoc.load_file(tsxPath.c_str())) {
      errStream << "Error: Could not found tsx file with path: " << tsxPath << std::endl;
      return false;
    }

    tilesetDoc.traverse(background_walker);

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
      folderNode->set_name(GetFolderName(resType));
    }
    else
      folderNode = resourceFolderRefs[resType];

    if(folderNode == NULL) {
      errStream << "Folder with name \"" << resType << "\" not found" << std::endl;
      return false;
    }

    buffers::TreeNode *resNode = folderNode->mutable_folder()->add_children();
    resNode->set_name(name);
    AddTiledResource(resNode, resType, mapNode, resourceTypeIdCountMap, tmxPath);

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

  std::string GetFolderName(const std::string& resType) {
    std::string fixName = resType + 's';  // add an 's' at the end
    fixName[0] -= 32;                     // make first character capital
    return fixName;
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
        PackTiledRes(objectChild, tile, resourceTypeIdCountMap, tmxPath);

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

      if(dataNode.empty()) {
        outStream << "Error loading tiles, layer node does not contain data node" << std::endl;
        return false;
      }

      std::string encoding = dataNode.attribute("encoding").as_string();

      std::string dataStr = dataNode.first_child().value();
      dataStr = StrTrim(dataStr);
      if(dataStr.empty()) {
        errStream << "Error while loading tiles, Layer Data contains empty string" << std::endl;
        return false;
      }

      if(encoding == "base64") {
        std::string compression = dataNode.attribute("compression").as_string();
        if(compression == "zlib") {
          bool ok = LoadBase64ZlibLayerData(layer, dataStr, resNode, tileWidth, tileHeight);
          if(!ok)
            return false;
        }
        else if(compression == "gzip") {

        }
        else if(compression == "zstd") {

        }
        else {
          // compression isnt specified for uncompressed format
          outStream << "Error loading tiles, unsupported compression type: " << compression << std::endl;
          return false;
        }
      }
      else if(encoding == "csv") {
        return LoadCsvLayerData(layer, dataStr, resNode, tileWidth, tileHeight);
      }
      else {
        outStream << "Error loading tiles, unsupported encoding type: " << encoding << std::endl;
        return false;
      }
    }

    return true;
  }

  bool LoadBase64ZlibLayerData(const pugi::xml_node &layerNode, const std::string &dataStr, buffers::TreeNode *resNode,
                          const int& tileWidth, const int& tileHeight) {
    // decode base64 data
    std::string decodedStr = base64_decode(dataStr);

    // decompress zlib compressed data
    std::istringstream istr(decodedStr);
    Decoder decoder(istr);
    size_t dataSize = decodedStr.size();
    std::unique_ptr<char[]> layerDataCharPtr = decoder.decompress(dataSize, 0);

    int layerWidth = layerNode.attribute("width").as_int();
    int layerHeight = layerNode.attribute("height").as_int();
    size_t expectedSize = layerWidth * layerHeight * 4;
    if(dataSize != expectedSize) {
      errStream << "Error loading tiles, zlib decompressed layer data corrupted." << std::endl;
      return false;
    }

    unsigned int tileIndex = 0;
    for (int y=0; y < layerHeight; ++y) {
      for (int x=0; x < layerWidth; ++x) {
        // loading 4 bytes of stream into uint following little endian order
        unsigned int globalTileId = static_cast<unsigned char>(layerDataCharPtr[tileIndex]) |
                                    static_cast<unsigned char>(layerDataCharPtr[tileIndex + 1]) << 8 |
                                    static_cast<unsigned char>(layerDataCharPtr[tileIndex + 2]) << 16 |
                                    static_cast<unsigned char>(layerDataCharPtr[tileIndex + 3]) << 24;

        tileIndex += 4;

        CreateTileFromGlobalId(globalTileId, resNode, tileWidth, tileHeight, x, y);
      }
    }

    return true;
  }

  bool LoadCsvLayerData(const pugi::xml_node &layerNode, const std::string &dataStr, buffers::TreeNode *resNode,
                        const int& tileWidth, const int& tileHeight) {
    std::vector<unsigned int> globalTileIds;
    std::istringstream istr(dataStr);
    std::string line;

    while(std::getline(istr, line)) {
      std::stringstream lineStream(line);
      std::string globalId;
      while(std::getline(lineStream, globalId, ',')) {
        // get the global tile ids into unsigned long, as stoui does not exist(it is guranteed to not overflow uint)
        unsigned long gidULong = std::stoul(globalId);
        if(gidULong > std::numeric_limits<unsigned int>::max()) {
          errStream << "Error laoding tiles, global id out of range." << std::endl;
          return false;
        }

        unsigned int gidUInt = static_cast<unsigned int>(gidULong);
        globalTileIds.push_back(gidUInt);
      }
    }

    int layerWidth = layerNode.attribute("width").as_int();
    int layerHeight = layerNode.attribute("height").as_int();
    size_t expectedSize = layerWidth * layerHeight;
    if(globalTileIds.size() != expectedSize) {
      errStream << "Error laoding tiles, csv layer data corrupted." << std::endl;
      return false;
    }

    for (int y=0; y < layerHeight; ++y) {
      for (int x=0; x < layerWidth; ++x) {
        unsigned int globalTileId = globalTileIds[y*layerWidth + x];
        CreateTileFromGlobalId(globalTileId, resNode, tileWidth, tileHeight, x, y);
      }
    }

    return true;
  }

  void CreateTileFromGlobalId(const unsigned int &globalTileId, buffers::TreeNode *resNode, const int& tileWidth,
                              const int& tileHeight, const int& currX, const int& currY) {
    bool hasHorizontalFlip=false, hasVerticalFlip=false;
    std::string tilesetName = "";
    int localTileId = GetLocalTileIdInfo(globalTileId, hasHorizontalFlip, hasVerticalFlip, tilesetName);

    std::string backgroundName = tilesetName;

    // get pointer to the background to be used in setting Room.tile properties
    buffers::resources::Background* bgPtr = tilesetBgNamePtrMap[backgroundName]->mutable_background();

    if(localTileId < 0 || bgPtr == NULL)
      return;

    buffers::resources::Room::Tile* tile = resNode->mutable_room()->add_tiles();

    tile->set_background_name(backgroundName);
    tile->set_name(backgroundName+"_"+std::to_string(idx++));

    tile->set_x(currX*tileWidth);
    tile->set_y(currY*tileHeight);

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
  if(!success) {
    std::cout << "Error occured while loading selected Tiled map." << std::endl;
    return NULL;
  }

  // temp code start
  std::string str;
  google::protobuf::TextFormat::PrintToString(proj->game(), &str);
  std::ofstream out("/home/kash/github/radialgm-stuff/textProtos/test1tmx.txt");
  out<<str<<std::endl;
  out.close();
  std::cout<<"END LOADING TMX"<<std::endl;
  // temp code end

  return proj;
}

} // namespace egm
