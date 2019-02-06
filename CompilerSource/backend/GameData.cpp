/*
 * Copyright (C) 2018 Josh Ventura
 *
 * This file is part of ENIGMA.
 * ENIGMA is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#include "GameData.h"

#include "lodepng.h"

#include <map>
#include <string>
#include <iostream>

#include <zlib.h>

using std::cout;
using std::cerr;
using std::endl;

using TypeCase = buffers::TreeNode::TypeCase;

static inline uint32_t javaColor(int c) {
  if (!(c & 0xFF)) return 0xFFFFFFFF;
  return ((c & 0xFF0000) >> 8) | ((c & 0xFF00) << 8) | ((c & 0xFF000000) >> 24);
}

static inline std::string string_replace_all(std::string str, std::string substr, std::string nstr)
{
  size_t pos = 0;
  while ((pos = str.find(substr, pos)) != std::string::npos)
  {
    str.replace(pos, substr.length(), nstr);
    pos += nstr.length();
  }
  return str;
}

static unsigned char* zlib_compress(unsigned char* inbuffer, int &actualsize)
{
  uLongf outsize=(int)(actualsize*1.1)+12;
  Bytef* outbytef=new Bytef[outsize];

  compress(outbytef,&outsize,(Bytef*)inbuffer,actualsize);

  actualsize = outsize;

  return (unsigned char*)outbytef;
}

static std::string Argument2Code(const buffers::resources::Argument& arg) {
  using buffers::resources::ArgumentKind;
  std::string val = arg.string();

  switch (arg.kind()) {
    case ArgumentKind::ARG_BOTH:
      // treat as literal if starts with quote (")
      if (val[0] == '"' || val[0] == '\'') return val;
      // else fall through
    case ArgumentKind::ARG_STRING:
      return '\"' + string_replace_all(string_replace_all(val, "\\", "\\\\"), "\"", "\"+'\"'+\"") + '\"';
    case ArgumentKind::ARG_BOOLEAN:
      return std::to_string(val != "0");
    case ArgumentKind::ARG_SPRITE:
      return arg.has_sprite() ? arg.sprite() : "-1";
    case ArgumentKind::ARG_SOUND:
      return arg.has_sound() ? arg.sound() : "-1";
    case ArgumentKind::ARG_BACKGROUND:
      return arg.has_background() ? arg.background() : "-1";
    case ArgumentKind::ARG_PATH:
      return arg.has_path() ? arg.path() : "-1";
    case ArgumentKind::ARG_SCRIPT:
      return arg.has_script() ? arg.script() : "-1";
    case ArgumentKind::ARG_OBJECT:
      return arg.has_object() ? arg.object() : "-1";
    case ArgumentKind::ARG_ROOM:
      return arg.has_room() ? arg.room() : "-1";
    case ArgumentKind::ARG_FONT:
      return arg.has_font() ? arg.font() : "-1";
    case ArgumentKind::ARG_TIMELINE:
      return arg.has_timeline() ? arg.timeline() : "-1";
    case ArgumentKind::ARG_MENU:
    case ArgumentKind::ARG_COLOR:
    default:
      if (val.empty()) {
        if (arg.kind() == ArgumentKind::ARG_STRING)
          return "\"\"";
        else
          return "0";
      }
      // else fall all the way through
  }

  return val;
}

static std::string Actions2Code(const ::google::protobuf::RepeatedPtrField< buffers::resources::Action >& actions) {
  using buffers::resources::ActionKind;
  using buffers::resources::ActionExecution;
  std::string code = "";

  int numberOfBraces = 0; // gm ignores brace actions which are in the wrong place or missing
  int numberOfIfs = 0; // gm allows multipe else actions after 1 if, so its important to track the number

  for (const auto &action : actions) {
    const auto &args = action.arguments();

    bool in_with = action.use_apply_to() && action.who_name() != "self";
    if (in_with)
      code += "with (" + action.who_name() + ")\n";

    switch (action.kind()) {
      case ActionKind::ACT_BEGIN:
        code += '{';
        numberOfBraces++;
        break;
      case ActionKind::ACT_END:
        if (numberOfBraces > 0) {
          code += '}';
          numberOfBraces--;
        }
        break;
      case ActionKind::ACT_ELSE:
        if (numberOfIfs > 0) {
          code += "else ";
          numberOfIfs--;
        }
        break;
      case ActionKind::ACT_EXIT:
        code += "exit;";
        break;
      case ActionKind::ACT_REPEAT:
        code += "repeat (" + args.Get(0).string() + ")";
        break;
      case ActionKind::ACT_VARIABLE:
        code += args.Get(0).string();
        if (action.relative())
          code += " += ";
        else
          code += " = ";
        code += args.Get(1).string();
        break;
      case ActionKind::ACT_CODE:
        code += "{\n" + args.Get(0).string() + "\n/**/\n}";
        break;
      case ActionKind::ACT_NORMAL:
        if (action.exe_type() == ActionExecution::EXEC_NONE) break;

        if (action.is_question()) {
          code += "__if__ = ";
          numberOfIfs++;
        }

        if (action.is_not())
          code += "!";

        if (action.relative()) {
          if (action.is_question())
            code += "(argument_relative = " + std::to_string(action.relative()) + ", ";
          else
            code += "{\nargument_relative = " + std::to_string(action.relative()) + ";\n";
        }

        if (action.is_question() && action.exe_type() == ActionExecution::EXEC_CODE)
          code += action.code_string();
        else
          code += action.function_name();

        if (action.exe_type() == ActionExecution::EXEC_FUNCTION) {
          code += '(';
          for (int i = 0; i < args.size(); i++) {
            if (i != 0)
              code += ',';
            code += Argument2Code(args.Get(i));
          }
          code += ')';
        }

        if (action.relative())
          code += action.is_question() ? ");" : "\n}";
        if (action.is_question()) {
          code += "\nif (__if__)";
        }
        break;
      default:
        break;
    }
    code += '\n';
  }

  // someone forgot the closing block action
  if (numberOfBraces > 0)
    for (int i = 0; i < numberOfBraces; i++)
      code += "\n}";

  if (numberOfIfs > 0)
    code = "var __if__ = false;\n" + code;

  return code;
}

struct ESLookup {
  struct Lookup {
    std::string kind;
    std::map<int, std::string> names;
    template<class T> Lookup(T* d, int sz, std::string &&k): kind(k) {
      for (int i = 0; i < sz; ++i) names[d[i].id] = d[i].name;
    }
    std::string operator[](int x) const {
      auto res = names.find(x);
      if (res == names.end()) {
        cerr << "ERROR: " << kind << ' ' << x << " has no name; "
                "this will almost certainly result in compile errors!" << endl;
        return "ERROR_RESOURCE_NOT_FOUND";
      }
      return res->second;
    }
  };
  Lookup sprite, background, script, object;
  ESLookup(EnigmaStruct *es):
    sprite(es->sprites, es->spriteCount, "sprite"),
    background(es->backgrounds, es->backgroundCount, "background"),
    script(es->scripts, es->scriptCount, "script"),
    object(es->gmObjects, es->gmObjectCount, "object") {}
};

SpriteData::SpriteData(const buffers::resources::Sprite &q, const std::string& name):
  buffers::resources::Sprite(q), name(name) {
  for (auto sub : q.subimages())
    image_data.emplace_back(sub);
}
SpriteData::SpriteData(const ::Sprite &sprite):
  name(sprite.name) {
  set_id(sprite.id);

  // Discarded: sprite.transparent
  // Discarded: sprite.maskShapes

  set_shape((buffers::resources::Sprite_Shape) sprite.shape);  // Enum correspondence guaranteed
  set_alpha_tolerance(sprite.alphaTolerance);
  set_separate_mask(sprite.separateMask);
  set_smooth_edges(sprite.smoothEdges);
  set_preload(sprite.preload);
  set_origin_x(sprite.originX);
  set_origin_y(sprite.originY);

  set_bbox_mode((buffers::resources::Sprite_BoundingBox) sprite.bbMode);  // Enum correspondence guaranteed
  set_bbox_left(sprite.bbLeft);
  set_bbox_right(sprite.bbRight);
  set_bbox_top(sprite.bbTop);
  set_bbox_bottom(sprite.bbBottom);

  for (int i = 0; i < sprite.subImageCount; ++i)
    image_data.emplace_back(sprite.subImages[i].image);
}

SoundData::SoundData(const buffers::resources::Sound &q, const std::string& name):
  buffers::resources::Sound(q), name(name) {
  // Open sound
  FILE *afile = fopen(q.data().c_str(),"rb");
  if (!afile)
    return;

  // Buffer sound
  fseek(afile,0,SEEK_END);
  const size_t flen = ftell(afile);
  unsigned char *fdata = new unsigned char[flen];
  fseek(afile,0,SEEK_SET);
  if (fread(fdata,1,flen,afile) != flen)
    puts("WARNING: Resource stream cut short while loading sound data");
  fclose(afile);

  audio = BinaryData(fdata, fdata + flen);
}
SoundData::SoundData(const ::Sound &sound):
  name(sound.name),
  audio(sound.data, sound.data + sound.size) {
  set_id(sound.id);

  // Discarded: sound.chorus
  // Discarded: sound.echo
  // Discarded: sound.flanger
  // Discarded: sound.gargle
  // Discarded: sound.reverb

  set_kind((buffers::resources::Sound_Kind) sound.kind);
  set_file_extension(sound.fileType);
  set_file_name(sound.fileName);
  set_volume(sound.volume);
  set_pan(sound.pan);
  set_preload(sound.preload);
}

BackgroundData::BackgroundData(const buffers::resources::Background &q, const std::string& name):
  buffers::resources::Background(q), name(name), image_data(q.image()) {}
BackgroundData::BackgroundData(const ::Background &background):
  name(background.name),
  image_data(background.backgroundImage) {
  set_id(background.id);

  legacy_transparency = background.transparent;

  // Discarded: background.smoothEdges

  set_preload(background.preload);
  set_use_as_tileset(background.useAsTileset);
  set_tile_width(background.tileWidth);
  set_tile_height(background.tileHeight);
  set_horizontal_offset(background.hOffset);
  set_vertical_offset(background.vOffset);
  set_horizontal_spacing(background.hSep);
  set_vertical_spacing(background.vSep);
}

FontData::FontData(const buffers::resources::Font &q, const std::string& name):
  buffers::resources::Font(q), name(name) {}
FontData::FontData(const ::Font &font):
  name(font.name) {
  set_id(font.id);

  set_font_name(font.fontName);
  set_size(font.size);
  set_bold(font.bold);
  set_italic(font.italic);

  for (int i = 0; i < font.glyphRangeCount; ++i) {
    normalized_ranges.emplace_back(font.glyphRanges[i]);
    for (const auto &glyph : normalized_ranges.back().glyphs) {
      *add_glyphs() = glyph.metrics;
    }
  }
}

FontData::NormalizedRange::NormalizedRange(const ::GlyphRange &range):
    min(range.rangeMin), max(range.rangeMax) {
  const int gcount = range.rangeMax - range.rangeMin + 1;
  glyphs.reserve(gcount);
  for (int i = 0; i < gcount; ++i) {
    glyphs.emplace_back(range.glyphs[i]);
  }
}

FontData::GlyphData::GlyphData(const ::Glyph &glyph):
    ImageData(glyph.width, glyph.height, glyph.data,
              glyph.width * glyph.height) { // Glyph images are alpha-only.
  metrics.set_origin(glyph.origin);
  metrics.set_baseline(glyph.baseline);
  metrics.set_advance(glyph.advance);
  metrics.set_width(glyph.width);
  metrics.set_height(glyph.height);
}

PathData::PathData(const buffers::resources::Path &q, const std::string& name):
  buffers::resources::Path(q), name(name) {}
PathData::PathData(const ::Path &path):
  name(path.name) {
  set_id(path.id);

  set_smooth(path.smooth);
  set_closed(path.closed);
  set_precision(path.precision);

	// Discarded: backgroundRoomId;
  set_hsnap(path.snapX);
  set_vsnap(path.snapY);

  for (int i = 0; i < path.pointCount; ++i) {
    auto *p = add_points();
    p->set_x(path.points[i].x);
    p->set_y(path.points[i].y);
    p->set_speed(path.points[i].speed);
  }
}

ScriptData::ScriptData(const buffers::resources::Script &q, const std::string& name):
  buffers::resources::Script(q), name(name) {}
ScriptData::ScriptData(const ::Script &script):
  name(script.name) {
  set_id(script.id);
  set_code(script.code);
}

ShaderData::ShaderData(const buffers::resources::Shader &q, const std::string& name):
  buffers::resources::Shader(q), name(name) {}
ShaderData::ShaderData(const ::Shader &shader):
  name(shader.name) {
  set_id(shader.id);

  set_vertex_code(shader.vertex);
  set_fragment_code(shader.fragment);
  set_type(shader.type);
  set_precompile(shader.precompile);

}

TimelineData::TimelineData(const buffers::resources::Timeline &q, const std::string& name):
  buffers::resources::Timeline(q), name(name) {
  for (int i = 0; i < moments_size(); ++i) {
    auto* mmt = mutable_moments(i);
    mmt->set_code(Actions2Code(mmt->actions()));
  }
}
TimelineData::TimelineData(const ::Timeline &timeline):
  name(timeline.name) {
  set_id(timeline.id);

  for (int i = 0; i < timeline.momentCount; ++i) {
    auto *moment = add_moments();
    moment->set_step(timeline.moments[i].stepNo);
    moment->set_code(timeline.moments[i].code);
  }
}

ObjectData::ObjectData(const buffers::resources::Object &q, const std::string& name):
  buffers::resources::Object(q), name(name) {
  for (int i = 0; i < events_size(); ++i) {
    auto* evt = mutable_events(i);
    evt->set_code(Actions2Code(evt->actions()));
  }
}
ObjectData::ObjectData(const ::GmObject &object, const ESLookup &lookup):
  name(object.name) {
  set_id(object.id);

  if (object.spriteId >= 0)
    set_sprite_name(lookup.sprite[object.spriteId]);
  set_solid(object.solid);
  set_visible(object.visible);
  set_depth(object.depth);
  set_persistent(object.persistent);
  if (object.parentId >= 0)
    set_parent_name(lookup.object[object.parentId]);
  if (object.maskId >= 0)
    set_mask_name(lookup.sprite[object.maskId]);

  for (int i = 0; i < object.mainEventCount; ++i) {
    for (int j = 0; j < object.mainEvents[i].eventCount; ++j) {
      auto *event = add_events();
      event->set_type(object.mainEvents[i].id);
      event->set_number(object.mainEvents[i].events[j].id);
      event->set_code(object.mainEvents[i].events[j].code);
    }
  }
}

RoomData::RoomData(const buffers::resources::Room &q, const std::string& name):
  buffers::resources::Room(q), name(name) {}
RoomData::RoomData(const ::Room &room, const ESLookup &lookup):
  name(room.name) {
  cout << "Import room." << endl;
  set_id(room.id);

  if (room.caption)
    set_caption(room.caption);
  set_width(room.width);
  set_height(room.height);

  set_hsnap(room.snapX);
  set_vsnap(room.snapY);
  set_isometric(room.isometric);

  set_speed(room.speed);
  set_persistent(room.persistent);
  set_color(javaColor(room.backgroundColor));  // RGBA
  set_show_color(room.drawBackgroundColor);
  if (room.creationCode)
    set_creation_code(room.creationCode);

  set_enable_views(room.enableViews);

  for (int i = 0; i < room.backgroundDefCount; ++i) {
    auto *background = add_backgrounds();
    const auto &es_background = room.backgroundDefs[i];

    background->set_visible(es_background.visible);
    background->set_foreground(es_background.foreground);
    background->set_x(es_background.x);
    background->set_y(es_background.y);
    background->set_htiled(es_background.tileHoriz);
    background->set_vtiled(es_background.tileVert);
    background->set_hspeed(es_background.hSpeed);
    background->set_vspeed(es_background.vSpeed);
    background->set_stretch(es_background.stretch);
    if (es_background.backgroundId >= 0)
      background->set_background_name(lookup.background[es_background.backgroundId]);
  }

  for (int i = 0; i < room.viewCount; ++i) {
    auto *view = add_views();
    const auto &es_view = room.views[i];

    view->set_visible(es_view.visible);
    view->set_xview  (es_view.viewX);
    view->set_yview  (es_view.viewY);
    view->set_wview  (es_view.viewW);
    view->set_hview  (es_view.viewH);
    view->set_xport  (es_view.portX);
    view->set_yport  (es_view.portY);
    view->set_wport  (es_view.portW);
    view->set_hport  (es_view.portH);
    view->set_hborder(es_view.borderH);
    view->set_vborder(es_view.borderV);
    view->set_hspeed (es_view.speedH);
    view->set_vspeed (es_view.speedV);
    if (es_view.objectId >= 0)
      view->set_object_following(lookup.object[es_view.objectId]);
  }

  for (int i = 0; i < room.instanceCount; ++i) {
    auto *instance = add_instances();
    const auto &es_instance = room.instances[i];
    // Discarded: es_instance.locked

    instance->set_id(es_instance.id);
    instance->set_object_type(lookup.object[es_instance.objectId]);
    instance->set_x(es_instance.x);
    instance->set_y(es_instance.y);
    if (es_instance.creationCode)
      instance->set_creation_code(es_instance.creationCode);
    if (es_instance.preCreationCode)
      instance->set_initialization_code(es_instance.preCreationCode);
  }

  for (int i = 0; i < room.tileCount; ++i) {
    auto *tile = add_tiles();
    const auto &es_tile = room.tiles[i];

    tile->set_id(es_tile.id);
    tile->set_x(es_tile.roomX);
    tile->set_y(es_tile.roomY);
    tile->set_xoffset(es_tile.bgX);
    tile->set_yoffset(es_tile.bgY);
    tile->set_width(es_tile.width);
    tile->set_height(es_tile.height);
    tile->set_depth(es_tile.depth);
    tile->set_background_name(lookup.background[es_tile.backgroundId]);
    tile->mutable_editor_settings()->set_locked(es_tile.locked);
  }
}

static void ImportSettings(const ::GameSettings &settings, const ESLookup &lookup, buffers::resources::Settings& set) {
  cout << "Import game settings." << endl;

  buffers::resources::General *gen = set.mutable_general();
  gen->set_game_id(settings.gameId);
  gen->set_version_major(settings.versionMajor);
  gen->set_version_minor(settings.versionMinor);
  gen->set_version_release(settings.versionRelease);
  gen->set_version_build(settings.versionBuild);
  if (settings.company)
    gen->set_company(settings.company);
  if (settings.product)
    gen->set_product(settings.product);
  if (settings.copyright)
    gen->set_copyright(settings.copyright);
  if (settings.description)
    gen->set_description(settings.description);
  if (settings.gameIcon)
    gen->set_game_icon(settings.gameIcon);
  gen->set_show_cursor(settings.displayCursor);

  buffers::resources::Graphics *gfx = set.mutable_graphics();
  gfx->set_color_outside_room_region(javaColor(settings.colorOutsideRoom));
  gfx->set_allow_fullscreen_change(settings.letF4SwitchFullscreen);
  gfx->set_interpolate_textures(settings.interpolate);
  gfx->set_force_software_vertex_processing(settings.forceSoftwareVertexProcessing);
  gfx->set_use_synchronization(settings.useSynchronization);
  gfx->set_view_scale(settings.scaling);

  buffers::resources::Windowing *win = set.mutable_windowing();
  win->set_start_in_fullscreen(settings.startFullscreen);
  win->set_freeze_on_lose_focus(settings.freezeOnLoseFocus);
  win->set_is_sizeable(settings.allowWindowResize);
  win->set_show_border(!settings.dontDrawBorder);
  win->set_show_icons(!settings.dontShowButtons);
  win->set_stay_on_top(settings.alwaysOnTop);
  win->set_treat_close_as_escape(settings.treatCloseAsEscape);

  buffers::resources::Info *inf = set.mutable_info();
  if (settings.author)
    inf->set_author_name(settings.author);
  if (settings.version)
    inf->set_version(settings.version);
  if (settings.lastChanged)
    inf->set_last_changed(settings.lastChanged);
  if (settings.information)
    inf->set_information(settings.information);

  //TODO: do keyboard mapping
  buffers::resources::Shortcuts *sht = set.mutable_shortcuts();
  sht->set_let_escape_end_game(settings.letEscEndGame);
}

ImageData::ImageData(const Image &img):
    width(img.width), height(img.height),
    pixels(img.data, img.data + img.dataSize){}
ImageData::ImageData(int w, int h, const uint8_t *data, size_t size):
    width(w), height(h), pixels(data, data + size) {}
ImageData::ImageData(const std::string& filePath) {
  unsigned error;
  unsigned char* image;
  unsigned pngwidth, pngheight;

  error = lodepng_decode32_file(&image, &pngwidth, &pngheight, filePath.c_str());
  if (error)
  {
    printf("error %u: %s\n", error, lodepng_error_text(error));
    return;
  }

  unsigned ih,iw;
  const int bitmap_size = pngwidth*pngheight*4;
  unsigned char* bitmap = new unsigned char[bitmap_size](); // Initialize to zero.

  for (ih = 0; ih < pngheight; ih++) {
    unsigned tmp = ih*pngwidth*4;
    for (iw = 0; iw < pngwidth; iw++) {
      bitmap[tmp+0] = image[4*pngwidth*ih+iw*4+2];
      bitmap[tmp+1] = image[4*pngwidth*ih+iw*4+1];
      bitmap[tmp+2] = image[4*pngwidth*ih+iw*4+0];
      bitmap[tmp+3] = image[4*pngwidth*ih+iw*4+3];
      tmp+=4;
    }
  }

  free(image);
  width  = pngwidth;
  height = pngheight;
  int dataSize = bitmap_size;
  const uint8_t* data = reinterpret_cast<const uint8_t*>(zlib_compress(bitmap, dataSize));
  pixels = BinaryData(data, data + dataSize);
}

GameData::GameData(EnigmaStruct *es): filename(es->filename ?: "") {
  cout << "Translating EnigmaStruct" << endl;
  cout << "- Indexing names" << endl;
  ESLookup lookup(es);

  cout << "- Transferring " << es->spriteCount << " sprites" << endl;
  for (int i = 0; i < es->spriteCount; ++i)
    sprites.emplace_back(es->sprites[i]);
  cout << "- Transferring " << es->soundCount << " sounds" << endl;
  for (int i = 0; i < es->soundCount; ++i)
    sounds.emplace_back(es->sounds[i]);
  cout << "- Transferring " << es->backgroundCount << " backgrounds" << endl;
  for (int i = 0; i < es->backgroundCount; ++i)
    backgrounds.emplace_back(es->backgrounds[i]);
  cout << "- Transferring " << es->fontCount << " fonts" << endl;
  for (int i = 0; i < es->fontCount; ++i)
    fonts.emplace_back(es->fonts[i]);
  cout << "- Transferring " << es->pathCount << " paths" << endl;
  for (int i = 0; i < es->pathCount; ++i)
    paths.emplace_back(es->paths[i]);
  cout << "- Transferring " << es->scriptCount << " scripts" << endl;
  for (int i = 0; i < es->scriptCount; ++i)
    scripts.emplace_back(es->scripts[i]);
  cout << "- Transferring " << es->shaderCount << " shaders" << endl;
  for (int i = 0; i < es->shaderCount; ++i)
    shaders.emplace_back(es->shaders[i]);
  cout << "- Transferring " << es->timelineCount << " timelines" << endl;
  for (int i = 0; i < es->timelineCount; ++i)
    timelines.emplace_back(es->timelines[i]);
  cout << "- Transferring " << es->gmObjectCount << " objects" << endl;
  for (int i = 0; i < es->gmObjectCount; ++i)
    objects.emplace_back(es->gmObjects[i], lookup);
  cout << "- Transferring " << es->roomCount << " rooms" << endl;
  for (int i = 0; i < es->roomCount; ++i)
    rooms.emplace_back(es->rooms[i], lookup);

  cout << "- Transferring " << es->constantCount <<  " constants" << endl;
  for (int i = 0; i < es->constantCount; ++i)
    constants.emplace_back(es->constants[i].name, es->constants[i].value);
  cout << "- Transferring " << es->extensionCount <<  " extensions" << endl;
  for (int i = 0; i < es->extensionCount; ++i)
    extensions.emplace_back(es->extensions[i].name, es->extensions[i].path);
  cout << "- Transferring " << es->packageCount <<  " packages" << endl;
  for (int i = 0; i < es->packageCount; ++i)
    packages.push_back(es->packages[i]);

  cout << "- Not transferring game info" << endl;
  buffers::resources::GameInformation gameInfo;
  ImportSettings(es->gameSettings, lookup, settings);

  cout << "Transfer complete." << endl << endl;
}

GameData::GameData(const buffers::Project &proj): filename("") {
  cout << "Flattening tree." << endl;

  FlattenTree(proj.game().root());

  cout << "Transfer complete." << endl << endl;
}

void GameData::FlattenTree(const buffers::TreeNode &root) {
  switch (root.type_case()) {
    case TypeCase::kFolder: break;
    case TypeCase::kSprite: sprites.emplace_back(root.sprite(), root.name()); break;
    case TypeCase::kSound: sounds.emplace_back(root.sound(), root.name()); break;
    case TypeCase::kBackground: backgrounds.emplace_back(root.background(), root.name()); break;
    case TypeCase::kPath: paths.emplace_back(root.path(), root.name()); break;
    case TypeCase::kScript: scripts.emplace_back(root.script(), root.name()); break;
    case TypeCase::kShader: shaders.emplace_back(root.shader(), root.name()); break;
    case TypeCase::kFont: fonts.emplace_back(root.font(), root.name()); break;
    case TypeCase::kTimeline: timelines.emplace_back(root.timeline(), root.name()); break;
    case TypeCase::kObject: objects.emplace_back(root.object(), root.name()); break;
    case TypeCase::kRoom: rooms.emplace_back(root.room(), root.name()); break;
    case TypeCase::kInclude: /*includes.emplace_back(root.include());*/ break;
    case TypeCase::kSettings: /*settings.emplace_back(root.settings());*/ break;
    default: cout << "- Not transferring " << root.name() << endl; break;
  }

  for (auto child : root.child()) {
    FlattenTree(child);
  }
}
