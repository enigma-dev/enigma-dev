#ifndef EGM_ROOMS_UTIL_h
#define EGM_ROOMS_UTIL_h

#include "project.pb.h"

#include <map>
#include <vector>

namespace egm {

struct InstanceLayer {
  // The compaction format, eg, "svg-d".
    std::string format;
  // The raw string encoding of the compacted data.
    std::string data;
  // Any instances with noncompactable attributes.
  std::vector<buffers::resources::Room::Instance> snowflakes;
};

struct TileLayer {
  // The compaction format, eg, "svg-d".
    std::string format;
  // The raw string encoding of the compacted data.
    std::string data;
  // Any instances with noncompactable attributes.
  std::vector<buffers::resources::Room::Tile> snowflakes;
};

namespace util {

std::vector<InstanceLayer> BuildInstanceLayers(const buffers::resources::Room &room);
std::vector<TileLayer> BuildTileLayers(const buffers::resources::Room &room);

}  // namespace util
}  // namespace egm

#endif
