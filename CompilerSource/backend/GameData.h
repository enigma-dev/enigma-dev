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

template<class Proto> struct ProtoMessageInheritor {
  Proto data;
  typedef Proto BaseProtoClass;
  typedef ProtoMessageInheritor<Proto> BaseProtoWrapper;

  decltype(((Proto*)nullptr)->id()) id() const {
    return data.id();
  }

  Proto *operator->() {
    return &data;
  }
  const Proto *operator->() const {
    return &data;
  }

  ProtoMessageInheritor() {}
  ProtoMessageInheritor(const Proto &p): data(p) {}
};

typedef std::vector<uint8_t> BinaryData;
struct ImageData {
  int width, height;
  // TODO: std::string filename;
  BinaryData pixels;

  ImageData(const ::Image &image);
  ImageData(int w, int h, const uint8_t *data, size_t size);
};

struct SpriteData : ProtoMessageInheritor<buffers::resources::Sprite> {
  std::string name;
  std::vector<ImageData> image_data;

  SpriteData(const BaseProtoClass &sprite,
             const std::string& name, const std::vector<ImageData>& subimages);
  SpriteData(const ::Sprite &sprite);
};

struct SoundData : ProtoMessageInheritor<buffers::resources::Sound> {
  std::string name;
  BinaryData audio;

  SoundData(const BaseProtoClass &sound,
            const std::string& name, const BinaryData& data);
  SoundData(const ::Sound &sound);
};

struct BackgroundData : ProtoMessageInheritor<buffers::resources::Background> {
  std::string name;
  ImageData image_data;
  bool legacy_transparency;

  BackgroundData(const BaseProtoClass &background,
                 const std::string& name, const ImageData& image);
  BackgroundData(const ::Background &background);
};

struct FontData : ProtoMessageInheritor<buffers::resources::Font> {
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

  FontData(const BaseProtoClass &font, const std::string& name);
  FontData(const ::Font &font);
};

struct PathData : ProtoMessageInheritor<buffers::resources::Path> {
  std::string name;
  PathData(const BaseProtoClass &q, const std::string& name);
  PathData(const ::Path &path);
};
struct ScriptData : ProtoMessageInheritor<buffers::resources::Script> {
  std::string name;
  ScriptData(const BaseProtoClass &q, const std::string& name);
  ScriptData(const ::Script &script);
};
struct ShaderData : ProtoMessageInheritor<buffers::resources::Shader> {
  std::string name;
  ShaderData(const BaseProtoClass &q, const std::string& name);
  ShaderData(const ::Shader &shader);
};
struct TimelineData : ProtoMessageInheritor<buffers::resources::Timeline> {
  std::string name;
  TimelineData(const BaseProtoClass &q, const std::string& name);
  TimelineData(const ::Timeline &timeline);
};
struct ObjectData : ProtoMessageInheritor<buffers::resources::Object> {
  std::string name;
  ObjectData(const BaseProtoClass &q, const std::string& name);
  ObjectData(const ::GmObject &object, const ESLookup &lookup);
};
struct RoomData : ProtoMessageInheritor<buffers::resources::Room> {
  std::string name;
  RoomData(const BaseProtoClass &q, const std::string& name);
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

int FlattenProto(const buffers::Project &proj, GameData *gameData);

#endif // ENIGMA_GAMEDATA_H
