/** Copyright (C) 2018 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "egm-rooms.h"
#include "serialization-helpers.h"
#include "svg-d.h"

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/util/message_differencer.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>

using std::string;
using buffers::resources::Room;
using google::protobuf::RepeatedPtrField;
using google::protobuf::util::MessageDifferencer;
using namespace egm::serialization;

namespace {

template<typename Entities, typename Layer>
void BuildLayers(const Entities &entities, std::vector<Layer> *out) {
  out->emplace_back(egm::svg_d::BuildLayer(entities));
}

}  // namespace

namespace egm {
namespace util {

std::vector<InstanceLayer> BuildInstanceLayers(const Room &room) {
  std::vector<InstanceLayer> res;
  BuildLayers(room.instances(), &res);
  return res;
}

std::vector<TileLayer> BuildTileLayers(const Room &room) {
  std::vector<TileLayer> res;
  BuildLayers(room.tiles(), &res);
  return res;
}

}  // namespace util
}  // namespace egm
