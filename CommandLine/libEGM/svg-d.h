
#ifndef EGM_SVG_H
#define EGM_SVG_H

#include "egm-rooms.h"
#include <google/protobuf/util/message_differencer.h>

#include <string>
#include <utility>
#include <vector>

namespace egm {
namespace svg_d {

using buffers::resources::Room;
using google::protobuf::RepeatedPtrField;

InstanceLayer BuildLayer(const RepeatedPtrField<Room::Instance> &instances);
TileLayer     BuildLayer(const RepeatedPtrField<Room::Tile> &tiles);

void ParseInstances(
    const std::string &layer_data, Room *dest_message,
    const std::string &error_details);
void ParseTiles(
    const std::string &layer_data, Room *dest_message,
    const std::string &error_details);

}  // namespace svg_d
}  // namespace egm

#endif
