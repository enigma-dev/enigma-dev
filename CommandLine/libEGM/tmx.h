#ifndef TMX_H
#define TMX_H

#include "file-format.h"
#include "tsx.h"

#include "pugixml.hpp"

namespace egm {


/**
 * @brief The TMXFileFormat class provides functionality to load Tiled .tmx files
 */
class TMXFileFormat : public FileFormat {
public:
  /**
   * @brief TMXFileFormat constructor
   * @param event_data
   */
  TMXFileFormat(const EventData* event_data) : FileFormat(event_data) {}

  /**
   * @brief Loads .tmx file to EGM Project
   * @param fPath .tmx file path
   * @return Pointer to EGM Project
   */
  virtual std::unique_ptr<Project> LoadProject(const fs::path& fPath) const override;
};


/**
 * @brief The HexMapUtil struct stores details of hexagonal Tiled map
 */
struct HexMapUtil {
  /**
   * @brief Length of side of hexagonal tile
   */
  unsigned int hexSideLength;

  /**
   * @brief Specifies the alignment of hex tiles, can be either "x" or "y"
   */
  std::string staggerAxis;

  /**
   * @brief Specifies index(among each pair of neighbouring indices) to be shifted to create hex map,
   * can be either "even" or "odd"
   */
  std::string staggerIndex;
};

enum RoomOrientation {
  orthogonal,
  hexagonal,
  isometric,
  unknown
};

/**
 * @brief The TMXMapLoader class manages parsing of Tiled .tmx file format
 */
class TMXMapLoader {
public:
  /**
   * @brief Constructor to init variables required to load Tiled .tmx file
   * @param root TreeNode pointer to the root of EGM project hierarchy
   * @param fPath .tmx file path
   */
  TMXMapLoader(buffers::TreeNode *root, const fs::path &fPath);

  /**
   * @brief Parses .tmx map file and builds EGM project
   * @param Reference to document node of .tmx xml node hierarchy
   * @return Returns true if parsing succeeds else false
   */
  bool Load(pugi::xml_document &xmlDoc);

private:
  /**
   * @brief Stores .tmx file path
   */
  fs::path tmxPath;

  /**
   * @brief Stores TreeNode heirarchy of EGM project
   */
  std::vector<buffers::TreeNode *> nodes;

  /**
   * @brief Tree map of <tileset> node "firstgid" and name of Background loaded with corresponding tileset
   */
  std::map<int, std::string> tilesetIdNameMap;

  /**
   * @brief Hash map of EGM resource name and its corresponding EGM resource pointer
   */
  std::unordered_map<std::string, buffers::TreeNode *> resourceFolderRefs;

  /**
   * @brief Hash map to generate unique ids for repetable messages present in EGM resources like Room.proto
   */
  std::unordered_map<std::string, int> resourceTypeIdCountMap;

  /**
   * @brief Hash map of EGM Background resource name and its pointer, only for Backgrounds based on image atlas tilesets
   */
  std::unordered_map<std::string, buffers::TreeNode *> tilesetBgNamePtrMap;

  /**
   * @brief Stores hexagonal map details in heap
   */
  std::unique_ptr<HexMapUtil> hexMapUtil;

  /**
   * @brief Stores current Room or Tiled Map orientation
   */
  RoomOrientation roomOrientation;

  // TODO: Remove this hack and use "resource name generator"
  int idx=0;

  /**
   * @brief Adds Background resource(s), to EGM project, corresponding to <tileset> node(s) present in .tmx file
   * @param mapNode Reference to <map> xml node of .tmx file
   * @return Returns true if parsing succeeds else false
   */
  bool LoadTilesets(pugi::xml_node &mapNode);

  /**
   * @brief Helper method to create Background resource(s) based on internal tileset(s) present in .tmx file
   * @param tileset Reference to <tileset> xml node representing internal tileset
   * @param background_walker Reference to TSXTilesetLoader object
   */
  void LoadInternalTileset(pugi::xml_node &tileset, TSXTilesetLoader &background_walker);

  /**
   * @brief Helper method to create Background resource(s) based on external tileset(s) present in .tmx file
   * @param tsxPath .tsx file path representing external tileset
   * @param background_walker Reference to TSXTilesetLoader object
   * @return Returns true if parsing succeeds else false
   */
  bool LoadExternalTileset(const std::string &tsxPath, TSXTilesetLoader &background_walker);

  /**
   * @brief Adds Room resource, to EGM project, based on <map> node present in .tmx file
   * @param mapNode Reference to <map> xml node of .tmx file
   * @return Returns true if parsing succeeds else false
   */
  bool LoadMap(pugi::xml_node &mapNode);

  /**
   * @brief Helper method to generate folder name from resource type
   * @param resType EGM resource name
   * @return Capitalized folder name
   */
  std::string GetFolderName(const std::string &resType);

  /**
   * @brief Adds Tiles to Room resource, based on <object> nodes present in .tmx file
   * @param mapNode Reference to <map> xml node of .tmx file
   * @param resNode Pointer to EGM Room resource
   * @return Returns true if parsing succeeds else false
   */
  bool LoadObjects(pugi::xml_node &mapNode, buffers::TreeNode *resNode);

  /**
   * @brief Adds Tiles to Room Resource, based on data present in <layer><data> nodes of .tmx file
   * @param mapNode Reference to <map> xml node of .tmx file
   * @param resNode Pointer to EGM Room resource
   * @param tileWidth Width of tile in pixels
   * @param tileHeight Height of tile in pixels
   * @return Returns true if parsing succeeds else false
   */
  bool LoadLayerData(pugi::xml_node &mapNode, buffers::TreeNode *resNode, int tileWidth, int tileHeight);

  /**
   * @brief Helper method to decompress and add tile data stored in Glib and Zlib streams to Room Resource
   * @param decodedStr Base64 decoded string data
   * @param expectedSize Expected number of bytes stored in glib/zlib stream data
   * @param resNode Pointer to EGM Room resource
   * @param tileWidth Width of tile in pixels
   * @param tileHeight Height of tile in pixels
   * @param layerWidth Number of tiles in horizontal direction
   * @param layerHeight Number of tiles in vertical direction
   * @return Returns true if parsing succeeds else false
   */
  bool LoadBase64ZlibLayerData(const std::string &decodedStr, const size_t &expectedSize, buffers::TreeNode *resNode,
                           const int &tileWidth, const int &tileHeight, const int &layerWidth, const int &layerHeight);

  /**
   * @brief Helper method to decompress and add tile data stored in Zstd stream to Room Resource
   * @param decodedStr Base64 decoded string data
   * @param expectedSize Expected number of bytes stored in zstd stream data
   * @param resNode Pointer to EGM Room resource
   * @param tileWidth Width of tile in pixels
   * @param tileHeight Height of tile in pixels
   * @param layerWidth Number of tiles in horizontal direction
   * @param layerHeight Number of tiles in vertical direction
   * @return Returns true if parsing succeeds else false
   */
  bool LoadBase64ZstdLayerData(const std::string &decodedStr, const size_t &expectedSize, buffers::TreeNode *resNode,
                           const int &tileWidth, const int &tileHeight, const int &layerWidth, const int &layerHeight);

  /**
   * @brief Helper method to decompress and add tile data stored in base64 stream to Room Resource
   * @param decodedStr Base64 decoded string data
   * @param expectedSize Expected number of bytes stored in zstd stream data
   * @param resNode Pointer to EGM Room resource
   * @param tileWidth Width of tile in pixels
   * @param tileHeight Height of tile in pixels
   * @param layerWidth Number of tiles in horizontal direction
   * @param layerHeight Number of tiles in vertical direction
   * @return Returns true if parsing succeeds else false
   */
  bool LoadBase64UncompressedLayerData(const std::string &decodedStr, const size_t &expectedSize, buffers::TreeNode *resNode,
                           const int &tileWidth, const int &tileHeight, const int &layerWidth, const int &layerHeight);

  /**
   * @brief Helper method to load and add tile data stored in CSV format to Room Resource
   * @param dataStr CSV string data
   * @param resNode Pointer to EGM Room resource
   * @param tileWidth Width of tile in pixels
   * @param tileHeight Height of tile in pixels
   * @param layerWidth Number of tiles in horizontal direction
   * @param layerHeight Number of tiles in vertical direction
   * @return Returns true if parsing succeeds else false
   */
  bool LoadCsvLayerData(const std::string &dataStr, buffers::TreeNode *resNode, const int &tileWidth,
                        const int &tileHeight, const int &layerWidth, const int &layerHeight);

  /**
   * @brief Creates Tile instances for EGM Room resource
   * @param globalTileId Global id of tile(with flip flags)
   * @param resNode Pointer to EGM Room resource
   * @param mapTileWidth Width of tile in pixels
   * @param mapTileHeight Height of tile in pixels
   * @param currX Current column of tile in Room
   * @param currY Current row of tile in Room
   * @return Returns true if parsing succeeds else false
   */
  bool CreateTileFromGlobalId(const unsigned int &globalTileId, buffers::TreeNode *resNode, const int &mapTileWidth,
                              const int &mapTileHeight, const int &currX, const int &currY, const int &layerWidth);

  /**
   * @brief Get local tile id by selecting tileset it belongs to
   * @param tilesetName Name of the tileset this tile belongs to
   * @param globalTileId Global id of tile(with flip flags)
   * @param hasHorizontalFlip Reference to horizontal flip flag
   * @param hasVerticalFlip Reference to vertical flip flag
   * @param flippedDiagonally Reference to diagonal flip flag
   * @param rotatedHex120 Reference to rotated hex flip flag
   * @return Returns true if parsing succeeds else false
   */
  int GetLocalTileIdInfo(std::string &tilesetName, const unsigned int &globalTileId,
                         bool &hasHorizontalFlip, bool &hasVerticalFlip, bool &flippedDiagonally, bool &rotatedHex120);

  /**
   * @brief Convert global tile id to local id by removing flip flags
   * @param globalTileId Global id of tile(with flip flags)
   * @param hasHorizontalFlip Reference to horizontal flip flag
   * @param hasVerticalFlip Reference to vertical flip flag
   * @param flippedDiagonally Reference to diagonal flip flag
   * @param rotatedHex120 Reference to rotated hex flip flag
   * @return Returns true if parsing succeeds else false
   */
  int ConvertGlobalTileIdToLocal(const unsigned int &globalTileId, bool &hasHorizontalFlip, bool &hasVerticalFlip,
                                 bool &flippedDiagonally, bool &rotatedHex120);
};

} // namespace egm

#endif // TMX_H
