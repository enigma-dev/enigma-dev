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

#include "serialization-helpers.h"

#include <map>
#include <string>
#include <sstream>
#include <iomanip>
using std::string;

namespace {

const std::map<int32_t, string> kColorNames {
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

template<typename T, typename U> std::map<U, T> invert(std::map<T, U> m) {
  std::map<U, T> res;
  for (const auto &p : m) res[p.second] = p.first;
  return res;
}

const std::map<string, int32_t> kColorsByName = invert(kColorNames);

}  // namespace

namespace egm {
namespace serialization {


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

int32_t ColorFromStr(string color) {
  auto f = kColorsByName.find(color);
  if (f != kColorsByName.end()) return f->second;
  if ((color.length() == 4 || color.length() == 7) && color[0] == '#') {
    int32_t res;
    std::stringstream ss;
    ss << std::hex << color.substr(1);
    ss >> res;
    return res;
  }
  return -1;
}

int ParseHex(const std::string &hex) {
  int res;
  std::stringstream ss;
  ss << std::hex << hex;
  ss >> res;
  return res;
}

std::pair<string, size_t> ReadQuotedString(const string &data, size_t i) {
  std::string str;
  const char quote_c = data[i++];
  while (i < data.length() && data[i] != quote_c) {
    if (data[i] != '\\') {
      str.append(1, data[i++]);
      continue;
    }

    switch (const char e = data[++i]) {
      case 'b':  str.append(1, '\b'); ++i; continue;
      case 'f':  str.append(1, '\f'); ++i; continue;
      case 'n':  str.append(1, '\n'); ++i; continue;
      case 'r':  str.append(1, '\r'); ++i; continue;
      case 't':  str.append(1, '\t'); ++i; continue;
      case 'v':  str.append(1, '\v'); ++i; continue;
      case '\"': str.append(1, '\"'); ++i; continue;
      case '\'': str.append(1, '\''); ++i; continue;
      case '\\': str.append(1, '\\'); ++i; continue;

      case '0': case '1': case '2': case '3':
      case '4': case '5': case '6': case '7': {
        int oct = e - '0';
        if (++i < data.length() && data[i] >= '0' && data[i] <= '7') {
          oct = oct * 8 + data[i] - '0';
          if (++i < data.length() && data[i] >= '0' && data[i] <= '7'
              && oct * 8 < 256) {
            oct = oct * 8 + data[i] - '0';
            ++i;
          }
        }
        str.append(1, char(oct));
        continue;
      }

      case 'x': {
        if (++i >= data.length() || !IsHex(data[i])) {
          std::cerr << "Expected hex data following \\x escape" << std::endl;
          str += "\\x";
          continue;
        }
        char n[3] = {0, 0, 0};
        n[0] = data[i];
        if (++i < data.length() && IsHex(data[i])) n[1] = data[i++];
        str.append(1, ParseHex(n));
        continue;
      }

      // TODO: Handle '\u123ABC'
      default: {
        std::cerr << "Unknown escape sequence '\\" << e << '\'' << std::endl;
        str.append(1, '\\');
        str.append(1, e);
      }
    }
  }
  return std::make_pair(str, i);
}

}  // namespace serialization
}  // namespace egm
