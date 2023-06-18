#include "tmx.h"
#include "General/zlib_util.h"
#include "General/tiled_util.h"
#include "strings_util.h"
#include "libbase64_util/libbase64_util.h"

#include <google/protobuf/text_format.h>
#include <zstd.h>

#include <cmath>

using CppType = google::protobuf::FieldDescriptor::CppType;

namespace egm {

std::unique_ptr<buffers::Project> TMXFileFormat::LoadProject(const fs::path& fPath) const {
  pugi::xml_document doc;
  if(!doc.load_file(fPath.c_str())) return nullptr;

  std::unique_ptr<buffers::Project> proj = std::make_unique<buffers::Project>();
  buffers::Game* game = proj->mutable_game();
  buffers::TreeNode* root = game->mutable_root();

  TMXMapLoader mapWalker(root, fPath);

  bool success = mapWalker.Load(doc);
  if(!success) {
    outStream << "Error occured while loading selected Tiled map." << std::endl;
    return NULL;
  }

  return proj;
}

TMXMapLoader::TMXMapLoader(buffers::TreeNode *root, const fs::path &fPath) : tmxPath(fPath) {
  root->set_name("/");
  nodes.push_back(root);
  roomOrientation = RoomOrientation::unknown;

  hexMapInfo.staggerAxis = StaggerAxis::unknown;
  hexMapInfo.staggerIndex = StaggerIndex::unknown;
  hexMapInfo.hexSideLength = 0;

  staggeredIsoMapInfo.staggerAxis = StaggerAxis::unknown;
  staggeredIsoMapInfo.staggerIndex = StaggerIndex::unknown;
}

bool TMXMapLoader::Load(pugi::xml_document &xmlDoc) {
  pugi::xml_node mapNode = xmlDoc.child("map");

  // Init EGMRoom proto node
  std::string resType = "room";
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
    errStream << "Folder with name \"" << resType << "\" not found." << std::endl;
    return false;
  }
  buffers::TreeNode *resNode = folderNode->mutable_folder()->add_children();
  resNode->set_name(name);
  AddTiledResource(resNode, resType, mapNode, resourceTypeIdCountMap, tmxPath);

  // first load all tileset(s) in EGMRoom.Tilesets as well background(s)
  bool backgroundsOk = LoadTilesets(mapNode, resNode);
  if(!backgroundsOk)
    return false;

  // now load map, backgrounds are available for setting room.tile and/or room.background
  bool roomOk = LoadMap(mapNode, resNode);
  if(!roomOk)
    return false;

  return true;
}

bool TMXMapLoader::LoadTilesets(pugi::xml_node& mapNode, buffers::TreeNode *resNode) {
  std::string resType = "background";

  pugi::xml_object_range<pugi::xml_named_node_iterator> tilesets = mapNode.children("tileset");
  for(pugi::xml_node &tileset : tilesets) {
    // Load tileset into EGMRoom.tilesets
    buffers::resources::EGMRoom::Tileset* tilesetProto = resNode->mutable_egm_room()->add_tilesets();
    PackTiledRes(tileset, tilesetProto, resourceTypeIdCountMap, tmxPath);
    // load internal
    if(tilesetProto->has_source()) {
      std::string absolutePath = tmxPath.parent_path().string() + "/" + tilesetProto->mutable_source()->data();
      tilesetProto->set_source(absolutePath);
    }

    buffers::TreeNode *folderNode;
    if(resourceFolderRefs.find(resType) == resourceFolderRefs.end()) {
      folderNode = nodes.back()->mutable_folder()->add_children();
      folderNode->set_name(GetFolderName(resType));
      resourceFolderRefs[resType] = folderNode;
    }
    else
      folderNode = resourceFolderRefs[resType];

    if(folderNode == NULL) {
      errStream << "Folder with name \""<< resType <<"\" not found." << std::endl;
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
      std::string tsxPath = parentDirPath;
      std::string backgroundName = tileset.attribute("name").as_string();

      TSXTilesetLoader background_walker(tsxPath, nodes, folderNode, backgroundName, &tilesetBgNamePtrMap);

      LoadInternalTileset(tileset, background_walker);

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

      tilesetIdNameMap[tilesetFirstGid] = backgroundName;
    }
  }

  return true;
}

void TMXMapLoader::LoadInternalTileset(pugi::xml_node &tileset, TSXTilesetLoader &background_walker) {
  // NEED IMPROVEMENT: This call of traverse using xml node can cause lot of trouble, obvious one is, in case
  // of TMX file with multiple tilesets, where internal and external tilesets are mixed matched, we will load very
  // first internal tileset total_internal_tilesets-1 times.
  // CURRENT FIX: This is currently being handled by a hack to return false(and end iteration) from tsx walker for_each loop
  tileset.parent().traverse(background_walker);
}

bool TMXMapLoader::LoadExternalTileset(const std::string &tsxPath, TSXTilesetLoader &background_walker) {
  // load tsx xml
  pugi::xml_document tilesetDoc;
  if(!tilesetDoc.load_file(tsxPath.c_str())) {
    errStream << "Error: Could not found tsx file with path: " << tsxPath << std::endl;
    return false;
  }

  tilesetDoc.traverse(background_walker);

  return true;
}

bool TMXMapLoader::LoadMap(pugi::xml_node& mapNode, buffers::TreeNode* resNode) {
  /*std::string resType = "room";

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
    errStream << "Folder with name \"" << resType << "\" not found." << std::endl;
    return false;
  }

  buffers::TreeNode *resNode = folderNode->mutable_folder()->add_children();
  resNode->set_name(name);
  AddTiledResource(resNode, resType, mapNode, resourceTypeIdCountMap, tmxPath);*/

  // correct width and height, convert from no. of tiles to no. of pixels
  buffers::resources::EGMRoom* rm = resNode->mutable_egm_room();
  unsigned int nHoriTiles = rm->width();
  unsigned int nVertTiles = rm->height();
  unsigned int tileWidthPixels = rm->tilewidth();
  unsigned int tileHeightPixels = rm->tileheight();

  std::string orientation = "";
  if(rm->has_orientation())
    orientation = rm->mutable_orientation()->data();

  if(orientation == "orthogonal" || orientation == "isometric") {
    if(orientation == "orthogonal")
      roomOrientation = RoomOrientation::orthogonal;
    else if(orientation == "isometric")
      roomOrientation = RoomOrientation::isometric;

    rm->set_width(nHoriTiles * tileWidthPixels);
    rm->set_height(nVertTiles * tileHeightPixels);
  }
  else if(orientation == "hexagonal") {
    roomOrientation = RoomOrientation::hexagonal;

    hexMapInfo.hexSideLength = rm->hexsidelength();

    if(rm->staggeraxis() == "x") {
      hexMapInfo.staggerAxis = StaggerAxis::x;

      rm->set_width((3 * nHoriTiles + 1) * hexMapInfo.hexSideLength / 2);
      rm->set_height((nVertTiles * tileHeightPixels) + hexMapInfo.hexSideLength);
    }
    else if(rm->staggeraxis() == "y") {
      hexMapInfo.staggerAxis = StaggerAxis::y;

      rm->set_width((nHoriTiles * tileWidthPixels) + hexMapInfo.hexSideLength);
      rm->set_height((3 * nVertTiles + 1) * hexMapInfo.hexSideLength / 2);
    }

    if(rm->staggerindex() == "even")
      hexMapInfo.staggerIndex = StaggerIndex::even;
    else if(rm->staggerindex() == "odd")
      hexMapInfo.staggerIndex = StaggerIndex::odd;
  }
  else if(orientation == "staggered") {
    roomOrientation = RoomOrientation::staggered;

    if(rm->staggeraxis() == "x") {
      staggeredIsoMapInfo.staggerAxis = StaggerAxis::x;

      rm->set_width(nHoriTiles * tileWidthPixels / 2);
      rm->set_height(nVertTiles * tileHeightPixels);
    }
    else if(rm->staggeraxis() == "y") {
      staggeredIsoMapInfo.staggerAxis = StaggerAxis::y;

      rm->set_width(nHoriTiles * tileWidthPixels);
      rm->set_height(nVertTiles * tileHeightPixels / 2);
    }

    if(rm->staggerindex() == "even")
      staggeredIsoMapInfo.staggerIndex = StaggerIndex::even;
    if(rm->staggerindex() == "odd")
      staggeredIsoMapInfo.staggerIndex = StaggerIndex::odd;
  }
  else {
    errStream << "Error loading map, unsupported map orientation: " << rm->orientation() << std::endl;
    return false;
  }

  // Tiled map stores backgroundcolor in hex format, so we convert it to int32 for Room color
  string hexColorStr = mapNode.attribute("backgroundcolor").as_string();
  if(!hexColorStr.empty()) {
    if(hexColorStr[0] == '#')
      hexColorStr = hexColorStr.substr(1,6);
    int hexColorInt = 0;
    std::stringstream strStream;
    strStream << std::hex << hexColorStr;
    strStream >> hexColorInt;
    rm->set_color(hexColorInt);
    rm->set_show_color(true);
  }

  bool room_tilesFromObjectsOk = LoadObjects(mapNode, resNode);
  if(!room_tilesFromObjectsOk) {
    errStream << "Something went wrong while loading Room.Tiles from Objects." << std::endl;
    return false;
  }

  bool room_tiledFromLayerDataOk = LoadLayerData(mapNode, resNode, rm->tilewidth(), rm->tileheight());
  if(!room_tiledFromLayerDataOk) {
    errStream << "Something went wrong while laoding Room.Tiles from Layer Data." << std::endl;
    return false;
  }

  return true;
}

std::string TMXMapLoader::GetFolderName(const std::string& resType) {
  std::string fixName = resType + 's';  // add an 's' at the end
  fixName[0] -= 32;                     // make first character capital
  return fixName;
}

bool TMXMapLoader::LoadObjects(pugi::xml_node& mapNode, buffers::TreeNode *resNode) {
  pugi::xml_object_range<pugi::xml_named_node_iterator> objectGroups = mapNode.children("objectgroup");
  // iterate over all objectGroups
  for(const pugi::xml_node &objectGroupChild : objectGroups) {

    buffers::resources::EGMRoom::ObjectGroup* objectGroup = resNode->mutable_egm_room()->add_objectgroups();
    PackTiledRes(objectGroupChild, objectGroup, resourceTypeIdCountMap, tmxPath);

    pugi::xml_object_range<pugi::xml_named_node_iterator> objects = objectGroupChild.children("object");
    for(const pugi::xml_node &objectChild : objects) {
      buffers::resources::EGMRoom::ObjectGroup::Object* object = objectGroup->add_objects();
      PackTiledRes(objectChild, object, resourceTypeIdCountMap, tmxPath);

      // get-set backgroundname (IMPROVEMENT: Try using tileset id directly)
      unsigned int globalTileId = object->gid();
      bool hasHorizontalFlip=false, hasVerticalFlip=false, flippedDiagonally=false, rotatedHex120=false;
      std::string tilesetName = "";
      int localTileId = GetLocalTileIdInfo(tilesetName, globalTileId, hasHorizontalFlip, hasVerticalFlip,
                                           flippedDiagonally, rotatedHex120);
      if(localTileId == 0){
        errStream << "localId attribute not present(templates are not yet supported) or its value is zero." << std::endl;
        continue;
      }
      std::string backgroundName = tilesetName + "_" + std::to_string(localTileId);
      object->set_background_name(backgroundName);

      /*object->set_has_horizontal_flip(hasHorizontalFlip);
      object->set_has_vertical_flip(hasVerticalFlip);
      object->set_flipped_diagonally(flippedDiagonally);
      object->set_rotated_hex120(rotatedHex120);*/

      if(hasHorizontalFlip)
        object->set_xscale(-1.0);
      else
        object->set_xscale(1.0);

      if(hasVerticalFlip)
        object->set_yscale(-1.0);
      else
        object->set_yscale(1.0);

      object->set_y(object->y() - object->height());

      // set properties
      pugi::xml_node propertiesTag = objectChild.child("properties");
      if(!propertiesTag.empty()) {
        buffers::resources::EGMRoom::ObjectGroup::Object::Properties* properties = object->mutable_propertiestag();
        pugi::xml_object_range<pugi::xml_named_node_iterator> propertiesTagItr = propertiesTag.children("property");

        for(const pugi::xml_node &propertyTag : propertiesTagItr) {
          buffers::resources::EGMRoom::ObjectGroup::Object::Properties::Property* property = properties->add_propertytags();
          PackTiledRes(propertyTag, property, resourceTypeIdCountMap, tmxPath);

        }
      }
    }
  }

  return true;
}

bool TMXMapLoader::LoadLayerData(pugi::xml_node& mapNode, buffers::TreeNode *resNode, int tileWidth, int tileHeight) {
  pugi::xml_object_range<pugi::xml_named_node_iterator> layers = mapNode.children("layer");
  for(const pugi::xml_node &layer : layers) {
    buffers::resources::EGMRoom::TileLayer* layerProto = resNode->mutable_egm_room()->add_tilelayers();
    PackTiledRes(layer, layerProto, resourceTypeIdCountMap, tmxPath);

    const pugi::xml_node &dataNode = layer.child("data");

    if(dataNode.empty()) {
      outStream << "Error loading tiles, layer node does not contain data node." << std::endl;
      return false;
    }

    buffers::resources::EGMRoom::TileLayer::Data *layerDataProto = layerProto->mutable_data();
    PackTiledRes(dataNode, layerDataProto, resourceTypeIdCountMap, tmxPath);

    std::string encoding = layerDataProto->encoding();
    std::string compression = layerDataProto->compression();

    // In infinite Tiled maps, data string is stored in chunk nodes
    pugi::xml_object_range<pugi::xml_named_node_iterator> chunks = dataNode.children("chunk");

    // TODO: Add support to load data present in <tile> nodes instead as a string node

    // data string can be found in child of <chunk> node(s) (for infinite Tiled maps) or directly in child node
    int chunkIdx = 0;
    for(const pugi::xml_node &chunk : chunks) {
      buffers::resources::EGMRoom::TileLayer::Data::Chunk *layerDataChunkProto = layerDataProto->add_chunks();
      PackTiledRes(chunk, layerDataChunkProto, resourceTypeIdCountMap, tmxPath);

      int chunkXIdx = layerDataChunkProto->x();
      int chunkYIdx = layerDataChunkProto->y();
      int chunkWidth = layerDataChunkProto->width();
      int chunkHeight = layerDataChunkProto->height();

      std::string chunkDataStr = chunk.first_child().value();
      chunkDataStr = StrTrim(chunkDataStr);

      bool ok = LoadLayerDataHelper(chunkDataStr, chunkWidth, chunkHeight, encoding, compression, layerProto, tileWidth,
                                    tileHeight, chunkXIdx, chunkYIdx, chunkIdx);
      chunkIdx++;

      if(!ok) {
        errStream << "Error while loading tiles from Layer Data Chunk." << std::endl;
        return false;
      }
    }

    // if no chunks found then data/tiles are directly present as first child
    if(chunkIdx == 0) {
      int layerWidth = layerProto->width();
      int layerHeight = layerProto->height();

      std::string dataStr = dataNode.first_child().value();
      dataStr = StrTrim(dataStr);

      bool ok = LoadLayerDataHelper(dataStr, layerWidth, layerHeight, encoding, compression, layerProto, tileWidth,
                                    tileHeight);
      if(!ok) {
        errStream << "Error while loading tiles from Layer Data." << std::endl;
        return false;
      }
    }
  }

  return true;
}

bool TMXMapLoader::LoadLayerDataHelper(const std::string &dataStr, const int layerWidth, const int layerHeight,
                                       const std::string &encoding, const std::string &compression,
                                       buffers::resources::EGMRoom::TileLayer *tileLayer, const int tileWidth,
                                       const int tileHeight, const int chunkXIdx, const int chunkYIdx, const int chunkIdx) {
  if(!dataStr.empty()) {
    if(encoding == "base64") {
      // decode base64 data
      std::string decodedStr = base64_decode(dataStr);

      size_t expectedSize = layerWidth * layerHeight * 4;

      if(compression == "zlib" || compression == "gzip") {
        bool ok = LoadBase64ZlibLayerData(decodedStr, expectedSize, tileLayer, tileWidth, tileHeight, layerWidth,
                                          layerHeight, chunkXIdx, chunkYIdx, chunkIdx);
        if(!ok)
          return false;
      }
      else if(compression == "zstd") {
        bool ok = LoadBase64ZstdLayerData(decodedStr, expectedSize, tileLayer, tileWidth, tileHeight, layerWidth,
                                          layerHeight, chunkXIdx, chunkYIdx, chunkIdx);
        if(!ok)
          return false;
      }
      else {
        // compression isnt specified for uncompressed format
        bool ok = LoadBase64UncompressedLayerData(decodedStr, expectedSize, tileLayer, tileWidth, tileHeight, layerWidth,
                                          layerHeight, chunkXIdx, chunkYIdx, chunkIdx);
        if(!ok)
          return false;
      }
    }
    else if(encoding == "csv") {
      bool ok = LoadCsvLayerData(dataStr, tileLayer, tileWidth, tileHeight, layerWidth, layerHeight, chunkXIdx, chunkYIdx,
                                 chunkIdx);
      if(!ok)
        return false;
    }
    else {
      outStream << "Error loading tiles, unsupported encoding type: " << encoding << std::endl;
      return false;
    }
  }
  else {
    errStream << "Error while loading tiles, empty data string passed." << std::endl;
    return false;
  }

  return true;
}

bool TMXMapLoader::LoadBase64ZlibLayerData(const std::string &decodedStr, const size_t expectedSize,
                                           buffers::resources::EGMRoom::TileLayer *tileLayer, const int tileWidth,
                                           const int tileHeight, const int layerWidth, const int layerHeight,
                                           const int xStartIdx, const int yStartIdx, const int chunkIdx) {
  // decompress zlib/gzip compressed data
  std::istringstream istr(decodedStr);
  Decoder decoder(istr);
  size_t dataSize = decodedStr.size();
  std::unique_ptr<char[]> layerDataCharPtr = decoder.decompress(dataSize, 0);

  if(dataSize != expectedSize) {
    errStream << "Error loading tiles, zlib decompressed layer data corrupted." << std::endl;
    return false;
  }

  unsigned int tileIndex = 0;
  for (int y = yStartIdx; y < yStartIdx + layerHeight; ++y) {
    for (int x = xStartIdx; x < xStartIdx + layerWidth; ++x) {
      // loading 4 bytes of stream into uint following little endian order
      unsigned int globalTileId = static_cast<unsigned char>(layerDataCharPtr[tileIndex]) |
                                  static_cast<unsigned char>(layerDataCharPtr[tileIndex + 1]) << 8 |
                                  static_cast<unsigned char>(layerDataCharPtr[tileIndex + 2]) << 16 |
                                  static_cast<unsigned char>(layerDataCharPtr[tileIndex + 3]) << 24;

      tileIndex += 4;

      if(!CreateTileFromGlobalId(globalTileId, tileLayer, tileWidth, tileHeight, x, y, layerWidth, chunkIdx))
        return false;
    }
  }

  return true;
}

bool TMXMapLoader::LoadBase64ZstdLayerData(const std::string &decodedStr, const size_t expectedSize,
                                           buffers::resources::EGMRoom::TileLayer *tileLayer, const int tileWidth, const int tileHeight,
                                           const int layerWidth, const int layerHeight, const int xStartIdx,
                                           const int yStartIdx, const int chunkIdx) {

  std::vector<unsigned char> outTileData;
  outTileData.resize(expectedSize);

  // decompress zstd compressed data
  size_t const outSize = ZSTD_decompress(outTileData.data(), outTileData.size(),
                                       decodedStr.data(), decodedStr.size());
  if(ZSTD_isError(outSize)) {
    errStream << "Error loading tile layer data, zstd compressed stream corrupted." << std::endl;
    return false;
  }

  unsigned int tileIndex = 0;
  for (int y = yStartIdx; y < yStartIdx + layerHeight; ++y) {
    for (int x = xStartIdx; x < xStartIdx + layerWidth; ++x) {
      // loading 4 bytes of stream into uint following little endian order
      unsigned int globalTileId = outTileData[tileIndex] |
                                  outTileData[tileIndex + 1] << 8 |
                                  outTileData[tileIndex + 2] << 16 |
                                  outTileData[tileIndex + 3] << 24;

      tileIndex += 4;

      if(!CreateTileFromGlobalId(globalTileId, tileLayer, tileWidth, tileHeight, x, y, layerWidth, chunkIdx))
         return false;
    }
  }

  return true;
}

bool TMXMapLoader::LoadBase64UncompressedLayerData(const std::string &decodedStr, const size_t expectedSize,
                                                   buffers::resources::EGMRoom::TileLayer *tileLayer, const int tileWidth,
                                                   const int tileHeight, const int layerWidth, const int layerHeight,
                                                   const int xStartIdx, const int yStartIdx, const int chunkIdx) {
  if(decodedStr.size() != expectedSize) {
    errStream << "Error loading tile layer data, uncompressed stream corrupted." << std::endl;
    return false;
  }

  unsigned int tileIndex = 0;
  for (int y = yStartIdx; y < yStartIdx + layerHeight; ++y) {
    for (int x = xStartIdx; x < xStartIdx + layerWidth; ++x) {
      // loading 4 bytes of stream into uint following little endian order
      unsigned int globalTileId = static_cast<unsigned char>(decodedStr[tileIndex]) |
                                  static_cast<unsigned char>(decodedStr[tileIndex + 1]) << 8 |
                                  static_cast<unsigned char>(decodedStr[tileIndex + 2]) << 16 |
                                  static_cast<unsigned char>(decodedStr[tileIndex + 3]) << 24;

      tileIndex += 4;

      if(!CreateTileFromGlobalId(globalTileId, tileLayer, tileWidth, tileHeight, x, y, layerWidth, chunkIdx))
        return false;
    }
  }

  return true;
}

bool TMXMapLoader::LoadCsvLayerData(const std::string &dataStr, buffers::resources::EGMRoom::TileLayer *tileLayer, const int tileWidth,
                                    const int tileHeight, const int layerWidth, const int layerHeight,
                                    const int xStartIdx, const int yStartIdx, const int chunkIdx) {
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

  size_t expectedSize = layerWidth * layerHeight;
  if(globalTileIds.size() != expectedSize) {
    errStream << "Error laoding tiles, csv layer data corrupted." << std::endl;
    return false;
  }

  for (int y = yStartIdx; y < yStartIdx + layerHeight; ++y) {
    for (int x = xStartIdx; x < xStartIdx + layerWidth; ++x) {
      unsigned int globalTileId = globalTileIds[y*layerWidth + x];
      if(!CreateTileFromGlobalId(globalTileId, tileLayer, tileWidth, tileHeight, x, y, layerWidth, chunkIdx))
         return false;
    }
  }

  return true;
}

bool TMXMapLoader::CreateTileFromGlobalId(const unsigned int globalTileId, buffers::resources::EGMRoom::TileLayer *tileLayer,
                                          const int mapTileWidth, const int mapTileHeight, const int currX,
                                          const int currY, const int layerWidth, const int chunkIdx) {
  // create a tile instance irrespective of existence of tile in that place, as it is required as a placeholder while
  // loading tiles again in egm plugin of Tiled
  buffers::resources::EGMRoom::Tile* tile;
  if(chunkIdx == -1)
    tile = tileLayer->mutable_data()->add_tiles();
  else
    tile = tileLayer->mutable_data()->mutable_chunks(chunkIdx)->add_tiles();

  // do not fill empty tiles i.e. with globalId = 0
  if(globalTileId <= 0)
    return true;

  bool hasHorizontalFlip=false, hasVerticalFlip=false, flippedDiagonally=false, rotatedHex120=false;
  std::string tilesetName = "";
  int localTileId = GetLocalTileIdInfo(tilesetName, globalTileId, hasHorizontalFlip, hasVerticalFlip,
                                       flippedDiagonally, rotatedHex120);

  std::string backgroundName = tilesetName;

  // get pointer to the background to be used in setting Room.tile properties
  buffers::resources::Background* bgPtr;

  if(tilesetBgNamePtrMap.find(backgroundName) != tilesetBgNamePtrMap.end())
    bgPtr = tilesetBgNamePtrMap[backgroundName]->mutable_background();
  else {
    errStream << "Error loading tiles, background not found." << std::endl;
    return false;
  }

  tile->set_gid(globalTileId);

  tile->set_background_name(backgroundName);
  tile->set_name(backgroundName+"_"+std::to_string(idx++));

  if(roomOrientation == RoomOrientation::orthogonal) {
    tile->set_x(currX * mapTileWidth);
    tile->set_y(currY * mapTileHeight);
  }
  else if(roomOrientation == RoomOrientation::hexagonal) {
    if(hexMapInfo.staggerAxis == StaggerAxis::x) {
      tile->set_x(currX * (mapTileWidth - (hexMapInfo.hexSideLength / 2)));

      if((currX%2 == 0 && hexMapInfo.staggerIndex == StaggerIndex::odd) ||
         (currX%2 != 0 && hexMapInfo.staggerIndex == StaggerIndex::even))
        tile->set_y(currY * mapTileHeight);
      else
        tile->set_y((currY * mapTileHeight) + hexMapInfo.hexSideLength);
    }
    else if(hexMapInfo.staggerAxis == StaggerAxis::y) {
      tile->set_y(currY * (mapTileHeight - (hexMapInfo.hexSideLength / 2)));

      if((currY%2 == 0 && hexMapInfo.staggerIndex == StaggerIndex::odd) ||
         (currY%2 != 0 && hexMapInfo.staggerIndex == StaggerIndex::even))
        tile->set_x(currX * mapTileWidth);
      else
        tile->set_x((currX * mapTileWidth) + hexMapInfo.hexSideLength);
    }
  }
  else if(roomOrientation == RoomOrientation::isometric) {
    int xStart = ((layerWidth * mapTileWidth) / 2)  /*Mid x coordinate of the room in pixels*/
                 - (mapTileWidth / 2)               /*Reposition by an offset of half of tileWidth*/
                 - (currY * (mapTileWidth / 2));    /*Reposition x coordinate depending on the row index or currY*/
    int x = xStart + (currX * mapTileWidth / 2);
    int y = (currY * mapTileHeight / 2) + (currX * mapTileHeight / 2);
    tile->set_x(x);
    tile->set_y(y);
  }
  else if(roomOrientation == RoomOrientation::staggered) {
    if(staggeredIsoMapInfo.staggerAxis == StaggerAxis::x) {
      tile->set_x(currX * mapTileWidth / 2);

      if((currX%2 == 0 && staggeredIsoMapInfo.staggerIndex == StaggerIndex::odd) ||
         (currX%2 != 0 && staggeredIsoMapInfo.staggerIndex == StaggerIndex::even))
        tile->set_y(currY * mapTileHeight);
      else
        tile->set_y((currY * mapTileHeight) + mapTileHeight / 2);
    }
    else if(staggeredIsoMapInfo.staggerAxis == StaggerAxis::y) {
      tile->set_y(currY * mapTileHeight / 2);

      if((currY%2 == 0 && staggeredIsoMapInfo.staggerIndex == StaggerIndex::odd) ||
         (currY%2 != 0 && staggeredIsoMapInfo.staggerIndex == StaggerIndex::even))
        tile->set_x(currX * mapTileWidth);
      else
        tile->set_x((currX * mapTileWidth) + mapTileWidth / 2);
    }
  }
  else {
    errStream << "Error loading tiles, unsupported map format." << std::endl;
    return false;
  }

  // In some cases numColumns is not present/specified, so deduce numColumns using (imageWidth / tileWidth)
  int numColumns = 0;
  if(!bgPtr->has_columns() || bgPtr->columns() == 0) {
    // TODO: Find an alternative for following (hint: load image in memory and get its dimensions)
    // In some cases even imageWidth is not present so user is prompted to fix the tsx file first
    if(bgPtr->has_width()) {
      numColumns = bgPtr->width() / bgPtr->tile_width();
      bgPtr->set_columns(numColumns);
    }
  }
  else
    numColumns = bgPtr->columns();

  if(numColumns == 0) {
    errStream << "Error loading tiles, source image width/height is zero in .tsx file." << std::endl;
    return false;
  }

  // tilesetTile[Width[Height]] is different from mapTile[Width[Height]] and should be used appropriately
  int tilesetTileWidth = bgPtr->tile_width();
  int tilesetTileHeight = bgPtr->tile_height();

  tile->set_width(tilesetTileWidth);
  tile->set_height(tilesetTileHeight);

  int xOffset = localTileId % numColumns;
  xOffset = xOffset * (tilesetTileWidth + bgPtr->horizontal_spacing()) + bgPtr->horizontal_offset();

  int yOffset = floor(localTileId / (1.0f * numColumns));
  yOffset = yOffset * (tilesetTileHeight + bgPtr->vertical_spacing()) + bgPtr->vertical_offset();

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

  // below two flags are only tested in hex maps, doing a little hack by using hexMapUtil to know if this map is
  // hexagoanl or not
  if(roomOrientation == RoomOrientation::hexagonal) {
    if(flippedDiagonally)
      tile->set_rotation(60.0);
    if(rotatedHex120)
      tile->set_rotation(120.0);
  }

  // TODO: Find a good implementation to set depth and id
  tile->set_depth(0);
  tile->set_id(10000001 + resourceTypeIdCountMap["buffers.resources.Room.Tile"]++);

  return true;
}

int TMXMapLoader::GetLocalTileIdInfo(std::string& tilesetName, const unsigned int globalTileId, bool &hasHorizontalFlip,
                                     bool &hasVerticalFlip, bool &flippedDiagonally, bool &rotatedHex120) {
  int localId = ConvertGlobalTileIdToLocal(globalTileId, hasHorizontalFlip, hasVerticalFlip, flippedDiagonally,
                                           rotatedHex120);

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

int TMXMapLoader::ConvertGlobalTileIdToLocal(const unsigned int globalTileId, bool& hasHorizontalFlip, bool& hasVerticalFlip,
                                             bool& flippedDiagonally, bool& rotatedHex120) {
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
  flippedDiagonally = tileId & FLIPPED_DIAGONALLY_FLAG;
  rotatedHex120 = tileId & ROTATED_HEXAGONAL_120_FLAG;

  tileId &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG | ROTATED_HEXAGONAL_120_FLAG);

  return tileId;
}

} // namespace egm
