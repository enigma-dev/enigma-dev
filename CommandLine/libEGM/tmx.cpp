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

  TMXMapLoader mapWalker(game->mutable_root(), fPath);

  bool success = mapWalker.Load(doc);
  if(!success) {
    outStream << "Error occured while loading selected Tiled map." << std::endl;
    return NULL;
  }

  // temp code start
  /*std::string str;
  google::protobuf::TextFormat::PrintToString(proj->game(), &str);
  std::ofstream out("/home/kash/github/radialgm-stuff/textProtos/test1tmx.txt");
  out<<str<<std::endl;
  out.close();*/
  std::cout<<"END LOADING TMX"<<std::endl;
  // temp code end

  return proj;
}

TMXMapLoader::TMXMapLoader(buffers::TreeNode *root, const fs::path &fPath) : tmxPath(fPath) {
  root->set_name("/");
  nodes.push_back(root);
  roomOrientation = RoomOrientation::unknown;
}

bool TMXMapLoader::Load(pugi::xml_document &xmlDoc) {
  pugi::xml_node mapNode = xmlDoc.child("map");

  // first load all tileset(s) in background(s)
  bool backgroundsOk = LoadTilesets(mapNode);
  if(!backgroundsOk)
    return false;

  // now load map, backgrounds are available for setting room.tile and/or room.background
  bool roomOk = LoadMap(mapNode);
  if(!roomOk)
    return false;

  return true;
}

bool TMXMapLoader::LoadTilesets(pugi::xml_node& mapNode) {
  std::string resType = "background";

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
  // CURRENT FIX: This is currently being handled by a hack present in tsx walker for_each loop
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

bool TMXMapLoader::LoadMap(pugi::xml_node& mapNode) {
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

  // correct width and height, convert from no. of tiles to no. of pixels
  unsigned int nHoriTiles = resNode->room().width();
  unsigned int nVertTiles = resNode->room().height();
  unsigned int tileWidthPixels = resNode->room().hsnap();
  unsigned int tileHeightPixels = resNode->room().vsnap();

  const std::string &orientation = resNode->mutable_room()->orientation();

  if(orientation == "orthogonal" || orientation == "isometric") {
    if(orientation == "orthogonal")
      roomOrientation = RoomOrientation::orthogonal;
    else if(orientation == "isometric")
      roomOrientation = RoomOrientation::isometric;

    resNode->mutable_room()->set_width(nHoriTiles * tileWidthPixels);
    resNode->mutable_room()->set_height(nVertTiles * tileHeightPixels);
  }
  else if(orientation == "hexagonal") {
    roomOrientation = RoomOrientation::hexagonal;
    hexMapUtil = std::make_unique<HexMapUtil>();
    hexMapUtil->hexSideLength = resNode->room().hexsidelength();
    hexMapUtil->staggerUtil.staggerAxis = resNode->room().staggeraxis();
    hexMapUtil->staggerUtil.staggerIndex = resNode->room().staggerindex();

    if(hexMapUtil->staggerUtil.staggerAxis == "x") {
      resNode->mutable_room()->set_width((3 * nHoriTiles + 1) * hexMapUtil->hexSideLength / 2);
      resNode->mutable_room()->set_height((nVertTiles * tileHeightPixels) + hexMapUtil->hexSideLength);
    }
    else {
      resNode->mutable_room()->set_width((nHoriTiles * tileWidthPixels) + hexMapUtil->hexSideLength);
      resNode->mutable_room()->set_height((3 * nVertTiles + 1) * hexMapUtil->hexSideLength / 2);
    }
  }
  else if(orientation == "staggered") {
    roomOrientation = RoomOrientation::staggered;
    staggeredIsoMapUtil = std::make_unique<StaggerUtil>();
    staggeredIsoMapUtil->staggerAxis = resNode->room().staggeraxis();
    staggeredIsoMapUtil->staggerIndex = resNode->room().staggerindex();

    if(staggeredIsoMapUtil->staggerAxis == "x") {
      resNode->mutable_room()->set_width(nHoriTiles * tileWidthPixels / 2);
      resNode->mutable_room()->set_height(nVertTiles * tileHeightPixels);
    }
    else {
      resNode->mutable_room()->set_width(nHoriTiles * tileWidthPixels);
      resNode->mutable_room()->set_height(nVertTiles * tileHeightPixels / 2);
    }
  }
  else {
    errStream << "Error loading map, unsupported map orientation: " << resNode->mutable_room()->orientation() << std::endl;
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
    resNode->mutable_room()->set_color(hexColorInt);
    resNode->mutable_room()->set_show_color(true);
  }

  bool room_tilesFromObjectsOk = LoadObjects(mapNode, resNode);
  if(!room_tilesFromObjectsOk) {
    errStream << "Something went wrong while loading Room.Tiles from Objects." << std::endl;
    return false;
  }

  bool room_tiledFromLayerDataOk = LoadLayerData(mapNode, resNode, resNode->room().hsnap(), resNode->room().vsnap());
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
    bool hasOpacity = false;
    pugi::xml_attribute opacityAttr = objectGroupChild.attribute("opacity");
    if(!opacityAttr.empty())
      hasOpacity = true;

    // iterate over all children, which is most probably objects, create new tiles out of them
    pugi::xml_object_range<pugi::xml_named_node_iterator> objects = objectGroupChild.children("object");
    for(const pugi::xml_node &objectChild : objects) {
      unsigned int globalTileId = objectChild.attribute("gid").as_uint();

      bool hasHorizontalFlip=false, hasVerticalFlip=false, flippedDiagonally=false, rotatedHex120=false;
      std::string tilesetName = "";
      int localTileId = GetLocalTileIdInfo(tilesetName, globalTileId, hasHorizontalFlip, hasVerticalFlip,
                                           flippedDiagonally, rotatedHex120);

      if(localTileId == 0){
        errStream << "localId attribute not present(templates are not yet supported) or its value is zero." << std::endl;
        continue;
      }

      std::string backgroundName = tilesetName + "_" + std::to_string(localTileId);

      buffers::resources::EGMRoom::Tile* tile = resNode->mutable_room()->add_tiles();

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

bool TMXMapLoader::LoadLayerData(pugi::xml_node& mapNode, buffers::TreeNode *resNode, int tileWidth, int tileHeight) {
  pugi::xml_object_range<pugi::xml_named_node_iterator> layers = mapNode.children("layer");
  for(const pugi::xml_node &layer : layers) {
    const pugi::xml_node &dataNode = layer.child("data");

    if(dataNode.empty()) {
      outStream << "Error loading tiles, layer node does not contain data node." << std::endl;
      return false;
    }

    std::string encoding = dataNode.attribute("encoding").as_string();
    std::string compression = dataNode.attribute("compression").as_string();

    std::string dataStr = dataNode.first_child().value();
    dataStr = StrTrim(dataStr);

    // In infinite Tiled maps, data string is stored in chunk nodes
    pugi::xml_object_range<pugi::xml_named_node_iterator> chunks = dataNode.children("chunk");

    // data string can be found in child of <chunk> node(s) (for infinite Tiled maps) or directly in child node
    if(!chunks.empty()) {
      for(const pugi::xml_node &chunk : chunks) {
        std::string chunkDataStr = chunk.first_child().value();
        chunkDataStr = StrTrim(chunkDataStr);

        int chunkXIdx = chunk.attribute("x").as_int();
        int chunkYIdx = chunk.attribute("y").as_int();
        int chunkWidth = chunk.attribute("width").as_int();
        int chunkHeight = chunk.attribute("height").as_int();

        bool ok = LoadLayerDataHelper(chunkDataStr, chunkWidth, chunkHeight, encoding, compression, resNode, tileWidth,
                                      tileHeight, chunkXIdx, chunkYIdx);
        if(!ok) {
          errStream << "Error while loading tiles from Layer Data Chunk." << std::endl;
          return false;
        }
      }
    }
    else {
      int layerWidth = layer.attribute("width").as_int();
      int layerHeight = layer.attribute("height").as_int();

      bool ok = LoadLayerDataHelper(dataStr, layerWidth, layerHeight, encoding, compression, resNode, tileWidth,
                                    tileHeight);
      if(!ok) {
        errStream << "Error while loading tiles from Layer Data." << std::endl;
        return false;
      }
    }
  }

  return true;
}

bool TMXMapLoader::LoadLayerDataHelper(const std::string &dataStr, const int &layerWidth, const int &layerHeight,
                                       const std::string &encoding, const std::string &compression, buffers::TreeNode *resNode,
                                       const int& tileWidth, const int& tileHeight, const int &chunkXIdx, const int &chunkYIdx) {
  if(!dataStr.empty()) {
    if(encoding == "base64") {
      // decode base64 data
      std::string decodedStr = base64_decode(dataStr);

      size_t expectedSize = layerWidth * layerHeight * 4;

      if(compression == "zlib" || compression == "gzip") {
        bool ok = LoadBase64ZlibLayerData(decodedStr, expectedSize, resNode, tileWidth, tileHeight, layerWidth,
                                          layerHeight, chunkXIdx, chunkYIdx);
        if(!ok)
          return false;
      }
      else if(compression == "zstd") {
        bool ok = LoadBase64ZstdLayerData(decodedStr, expectedSize, resNode, tileWidth, tileHeight, layerWidth,
                                          layerHeight, chunkXIdx, chunkYIdx);
        if(!ok)
          return false;
      }
      else {
        // compression isnt specified for uncompressed format
        bool ok = LoadBase64UncompressedLayerData(decodedStr, expectedSize, resNode, tileWidth, tileHeight, layerWidth,
                                          layerHeight, chunkXIdx, chunkYIdx);
        if(!ok)
          return false;
      }
    }
    else if(encoding == "csv") {
      bool ok = LoadCsvLayerData(dataStr, resNode, tileWidth, tileHeight, layerWidth, layerHeight, chunkXIdx, chunkYIdx);
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

bool TMXMapLoader::LoadBase64ZlibLayerData(const std::string &decodedStr, const size_t &expectedSize,
                                           buffers::TreeNode *resNode, const int& tileWidth, const int& tileHeight,
                                           const int& layerWidth, const int& layerHeight, const int &xStartIdx,
                                           const int &yStartIdx) {
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

      if(!CreateTileFromGlobalId(globalTileId, resNode, tileWidth, tileHeight, x, y, layerWidth))
        return false;
    }
  }

  return true;
}

bool TMXMapLoader::LoadBase64ZstdLayerData(const std::string &decodedStr, const size_t &expectedSize,
                                           buffers::TreeNode *resNode, const int& tileWidth, const int& tileHeight,
                                           const int& layerWidth, const int& layerHeight, const int &xStartIdx,
                                           const int &yStartIdx) {

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

      if(!CreateTileFromGlobalId(globalTileId, resNode, tileWidth, tileHeight, x, y, layerWidth))
         return false;
    }
  }

  return true;
}

bool TMXMapLoader::LoadBase64UncompressedLayerData(const std::string &decodedStr, const size_t &expectedSize,
                                                   buffers::TreeNode *resNode, const int& tileWidth,
                                                   const int& tileHeight, const int& layerWidth, const int& layerHeight,
                                                   const int &xStartIdx, const int &yStartIdx) {
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

      if(!CreateTileFromGlobalId(globalTileId, resNode, tileWidth, tileHeight, x, y, layerWidth))
        return false;
    }
  }

  return true;
}

bool TMXMapLoader::LoadCsvLayerData(const std::string &dataStr, buffers::TreeNode *resNode, const int& tileWidth,
                                    const int& tileHeight, const int& layerWidth, const int& layerHeight,
                                    const int &xStartIdx, const int &yStartIdx) {
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
      if(!CreateTileFromGlobalId(globalTileId, resNode, tileWidth, tileHeight, x, y, layerWidth))
         return false;
    }
  }

  return true;
}

bool TMXMapLoader::CreateTileFromGlobalId(const unsigned int &globalTileId, buffers::TreeNode *resNode,
                                          const int &mapTileWidth, const int &mapTileHeight, const int &currX,
                                          const int &currY, const int &layerWidth) {
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

  // for negative localTileId dont quit loading map, just skip this tile
  if(localTileId < 0)
    return true;

  buffers::resources::EGMRoom::Tile* tile = resNode->mutable_room()->add_tiles();

  tile->set_background_name(backgroundName);
  tile->set_name(backgroundName+"_"+std::to_string(idx++));

  if(roomOrientation == RoomOrientation::orthogonal) {
    tile->set_x(currX * mapTileWidth);
    tile->set_y(currY * mapTileHeight);
  }
  else if(roomOrientation == RoomOrientation::hexagonal) {
    if(!hexMapUtil) {
      errStream << "Error loading tiles for hex map, hexagonal map details incomplete." << std::endl;
      return false;
    }

    if(hexMapUtil->staggerUtil.staggerAxis == "x") {
      tile->set_x(currX * (mapTileWidth - (hexMapUtil->hexSideLength / 2)));

      if((currX%2 == 0 && hexMapUtil->staggerUtil.staggerIndex == "odd") ||
         (currX%2 != 0 && hexMapUtil->staggerUtil.staggerIndex == "even"))
        tile->set_y(currY * mapTileHeight);
      else
        tile->set_y((currY * mapTileHeight) + hexMapUtil->hexSideLength);
    }
    else {
      tile->set_y(currY * (mapTileHeight - (hexMapUtil->hexSideLength / 2)));

      if((currY%2 == 0 && hexMapUtil->staggerUtil.staggerIndex == "odd") ||
         (currY%2 != 0 && hexMapUtil->staggerUtil.staggerIndex == "even"))
        tile->set_x(currX * mapTileWidth);
      else
        tile->set_x((currX * mapTileWidth) + hexMapUtil->hexSideLength);
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
    if(!staggeredIsoMapUtil) {
      errStream << "Error loading tiles for staggered iso map, staggered map details incomplete." << std::endl;
      return false;
    }

    if(staggeredIsoMapUtil->staggerAxis == "x") {
      tile->set_x(currX * mapTileWidth / 2);

      if((currX%2 == 0 && staggeredIsoMapUtil->staggerIndex == "odd") ||
         (currX%2 != 0 && staggeredIsoMapUtil->staggerIndex == "even"))
        tile->set_y(currY * mapTileHeight);
      else
        tile->set_y((currY * mapTileHeight) + mapTileHeight / 2);
    }
    else {
      tile->set_y(currY * mapTileHeight / 2);

      if((currY%2 == 0 && staggeredIsoMapUtil->staggerIndex == "odd") ||
         (currY%2 != 0 && staggeredIsoMapUtil->staggerIndex == "even"))
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

int TMXMapLoader::GetLocalTileIdInfo(std::string& tilesetName, const unsigned int &globalTileId, bool &hasHorizontalFlip,
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

int TMXMapLoader::ConvertGlobalTileIdToLocal(const unsigned int& globalTileId, bool& hasHorizontalFlip, bool& hasVerticalFlip,
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
