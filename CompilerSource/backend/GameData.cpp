/*
 * Copyright (C) 2018 Josh Ventura
 *
 * This file is part of ENIGMA.
 * ENIGMA is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#include "GameData.h"

#include "libpng-util.h"

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

static unsigned char* zlib_compress(unsigned char* inbuffer, int &actualsize)
{
  uLongf outsize=(int)(actualsize*1.1)+12;
  Bytef* outbytef=new Bytef[outsize];

  compress(outbytef,&outsize,(Bytef*)inbuffer,actualsize);

  actualsize = outsize;

  return (unsigned char*)outbytef;
}

BinaryData loadBinaryData(const std::string &filePath, int &errorc) {
  FILE *afile = fopen(filePath.c_str(),"rb");
  if (!afile) {
    errorc = -1;
    return BinaryData();
  }

  fseek(afile,0,SEEK_END);
  const size_t flen = ftell(afile);
  unsigned char *fdata = new unsigned char[flen];
  fseek(afile,0,SEEK_SET);
  if (fread(fdata,1,flen,afile) != flen)
    puts("WARNING: Resource stream cut short while loading data");
  fclose(afile);

  errorc = 0;
  return BinaryData(fdata, fdata + flen);
}

ImageData loadImageData(const std::string &filePath, int &errorc) {
  unsigned error;
  unsigned char* image;
  unsigned pngwidth, pngheight;

  error = libpng_decode32_file(&image, &pngwidth, &pngheight, filePath.c_str());
  if (error) {
    errorc = -1;
    printf("libpng-util error %u\n", error);
    return ImageData(0, 0, 0, 0);
  }

  const unsigned pitch = pngwidth * 4;
  for (unsigned i = 0; i < pngheight; i++) {
    const unsigned row = i*pitch;
    for (unsigned j = 0; j < pngwidth; j++) {
      const unsigned px = row+j*4;
      unsigned char temp = image[px+2];
      image[px+2] = image[px+0];
      image[px+0] = temp;
    }
  }

  int dataSize = pngwidth*pngheight*4;
  const unsigned char* data = zlib_compress(image, dataSize);
  delete[] image;

  errorc = 0;
  return ImageData(pngwidth, pngheight, data, dataSize);;
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
  ESLookup(deprecated::JavaStruct::EnigmaStruct *es):
    sprite(es->sprites, es->spriteCount, "sprite"),
    background(es->backgrounds, es->backgroundCount, "background"),
    script(es->scripts, es->scriptCount, "script"),
    object(es->gmObjects, es->gmObjectCount, "object") {}
};

SpriteData::SpriteData(const buffers::resources::Sprite &q, const std::string& name, const std::vector<ImageData>& subimages):
  BaseProtoWrapper(q), name(name), image_data(subimages) {}
SpriteData::SpriteData(const deprecated::JavaStruct::Sprite &sprite):
  name(sprite.name) {
  data.set_id(sprite.id);

  // Discarded: sprite.transparent
  // Discarded: sprite.maskShapes

  data.set_shape((buffers::resources::Sprite_Shape) sprite.shape);  // Enum correspondence guaranteed
  data.set_alpha_tolerance(sprite.alphaTolerance);
  data.set_separate_mask(sprite.separateMask);
  data.set_smooth_edges(sprite.smoothEdges);
  data.set_preload(sprite.preload);
  data.set_origin_x(sprite.originX);
  data.set_origin_y(sprite.originY);

  data.set_bbox_mode((buffers::resources::Sprite_BoundingBox) sprite.bbMode);  // Enum correspondence guaranteed
  data.set_bbox_left(sprite.bbLeft);
  data.set_bbox_right(sprite.bbRight);
  data.set_bbox_top(sprite.bbTop);
  data.set_bbox_bottom(sprite.bbBottom);

  for (int i = 0; i < sprite.subImageCount; ++i)
    image_data.emplace_back(sprite.subImages[i].image);
}

SoundData::SoundData(const buffers::resources::Sound &q, const std::string& name, const BinaryData& data):
  BaseProtoWrapper(q), name(name), audio(data) {}
SoundData::SoundData(const deprecated::JavaStruct::Sound &sound):
  name(sound.name),
  audio(sound.data, sound.data + sound.size) {
  data.set_id(sound.id);

  // Discarded: sound.chorus
  // Discarded: sound.echo
  // Discarded: sound.flanger
  // Discarded: sound.gargle
  // Discarded: sound.reverb

  data.set_kind((buffers::resources::Sound_Kind) sound.kind);
  data.set_file_extension(sound.fileType);
  data.set_file_name(sound.fileName);
  data.set_volume(sound.volume);
  data.set_pan(sound.pan);
  data.set_preload(sound.preload);
}

BackgroundData::BackgroundData(const buffers::resources::Background &q, const std::string& name, const ImageData& image):
  BaseProtoWrapper(q), name(name), image_data(image) {}
BackgroundData::BackgroundData(const deprecated::JavaStruct::Background &background):
  name(background.name),
  image_data(background.backgroundImage) {
  data.set_id(background.id);

  legacy_transparency = background.transparent;

  // Discarded: background.smoothEdges

  data.set_preload(background.preload);
  data.set_use_as_tileset(background.useAsTileset);
  data.set_tile_width(background.tileWidth);
  data.set_tile_height(background.tileHeight);
  data.set_horizontal_offset(background.hOffset);
  data.set_vertical_offset(background.vOffset);
  data.set_horizontal_spacing(background.hSep);
  data.set_vertical_spacing(background.vSep);
}

FontData::FontData(const buffers::resources::Font &q, const std::string& name):
  BaseProtoWrapper(q), name(name) {}
FontData::FontData(const deprecated::JavaStruct::Font &font):
  name(font.name) {
  data.set_id(font.id);

  data.set_font_name(font.fontName);
  data.set_size(font.size);
  data.set_bold(font.bold);
  data.set_italic(font.italic);

  for (int i = 0; i < font.glyphRangeCount; ++i) {
    normalized_ranges.emplace_back(font.glyphRanges[i]);
    for (const auto &glyph : normalized_ranges.back().glyphs) {
      *data.add_glyphs() = glyph.metrics;
    }
  }
}

FontData::NormalizedRange::NormalizedRange(const deprecated::JavaStruct::GlyphRange &range):
    min(range.rangeMin), max(range.rangeMax) {
  const int gcount = range.rangeMax - range.rangeMin + 1;
  glyphs.reserve(gcount);
  for (int i = 0; i < gcount; ++i) {
    glyphs.emplace_back(range.glyphs[i]);
  }
}

FontData::GlyphData::GlyphData(const deprecated::JavaStruct::Glyph &glyph):
    ImageData(glyph.width, glyph.height, glyph.data,
              glyph.width * glyph.height) { // Glyph images are alpha-only.
  metrics.set_origin(glyph.origin);
  metrics.set_baseline(glyph.baseline);
  metrics.set_advance(glyph.advance);
  metrics.set_width(glyph.width);
  metrics.set_height(glyph.height);
}

PathData::PathData(const buffers::resources::Path &q, const std::string& name):
  BaseProtoWrapper(q), name(name) {}
PathData::PathData(const deprecated::JavaStruct::Path &path):
  name(path.name) {
  data.set_id(path.id);

  data.set_smooth(path.smooth);
  data.set_closed(path.closed);
  data.set_precision(path.precision);

	// Discarded: backgroundRoomId;
  data.set_hsnap(path.snapX);
  data.set_vsnap(path.snapY);

  for (int i = 0; i < path.pointCount; ++i) {
    auto *p = data.add_points();
    p->set_x(path.points[i].x);
    p->set_y(path.points[i].y);
    p->set_speed(path.points[i].speed);
  }
}

ScriptData::ScriptData(const buffers::resources::Script &q, const std::string& name):
  BaseProtoWrapper(q), name(name) {}
ScriptData::ScriptData(const deprecated::JavaStruct::Script &script):
  name(script.name) {
  data.set_id(script.id);
  data.set_code(script.code);
}

ShaderData::ShaderData(const buffers::resources::Shader &q, const std::string& name):
  BaseProtoWrapper(q), name(name) {}
ShaderData::ShaderData(const deprecated::JavaStruct::Shader &shader):
  name(shader.name) {
  data.set_id(shader.id);

  data.set_vertex_code(shader.vertex);
  data.set_fragment_code(shader.fragment);
  data.set_type(shader.type);
  data.set_precompile(shader.precompile);

}

TimelineData::TimelineData(const buffers::resources::Timeline &q, const std::string& name):
  BaseProtoWrapper(q), name(name) {}
TimelineData::TimelineData(const deprecated::JavaStruct::Timeline &timeline):
  name(timeline.name) {
  data.set_id(timeline.id);

  for (int i = 0; i < timeline.momentCount; ++i) {
    auto *moment = data.add_moments();
    moment->set_step(timeline.moments[i].stepNo);
    moment->set_code(timeline.moments[i].code);
  }
}

ObjectData::ObjectData(const buffers::resources::Object &q, const std::string& name):
  BaseProtoWrapper(q), name(name) {}
ObjectData::ObjectData(const deprecated::JavaStruct::GmObject &object, const ESLookup &lookup):
  name(object.name) {
  data.set_id(object.id);

  if (object.spriteId >= 0)
    data.set_sprite_name(lookup.sprite[object.spriteId]);
  data.set_solid(object.solid);
  data.set_visible(object.visible);
  data.set_depth(object.depth);
  data.set_persistent(object.persistent);
  if (object.parentId >= 0)
    data.set_parent_name(lookup.object[object.parentId]);
  if (object.maskId >= 0)
    data.set_mask_name(lookup.sprite[object.maskId]);

  for (int i = 0; i < object.mainEventCount; ++i) {
    for (int j = 0; j < object.mainEvents[i].eventCount; ++j) {
      auto *event = data.add_events();
      event->set_type(object.mainEvents[i].id);
      event->set_number(object.mainEvents[i].events[j].id);
      event->set_code(object.mainEvents[i].events[j].code);
    }
  }
}

RoomData::RoomData(const buffers::resources::Room &q, const std::string& name):
  BaseProtoWrapper(q), name(name) {}
RoomData::RoomData(const deprecated::JavaStruct::Room &room, const ESLookup &lookup):
  name(room.name) {
  cout << "Import room." << endl;
  data.set_id(room.id);

  if (room.caption)
    data.set_caption(room.caption);
  data.set_width(room.width);
  data.set_height(room.height);

  data.set_hsnap(room.snapX);
  data.set_vsnap(room.snapY);
  data.set_isometric(room.isometric);

  data.set_speed(room.speed);
  data.set_persistent(room.persistent);
  data.set_color(javaColor(room.backgroundColor));  // RGBA
  data.set_show_color(room.drawBackgroundColor);
  if (room.creationCode)
    data.set_creation_code(room.creationCode);

  data.set_enable_views(room.enableViews);

  for (int i = 0; i < room.backgroundDefCount; ++i) {
    auto *background = data.add_backgrounds();
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
    auto *view = data.add_views();
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
    auto *instance = data.add_instances();
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
    auto *tile = data.add_tiles();
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

static void ImportSettings(const deprecated::JavaStruct::GameSettings &settings,
    const ESLookup &lookup, buffers::resources::Settings& set) {
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

GameData::GameData(deprecated::JavaStruct::EnigmaStruct *es): filename(es->filename ?: "") {
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

GameData::GameData(const buffers::Project &proj): filename(""), events(proj.game().events()) {}

int FlattenTree(const buffers::TreeNode &root, GameData *gameData) {
  int error = 0;
  switch (root.type_case()) {
    case TypeCase::kFolder: break;
    case TypeCase::kSprite: {
      std::vector<ImageData> subimages;
      const buffers::resources::Sprite &sprite = root.sprite();
      for (auto sub : sprite.subimages()) {
        ImageData data = loadImageData(sub, error);
        if (error) return -1; // sprite load error
        subimages.emplace_back(data);
      }
      gameData->sprites.emplace_back(sprite, root.name(), subimages);
      break;
    }
    case TypeCase::kSound: {
      BinaryData data = loadBinaryData(root.sound().data(), error);
      if (error) return -2; // sound load error
      gameData->sounds.emplace_back(root.sound(), root.name(), data);
      break;
    }
    case TypeCase::kBackground: {
      ImageData data = loadImageData(root.background().image(), error);
      if (error) return -3; // background load error
      gameData->backgrounds.emplace_back(root.background(), root.name(), data);
      break;
    }
    case TypeCase::kPath:     gameData->paths.emplace_back(root.path(), root.name()); break;
    case TypeCase::kScript:   gameData->scripts.emplace_back(root.script(), root.name()); break;
    case TypeCase::kShader:   gameData->shaders.emplace_back(root.shader(), root.name()); break;
    case TypeCase::kFont:     gameData->fonts.emplace_back(root.font(), root.name()); break;
    case TypeCase::kTimeline: gameData->timelines.emplace_back(root.timeline(), root.name()); break;
    case TypeCase::kObject:   gameData->objects.emplace_back(root.object(), root.name()); break;
    case TypeCase::kRoom:     gameData->rooms.emplace_back(root.room(), root.name()); break;
    case TypeCase::kInclude:  /*gameData->includes.emplace_back(root.include());*/ break;
    case TypeCase::kSettings: /*gameData->settings.emplace_back(root.settings());*/ break;
    default: cout << "- Not transferring unknown " << root.name() << endl; break;
  }

  for (auto child : root.child()) {
    int res = FlattenTree(child, gameData);
    if (res) return res;
  }

  return 0; // success
}

int FlattenProto(const buffers::Project &proj, GameData *gameData) {
  cout << "Flattening tree." << endl;

  int ret = FlattenTree(proj.game().root(), gameData);

  if (ret)
    cout << "Transfer error, see log for details." << endl << endl;
  else
    cout << "Transfer complete." << endl << endl;

  return ret; // success
}
