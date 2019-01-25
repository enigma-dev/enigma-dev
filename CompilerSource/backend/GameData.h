/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 * Copyright (C) 2018 Josh Ventura
 *
 * This file is part of ENIGMA.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_GAMEDATA_H
#define ENIGMA_GAMEDATA_H

#include <string>
#include <vector>

// Legacy constructors.
#include "EnigmaStruct.h"
#include "project.pb.h"
#include "GameInformation.pb.h"

struct ESLookup;

typedef std::vector<uint8_t> BinaryData;
struct ImageData {
  int width, height;
  // TODO: std::string filename;
  BinaryData pixels;

  ImageData(const ::Image &image);
  ImageData(int w, int h, const uint8_t *data, size_t size);
};

struct SpriteData : buffers::resources::Sprite {
  std::string name;
  std::vector<ImageData> image_data;

  SpriteData(const buffers::resources::Sprite &sprite);
  SpriteData(const ::Sprite &sprite);
};

struct SoundData : buffers::resources::Sound {
  std::string name;
  BinaryData audio;

  SoundData(const buffers::resources::Sound &sound);
  SoundData(const ::Sound &sound);
};

struct BackgroundData : buffers::resources::Background {
  std::string name;
  ImageData image_data;
  bool legacy_transparency;

  BackgroundData(const buffers::resources::Background &background);
  BackgroundData(const ::Background &background);
};

struct FontData : buffers::resources::Font {
  std::string name;
  struct GlyphData : ImageData {
    buffers::resources::Font::Glyph metrics;

    // TODO: Need to move the font packing logic to allow pre-packed sprite
    // fonts; cannot construct image here in the meantime
    GlyphData(const ::Glyph &glyph);
  };
  struct NormalizedRange {
    int min, max;
    std::vector<GlyphData> glyphs;
    NormalizedRange(int min_, int max_): min(min_), max(max_) {
      glyphs.reserve(max_ - min_ + 1);
    }
    NormalizedRange(const ::GlyphRange &range);
  };
  std::vector<NormalizedRange> normalized_ranges;

  FontData(const buffers::resources::Font &font);
  FontData(const ::Font &font);
};

struct PathData : buffers::resources::Path {
  std::string name;
  PathData(const buffers::resources::Path &q);
  PathData(const ::Path &path);
};
struct ScriptData : buffers::resources::Script {
  std::string name;
  ScriptData(const buffers::resources::Script &q);
  ScriptData(const ::Script &script);
};
struct ShaderData : buffers::resources::Shader {
  std::string name;
  ShaderData(const buffers::resources::Shader &q);
  ShaderData(const ::Shader &shader);
};
struct TimelineData : buffers::resources::Timeline {
  std::string name;
  TimelineData(const buffers::resources::Timeline &q);
  TimelineData(const ::Timeline &timeline);
};
struct ObjectData : buffers::resources::Object {
  std::string name;
  ObjectData(const buffers::resources::Object &q);
  ObjectData(const ::GmObject &object, const ESLookup &lookup);
};
struct RoomData : buffers::resources::Room {
  std::string name;
  RoomData(const buffers::resources::Room &q);
  RoomData(const ::Room &room, const ESLookup &lookup);
};

/// Flat view of a buffers::Project.
struct GameData {
  /// Name of project file. Used in metadata, and later, resource loading.
  std::string filename;

  std::vector<SpriteData>     sprites;
  std::vector<SoundData>      sounds;
  std::vector<BackgroundData> backgrounds;
  std::vector<FontData>       fonts;
  std::vector<PathData>       paths;
  std::vector<ScriptData>     scripts;
  std::vector<ShaderData>     shaders;
  std::vector<TimelineData>   timelines;
  std::vector<ObjectData>     objects;
  std::vector<RoomData>       rooms;

  // TODO(Robert): Why do none of these have Proto equivalents?
  struct Extension {
    std::string name, path;
    Extension(std::string n, std::string p): name(n), path(p) {}
  };
  struct Constant {
    std::string name, value;
    Constant(std::string n, std::string v): name(n), value(v) {}
  };

  // std::vector<Trigger> triggers;
  std::vector<Constant> constants;
  // std::vector<Include> includes;
  std::vector<Extension> extensions;
  std::vector<std::string> packages;

  buffers::resources::GameInformation gameInfo;
  buffers::resources::Settings settings;

  GameData(struct EnigmaStruct *es);
  GameData(const buffers::Project &proj);
};

#endif
