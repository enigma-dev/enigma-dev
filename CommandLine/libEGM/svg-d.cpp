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
#include "Util.h"

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
  int32_t color;
  string  name;
  int32_t id;

  InstSimple(const Room::Instance &inst):
      object_name(inst.object_type()),
      xscale(inst.xscale()), yscale(inst.yscale()), zscale(inst.zscale()),
      rotation(inst.rotation()), zrotation(inst.zrotation()),
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

string SVGDInstanceDescriptors(char ich, const InstSimple &inst, const InstSimple &prev) {
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
      str << " S " << inst.xscale << ", " << inst.yscale;
      if (inst.zscale != 1)   str << ", " << inst.zscale;
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

string SVGDTileDescriptors(char ich, const TileSimple &tile, const TileSimple &prev) {
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
    //TODO: ask josh about abs which used to be in place of str for these two
    SVGDWriteHVComponents(str, 'O', 'U', 'V', tile.width, tile.height,
                          TileSimple::DEFAULTS.width, TileSimple::DEFAULTS.height);
    SVGDWriteHVComponents(str, 'o', 'u', 'v', tile.width, tile.height,
                          prev.width, prev.height);
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
  res.clear_code();

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


struct Command {
  size_t minArgs = 0;
  std::vector<std::string> field_names;
};

const std::map<char, Command> kInstanceParameters = {
  { 'I', { 0, {"id"}                         } },
  { 'i', { 0, {"id"}                         } },
  { 'n', { 1, {"name"}                       } },
  { 'o', { 1, {"object_type"}                } },
  { 'p', { 1, {"x", "y", "z"}                } },
  { 'x', { 1, {"x"}                          } },
  { 'y', { 1, {"y"}                          } },
  { 'z', { 1, {"z"}                          } },
  { 's', { 1, {"xscale", "yscale", "zscale"} } },
  { 'w', { 1, {"xscale"}                     } },
  { 'h', { 1, {"yscale"}                     } },
  { 'd', { 1, {"zscale"}                     } },
  { 'r', { 1, {"rotation"}                   } },
  { 'c', { 1, {"color"}                      } },
  { 'l', { 1, {}                             } },
  { 'f', { 4, {}                             } },
  { 'g', { 1, {}                             } },
};

const std::map<char, Command> kTileParameters = {
  { 'T', { 0, {"id"}                 } },
  { 't', { 0, {"id"}                 } },
  { 'n', { 1, {"name"}               } },
  { 'b', { 1, {"background_name"}    } },
  { 'd', { 1, {"depth"}              } },
  { 'p', { 1, {"x", "y"}             } },
  { 'x', { 1, {"x"}                  } },
  { 'y', { 1, {"y"}                  } },
  { 'o', { 1, {"xoffset", "yoffset"} } },
  { 'u', { 1, {"xoffset"}            } },
  { 'v', { 1, {"yoffset"}            } },
  { 'D', { 1, {"width", "height"}    } },
  { 'w', { 1, {"width"}              } },
  { 'h', { 1, {"height"}             } },
  { 's', { 1, {"xscale", "yscale"}   } },
  { 'W', { 1, {"xscale"}             } },
  { 'H', { 1, {"yscale"}             } },
  { 'c', { 1, {"color"}              } },
  { 'l', { 1, {}                     } },
  { 'f', { 4, {}                     } },
  { 'g', { 1, {}                     } },
};


template<typename T> struct Simplifier {};
template<> struct Simplifier<Room::Instance> { typedef InstSimple T; };
template<> struct Simplifier<Room::Tile> { typedef TileSimple T; };
template<typename T> using Simplify = typename Simplifier<T>::T;

template<typename T> struct LayerT {};
template<> struct LayerT<Room::Instance> { using T = egm::InstanceLayer; };
template<> struct LayerT<Room::Tile> { using T = egm::TileLayer; };
template<typename T> using Layer = typename LayerT<T>::T;


inline bool isCommandToken(const std::string& token) {
  return (token.length() == 1 && isalpha(token[0]));
}

inline void tooManyArgsGiven(char cmd, unsigned maxArgs,
    const std::vector<std::string>& args, unsigned argNum,
    const string& error_details) {
  std::cerr << std::endl << "Error: too many arguments given to the \""
            << cmd << "\" command" << std::endl;
  std::cerr << "maximum supported arguments: " << maxArgs << std::endl;
  std::cerr << "argument " << argNum
            << " (" << args[argNum] << ") will be ignored" << std::endl;
  if (!error_details.empty()) std::cerr << error_details << std::endl;
  std::cerr << std::endl;
}

inline void tooFewArgsGiven(char cmd, unsigned minArgs,
    const string& error_details) {
  std::cerr << std::endl << "Error: too few arguments given to the \""
            << cmd << "\" command" << std::endl;
  std::cerr << "minimum required arguments: " << minArgs << std::endl;
  std::cerr << "the command will be ignored" << std::endl;
  if (!error_details.empty()) std::cerr << error_details << std::endl;
  std::cerr << std::endl;
}

// =============================================================================
// == Construction Logic =======================================================
// =============================================================================


template<typename Entity, typename Simple = Simplify<Entity>>
Layer<Entity> BuildSVGDLayer(const RepeatedPtrField<Entity> &entities) {
  Layer<Entity> res;

  // Instances which have no uniquely-identifying characteristics and therefore
  // can be lumped into various RLE schemes. Right now, those schemes consist
  // exclusively of lines.
  std::map<Simple, std::multiset<Coords>> lumpable;

  // Sort out snowflakes (Instances which cannot be lumped).
  for (const Entity &entity : entities) {
    if (!StripForLump(entity).ByteSize()) {
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
    protobuf::Message *dest_message, const protobuf::FieldDescriptor *dest_field,
    char createCMD, const std::map<char, Command>& parameters,
    const string &error_details) {

  const google::protobuf::Reflection *refl = dest_message->GetReflection();

  // split at spaces or comma etc
  std::vector<std::string> tokens = Tokenize(layer_data, " ,\n\t");

  google::protobuf::Message* currInstance = nullptr;
  std::vector<google::protobuf::Message*> instances;

  char currentCmd = '\0';
  std::vector<std::pair<char, std::vector<std::string> > > arguments;

  // Group all our commands into <char, vector<string>> (cmd, args)
  auto cmdIt = arguments.end();
  for (unsigned i = 0; i < tokens.size(); ++i) {
    std::string token = std::string(tokens[i]);
    if (isCommandToken(token)) {
      currentCmd = token[0];
      arguments.emplace_back(currentCmd, std::vector<std::string>());
      cmdIt = arguments.end() - 1;
    } else if (cmdIt != arguments.end()) {
      (*cmdIt).second.emplace_back(token);
    }
  }

  // Proccess commands here
  for (auto cmdPair : arguments) {
    char cmd = cmdPair.first;
    std::vector<std::string> args = cmdPair.second;

    auto pit = parameters.find(cmd);
    bool relative = islower(cmd); //TODO: fix color
    if (pit == parameters.end()) {
      // if this command does not have uppercase equivalent
      // we can still allow a lowercase version for relative
      pit = parameters.find(cmd = tolower(cmd));
      if (pit == parameters.end()) {
        // no upper or lowercase version seems to be supported...
        std::cerr << "Error: unsupported command \"" << cmd << "\"" << std::endl;
        if (!error_details.empty()) std::cerr << error_details << std::endl;
        continue;
      }
    }
    auto sig = (*pit).second;
    auto pars = sig.field_names;

    // Too few args, continue here
    if (args.size() < sig.minArgs) {
      tooFewArgsGiven(cmd, sig.minArgs, error_details);
      continue;
    }

    // Special case for create cmds
    if (tolower(cmd) == tolower(createCMD)) {
        currInstance = refl->AddMessage(dest_message, dest_field);

        if (cmd == tolower(createCMD)) {
          if (!instances.empty()) {
            currInstance->CopyFrom(*instances.back());
            const google::protobuf::Descriptor *instDesc = currInstance->GetDescriptor();
            const google::protobuf::Reflection *instRefl = currInstance->GetReflection();

            // Don't copy this crap
            for (const std::string& field_name : {"id", "name", "code"}) {
              const google::protobuf::FieldDescriptor *instField = instDesc->FindFieldByName(field_name);
              if (instField != nullptr)
                instRefl->ClearField(currInstance, instField);
            }
          }
          else
            std::cerr << "Error: \"" << cmd << "\" called but there exists no previous instance to copy the attributes from" << std::endl;
        }

        instances.push_back(currInstance);
    }

    // Other special cases
    switch(cmd) {
      case 'l': {
        const protobuf::Descriptor *instDesc = currInstance->GetDescriptor();
        const protobuf::Reflection *instRefl = currInstance->GetReflection();
        const protobuf::FieldDescriptor
            *xField = instDesc->FindFieldByName("x"),
            *yField = instDesc->FindFieldByName("y"),
            *xScaleField = instDesc->FindFieldByName("xscale"),
            *yScaleField = instDesc->FindFieldByName("yscale"),
            *widthField = instDesc->FindFieldByName("width"),
            *heightField = instDesc->FindFieldByName("height");
        double xfrom = instRefl->GetDouble(*currInstance, xField),
               yfrom = instRefl->GetDouble(*currInstance, yField),
               // TODO: xscale and yscale need defaults of 1 not 0
               width = instRefl->GetUInt32(*currInstance, widthField),   // * instRefl->GetDouble(*currInstance, xScaleField),
               height = instRefl->GetUInt32(*currInstance, heightField), // * instRefl->GetDouble(*currInstance, yScaleField),
               xto = stoi(args[0]),
               yto = (args.size() > 1) ? stoi(args[1]) : yfrom,
               dx = xto - xfrom,
               dy = yto - yfrom;

        // start at plus width because the first tile is already there
        // we're just copying from it to complete the rest of the line
        for (double x = xfrom + width; x <= xto; x += width) {
          double y = yfrom + dy * (x - xfrom) / dx;
          std::cerr << "bing " << x << " " << y << std::endl;

          currInstance = refl->AddMessage(dest_message, dest_field);
          currInstance->CopyFrom(*instances.back());
          instances.push_back(currInstance);

          instRefl = currInstance->GetReflection();
          instRefl->SetDouble(currInstance, xField, x);
          instRefl->SetDouble(currInstance, yField, y);
        }
        continue;
      }
      case 'f':
        continue;
      case 'g':
        continue;
    }

    // General case for rest of command args
    for (size_t i = 0; i < args.size(); ++i) {
      if (i < pars.size()) {
        std::string field_name = pars[i];
        std::string arg = args[i];

        const google::protobuf::Descriptor *desc = currInstance->GetDescriptor();
        const google::protobuf::Reflection *refl = currInstance->GetReflection();
        const google::protobuf::FieldDescriptor *field = desc->FindFieldByName(field_name);

        // allow all integer parameters to have hexadecimal formatted arguments
        switch (field->cpp_type()) {
          case CppType::CPPTYPE_INT32:
          case CppType::CPPTYPE_INT64:
          case CppType::CPPTYPE_UINT32:
          case CppType::CPPTYPE_UINT64:
            arg = escape_hex(arg);
            break;
          default:
            break;
        };

        SetProtoField(refl, currInstance, field, unquote(arg), relative);
      } else {
        tooManyArgsGiven(cmd, pars.size(), args, i, error_details);
      }
    }
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
                     instances, 'I', kInstanceParameters, error_details);
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
                     tiles, 'T', kTileParameters, error_details);
}


}  // namespace svg_d
}  // namespace egm


