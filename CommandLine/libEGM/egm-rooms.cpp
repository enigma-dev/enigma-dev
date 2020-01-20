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

namespace {

constexpr bool kIncrediblyVerboseDebugOutput = false;
constexpr bool kMinorlyVerboseDebuggingOutput = true;

// TODO: The following three functions don't belong here.
const std::map<int, string> kColorNames {
  { 0xFFF8F0, "Alice Blue"          }, { 0xD7EBFA, "Antique White"     },
  { 0xFFFF00, "Aqua"                }, { 0xD4FF7F, "Aquamarine"        },
  { 0xFFFFF0, "Azure"               }, { 0xDCF5F5, "Beige"             },
  { 0xC4E4FF, "Bisque"              }, { 0x000000, "Black"             },
  { 0xCDEBFF, "Blanched Almond"     }, { 0xFF0000, "Blue"              },
  { 0xE22B8A, "Blue Violet"         }, { 0x2A2AA5, "Brown"             },
  { 0x87B8DE, "Burlywood"           }, { 0xA09E5F, "Cadet Blue"        },
  { 0x00FF7F, "Chartreuse"          }, { 0x1E69D2, "Chocolate"         },
  { 0x507FFF, "Coral"               }, { 0xED9564, "Cornflower"        },
  { 0xDCF8FF, "Cornsilk"            }, { 0x3C14DC, "Crimson"           },
  { 0xFFFF00, "Cyan"                }, { 0x8B0000, "Dark Blue"         },
  { 0x8B8B00, "Dark Cyan"           }, { 0x0B86B8, "Dark Goldenrod"    },
  { 0xA9A9A9, "Dark Gray"           }, { 0x006400, "Dark Green"        },
  { 0x6BB7BD, "Dark Khaki"          }, { 0x8B008B, "Dark Magenta"      },
  { 0x2F6B55, "Dark Olive Green"    }, { 0x008CFF, "Dark Orange"       },
  { 0xCC3299, "Dark Orchid"         }, { 0x00008B, "Dark Red"          },
  { 0x7A96E9, "Dark Salmon"         }, { 0x8FBC8F, "Dark Sea Green"    },
  { 0x8B3D48, "Dark Slate Blue"     }, { 0x4F4F2F, "Dark Slate Gray"   },
  { 0xD1CE00, "Dark Turquoise"      }, { 0xD30094, "Dark Violet"       },
  { 0x9314FF, "Deep Pink"           }, { 0xFFBF00, "Deep Sky Blue"     },
  { 0x696969, "Dim Gray"            }, { 0xFF901E, "Dodger Blue"       },
  { 0x2222B2, "Firebrick"           }, { 0xF0FAFF, "Floral White"      },
  { 0x228B22, "Forest Green"        }, { 0xFF00FF, "Fuchsia"           },
  { 0xDCDCDC, "Gainsboro"           }, { 0xFFF8F8, "Ghost White"       },
  { 0x00D7FF, "Gold"                }, { 0x20A5DA, "Goldenrod"         },
  { 0xBEBEBE, "Gray"                }, { 0x808080, "Web Gray"          },
  { 0x00FF00, "Green"               }, { 0x008000, "Web Green"         },
  { 0x2FFFAD, "Green Yellow"        }, { 0xF0FFF0, "Honeydew"          },
  { 0xB469FF, "Hot Pink"            }, { 0x5C5CCD, "Indian Red"        },
  { 0x82004B, "Indigo"              }, { 0xF0FFFF, "Ivory"             },
  { 0x8CE6F0, "Khaki"               }, { 0xFAE6E6, "Lavender"          },
  { 0xF5F0FF, "Lavender Blush"      }, { 0x00FC7C, "Lawn Green"        },
  { 0xCDFAFF, "Lemon Chiffon"       }, { 0xE6D8AD, "Light Blue"        },
  { 0x8080F0, "Light Coral"         }, { 0xFFFFE0, "Light Cyan"        },
  { 0xD2FAFA, "Light Goldenrod"     }, { 0xD3D3D3, "Light Gray"        },
  { 0x90EE90, "Light Green"         }, { 0xC1B6FF, "Light Pink"        },
  { 0x7AA0FF, "Light Salmon"        }, { 0xAAB220, "Light Sea Green"   },
  { 0xFACE87, "Light Sky Blue"      }, { 0x998877, "Light Slate Gray"  },
  { 0xDEC4B0, "Light Steel Blue"    }, { 0xE0FFFF, "Light Yellow"      },
  { 0x00FF00, "Lime"                }, { 0x32CD32, "Lime Green"        },
  { 0xE6F0FA, "Linen"               }, { 0xFF00FF, "Magenta"           },
  { 0x6030B0, "Maroon"              }, { 0x00007F, "Web Maroon"        },
  { 0xAACD66, "Medium Aquamarine"   }, { 0xCD0000, "Medium Blue"       },
  { 0xD355BA, "Medium Orchid"       }, { 0xDB7093, "Medium Purple"     },
  { 0x71B33C, "Medium Sea Green"    }, { 0xEE687B, "Medium Slate Blue" },
  { 0x9AFA00, "Medium Spring Green" }, { 0xCCD148, "Medium Turquoise"  },
  { 0x8515C7, "Medium Violet Red"   }, { 0x701919, "Midnight Blue"     },
  { 0xFAFFF5, "Mint Cream"          }, { 0xE1E4FF, "Misty Rose"        },
  { 0xB5E4FF, "Moccasin"            }, { 0xADDEFF, "Navajo White"      },
  { 0x800000, "Navy Blue"           }, { 0xE6F5FD, "Old Lace"          },
  { 0x008080, "Olive"               }, { 0x238E6B, "Olive Drab"        },
  { 0x00A5FF, "Orange"              }, { 0x0045FF, "Orange Red"        },
  { 0xD670DA, "Orchid"              }, { 0xAAE8EE, "Pale Goldenrod"    },
  { 0x98FB98, "Pale Green"          }, { 0xEEEEAF, "Pale Turquoise"    },
  { 0x9370DB, "Pale Violet Red"     }, { 0xD5EFFF, "Papaya Whip"       },
  { 0xB9DAFF, "Peach Puff"          }, { 0x3F85CD, "Peru"              },
  { 0xCBC0FF, "Pink"                }, { 0xDDA0DD, "Plum"              },
  { 0xE6E0B0, "Powder Blue"         }, { 0xF020A0, "Purple"            },
  { 0x7F007F, "Web Purple"          }, { 0x993366, "Rebecca Purple"    },
  { 0x0000FF, "Red"                 }, { 0x8F8FBC, "Rosy Brown"        },
  { 0xE16941, "Royal Blue"          }, { 0x13458B, "Saddle Brown"      },
  { 0x7280FA, "Salmon"              }, { 0x60A4F4, "Sandy Brown"       },
  { 0x578B2E, "Sea Green"           }, { 0xEEF5FF, "Seashell"          },
  { 0x2D52A0, "Sienna"              }, { 0xC0C0C0, "Silver"            },
  { 0xEBCE87, "Sky Blue"            }, { 0xCD5A6A, "Slate Blue"        },
  { 0x908070, "Slate Gray"          }, { 0xFAFAFF, "Snow"              },
  { 0x7FFF00, "Spring Green"        }, { 0xB48246, "Steel Blue"        },
  { 0x8CB4D2, "Tan"                 }, { 0x808000, "Teal"              },
  { 0xD8BFD8, "Thistle"             }, { 0x4763FF, "Tomato"            },
  { 0xD0E040, "Turquoise"           }, { 0xEE82EE, "Violet"            },
  { 0xB3DEF5, "Wheat"               }, { 0xFFFFFF, "White"             },
  { 0xF5F5F5, "White Smoke"         }, { 0x00FFFF, "Yellow"            },
  { 0x32CD9A, "Yellow Green"        }
};

string EscapeString(const string &str) {
  string res;
  res.reserve(str.length());
  for (char c : str) {
    if (c == '\\' || c == '"' || c == '\'') res.push_back('\\');
    res.push_back(c);
  }
  return res;
}

string ColorToStr(int color) {
  std::stringstream res;
  auto name = kColorNames.find(color);
  if (name != kColorNames.end())
    return (res << "\"" << name->second << "\"", res).str();
  return (res << "#"  << std::setfill('0') << std::setw(6)  << std::hex
              << color, res).str();
}

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
    SVGDWriteHVComponents(abs, 'O', 'U', 'V', tile.width, tile.height,
                          TileSimple::DEFAULTS.width, TileSimple::DEFAULTS.height);
    SVGDWriteHVComponents(abs, 'o', 'u', 'v', tile.width, tile.height,
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

struct Coords {
  double x, y, z;
  Coords(int xx, int yy, int zz): x(xx), y(yy), z(zz) {}
  Coords(double xx, double yy, double zz): x(xx), y(yy), z(zz) {}
  Coords(const Room::Instance &inst): x(inst.x()), y(inst.y()), z(inst.z()) {}
  Coords(const Room::Tile &tile): x(tile.x()), y(tile.y()), z(0) {}
  bool operator< (const Coords &o) const {
    return
        x < o.x || (!(o.x < x) && (
        y < o.y || (!(o.y < y) && (
        z < o.z ))));
  }
  Coords PositiveDelta(const Coords &o) const {
    if (*this < o)
      return { o.x - x, o.y - y, o.z - z };
    return { x - o.x, y - o.y, z - o.z };
  }
  Coords operator-(const Coords &o) const {
    return { x - o.x, y - o.y, z - o.z };
  }
  bool operator==(const Coords &o) const {
    return x == o.x && y == o.y && z == o.z;
  }
  bool operator!=(const Coords &o) const {
    return x != o.x || y != o.y || z != o.z;
  }
};

std::ostream &operator<<(std::ostream &s, const Coords &c) {
  if (c.z) return s << "(" << c.x << ", " << c.y << ", " << c.z << ")";
  return s << "(" << c.x << ", " << c.y << ")";
}

struct Line {
  Coords slope;
  std::vector<Coords> points;
  Line(Coords slope_in, std::set<Coords> points_in): slope(slope_in) {
    if (points_in.empty()) return;
    points.reserve(points_in.size());

    auto it = points_in.begin();
    const Coords *lastp = &*it;
    points.push_back(*lastp);
    while (++it != points_in.end()) {
      if (*it - *lastp != slope) {
        std::cerr << "Connected components is broken or unsorted!" << std::endl
                  << *it << " - "  << *lastp << " ≠ " << slope << std::endl
                  << "Complete list of points in this segment:" << std::endl;
        for (auto i2 = points_in.begin(); i2 != points_in.end(); ++i2) {
          std::cerr << " - " << i2->x << ", " << i2->y << std::endl;
        }
        break;
      }
      points.push_back(*it);
      lastp = &*it;
    }
  }
  const Coords &first() const { return points.front(); }
  const Coords &last() const { return points.back(); }
};

struct ConnectedComponents {
  typedef std::set<Coords> Component;
  std::map<Coords, size_t> connections;
  std::map<size_t, Component> components;
  size_t max_cid = 0;

  void Connect(Coords a, Coords b) {
    auto ai = connections.find(a);
    auto bi = connections.find(b);
    if (ai == connections.end()) {
      if (bi == connections.end()) {
        const size_t nid = max_cid++;
        connections[a] = connections[b] = nid;
        components[nid].insert(a);
        components[nid].insert(b);
      } else {
        size_t component = bi->second;
        connections[a] = component;
        components[component].insert(a);
      }
    } else {
      if (bi == connections.end()) {
        size_t component = ai->second;
        connections[b] = component;
        components[component].insert(b);
      } else {
        size_t aid = ai->second, bid = bi->second;
        if (aid == bid) return;
        auto &ac = components[aid], &bc = components[bid];
        if (aid < bid) {
          for (const Coords &c : bc) {
            connections[c] = aid;
            ac.insert(c);
          }
          bc.clear();
          components.erase(bid);
        } else {
          for (const Coords &c : ac) {
            connections[c] = bid;
            bc.insert(c);
          }
          ac.clear();
          components.erase(aid);
        }
      }
    }
  }

  long Score() const {
    long score = 0;
    for (const auto &p : components) score += p.second.size() - 3;
    return score;
  }

  long ScoreB() const {
    long score = 0;
    std::map<long, long> sizes;
    for (const auto &p : components) ++sizes[p.second.size()];
    long consider = 5;
    for (auto rit = sizes.rbegin(); rit != sizes.rend(); ++rit) {
      if (rit->first < consider) {
        consider -= rit->first;
        score += rit->second * rit->first;
      } else {
        score += rit->second * consider;
        break;
      }
    }
    return score;
  }
};

inline double nz(double x) { return x ? x : 1; }
std::vector<Line> ConnectTheDots(const std::multiset<Coords> &cloud) {
  if (cloud.size() < 3) return {};

  double xmin, xmax, ymin, ymax, zmin, zmax;
  xmin = xmax = cloud.begin()->x;
  ymin = ymax = cloud.begin()->y;
  zmin = zmax = cloud.begin()->z;
  for (const Coords &c : cloud) {
    if (c.x < xmin) xmin = c.x;
    if (c.y < ymin) ymin = c.y;
    if (c.z < zmin) zmin = c.z;
    if (c.x > xmax) xmax = c.x;
    if (c.y > ymax) ymax = c.y;
    if (c.z > zmax) zmax = c.z;
  }

  const double xspan = xmax - xmin, yspan = ymax - ymin, zspan = zmax - zmin;
  const double scale = std::max(std::max(xspan, yspan), zspan);

  if (scale <= .01) return {};

  const int hf = std::max(4, int(cloud.size() / 75));
  const int cx = std::max(1, int(ceil(hf * xspan / scale)));
  const int cy = std::max(1, int(ceil(hf * yspan / scale)));
  const int cz = std::max(1, int(ceil(hf * zspan / scale)));
  std::vector<std::vector<std::vector<std::vector<Coords>>>> cells;
  cells.resize(cx);
  for (int i = 0; i < cx; ++i) {
    cells[i].resize(cy);
    for (int j = 0; j < cy; ++j) {
      cells[i][j].resize(cz);
    }
  }

  const double
      xgrid = nz(xspan / cx),  // These zeros don't matter, since the deltas
      ygrid = nz(yspan / cy),  // will always be 0 if the denominator would have
      zgrid = nz(zspan / cz);  // been zero. We just want 0/0 to be 0, here.

  for (const Coords &c : cloud) {
    int cellx = std::min(int((c.x - xmin) / xgrid), cx - 1);
    int celly = std::min(int((c.y - ymin) / ygrid), cy - 1);
    int cellz = std::min(int((c.z - zmin) / zgrid), cz - 1);
    if (kIncrediblyVerboseDebugOutput) {
      std::cout << c << " in space"
                << " [ " << Coords{xmin, ymin, zmin} << " .. "
                         << Coords{xmax, ymax, zmax} << " ] "
                << "to cell " << Coords(cellx, celly, cellz)
                << " out of " << Coords(cx, cy, cz) << std::endl;
    }
    cells[cellx][celly][cellz].push_back(c);
  }

  // Don't spend too much time on this.
  for (int i = 0; i < cx; ++i) {
    for (int j = 0; j < cy; ++j) {
      for (int k = 0; k < cz; ++k) {
        if (cells[i][j][k].size() > 100) return {};
        if (kIncrediblyVerboseDebugOutput) {
          std::cout << "Cell" << Coords(i,j,k) << ": [";
          for (const Coords &c : cells[i][j][k]) std::cout << c << ", ";
          std::cout << "]" << std::endl;
        }
      }
    }
  }

  // We maintain a different set of connected components for each distance
  // connecting them. This way, all connected components are colinear.
  std::map<Coords, ConnectedComponents> edges;
  for (int i = 0; i < cx; ++i)
  for (int j = 0; j < cy; ++j)
  for (int k = 0; k < cz; ++k) {
    for (size_t l = 0; l < cells[i][j][k].size(); ++l) {
      for (int m = i; m < i + 2 && m < cx; ++m)
      for (int n = j; n < j + 2 && n < cy; ++n)
      for (int o = k; o < k + 2 && o < cz; ++o) {
        for (size_t p = 0; p < cells[m][n][o].size(); ++p)
        if (cells[i][j][k][l] != cells[m][n][o][p]) {
          Coords dist = cells[i][j][k][l].PositiveDelta(cells[m][n][o][p]);
          edges[dist].Connect(cells[i][j][k][l], cells[m][n][o][p]);
          if (kIncrediblyVerboseDebugOutput) {
            std::cout << "edges[" << dist << "].Connect(" << cells[i][j][k][l]
                      << ", " << cells[m][n][o][p] << ")" << std::endl;
          }
        }
      }
    }
  }

  long high_score = 0;
  typedef std::pair<const Coords, ConnectedComponents> Grouping;
  Grouping *best_grouping = nullptr;
  for (Grouping &dist : edges) {
    long score = dist.second.Score();
    if (kIncrediblyVerboseDebugOutput) {
      string sep;
      std::cout << "Group " << dist.first << " has "
                << dist.second.components.size() << " components [";
      for (const auto &c : dist.second.components) {
        std::cout << sep << c.second.size(); sep = ", ";
      }
      std::cout << "] and scored " << score << " / " << high_score << std::endl;
    }
    if (score > high_score) {
      high_score = score;
      best_grouping = &dist;
    }
  }

  if (!best_grouping) return {};

  if (kMinorlyVerboseDebuggingOutput) {
    std::cout << "INFO Best grouping chosen: stride = " << best_grouping->first
              << ", score = " << high_score << std::endl;
  }

  std::vector<Line> res;
  res.reserve(best_grouping->second.components.size());
  for (const auto &id_group : best_grouping->second.components) {
    if (id_group.second.size() > 2) {
      res.push_back(Line(best_grouping->first, id_group.second));
    }
  }
  return res;
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
  if (res.creation_code().empty()) res.clear_creation_code();

  // DO NOT SUBMIT: this should be an option or otherwise not exist
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

template<typename Layers, typename Entity, typename Simple = Simplify<Entity>>
void BuildLayers(const RepeatedPtrField<Entity> &entities, Layers *out) {
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
      out->snowflakes.push_back(entity);
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
  string data = layer.str();
  if (data.length()) out->layers.push_back({"svg-d", std::move(data)});
}

}  // namespace

namespace egm {
namespace util {

InstanceLayers BuildInstanceLayers(const Room &room) {
  InstanceLayers res;
  BuildLayers(room.instances(), &res);
  return res;
}

TileLayers BuildTileLayers(const Room &room) {
  TileLayers res;
  BuildLayers(room.tiles(), &res);
  return res;
}

}  // namespace util
}  // namespace egm
