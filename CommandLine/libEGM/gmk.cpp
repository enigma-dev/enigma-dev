/** Copyright (C) 2018 Robert B. Colton
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

#include "gmk.h"

#include "codegen/project.pb.h"

#include <zlib.h>

#include <fstream>
#include <utility>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>

using namespace buffers;
using namespace buffers::resources;

using TypeCase = TreeNode::TypeCase;
using IdMap = std::unordered_map<TypeCase, std::unordered_map<int, std::string>>;
using VersionMap = std::unordered_map<TypeCase, std::set<int>>;

using std::unordered_map;

namespace gmk {
std::ostream out(nullptr);
std::ostream err(nullptr);

class Decoder {
  public:
  explicit Decoder(std::istream &in): in(in), decodeTable(nullptr) {}
  ~Decoder() {}

  void skip(size_t count) {
    in.seekg(count, std::ios_base::cur);
  }

  int read() {
    int byte = in.get();
    if (decodeTable) {
      int pos = in.tellg();
      return (decodeTable[byte] - (pos - 1)) & 0xFF;
    }
    return byte;
  }

  int read3() {
    return (read() | (read() << 8) | (read() << 16));
  }

  int read4() {
    return (read() | (read() << 8) | (read() << 16) | (read() << 24));
  }

  bool readBool() {
    return read4();
  }

  std::unique_ptr<char[]> read(size_t length, size_t off=0) {
    char* bytes = new char[length];
    read(bytes, length, off);
    return std::unique_ptr<char[]>(bytes);
  }

  void read(char* bytes, size_t length, size_t off=0) {
    int pos = in.tellg();
    in.read(bytes, length);
    if (decodeTable) {
      for (size_t i = 0; i < length; i++) {
        int t = bytes[off + i] & 0xFF;
        int x = (decodeTable[t] - pos - i) & 0xFF;
        bytes[off + i] = (char) x;
      }
    }
  }

  std::string readStr() {
    size_t length = read4();
    std::unique_ptr<char[]> bytes = read(length);
    return std::string(bytes.get(), length);
  }

  double readD() {
    union { char bytes[8]; double dbl; } data;
    read(data.bytes, 8);
    return data.dbl;
  }

  std::unique_ptr<char[]> decompress(size_t length, size_t off=0) {
    std::unique_ptr<char[]> src = read(length, off);
    char* dest = new char[length];
    uLongf destLength = length;
    uncompress((Bytef*)dest, &destLength, (Bytef*)src.get(), length);
    return std::unique_ptr<char[]>(dest);
  }

  std::unique_ptr<char[]> readZlibImage() {
    size_t length = read4();
    std::unique_ptr<char[]> bytes = decompress(length);
    return bytes;
  }

  void setSeed(int seed) {
    if (seed >= 0)
      decodeTable = makeDecodeTable(seed);
    else
      decodeTable = nullptr;
  }

  private:
  std::istream &in;
  std::unique_ptr<int[]> decodeTable;

  std::unique_ptr<int[]> makeEncodeTable(int seed) {
    std::unique_ptr<int[]> table(new int[256]);
    int a = 6 + (seed % 250);
    int b = seed / 250;
    for (int i = 0; i < 256; i++)
      table[i] = i;
    for (int i = 1; i < 10001; i++) {
      int j = 1 + ((i * a + b) % 254);
      int t = table[j];
      table[j] = table[j + 1];
      table[j + 1] = t;
    }
    return table;
  }

  std::unique_ptr<int[]> makeDecodeTable(std::unique_ptr<int[]> encodeTable) {
    std::unique_ptr<int[]> table(new int[256]);
    for (int i = 1; i < 256; i++)
      table[encodeTable[i]] = i;
    return table;
  }

  std::unique_ptr<int[]> makeDecodeTable(int seed) {
    return makeDecodeTable(makeEncodeTable(seed));
  }
};

void LoadIncludes(Decoder &dec) {
  int no = dec.read4();
  for (int i = 0; i < no; i++) {
    dec.readStr(); // include filepath
  }
  dec.read4(); // INCLUDE_FOLDER 0=main 1=temp
  // OVERWRITE_EXISTING, REMOVE_AT_GAME_END
  dec.readBool(); dec.readBool();
}

int LoadSettings(Decoder &dec) {
  int ver = dec.read4();
  if (ver != 530 && ver != 542 && ver != 600 && ver != 702 && ver != 800 && ver != 810) {
    err << "Unsupported GMK Settings version: " << ver << std::endl;
    return 0;
  }

  //if (ver >= 800) beginInflate(in); // TODO: zlib beginInflate
  dec.readBool(); // start_fullscreen
  if (ver >= 600) {
    dec.readBool(); // interpolate
  }
  dec.readBool(); // dont_draw_border
  dec.readBool(); // display_cursor
  dec.read4(); // scaling
  if (ver == 530) {
    dec.skip(8); // "fullscreen scale" & "only scale w/ hardware support"
  } else {
    dec.readBool(); // allow_window_resize
    dec.readBool(); // always_on_top
    dec.read4(); // color_outside_room
  }
  dec.readBool(); // set_resolution

  int color_depth = 0, resolution, frequency;
  if (ver == 530) {
    dec.skip(8); //Color Depth, Exclusive Graphics
    resolution = dec.read4();
    int b = dec.read4();
    frequency = (b == 4) ? 0 : (b + 1);
    dec.skip(8); //vertical blank, caption in fullscreen
  } else {
    color_depth = dec.read4();
    resolution = dec.read4();
    frequency = dec.read4();
  }

  dec.readBool(); // DONT_SHOW_BUTTONS
  if (ver > 530) dec.readBool(); // USE_SYNCHRONIZATION
  if (ver >= 800) dec.readBool(); // DISABLE_SCREENSAVERS
  // LET_F4_SWITCH_FULLSCREEN, LET_F1_SHOW_GAME_INFO, LET_ESC_END_GAME, LET_F5_SAVE_F6_LOAD
  dec.readBool(); dec.readBool(); dec.readBool(); dec.readBool();
  if (ver == 530) dec.skip(8); //unknown bytes, both 0
  if (ver > 600) {
    // LET_F9_SCREENSHOT, TREAT_CLOSE_AS_ESCAPE
    dec.readBool(); dec.readBool();
  }
  dec.read4(); // GAME_PRIORITY
  dec.readBool(); // FREEZE_ON_LOSE_FOCUS
  int load_bar_mode = dec.read4(); // LOAD_BAR_MODE
  if (load_bar_mode == 2) { // 0=NONE 1=DEFAULT 2=CUSTOM
    if (ver < 800) {
      if (dec.read4() != -1) dec.readZlibImage(); // BACK_LOAD_BAR
      if (dec.read4() != -1) dec.readZlibImage(); // FRONT_LOAD_BAR
    } else { //ver >= 800
      if (dec.readBool()) dec.readZlibImage(); // BACK_LOAD_BAR
      if (dec.readBool()) dec.readZlibImage(); // FRONT_LOAD_BAR
    }
  }
  bool show_custom_load_image = dec.readBool(); // SHOW_CUSTOM_LOAD_IMAGE
  if (show_custom_load_image) {
    if (ver < 800) {
      if (dec.read4() != -1) dec.readZlibImage(); // LOADING_IMAGE
    } else if (dec.readBool()) {
      dec.readZlibImage(); // LOADING_IMAGE
    }
  }
  dec.readBool(); // IMAGE_PARTIALLY_TRANSPARENTY
  dec.read4(); // LOAD_IMAGE_ALPHA
  dec.readBool(); // SCALE_PROGRESS_BAR

  dec.skip(dec.read4()); // GAME_ICON

  // DISPLAY_ERRORS, WRITE_TO_LOG, ABORT_ON_ERROR
  dec.readBool(); dec.readBool(); dec.readBool();
  dec.read4(); // errors
  // TREAT_UNINIT_AS_0 = ((errors & 0x01) != 0)
  // ERROR_ON_ARGS = ((errors & 0x02) != 0)
  dec.readStr(); // AUTHOR
  if (ver > 600)
    dec.readStr(); // VERSION
  else
    dec.read4(); // VERSION std::to_string
  dec.readD(); // LAST_CHANGED
  dec.readStr(); // INFORMATION
  if (ver < 800) {
    int no = dec.read4(); // number of constants
    for (int i = 0; i < no; i++) {
      dec.readStr(); // constant name
      dec.readStr(); // constant value
    }
  }
  if (ver > 600) {
    // VERSION_MAJOR, VERSION_MINOR, VERSION_RELEASE, VERSION_BUILD
    dec.read4(); dec.read4(); dec.read4(); dec.read4();
    // COMPANY, PRODUCT, COPYRIGHT, DESCRIPTION
    dec.readStr(); dec.readStr(); dec.readStr(); dec.readStr();

    if (ver >= 800) dec.skip(8); //last changed
  } else if (ver > 530) {
    LoadIncludes(dec);
  }
  //in.endInflate(); TODO: zlib endInflate

  return 1;
}

int LoadTriggers(Decoder &dec) {
  int ver = dec.read4();
  if (ver != 800) {
    err << "Unsupported GMK Triggers version: " << ver << std::endl;
    return 0;
  }

  int no = dec.read4();
  for (int i = 0; i < no; i++) {
    //in.beginInflate();
    if (!dec.readBool()) {
      //in.endInflate();
      continue;
    }
    ver = dec.read4();
    if (ver != 800) {
      err << "Unsupported GMK Trigger version: " << ver << std::endl;
      return 0;
    }
    dec.readStr(); // trigger name
    dec.readStr(); // trigger condition
    dec.read4(); // trigger check step
    dec.readStr(); // trigger constant
    //in.endInflate();
  }
  dec.skip(8); //last changed

  return 1;
}

int LoadConstants(Decoder &dec) {
  int ver = dec.read4();
  if (ver != 800) {
    err << "Unsupported GMK Constants version: " << ver << std::endl;
    return 0;
  }

  int no = dec.read4();
  for (int i = 0; i < no; i++) {
    dec.readStr(); // constant name
    dec.readStr(); // constant value
  }
  dec.skip(8); //last changed

  return 1;
}

std::unique_ptr<Sound> LoadSound(Decoder &dec, int ver) {
  auto sound = std::make_unique<Sound>();

  int kind53 = -1;
  if (ver == 440)
    kind53 = dec.read4(); //kind (wav, mp3, etc)
  else
    sound->set_kind(static_cast<Sound_Kind>(dec.read4())); //normal, background, etc
  sound->set_file_extension(dec.readStr());

  if (ver == 440) {
    //-1 = no sound
    if (kind53 != -1) {
      std::unique_ptr<char[]> data = dec.decompress(dec.read4());
    }
    dec.skip(8);
    sound->set_preload(!dec.readBool());
  } else {
    sound->set_file_name(dec.readStr());
    if (dec.readBool()) {
      if (ver == 600) {
        std::unique_ptr<char[]> data = dec.decompress(dec.read4());
      } else {
        std::unique_ptr<char[]> data = dec.read(dec.read4());
      }
    }
    dec.read4(); // effects flags
    sound->set_volume(dec.readD());
    sound->set_pan(dec.readD());
    sound->set_preload(dec.readBool());
  }

  return sound;
}

int LoadGroup(Decoder &dec, TypeCase type, IdMap &idMap) {
  using FactoryFunction = std::function<std::unique_ptr<google::protobuf::Message>(Decoder&, int)>;
  using FactoryMap = std::unordered_map<TypeCase, FactoryFunction>;

  static VersionMap supportedGroupVersion({
    { TypeCase::kSound, { 400, 800 } }
  });
  static VersionMap supportedVersion({
    { TypeCase::kSound, { 440, 600, 800 } }
  });
  static const FactoryMap factoryMap({
    { TypeCase::kSound, LoadSound }
  });

  int ver = dec.read4();
  auto supportedGroupVersions = supportedGroupVersion[type];
  if (!supportedGroupVersions.count(ver)) {
    err << "GMK group '" << type << "' with version '" << ver << "' is unsupported" << std::endl;
    return 0;
  }

  const auto createFunc = factoryMap.find(type);
  if (createFunc == factoryMap.end()) {
    err << "GMK group '" << type << "' with version '" << ver << "' does not have a registered factory function" << std::endl;
    return 0;
  }

  int count = dec.read4();
  for (int i = 0; i < count; ++i) {
    //if (ver == 800) in.beginInflate();
    if (!dec.readBool()) {
      // was deleted
      //in.endInflate();
      continue;
    }
    std::string name = dec.readStr();
    if (ver == 800) dec.skip(8); //last changed
    ver = dec.read4();
    auto supportedVersions = supportedVersion[type];
    if (!supportedVersions.count(ver)) {
      err << "GMK resource of type '" << type << "' with id '" << i
          << "' has an unsupported version '" << ver << "'" << std::endl;
      return 0;
    }

    auto nameMap = idMap[type];
    nameMap[i] = name;
    auto res = createFunc->second(dec, ver);

    //in.endInflate();
  }

  return 1;
}

buffers::Project *LoadGMK(std::string fName) {
  std::ifstream in(fName, std::ios::binary);
  Decoder dec(in);

  int identifier = dec.read4();
  if (identifier != 1234321) {
    err << "Invalid GMK identifier: " << identifier << std::endl;
    return nullptr;
  } else {
    out << "identifier: " << identifier << std::endl;
  }
  int ver = dec.read4();
  if (ver != 530 && ver != 600 && ver != 701 && ver != 800 && ver != 810) {
    err << "Unsupported GMK version: " << ver << std::endl;
    return nullptr;
  } else {
    out << "game version: " << ver << std::endl;
  }

  if (ver == 530) dec.skip(4);
  int game_id = 0;
  if (ver == 701) {
    int s1 = dec.read4();
    int s2 = dec.read4();
    dec.skip(s1 * 4);
    //since only the first byte of the game id isn't encrypted, we have to do some acrobatics here
    int seed = dec.read4();
    dec.skip(s2 * 4);
    int b1 = dec.read();
    dec.setSeed(seed);
    game_id = b1 | dec.read3() << 8;
  } else {
    game_id = dec.read4();
  }
  out << "game id: " << game_id << std::endl;
  dec.skip(16); //16 bytes GAME_GUID

  if (!LoadSettings(dec)) return nullptr;

  if (ver >= 800) {
    if (!LoadTriggers(dec)) return nullptr;
    if (!LoadConstants(dec)) return nullptr;
  }

  IdMap idMap;
  static const std::vector<TypeCase> binaryOrderGroups({
    TypeCase::kSound,
    TypeCase::kSprite,
    TypeCase::kBackground,
    TypeCase::kPath,
    TypeCase::kScript,
    TypeCase::kFont,
    TypeCase::kTimeline,
    TypeCase::kObject,
    TypeCase::kRoom
  });

  for (TypeCase type : binaryOrderGroups) {
    out << type << std::endl;
    if (!LoadGroup(dec, type, idMap)) return nullptr;
  }

  auto proj = std::make_unique<buffers::Project>();
  buffers::Game *game = proj->mutable_game();

  return proj.release();
}

}  //namespace gmk
