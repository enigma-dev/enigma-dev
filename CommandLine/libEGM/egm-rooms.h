#ifndef EGM_ROOMS_UTIL_h
#define EGM_ROOMS_UTIL_h

#include "project.pb.h"

#include <map>
#include <vector>

namespace egm {
namespace util {

struct InstanceLayers {
  struct Layer {
    std::string format;
    std::string data;
  };
  // Layers of compacted instances.
  std::vector<Layer> layers;
  // Any instances with noncompactable attributes.
  std::vector<buffers::resources::Room::Instance> snowflakes;

};

struct TileLayers {
  struct Layer {
    std::string format;
    std::string data;
  };
  // Layers of compacted tiles.
  std::vector<Layer> layers;
  // Any tiles with noncompactable attributes.
  std::vector<buffers::resources::Room::Tile> snowflakes;

};

InstanceLayers BuildInstanceLayers(const buffers::resources::Room &room);
TileLayers BuildTileLayers(const buffers::resources::Room &room);

}  // namespace util
}  // namespace egm

#endif
