/** Copyright (C) 2018-2020 Robert B. Colton, Greg Williamson
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
#include "filesystem.h"
#include "action.h"
#include "General/zlib_util.h"

#include "libpng-util/libpng-util.h"

#include <fstream>
#include <utility>
#include <memory>
#include <future>
#include <functional>
#include <unordered_map>
#include <vector>
#include <set>

#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

#include <zlib.h>

using namespace buffers;
using namespace buffers::resources;
using namespace std;

using TypeCase = TreeNode::TypeCase;
using IdMap = unordered_map<int, std::unique_ptr<google::protobuf::Message> >;
using TypeMap = unordered_map<TypeCase, IdMap>;
using egm::errStream;


namespace gmk_internal {

static const std::string gmk_data = "gmk_data";

void LoadSettingsIncludes(Decoder &dec) {
  int no = dec.read4();
  for (int i = 0; i < no; i++) {
    auto include = std::make_unique<Include>();
    dec.readStr(); // include filepath
  }
  dec.read4(); // INCLUDE_FOLDER 0=main 1=temp
  // OVERWRITE_EXISTING, REMOVE_AT_GAME_END
  dec.readBool(); dec.readBool();
}

int LoadSettings(Decoder &dec, Settings& set) {
  int ver = dec.read4();
  if (ver != 530 && ver != 542 && ver != 600 && ver != 702 && ver != 800 && ver != 810) {
    errStream << "Unsupported GMK Settings version: " << ver << std::endl;
    return 0;
  }

  General* gen = set.mutable_general();
  Graphics* gfx = set.mutable_graphics();
  Windowing *win = set.mutable_windowing();
  Info* inf = set.mutable_info();

  if (ver >= 800) dec.beginInflate();
  win->set_start_in_fullscreen(dec.readBool());
  if (ver >= 600) {
    gfx->set_interpolate_textures(dec.readBool());
  }
  win->set_show_border(!dec.readBool()); // inverted because negative in GM (e.g, "don't")
  gen->set_show_cursor(dec.readBool());
  gfx->set_view_scale(dec.read4());
  if (ver == 530) {
    dec.skip(8); // "fullscreen scale" & "only scale w/ hardware support"
  } else {
    win->set_is_sizeable(dec.readBool());
    win->set_stay_on_top(dec.readBool());
    gfx->set_color_outside_room_region(dec.read4());
  }
  dec.readBool(); // set_resolution

  if (ver == 530) {
    dec.skip(8); //Color Depth, Exclusive Graphics
    dec.read4(); // resolution
    dec.read4(); // int b
    //frequency = (b == 4) ? 0 : (b + 1);
    dec.skip(8); //vertical blank, caption in fullscreen
  } else {
    dec.read4(); // color depth
    dec.read4(); // resolution
    dec.read4(); // frequency
  }

  win->set_show_icons(!dec.readBool()); // inverted because negative in GM (e.g, "don't")
  if (ver > 530) gfx->set_use_synchronization(dec.readBool());
  if (ver >= 800) dec.readBool(); // DISABLE_SCREENSAVERS
  gfx->set_allow_fullscreen_change(dec.readBool());
  // LET_F1_SHOW_GAME_INFO, LET_ESC_END_GAME, LET_F5_SAVE_F6_LOAD
  dec.readBool(); dec.readBool(); dec.readBool();
  if (ver == 530) dec.skip(8); //unknown bytes, both 0
  if (ver > 600) {
    // LET_F9_SCREENSHOT, TREAT_CLOSE_AS_ESCAPE
    dec.readBool(); dec.readBool();
  }
  dec.read4(); // GAME_PRIORITY
  win->set_freeze_on_lose_focus(dec.readBool());
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
  inf->set_author_name(dec.readStr());
  if (ver > 600)
    inf->set_version(dec.readStr());
  else
    inf->set_version(std::to_string(dec.read4()));
  inf->set_last_changed(dec.readD());
  inf->set_information(dec.readStr());
  if (ver < 800) {
    int no = dec.read4(); // number of constants
    for (int i = 0; i < no; i++) {
      dec.readStr(); // constant name
      dec.readStr(); // constant value
    }
  }
  if (ver > 600) {
    gen->set_version_major(dec.read4());
    gen->set_version_minor(dec.read4());
    gen->set_version_release(dec.read4());
    gen->set_version_build(dec.read4());

    gen->set_company(dec.readStr());
    gen->set_product(dec.readStr());
    gen->set_copyright(dec.readStr());
    gen->set_description(dec.readStr());

    if (ver >= 800) dec.skip(8); //last changed
  } else if (ver > 530) {
    LoadSettingsIncludes(dec);
  }
  dec.endInflate();

  return 1;
}

int LoadTriggers(Decoder &dec) {
  int ver = dec.read4();
  if (ver != 800) {
    errStream << "Unsupported GMK Triggers version: " << ver << std::endl;
    return 0;
  }

  int no = dec.read4();
  for (int i = 0; i < no; i++) {
    dec.beginInflate();
    if (!dec.readBool()) {
      dec.endInflate();
      continue;
    }
    ver = dec.read4();
    if (ver != 800) {
      errStream << "Unsupported GMK Trigger version: " << ver << std::endl;
      return 0;
    }
    dec.readStr(); // trigger name
    dec.readStr(); // trigger condition
    dec.read4(); // trigger check step
    dec.readStr(); // trigger constant
    dec.endInflate();
  }
  dec.skip(8); //last changed

  return 1;
}

int LoadConstants(Decoder &dec) {
  int ver = dec.read4();
  if (ver != 800) {
    errStream << "Unsupported GMK Constants version: " << ver << std::endl;
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

std::unique_ptr<Sound> LoadSound(Decoder &dec, int ver, const std::string& name) {
  auto sound = std::make_unique<Sound>();

  int kind53 = -1;
  if (ver == 440)
    kind53 = dec.read4(); //kind (wav, mp3, etc)
  else
    sound->set_kind(static_cast<Sound::Kind>(dec.read4())); //normal, background, etc
  sound->set_file_extension(dec.readStr());
  
  const std::filesystem::path fName = TempFileName(gmk_data)/(name + sound->file_extension());

  if (ver == 440) {
    //-1 = no sound
    if (kind53 != -1) {
      dec.readCompressedData(fName);
      sound->set_data(fName.u8string());
    }
    dec.skip(8);
    sound->set_preload(!dec.readBool());
  } else {
    sound->set_file_name(dec.readStr());
    if (dec.readBool()) {
      if (ver == 600) {
        dec.readCompressedData(fName);
        sound->set_data(fName.u8string());
      } else {
        dec.readUncompressedData(fName);
        sound->set_data(fName.u8string());
      }
    }
    dec.read4(); // effects flags
    sound->set_volume(dec.readD());
    sound->set_pan(dec.readD());
    sound->set_preload(dec.readBool());
  }

  return sound;
}

std::unique_ptr<Sprite> LoadSprite(Decoder &dec, int ver, const std::string& name) {
  auto sprite = std::make_unique<Sprite>();

  int w = 0, h = 0;
  bool transparent = false;
  if (ver < 800) {
    w = dec.read4();
    h = dec.read4();
    sprite->set_bbox_left(dec.read4());
    sprite->set_bbox_right(dec.read4());
    sprite->set_bbox_bottom(dec.read4());
    sprite->set_bbox_top(dec.read4());
    transparent = dec.readBool();
    if (ver > 400) {
      sprite->set_smooth_edges(dec.readBool());
      sprite->set_preload(dec.readBool());
    }
    sprite->set_bbox_mode(static_cast<Sprite::BoundingBox>(dec.read4()));
    bool precise = dec.readBool();
    sprite->set_shape(precise ? Sprite::Shape::Sprite_Shape_PRECISE : Sprite::Shape::Sprite_Shape_RECTANGLE);
    if (ver == 400) {
      dec.skip(4); //use video memory
      sprite->set_preload(!dec.readBool());
    }
  } else {
    transparent = false;
  }
  sprite->set_origin_x(dec.read4());
  sprite->set_origin_y(dec.read4());

  int nosub = dec.read4();
  std::filesystem::path tempName = TempFileName(gmk_data);
  for (int i = 0; i < nosub; i++) {
    std::filesystem::path fName = tempName/(name + "_" + std::to_string(i) + ".png");
    if (ver >= 800) {
      int subver = dec.read4();
      if (subver != 800 && subver != 810) {
        errStream << "GMK Sprite with inner version '" << ver << "' has a subimage with id '" << i
            << "' that has an unsupported version '" << subver << "'" << std::endl;
        return nullptr;
      }
      w = dec.read4();
      h = dec.read4();
      if (w != 0 && h != 0) {
        dec.readBGRAImage(fName, w, h);
        sprite->add_subimages(fName.u8string());
      }
    } else {
      if (dec.read4() == -1) continue;
      dec.readZlibImage(fName, transparent);
      sprite->add_subimages(fName.u8string());
    }
  }
  sprite->set_width(w);
  sprite->set_height(h);

  if (ver >= 800) {
    sprite->set_shape(static_cast<Sprite::Shape>(dec.read4()));
    sprite->set_alpha_tolerance(dec.read4());
    sprite->set_separate_mask(dec.readBool());
    sprite->set_bbox_mode(static_cast<Sprite::BoundingBox>(dec.read4()));
    sprite->set_bbox_left(dec.read4());
    sprite->set_bbox_right(dec.read4());
    sprite->set_bbox_bottom(dec.read4());
    sprite->set_bbox_top(dec.read4());
  }

  return sprite;
}

std::unique_ptr<Background> LoadBackground(Decoder &dec, int ver, const std::string& name) {
  auto background = std::make_unique<Background>();

  int w = 0, h = 0;
  bool transparent = false;
  if (ver < 710) {
    w = dec.read4();
    h = dec.read4();
    transparent = dec.readBool();
    if (ver > 400) {
      background->set_smooth_edges(dec.readBool());
      background->set_preload(dec.readBool());
    } else {
      dec.skip(4); //use video memory
      background->set_preload(!dec.readBool());
    }
  }

  if (ver > 400) {
    background->set_use_as_tileset(dec.readBool());
    background->set_tile_width(dec.read4());
    background->set_tile_height(dec.read4());
    background->set_horizontal_offset(dec.read4());
    background->set_vertical_offset(dec.read4());
    background->set_horizontal_spacing(dec.read4());
    background->set_vertical_spacing(dec.read4());
  }

  const std::filesystem::path fName = TempFileName(gmk_data)/(name + ".png");
  
  if (ver < 710) {
    if (dec.readBool()) {
      if (dec.read4() != -1) {
        dec.readZlibImage(fName, transparent);
        background->set_image(fName.u8string());
      }
    }
  } else { // >= 710
    int dataver = dec.read4();
    if (dataver != 800) {
      errStream << "GMK Background with inner version '" << ver << "' has image data with "
          << "unsupported version '" << dataver << "'" << std::endl;
      return nullptr;
    }
    w = dec.read4();
    h = dec.read4();
    if (w != 0 && h != 0) {
      dec.readBGRAImage(fName, w, h);
      background->set_image(fName.u8string());
    }
  }

  background->set_width(w);
  background->set_height(h);

  return background;
}

std::unique_ptr<Path> LoadPath(Decoder &dec, int /*ver*/, const std::string& /*name*/) {
  auto path = std::make_unique<Path>();

  path->set_smooth(dec.readBool());
  path->set_closed(dec.readBool());
  path->set_precision(dec.read4());
  dec.postponeName(path->mutable_background_room_name(), dec.read4(), TypeCase::kRoom);
  path->set_hsnap(dec.read4());
  path->set_vsnap(dec.read4());
  int nopoints = dec.read4();
  for (int i = 0; i < nopoints; i++) {
    auto point = path->add_points();
    point->set_x(dec.readD());
    point->set_y(dec.readD());
    point->set_speed(dec.readD());
  }

  return path;
}

std::unique_ptr<Script> LoadScript(Decoder &dec, int /*ver*/, const std::string& /*name*/) {
  auto script = std::make_unique<Script>();

  script->set_code(dec.readStr());

  return script;
}

std::unique_ptr<Font> LoadFont(Decoder &dec, int /*ver*/, const std::string& /*name*/) {
  auto font = std::make_unique<Font>();

  font->set_font_name(dec.readStr());
  font->set_size(dec.read4());
  font->set_bold(dec.readBool());
  font->set_italic(dec.readBool());

  int rangemin = dec.read2();
  font->set_charset(dec.read());
  int aa = dec.read();
  //if (aa == 0 && gmk < 810) aa = 3;
  font->set_antialias(aa);

  auto char_range = font->add_ranges();
  char_range->set_min(rangemin);
  char_range->set_max(dec.read4());

  return font;
}


struct PostponedAction {
  PostponedAction(std::string* field, std::vector<std::unique_ptr<Action>>&& actions) : _field(field), _actions(std::move(actions)) {}
  std::string* _field;
  std::vector<std::unique_ptr<Action>> _actions;
  void Parse() {
    std::vector<Action> actions;
    for (std::unique_ptr<Action>& action : _actions)
      actions.emplace_back(*action);
    _field->append(Actions2Code(actions)); 
    _actions.clear();
  }
};

static std::vector<PostponedAction> postponedActions;

int LoadActions(Decoder &dec, std::string* code, std::string eventName) {
  int ver = dec.read4();
  if (ver != 400) {
    errStream << "Unsupported GMK actions version '" << ver <<
      "' for event '" << eventName << "'" << std::endl;
    return 0;
  }

  int noacts = dec.read4();
  std::vector<std::unique_ptr<Action>> actions;
  for (int k = 0; k < noacts; k++) {
    auto action = std::make_unique<Action>();
    dec.skip(4);
    action->set_libid(dec.read4());
    action->set_id(dec.read4());
    action->set_kind(static_cast<ActionKind>(dec.read4()));
    action->set_use_relative(dec.readBool());
    action->set_is_question(dec.readBool());
    action->set_use_apply_to(dec.readBool());
    action->set_exe_type(static_cast<ActionExecution>(dec.read4()));
    action->set_function_name(dec.readStr());
    action->set_code_string(dec.readStr());

    int numofargs = dec.read4(); // number of library action's arguments
    int numofargkinds = dec.read4(); // number of library action's argument kinds
    auto argkinds = make_unique<int[]>(numofargkinds);
    for (int x = 0; x < numofargkinds; x++)
      argkinds[x] = dec.read4(); // argument x's kind

    int applies_to = dec.read4();
    switch (applies_to) {
      case -1:
        action->set_who_name("self");
        break;
      case -2:
        action->set_who_name("other");
        break;
      default:
        dec.postponeName(action->mutable_who_name(), applies_to, TypeCase::kObject);
    }
    action->set_relative(dec.readBool());

    int actualnoargs = dec.read4();
    for (int l = 0; l < actualnoargs; l++) {
      if (l >= numofargs) {
        dec.skip(dec.read4());
        continue;
      }
      auto argument = action->add_arguments();
      argument->set_kind(static_cast<ArgumentKind>(argkinds[l]));
      std::string strval = dec.readStr();

      using ArgumentMutator = std::pair<std::function<std::string*(Argument*)>, TypeCase>;
      using MutatorMap = std::unordered_map<ArgumentKind, ArgumentMutator>;

      static const MutatorMap mutatorMap({
        { ArgumentKind::ARG_SOUND,      {&Argument::mutable_sound,       TypeCase::kSound      }},
        { ArgumentKind::ARG_BACKGROUND, {&Argument::mutable_background,  TypeCase::kBackground }},
        { ArgumentKind::ARG_SPRITE,     {&Argument::mutable_sprite,      TypeCase::kSprite     }},
        { ArgumentKind::ARG_SCRIPT,     {&Argument::mutable_script,      TypeCase::kScript     }},
        { ArgumentKind::ARG_FONT,       {&Argument::mutable_font,        TypeCase::kFont       }},
        { ArgumentKind::ARG_OBJECT,     {&Argument::mutable_object,      TypeCase::kObject     }},
        { ArgumentKind::ARG_TIMELINE,   {&Argument::mutable_timeline,    TypeCase::kTimeline   }},
        { ArgumentKind::ARG_ROOM,       {&Argument::mutable_room,        TypeCase::kRoom       }},
        { ArgumentKind::ARG_PATH,       {&Argument::mutable_path,        TypeCase::kPath       }}
      });

      const auto &mutator = mutatorMap.find(argument->kind());
      if (mutator != mutatorMap.end()) {
        dec.postponeName(mutator->second.first(argument), std::stoi(strval), mutator->second.second);
      } else {
        argument->set_string(strval);
      }
    }

    action->set_is_not(dec.readBool());
    actions.emplace_back(std::move(action));
  }

  postponedActions.emplace_back(code, std::move(actions));

  return 1;
}

std::unique_ptr<Timeline> LoadTimeline(Decoder &dec, int /*ver*/, const std::string& /*name*/) {
  auto timeline = std::make_unique<Timeline>();

  int nomoms = dec.read4();
  for (int i = 0; i < nomoms; i++) {
    auto moment = timeline->add_moments();
    moment->set_step(dec.read4());
    if (!LoadActions(dec, moment->mutable_code(), "step_" + std::to_string(moment->step()))) return nullptr;
  }

  return timeline;
}

std::unique_ptr<Object> LoadObject(Decoder &dec, int /*ver*/, const std::string& /*name*/) {
  auto object = std::make_unique<Object>();

  dec.postponeName(object->mutable_sprite_name(), dec.read4(), TypeCase::kSprite);
  object->set_solid(dec.readBool());
  object->set_visible(dec.readBool());
  object->set_depth(dec.read4());
  object->set_persistent(dec.readBool());
  dec.postponeName(object->mutable_parent_name(), dec.read4(), TypeCase::kObject);
  dec.postponeName(object->mutable_mask_name(), dec.read4(), TypeCase::kSprite);

  int noEvents = dec.read4() + 1;
  for (int i = 0; i < noEvents; i++) {
    while (true) {
      int second = dec.read4();
      if (second == -1) break;

      auto *event = object->add_legacy_events();
      event->set_type(i);
      event->set_number(second);

      if (!LoadActions(dec, event->mutable_code(), event->name())) return nullptr;
    }
  }

  return object;
}

std::unique_ptr<Room> LoadRoom(Decoder &dec, int ver, const std::string& /*name*/) {
  auto room = std::make_unique<Room>();

  room->set_caption(dec.readStr());
  room->set_width(dec.read4());
  room->set_height(dec.read4());
  room->set_vsnap(dec.read4());
  room->set_hsnap(dec.read4());
  room->set_isometric(dec.readBool());
  room->set_speed(dec.read4());
  room->set_persistent(dec.readBool());
  room->set_color(dec.read4());
  //NOTE: GM8.1 is inconsistent packing a second boolean into this one.
  int clearBackgroundAndView = dec.read4();
  room->set_show_color((clearBackgroundAndView & 1) != 0);
  room->set_clear_view_background((clearBackgroundAndView & (1 << 1)) == 0);
  room->set_creation_code(dec.readStr());

  int nobackgrounds = dec.read4();
  for (int j = 0; j < nobackgrounds; j++) {
    auto background = room->add_backgrounds();
    background->set_visible(dec.readBool());
    background->set_foreground(dec.readBool());
    dec.postponeName(background->mutable_background_name(), dec.read4(), TypeCase::kBackground);
    background->set_x(dec.read4());
    background->set_y(dec.read4());
    background->set_htiled(dec.readBool());
    background->set_vtiled(dec.readBool());
    background->set_hspeed(dec.read4());
    background->set_vspeed(dec.read4());
    background->set_stretch(dec.readBool());
  }

  room->set_enable_views(dec.readBool());
  int noviews = dec.read4();
  for (int j = 0; j < noviews; j++) {
    auto view = room->add_views();
    view->set_visible(dec.readBool());
    view->set_xview(dec.read4());
    view->set_yview(dec.read4());
    view->set_wview(dec.read4());
    view->set_hview(dec.read4());
    view->set_xport(dec.read4());
    view->set_yport(dec.read4());
    if (ver > 520) {
      view->set_wport(dec.read4());
      view->set_hport(dec.read4());
    } else {
      //Older versions of GM assume port_size == view_size.
      view->set_wport(view->wview());
      view->set_hport(view->hview());
    }
    view->set_hborder(dec.read4());
    view->set_vborder(dec.read4());
    view->set_hspeed(dec.read4());
    view->set_vspeed(dec.read4());
    dec.postponeName(view->mutable_object_following(), dec.read4(), TypeCase::kObject);
  }

  int noinstances = dec.read4();
  for (int j = 0; j < noinstances; j++) {
    auto instance = room->add_instances();
    instance->set_x(dec.read4());
    instance->set_y(dec.read4());
    dec.postponeName(instance->mutable_object_type(), dec.read4(), TypeCase::kObject);
    instance->set_id(dec.read4());
    instance->set_creation_code(dec.readStr());
    instance->mutable_editor_settings()->set_locked(dec.readBool());
  }

  int notiles = dec.read4();
  for (int j = 0; j < notiles; j++) {
    auto tile = room->add_tiles();
    tile->set_x(dec.read4());
    tile->set_y(dec.read4());
    dec.postponeName(tile->mutable_background_name(), dec.read4(), TypeCase::kBackground);
    tile->set_xoffset(dec.read4());
    tile->set_yoffset(dec.read4());
    tile->set_width(dec.read4());
    tile->set_height(dec.read4());
    tile->set_depth(dec.read4());
    tile->set_id(dec.read4());
    tile->mutable_editor_settings()->set_locked(dec.readBool());
  }

  dec.readBool(); // REMEMBER_WINDOW_SIZE
  dec.read4(); // EDITOR_WIDTH
  dec.read4(); // EDITOR_HEIGHT
  dec.readBool(); // SHOW_GRID
  dec.readBool(); // SHOW_OBJECTS
  dec.readBool(); // SHOW_TILES
  dec.readBool(); // SHOW_BACKGROUNDS
  dec.readBool(); // SHOW_FOREGROUNDS
  dec.readBool(); // SHOW_VIEWS
  dec.readBool(); // DELETE_UNDERLYING_OBJECTS
  dec.readBool(); // DELETE_UNDERLYING_TILES
  if (ver == 520) dec.skip(6 * 4); //tile info
  // CURRENT_TAB, SCROLL_BAR_X, SCROLL_BAR_Y
  dec.read4(); dec.read4(); dec.read4();

  return room;
}

int LoadIncludes(Decoder &dec) {
  int ver = dec.read4();
  if (ver != 430 && ver != 600 && ver != 620 && ver != 800 && ver != 810) {
    errStream << "Unsupported GMK Includes version: " << ver << std::endl;
    return 0;
  }

  int no = dec.read4();
  for (int i = 0; i < no; i++) {
    if (ver >= 800) {
      dec.beginInflate();
      dec.skip(8); //last changed
    }
    ver = dec.read4();
    if (ver != 620 && ver != 800 && ver != 810) {
      errStream << "Unsupported GMK Include version: " << ver << std::endl;
      return 0;
    }

    auto include = std::make_unique<Include>();
    dec.readStr(); // filename
    dec.readStr(); // filepath
    dec.readBool(); // isOriginal
    include->set_size(dec.read4());
    include->set_store(dec.readBool());
    if (include->store()) { //store in editable?
      std::unique_ptr<char[]> data = dec.read(dec.read4()); // data
    }
    include->set_export_action(static_cast<Include::ExportAction>(dec.read4()));
    include->set_export_directory(dec.readStr());
    include->set_overwrite_existing(dec.readBool());
    include->set_free_after_export(dec.readBool());
    include->set_remove_at_game_end(dec.readBool());

    dec.endInflate();
  }

  return 1;
}

int LoadPackages(Decoder &dec) {
  int ver = dec.read4();
  if (ver != 700) {
    errStream << "Unsupported GMK Extension Packages version: " << ver << std::endl;
    return 0;
  }

  int no = dec.read4();
  for (int i = 0; i < no; i++) {
    dec.readStr(); // package name
  }

  return 1;
}

int LoadGameInformation(Decoder &dec) {
  int ver = dec.read4();
  if (ver != 430 && ver != 600 && ver != 620 && ver != 800 && ver != 810) {
    errStream << "Unsupported GMK Game Information version: " << ver << std::endl;
    return 0;
  }

  if (ver >= 800) dec.beginInflate();
  dec.read4(); // int bc, if non-zero then it's set
  // if (bc >= 0) // background color
  if (ver < 800)
    dec.readBool(); // embed game window
  else
    dec.readBool(); // show help in a separate window, inverted so negate it !
  if (ver > 430) {
    dec.readStr(); // form caption
    // LEFT, TOP, WIDTH, HEIGHT
    dec.read4(); dec.read4(); dec.read4(); dec.read4();
    // SHOW_BORDER, ALLOW_RESIZE, STAY_ON_TOP, PAUSE_GAME
    dec.readBool(); dec.readBool(); dec.readBool(); dec.readBool();
  }
  if (ver >= 800) dec.skip(8); //last changed
  dec.readStr(); // the rtf text
  dec.endInflate();

  return 1;
}

using FactoryFunction = std::function<std::unique_ptr<google::protobuf::Message>(Decoder&, int, const std::string&)>;

struct GroupFactory {
  TypeCase type;
  set<int> supportedGroupVersions;
  set<int> supportedVersions;
  FactoryFunction loadFunc;
};

int LoadGroup(Decoder &dec, TypeMap &typeMap, GroupFactory groupFactory) {
  TypeCase type = groupFactory.type;
  int groupVer = dec.read4();
  if (!groupFactory.supportedGroupVersions.count(groupVer)) {
    errStream << "GMK group '" << type << "' with version '" << groupVer << "' is unsupported" << std::endl;
    return 0;
  }

  int count = dec.read4();
  for (int i = 0; i < count; ++i) {
    if (groupVer >= 800) dec.beginInflate();
    if (!dec.readBool()) { // was deleted
      dec.endInflate();
      continue;
    }
    std::string name = dec.readStr();
    if (groupVer == 800) dec.skip(8); // last changed

    int ver = dec.read4();
    if (!groupFactory.supportedVersions.count(ver)) {
      errStream << "GMK resource of type '" << type << "' with name '" << name
          << "' has an unsupported version '" << ver << "'" << std::endl;
      return 0;
    }

    auto res = groupFactory.loadFunc(dec, ver, name);
    if (!res) {
      errStream << "There was a problem reading GMK resource of type '" << type << "' with name '" << name
          << "' and the project cannot be loaded" << std::endl;
      return 0;
    }

    const google::protobuf::Descriptor *desc = res->GetDescriptor();
    const google::protobuf::Reflection *refl = res->GetReflection();
    const google::protobuf::FieldDescriptor *field = desc->FindFieldByName("id");
    if (field) {
      refl->SetInt32(res.get(), field, i);
    }

    auto &resMap = typeMap[type];
    resMap[i] = std::move(res);
    dec.endInflate();
  }

  return 1;
}

void LoadTree(Decoder &dec, TypeMap &typeMap, TreeNode* root) {
  const int status = dec.read4();
  const int kind = dec.read4();
  const int id = dec.read4();
  const std::string name = dec.readStr();
  const int children = dec.read4();

  TreeNode *node = root->mutable_folder()->add_children();
  node->set_name(name);
  if (status <= 2) {
    node->mutable_folder();
    for (int i = 0; i < children; i++) {
      LoadTree(dec, typeMap, node);
    }
  } else {
    static const TypeCase RESOURCE_KIND[] = { TypeCase::TYPE_NOT_SET,TypeCase::kObject,TypeCase::kSprite,TypeCase::kSound,
      TypeCase::kRoom,TypeCase::TYPE_NOT_SET,TypeCase::kBackground,TypeCase::kScript,TypeCase::kPath,TypeCase::kFont,TypeCase::TYPE_NOT_SET,
      TypeCase::TYPE_NOT_SET,TypeCase::kTimeline,TypeCase::TYPE_NOT_SET,TypeCase::kShader };

    const TypeCase type = RESOURCE_KIND[kind];

    // Handle postponed id->name references
    dec.processPostoned(name, id, type);

    auto typeMapIt = typeMap.find(type);
    if (typeMapIt == typeMap.end()) {
      node->mutable_unknown();
      errStream << "No map of ids to protocol buffers for GMK kind '" << kind
          << "' so tree node with name '" << name << "' will not have "
          << "its protocol buffer set" << std::endl;
      return;
    }
    auto &resMap = typeMapIt->second;

    const google::protobuf::Descriptor *desc = node->GetDescriptor();
    const google::protobuf::Reflection *refl = node->GetReflection();
    const google::protobuf::FieldDescriptor *field = desc->FindFieldByNumber(type);
    if (!field) {
      errStream << "TreeNode protocol buffer does not have a field for GMK type '" << type << "'" << std::endl;
      return;
    }
    refl->SetAllocatedMessage(node, resMap[id].release(), field);
  }
}

} //namespace gmk_internal

namespace egm {

using namespace gmk_internal;

std::unique_ptr<buffers::Project> GMKFileFormat::LoadProject(const fs::path& fName) const {
  static const vector<GroupFactory> groupFactories({
    { TypeCase::kSound,      { 400, 800      }, { 440, 600, 800      }, gmk_internal::LoadSound      },
    { TypeCase::kSprite,     { 400, 800, 810 }, { 400, 542, 800, 810 }, gmk_internal::LoadSprite     },
    { TypeCase::kBackground, { 400, 800      }, { 400, 543, 710      }, gmk_internal::LoadBackground },
    { TypeCase::kPath,       { 420, 800      }, { 530                }, gmk_internal::LoadPath       },
    { TypeCase::kScript,     { 400, 800, 810 }, { 400, 800, 810      }, gmk_internal::LoadScript     },
    { TypeCase::kFont,       { 440, 540, 800 }, { 540, 800           }, gmk_internal::LoadFont       },
    { TypeCase::kTimeline,   { 500, 800      }, { 500                }, gmk_internal::LoadTimeline   },
    { TypeCase::kObject,     { 400, 800      }, { 430                }, gmk_internal::LoadObject     },
    { TypeCase::kRoom,       { 420, 800      }, { 520, 541           }, gmk_internal::LoadRoom       }
  });

  TypeMap typeMap;
  std::ifstream in(fName, std::ios::binary);
  if (!in) {
    errStream << "Could not open GMK for reading: " << fName << std::endl;
    return nullptr;
  }
  Decoder dec(in);

  int identifier = dec.read4();
  if (identifier != 1234321) {
    errStream << "Invalid GMK identifier: " << identifier << std::endl;
    return nullptr;
  } else {
    outStream << "identifier: " << identifier << std::endl;
  }
  int ver = dec.read4();
  if (ver != 530 && ver != 600 && ver != 701 && ver != 800 && ver != 810) {
    errStream << "Unsupported GMK version: " << ver << std::endl;
    return nullptr;
  } else {
    outStream << "game version: " << ver << std::endl;
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
  outStream << "game id: " << game_id << std::endl;
  dec.skip(16); //16 bytes GAME_GUID

  Settings settings;
  if (!LoadSettings(dec, settings)) return nullptr;

  if (ver >= 800) {
    if (!LoadTriggers(dec)) return nullptr;
    if (!LoadConstants(dec)) return nullptr;
  }

  for (auto factory : groupFactories) {
    if (!LoadGroup(dec, typeMap, factory)) return nullptr;
  }

  // last instance id, last tile id read after the final room is read
  dec.read4(); dec.read4();

  if (ver >= 700) {
    if (!LoadIncludes(dec)) return nullptr;
    if (!LoadPackages(dec)) return nullptr;
  }

  if (!LoadGameInformation(dec)) return nullptr;

  // Library Creation Code
  ver = dec.read4();
  if (ver != 500) {
    errStream << "Unsupported GMK Library Creation Code version: " << ver << std::endl;
    return 0;
  }
  int no = dec.read4();
  for (int i = 0; i < no; i++)
    dec.skip(dec.read4());

  // Room Execution Order
  ver = dec.read4();
  if (ver != 500 && ver != 540 && ver != 700) {
    errStream << "Unsupported GMK Room Execution Order version: " << ver << std::endl;
    return 0;
  }
  dec.skip(dec.read4() * 4);

  // Project Tree
  auto root = make_unique<TreeNode>();
  int rootnodes = (ver > 540) ? 12 : 11;
  while (rootnodes-- > 0) {
    LoadTree(dec, typeMap, root.get());
  }
  
  // Handle postponed DnD conversion
  for(auto&& a : postponedActions) a.Parse();
  postponedActions.clear();

  auto proj = std::make_unique<buffers::Project>();
  buffers::Game *game = proj->mutable_game();
  game->set_allocated_root(root.release());
  // ensure all temp data files are written and the paths are set in the protos
  dec.processTempFileFutures();
  
  LegacyEventsToEGM(proj.get(), _event_data);

  return proj;
}

}  //namespace egm
