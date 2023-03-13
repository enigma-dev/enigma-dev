/** Copyright (C) 2018 Josh Ventura
*** Copyright (C) 2018 Greg Williamson
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

#include "svg-d.h"
#include "algorithm/connected-components.h"
#include "serialization-helpers.h"
#include "proto_util.h"

#include <sstream>

namespace {

using namespace egm::algorithm;
using namespace egm::serialization;
namespace protobuf = google::protobuf;

using std::string;
using buffers::resources::Room;
using protobuf::RepeatedPtrField;
using protobuf::util::MessageDifferencer;
using CppType = protobuf::FieldDescriptor::CppType;

constexpr bool kIncrediblyVerboseDebugOutput = false;
constexpr bool kMinorlyVerboseDebuggingOutput = true;

struct InstSimple {
  string object_name;
  double xscale, yscale, zscale;
  double rotation, zrotation;
  bool has_zscale, has_zrotation;
  int32_t color;
  string  name;
  int32_t id;

  InstSimple(const Room::Instance &inst):
      object_name(inst.object_type()),
      xscale(inst.xscale()), yscale(inst.yscale()), zscale(inst.zscale()),
      rotation(inst.rotation()), zrotation(inst.zrotation()),
      has_zscale(inst.has_zscale()), has_zrotation(inst.has_zrotation()),
      color(inst.color()),
      name(inst.name()),
      id(inst.id()) {}
  static const InstSimple DEFAULTS;

  // Removes fields of this structure from an instance proto, returning rest.
  static Room::Instance Subtract(const Room::Instance &inst) {
    Room::Instance res = inst;
    res.clear_object_type();
    res.clear_xscale(), res.clear_yscale(), res.clear_zscale();
    res.clear_rotation(), res.clear_zrotation();
    res.clear_color();
    return res;
  }

  bool operator< (const InstSimple &o) const {
    return
        object_name < o.object_name || (!(o.object_name < object_name) && (
        xscale      < o.xscale      || (!(o.xscale      < xscale)      && (
        yscale      < o.yscale      || (!(o.yscale      < yscale)      && (
        zscale      < o.zscale      || (!(o.zscale      < zscale)      && (
        rotation    < o.rotation    || (!(o.rotation    < rotation)    && (
        zrotation   < o.zrotation   || (!(o.zrotation   < zrotation)   && (
        color       < o.color       || (!(o.color       < color)       && (
        name        < o.name        || (!(o.name        < name)        && (
        id          < o.id          ))))))))))))))));
  }

  string DebugGloss() const {
    return object_name;
  }
};

struct TileSimple {
  int32_t depth;
  string  bg_name;
  int32_t xoffset, yoffset;
  int32_t width,   height;
  double  xscale,  yscale;
  int32_t color;
  string  name;
  int32_t id;

  TileSimple(const Room::Tile &tile):
      depth(tile.depth()),
      bg_name(tile.background_name()),
      xoffset(tile.xoffset()), yoffset(tile.yoffset()),
      width(tile.width()),     height(tile.height()),
      xscale(tile.xscale()),   yscale(tile.yscale()),
      color(tile.color()) {}
  static const TileSimple DEFAULTS;

  // Removes fields of this structure from an instance proto, returning rest.
  static Room::Tile Subtract(const Room::Tile &tile) {
    Room::Tile res = tile;
    res.clear_depth();
    res.clear_background_name();
    res.clear_xoffset(),  res.clear_yoffset();
    res.clear_width(),    res.clear_height();
    res.clear_xscale(),   res.clear_yscale();
    res.clear_color();
    return res;
  }

  bool operator< (const TileSimple &o) const {
    return
        depth     < o.depth     || (!(o.depth   < depth)    && (
        bg_name   < o.bg_name   || (!(o.bg_name < bg_name)  && (
        xoffset   < o.xoffset   || (!(o.xoffset < xoffset)  && (
        yoffset   < o.yoffset   || (!(o.yoffset < yoffset)  && (
        xscale    < o.xscale    || (!(o.xscale  < xscale)   && (
        yscale    < o.yscale    || (!(o.yscale  < yscale)   && (
        yscale    < o.yscale    || (!(o.yscale  < yscale)   && (
        yscale    < o.yscale    || (!(o.yscale  < yscale)   && (
        color     < o.color     || (!(o.color   < color)    && (
        name      < o.name      || (!(o.name    < name)     && (
        id        < o.id        ))))))))))))))))))));
  }

  string DebugGloss() const {
    return bg_name + " at depth " + std::to_string(depth);
  }
};

const InstSimple InstSimple::DEFAULTS {Room::Instance::default_instance()};
const TileSimple TileSimple::DEFAULTS {Room::Tile::default_instance()};

string SVGDInstanceDescriptors(
    char ich, const InstSimple &inst, const InstSimple &prev) {
  std::stringstream str;
  str << ich;
  if (inst.id) str << ' ' << inst.id;
  if (inst.name.size()) str << " n \"" <<  EscapeString(inst.name) << "\"";
  if (inst.object_name != prev.object_name)
    str << " o \"" << EscapeString(inst.object_name) << "\"";
  int scales = (inst.xscale != prev.xscale)
             + (inst.yscale != prev.yscale)
             + (inst.zscale != prev.zscale);
  if (scales) {
    if (scales > 1) {
      str << " S " << inst.xscale;
      if (inst.yscale != inst.xscale) {
        str << ", " << inst.yscale;
        if (inst.zscale != prev.zscale) str << ", " << inst.zscale;
      } else if (inst.zscale != inst.xscale && prev.has_zscale) {
        str << ", " << inst.yscale << ", " << inst.zscale;
      }
    } else {
      if (inst.xscale != prev.xscale) str << " W " << inst.xscale;
      if (inst.yscale != prev.yscale) str << " H " << inst.yscale;
      if (inst.zscale != prev.zscale) str << " D " << inst.zscale;
    }
  }
  if (inst.rotation || inst.zrotation) {
    str << " R " << inst.rotation;
    if (inst.zrotation) str << ", " << inst.zrotation;
  }
  if (inst.color != prev.color) str << " C " << ColorToStr(inst.color);
  return str.str();
}

string SVGDBegin(const InstSimple &inst, const InstSimple &prev) {
  string rel = SVGDInstanceDescriptors('i', inst, prev);
  string abs = SVGDInstanceDescriptors('I', inst, InstSimple::DEFAULTS);
  return abs.length() <= rel.length() ? abs : rel;
}

template<typename T>
void SVGDWriteHVComponents(std::stringstream &str,
                           char bothc, char hc, char vc, T h, T v, T hp, T vp) {
  if (h != hp && v != vp) {
    str << ' ' << bothc << ' ' << h << ", " << v;
  } else if (h != hp) {
    str << ' ' << hc << ' ' << h;
  } else if (v != vp) {
    str << ' ' << vc << ' ' << v;
  }
}

string SVGDTileDescriptors(
    char ich, const TileSimple &tile, const TileSimple &prev) {
  std::stringstream str;
  str << ich;
  if (tile.id) str << ' ' << tile.id;
  if (tile.name.size()) str << " n \"" <<  EscapeString(tile.name) << "\"";
  if (tile.depth != prev.depth)
    str << " d " << tile.depth;
  if (tile.bg_name != prev.bg_name)
    str << " b \"" << EscapeString(tile.bg_name) << "\"";
  if (tile.xoffset != prev.xoffset && tile.yoffset != prev.yoffset) {
    std::stringstream abs, rel;
    SVGDWriteHVComponents(abs, 'O', 'U', 'V', tile.width, tile.height,
                          TileSimple::DEFAULTS.width, TileSimple::DEFAULTS.height);
    SVGDWriteHVComponents(rel, 'o', 'u', 'v', tile.width, tile.height,
                          prev.width, prev.height);
    string abss = abs.str(), rels = rel.str();
    str << (abss.length() < rels.length() ? abss : rels);
  }
  SVGDWriteHVComponents(str, 'D', 'w', 'h', tile.width, tile.height,
                        TileSimple::DEFAULTS.width,  TileSimple::DEFAULTS.height);
  SVGDWriteHVComponents(str, 'S', 'W', 'H', tile.xscale, tile.yscale,
                        TileSimple::DEFAULTS.xscale, TileSimple::DEFAULTS.yscale);
  if (tile.color != prev.color) str << " C " << ColorToStr(tile.color);
  return str.str();
}

string SVGDBegin(const TileSimple &inst, const TileSimple &prev) {
  string rel = SVGDTileDescriptors('t', inst, prev);
  string abs = SVGDTileDescriptors('T', inst, TileSimple::DEFAULTS);
  return abs.length() <= rel.length() ? abs : rel;
}

void WriteSVGDCoords(std::stringstream &str, const Coords &coords) {
  str << coords.x;
  if (coords.y || coords.z) str << ", " << coords.y;
  if (coords.z) str << ", " << coords.z;
}

string ShorterSVGDMove(char absc, char relc, const Coords &to, const Coords &from) {
  std::stringstream abs, rel;
  abs << ' ' << absc << ' '; WriteSVGDCoords(abs, to);
  rel << ' ' << relc << ' '; WriteSVGDCoords(rel, to - from);
  string rabs = abs.str(), rrel = rel.str();
  return rabs.length() <= rrel.length() ? rabs : rrel;
}

string SVGDComponentMove(char abs, char rel, double comp, double prev) {
  std::stringstream sabs;
  std::stringstream srel;
  sabs << ' ' << abs << ' ' << comp;
  srel << ' ' << rel << ' ' << comp - prev;
  string rabs = sabs.str();
  string rrel = srel.str();
  return rabs.length() <= rrel.length() ? rabs : rrel;
}

string SVGDMove(const Coords &coords, const Coords &prev) {
  std::stringstream rel, abs, comp;
  bool xc = (coords.x != prev.x);
  bool yc = (coords.y != prev.y);
  bool zc = (coords.z != prev.z);

  if (xc) comp << SVGDComponentMove('X', 'x', coords.x, prev.x);
  if (yc) comp << SVGDComponentMove('Y', 'y', coords.y, prev.y);
  if (zc) comp << SVGDComponentMove('Z', 'z', coords.z, prev.z);

  rel << " p " << coords.x - prev.x << ", " << coords.y - prev.y;
  abs << " P " << coords.x << ", " << coords.y;
  if (zc) {
    rel << ", " << coords.z - prev.z;
    abs << ", " << coords.z;
  }

  string rabs = abs.str(), rrel = rel.str(), rcomp = comp.str();
  if (rabs.length() <= rrel.length() && rabs.length() <= rcomp.length())
    return rabs;
  if (rrel.length() <= rcomp.length())
    return rrel;
  return rcomp;
}

Room::Instance StripForLump(const Room::Instance &inst) {
  Room::Instance res = InstSimple::Subtract(inst);
  res.clear_name(), res.clear_id();
  res.clear_x(), res.clear_y(), res.clear_z();
  if (MessageDifferencer::Equivalent(
          inst.editor_settings(),
          Room::Instance::EditorSettings::default_instance())) {
    res.clear_editor_settings();
  }
  res.clear_creation_code();

  // TODO: this should be an option or otherwise not exist
  res.clear_id();

  return res;
}

Room::Tile StripForLump(const Room::Tile &inst) {
  Room::Tile res = TileSimple::Subtract(inst);
  res.clear_name(), res.clear_id();
  res.clear_x(), res.clear_y();
  if (MessageDifferencer::Equivalent(
          inst.editor_settings(),
          Room::Tile::EditorSettings::default_instance())) {
    res.clear_editor_settings();
  }

  return res;
}

std::vector<Line> ExtractLines(std::multiset<Coords> *point_cloud) {
  std::vector<Line> res, add;
  while ((add = ConnectTheDots(*point_cloud)).size()) {
    for (const Line &line : add) {
      res.push_back(line);
      for (const Coords &coord : line.points) {
        auto find = point_cloud->find(coord);
        if (find == point_cloud->end()) {
          std::cerr << "Internal error: point from line cluster doesn't exist "
                       "or has gone missing from point cloud." << std::endl;
        } else {
          point_cloud->erase(find);
        }
      }
    }
  }
  return res;
}

template<typename T> struct Simplifier {};
template<> struct Simplifier<Room::Instance> { typedef InstSimple T; };
template<> struct Simplifier<Room::Tile> { typedef TileSimple T; };
template<typename T> using Simplify = typename Simplifier<T>::T;

template<typename T> struct LayerT {};
template<> struct LayerT<Room::Instance> { using T = egm::InstanceLayer; };
template<> struct LayerT<Room::Tile> { using T = egm::TileLayer; };
template<typename T> using Layer = typename LayerT<T>::T;


using std::vector;
std::pair<double, bool> strintodou(const std::string &strin) {
  const char *end = strin.c_str();
  double res = strtod(end, (char**) &end);
  return std::pair<double, bool>(res, end != strin.c_str());
}

class Command {
 public:
  string name;
  Command(string name): name(name) {}
  virtual int handle(
      const vector<string> &data, size_t pos, protobuf::Message *out) const = 0;
  virtual ~Command() {}
};

class StringAttributeCommand : public Command {
 public:
  int field;

  StringAttributeCommand(string name_, int field_):
      Command(name_), field(field_) {}

  int handle(const vector<string> &data, size_t pos, protobuf::Message *out)
  const final {
    if (pos + 1 >= data.size()) {
      std::cerr << "Expected value for '" << name << "' command "
                   "before end of input" << std::endl;
      return 0;
    }
    SetProtoField(out, field, data[pos + 1]);
    return 1;
  }

  ~StringAttributeCommand() final {}
};

std::vector<double> ReadValues(string command_name, const vector<string> &data,
                               size_t pos, int countMin, int countMax) {
  std::vector<double> res;
  std::pair<double, bool> val;
  for (int i = 0; i < countMax; ++i) {
    if (pos + i >= data.size() || !(val = strintodou(data[pos + i])).second) {
      if (i < countMin) {
        std::cerr << "Expected " << countMin << " values for '" << command_name
                  << "' command; found " << i << std::endl;
      }
      return res;
    }
    res.push_back(val.first);
  }
  return res;
}

class SingleAttributeCommand : public Command {
 public:
  int field;
  bool relative;

  SingleAttributeCommand(string name_, int field_, bool relative_):
      Command(name_), field(field_), relative(relative_) {}

  int handle(const vector<string> &data, size_t pos, protobuf::Message *out)
  const final {
    std::pair<double, bool> val;
    if (pos + 1 >= data.size() || !(val = strintodou(data[pos + 1])).second) {
      std::cerr << "Expected value for '" << name << "' command" << std::endl;
      return 0;
    }
    if (relative) val.first += GetNumericProtoField(*out, field);
    SetProtoField(out, field, val.first);
    return 1;
  }

  ~SingleAttributeCommand() final {}
};

class CoordinateAttributeCommand : public Command {
 public:
  int field1, field2, field3;
  bool relative;

  CoordinateAttributeCommand(string name_,
                             int field_1, int field_2, int field_3, bool rel_):
      Command(name_),
      field1(field_1), field2(field_2), field3(field_3), relative(rel_) {}

  int handle(const vector<string> &data, size_t pos, protobuf::Message *out)
  const final {
    auto args = ReadValues(name, data , pos+ 1, 2, 3);
    if (!args.size()) return 0;
    if (relative) args[0] += GetNumericProtoField(*out, field1);
    SetProtoField(out, field1, args[0]);

    if (args.size() == 1) return 1;
    if (relative) args[1] += GetNumericProtoField(*out, field2);
    SetProtoField(out, field2, args[1]);

    if (args.size() == 2) return 2;
    if (relative) args[2] += GetNumericProtoField(*out, field3);
    SetProtoField(out, field3, args[2]);
    return 3;
  }
  ~CoordinateAttributeCommand() final {}
};

class RotationCommand : public Command {
 public:
  RotationCommand(): Command("Rotation") {}

  int handle(const vector<string> &data, size_t pos, protobuf::Message *out)
  const final {
    if (pos + 1 >= data.size()) {
      std::cerr << "Expected value for '" << name << "' command "
                   "before end of input" << std::endl;
      return 0;
    }
    SetProtoField(out, Room::Instance::kRotationFieldNumber, data[pos + 1]);
    return 1;
  }

  ~RotationCommand() final {}
};

class ColorCommand : public Command {
 public:
  int field;

  ColorCommand(int field_): Command("color"), field(field_) {}

  int handle(const vector<string> &data, size_t pos, protobuf::Message *out)
  const final {
    if (pos + 1 >= data.size()) {
      std::cerr << "Expected value for '" << name << "' command "
                   "before end of input" << std::endl;
      return 0;
    }
    SetProtoField(out, field, data[pos + 1]);
    return 1;
  }

  ~ColorCommand() final {}
};

const std::map<char, Command*> kInstanceParameters = {
  { 'n', new StringAttributeCommand{
      "name", Room::Instance::kNameFieldNumber                 }},
  { 'o', new StringAttributeCommand{
      "object-type", Room::Instance::kObjectTypeFieldNumber    }},
  { 'P', new CoordinateAttributeCommand{
      "Position", Room::Instance::kXFieldNumber,
                  Room::Instance::kYFieldNumber,
                  Room::Instance::kZFieldNumber, false         }},
  { 'X', new SingleAttributeCommand{
      "X", Room::Instance::kXFieldNumber, false                }},
  { 'Y', new SingleAttributeCommand{
      "Y", Room::Instance::kYFieldNumber, false                }},
  { 'Z', new SingleAttributeCommand{
      "Z", Room::Instance::kZFieldNumber, false                }},
  { 'p', new CoordinateAttributeCommand{
      "position", Room::Instance::kXFieldNumber,
                  Room::Instance::kYFieldNumber,
                  Room::Instance::kZFieldNumber, true          }},
  { 'x', new SingleAttributeCommand{
      "x", Room::Instance::kXFieldNumber, true                 }},
  { 'y', new SingleAttributeCommand{
      "y", Room::Instance::kYFieldNumber, true                 }},
  { 'z', new SingleAttributeCommand{
      "z", Room::Instance::kZFieldNumber, true                 }},
  { 'S', new CoordinateAttributeCommand{
      "Scale", Room::Instance::kXscaleFieldNumber,
               Room::Instance::kYscaleFieldNumber,
               Room::Instance::kZscaleFieldNumber, false       }},
  { 'W', new SingleAttributeCommand{
      "x-scale (W)", Room::Instance::kXscaleFieldNumber, false }},
  { 'H', new SingleAttributeCommand{
      "y-scale (H)", Room::Instance::kYscaleFieldNumber, false }},
  { 'D', new SingleAttributeCommand{
      "z-scale (D)", Room::Instance::kZscaleFieldNumber, false }},
  { 'R', new RotationCommand{                                  }},
  { 'c', new ColorCommand{ Room::Instance::kColorFieldNumber   }},
};

const std::map<char, Command*> kTileParameters = {
  { 'n', new StringAttributeCommand{
      "name", Room::Tile::kNameFieldNumber                     }},
  { 'b', new StringAttributeCommand{
      "background", Room::Tile::kBackgroundNameFieldNumber     }},
  { 'P', new CoordinateAttributeCommand{
      "Position", Room::Tile::kXFieldNumber,
                  Room::Tile::kYFieldNumber, 0, false          }},
  { 'X', new SingleAttributeCommand{
      "X",        Room::Tile::kXFieldNumber, false             }},
  { 'Y', new SingleAttributeCommand{
      "Y",        Room::Tile::kYFieldNumber, false             }},
  { 'p', new CoordinateAttributeCommand{
      "position", Room::Tile::kXFieldNumber,
                  Room::Tile::kYFieldNumber, 0, true           }},
  { 'x', new SingleAttributeCommand{
      "x",        Room::Tile::kXFieldNumber, true              }},
  { 'y', new SingleAttributeCommand{
      "y",        Room::Tile::kYFieldNumber, true              }},
  { 'O', new CoordinateAttributeCommand{
      "Offset",   Room::Tile::kXoffsetFieldNumber,
                  Room::Tile::kYoffsetFieldNumber, 0, false    }},
  { 'U', new SingleAttributeCommand{
      "U-coord",  Room::Tile::kXoffsetFieldNumber, false       }},
  { 'V', new SingleAttributeCommand{
      "V-coord",  Room::Tile::kYoffsetFieldNumber, false       }},
  { 'o', new CoordinateAttributeCommand{
      "offset",   Room::Tile::kXoffsetFieldNumber,
                  Room::Tile::kYoffsetFieldNumber, 0, true     }},
  { 'u', new SingleAttributeCommand{
      "u-coord",  Room::Tile::kXoffsetFieldNumber, true        }},
  { 'v', new SingleAttributeCommand{
      "v-coord",  Room::Tile::kYoffsetFieldNumber, true        }},
  { 'D', new CoordinateAttributeCommand{
      "Dimensions", Room::Tile::kWidthFieldNumber,
                    Room::Tile::kHeightFieldNumber, 0, false   }},
  { 'w', new SingleAttributeCommand{
      "width",      Room::Tile::kWidthFieldNumber,  false      }},
  { 'h', new SingleAttributeCommand{
      "height",     Room::Tile::kHeightFieldNumber, false      }},
  { 'S', new CoordinateAttributeCommand{
      "Scale",       Room::Tile::kWidthFieldNumber,
                     Room::Tile::kHeightFieldNumber, 0, false  }},
  { 'W', new SingleAttributeCommand{
      "x-scale (W)", Room::Tile::kXscaleFieldNumber, false     }},
  { 'H', new SingleAttributeCommand{
      "y-scale (H)", Room::Tile::kYscaleFieldNumber, false     }},
  { 'c', new ColorCommand{ Room::Tile::kColorFieldNumber       }},
};

vector<string> TokenizeSVGD(const string &data) {
  vector<string> res;
  for (size_t i = 0; i < data.length();) {
    const auto io = i++;
    const char c = data[io];
    if (isspace(c) || c == ',' || c == ';') {
      continue;
    }
    if (isdigit(c)) {
      while (i < data.length() && isdigit(data[i])) ++i;
      if (i < data.length() && data[i] == '.') {
        while (++i < data.length() && isdigit(data[i]));
      }
      res.push_back(data.substr(io, i - io));
      continue;
    }
    if (isalpha(c)) {
      res.push_back(string(1, c));
      continue;
    }
    if (c == '+' || c == '-') {
      const string sign(1, c);
      while (i < data.length() && isspace(data[i])) ++i;
      const auto io = i;
      if (i >= data.length() || !isdigit(data[i])) {
        std::cerr << "Expected numeric literal in SVG-d input (at position "
                  << i << ')' << std::endl;
      }
      while (i < data.length() && isdigit(data[i])) ++i;
      if (i < data.length() && data[i] == '.') {
        while (++i < data.length() && isdigit(data[i]));
      }
      res.push_back(sign + data.substr(io, i - io));
      continue;
    }
    if (c == '"' || c == '\'') {
      auto str_i = ReadQuotedString(data, io);
      res.push_back(str_i.first);
      i = str_i.second + 1;
      continue;
    }
    std::cerr << "Unexpected character '" << c << "' in SVG-d data" << std::endl;
  }
  return res;
}

// =============================================================================
// == Construction Logic =======================================================
// =============================================================================

template<typename Entity, typename Simple = Simplify<Entity>>
Layer<Entity> BuildSVGDLayer(const RepeatedPtrField<Entity> &entities) {
  Layer<Entity> res;
  res.format = "svg-d";

  // Instances which have no uniquely-identifying characteristics and therefore
  // can be lumped into various RLE schemes. Right now, those schemes consist
  // exclusively of lines.
  std::map<Simple, std::multiset<Coords>> lumpable;

  // Sort out snowflakes (Instances which cannot be lumped).
  for (const Entity &entity : entities) {
    if (!StripForLump(entity).ByteSizeLong()) {
      lumpable[entity].insert(Coords{entity});
    } else {
      std::cout << "INFO: Cannot lump instance because it has special fields.\n"
                << StripForLump(entity).DebugString() << std::endl;
      res.snowflakes.push_back(entity);
    }
  }

  std::stringstream layer;

  string separator = "";
  const Simple *last_attribs = &Simple::DEFAULTS;
  Coords last_position = { 0, 0, 0 };
  for (auto &group : lumpable) {
    const Simple &attribs = group.first;
    std::multiset<Coords> &point_cloud = group.second;
    if (kIncrediblyVerboseDebugOutput) {
      std::cout << "########################################################\n"
                << "Group of " << attribs.DebugGloss() << "\n"
                << "########################################################\n";
    }

    std::vector<Line> lines = ExtractLines(&point_cloud);
    if (kIncrediblyVerboseDebugOutput)
      std::cout << "Extracted " << lines.size() << " lines." << std::endl;
    for (const Line &line : lines) {
      layer << separator; separator = "\n";
      layer << SVGDBegin(attribs, *last_attribs);
      last_attribs = &attribs;
      layer << SVGDMove(line.first(), last_position);
      layer << ShorterSVGDMove('L', 'l', line.last(), line.first());
      layer << " g "; WriteSVGDCoords(layer, line.slope);
      // layer << " # " << line.points.size() << " instances";
      last_position = line.last();
    }
    for (const Coords &rc : point_cloud) {
      layer << separator << SVGDBegin(attribs, *last_attribs);
      layer << SVGDMove(rc, last_position);
      last_attribs = &attribs;
      last_position = rc;
      separator = "\n";
    }
  }

  res.data = layer.str();
  return res;
}

// =============================================================================
// == Deconstruction Logic =====================================================
// =============================================================================

void RepackLayer(string layer_data,
                 protobuf::Message *dest_message,
                 const protobuf::FieldDescriptor *dest_field,
                 const protobuf::Message &default_inst,
                 char create_cmd, const std::map<char, Command*> &commands,
                 const string &error_details) {
  // General setup. Used to add repeated fields when encountering create_cmd.
  const auto *const refl = dest_message->GetReflection();
  char clone_cmd = create_cmd + 'a' - 'A';

  // This makes it valid to assign stuff before instantiating anything
  std::unique_ptr<protobuf::Message> sentinel(default_inst.New());
  protobuf::Message *cur_inst = sentinel.get();
  const protobuf::Message *prev_inst = cur_inst;

  // Reflection methods
  const protobuf::FieldDescriptor
      *id_field = sentinel->GetDescriptor()->FindFieldByName("id"),
      *name_field = sentinel->GetDescriptor()->FindFieldByName("name"),
      *x_field = sentinel->GetDescriptor()->FindFieldByName("x"),
      *y_field = sentinel->GetDescriptor()->FindFieldByName("y"),
      *z_field = sentinel->GetDescriptor()->FindFieldByName("z");

  auto CopyFields = [id_field, name_field](
      const protobuf::Message *from, protobuf::Message *to) {
    to->MergeFrom(*from);
    to->GetReflection()->ClearField(to, id_field);
    to->GetReflection()->ClearField(to, name_field);
  };
  auto CloneInstance = [&](const protobuf::Message *inst, int x, int y, int z) {
    auto *n = refl->AddMessage(dest_message, dest_field);
    CopyFields(inst, n);
    SetProtoField(n, x_field, x);
    SetProtoField(n, y_field, y);
    if (z_field) SetProtoField(n, z_field, z);
  };

  // Other parse state: Granularity.
  int gran_x = 1, gran_y = 1, gran_z = 1;

  // Tokenize and iterate.
  auto data = TokenizeSVGD(layer_data);
  for (size_t i = 0; i < data.size(); ++i) {
    const string &tk = data[i];
    if (tk.length() != 1 || !isalpha(tk[0])) {
      std::cerr << "Unexpected token \"" << tk << "\" (token " << i << ")"
                << std::endl;
      if (!error_details.empty()) std::cerr << error_details << std::endl;
      continue;
    }

    const char cmdch = tk[0];
    auto fnd = commands.find(cmdch);
    if (fnd != commands.end()) {
      const Command *cmd = fnd->second;
      i += cmd->handle(data, i, cur_inst);
      continue;
    }

    if (cmdch == create_cmd || cmdch == clone_cmd) {
      prev_inst = cur_inst;
      cur_inst = refl->AddMessage(dest_message, dest_field);
      if (cmdch == clone_cmd) CopyFields(prev_inst, cur_inst);
      if (i + 1 < data.size() && isdigit(*data[i + 1].c_str())) {
        int id = atoi(data[i + 1].c_str());
        cur_inst->GetReflection()->SetInt32(cur_inst, id_field, id);
        ++i;
      }
      continue;
    }

    // Other special cases
    switch(cmdch) {
      case 'l': case 'f': {
        auto coords = ReadValues("line", data, i + 1, 2, z_field ? 3 : 2);
        if (coords.size() < 2) continue;
        double x = GetNumericProtoField(*cur_inst, x_field);
        double y = GetNumericProtoField(*cur_inst, y_field);
        double z = z_field ? GetNumericProtoField(*cur_inst, y_field) : 0;
        double xto = coords[0];
        double yto = coords[1];
        double zto = coords.size() > 2 ? coords[2] : z;
        if (x > xto) std::swap(x, xto);
        if (y > yto) std::swap(y, yto);
        if (z > zto) std::swap(z, zto);
        if (cmdch == 'l') {
          while ((gran_x && (x += gran_x) <= xto)
              || (gran_y && (y += gran_y) <= yto)
              || (gran_z && (z += gran_z) <= zto)) {
            CloneInstance(cur_inst, x, y, z);
          }
        } else {
          for (double xi = x; xi <= xto; xi += gran_x) {
            for (double yi = y; yi <= yto; yi += gran_y) {
              for (double zi = z; zi <= zto; zi += gran_z) {
                CloneInstance(cur_inst, xi, yi, zi);
                if (!gran_z) break;
              }
              if (!gran_y) break;
            }
            if (!gran_x) break;
          }
        }
        continue;
      }
      case 'g': {
        auto grans = ReadValues("granularity", data, i + 1, 2, z_field ? 3 : 2);
        if (grans.size()) {
          gran_x = grans[0];
          if (gran_x < 0) gran_x = -gran_x;
          if (grans.size() > 1) {
            gran_y = grans[1];
            if (gran_y < 0) gran_x = -gran_y;
            if (grans.size() > 2) {
              gran_y = grans[2];
              if (gran_z < 0) gran_z = -gran_z;
            }
          }
        }
        continue;
      }
      default: ;
    }
    std::cerr << "Error: unsupported command \"" << cmdch << "\"" << std::endl;
    if (!error_details.empty()) std::cerr << error_details << std::endl;
  }
}

}  // namespace

namespace egm {
namespace svg_d {

InstanceLayer BuildLayer(const RepeatedPtrField<Room::Instance> &instances) {
  return BuildSVGDLayer(instances);
}
TileLayer     BuildLayer(const RepeatedPtrField<Room::Tile> &tiles) {
  return BuildSVGDLayer(tiles);
}

void ParseInstances(
    const string &layer_data, Room *dest_message, const string &error_details) {
  auto *instances = dest_message->GetDescriptor()->FindFieldByName("instances");
  if (!instances) {
    std::cerr << "Internal error: "
                 "Cannot find `instances' field of Room message." << std::endl;
    return;
  }
  return RepackLayer(layer_data, dest_message,
                     instances, Room::Instance::default_instance(),
                     'I', kInstanceParameters, error_details);
}

void ParseTiles(
    const string &layer_data, Room *dest_message, const string &error_details) {
  auto *tiles = dest_message->GetDescriptor()->FindFieldByName("tiles");
  if (!tiles) {
    std::cerr << "Internal error: "
                 "Cannot find `tiles' field of Room message." << std::endl;
    return;
  }
  return RepackLayer(layer_data, dest_message,
                     tiles, Room::Tile::default_instance(),
                     'T', kTileParameters, error_details);
}


}  // namespace svg_d
}  // namespace egm


