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

#include "Proto2ES.h"

Sprite AddSprite(const buffers::resources::Sprite& spr);
SubImage AddSubImage(const std::string fPath);
Sound AddSound(const buffers::resources::Sound& snd);
Background AddBackground(const buffers::resources::Background& bkg);
Path AddPath(const buffers::resources::Path& pth);
PathPoint AddPathPoint(const buffers::resources::Path::Point& pnt);
Script AddScript(const buffers::resources::Script& scr);
Shader AddShader(const buffers::resources::Shader& shr);
Font AddFont(const buffers::resources::Font& fnt);
GmObject AddObject(const buffers::resources::Object& obj);
Room AddRoom(const buffers::resources::Room& rmn);
Instance AddInstance(const buffers::resources::Room::Instance& inst);

EnigmaStruct* ProtoBuf2ES(buffers::Project* protobuf) {
  EnigmaStruct *es = new EnigmaStruct();

  es->gameSettings.gameIcon = "";
  es->gameSettings.letEscEndGame = true;
  es->gameSettings.letF4SwitchFullscreen = true;
  es->gameSettings.treatCloseAsEscape = true;
  es->gameSettings.alwaysOnTop = true;
  es->gameSettings.gameId = 0;
  es->gameSettings.company = "";
  es->gameSettings.description = "";
  es->gameSettings.version = "";
  es->gameSettings.product = "";
  es->gameSettings.version = "";
  es->gameSettings.copyright = "";
  es->gameInfo.gameInfoStr = "";
  es->gameInfo.formCaption = "";

  es->spriteCount = protobuf->sprites_size();
  if (es->spriteCount > 0) {
    es->sprites = new Sprite[es->spriteCount];
    for (int i = 0; i < es->spriteCount; ++i) {
        es->sprites[i] = AddSprite(protobuf->sprites(i));
    }
  }

  es->soundCount = protobuf->sounds_size();
  if (es->soundCount > 0) {
    es->sounds = new Sound[es->soundCount];
    for (int i = 0; i < es->soundCount; ++i) {
        es->sounds[i] = AddSound(protobuf->sounds(i));
    }
  }

  es->backgroundCount = protobuf->backgrounds_size();
  if (es->backgroundCount > 0) {
    es->backgrounds = new Background[es->backgroundCount];
    for (int i = 0; i < es->backgroundCount; ++i) {
        es->backgrounds[i] = AddBackground(protobuf->backgrounds(i));
    }
  }

  es->pathCount = protobuf->paths_size();
  if (es->pathCount > 0) {
    es->paths = new Path[es->pathCount];
    for (int i = 0; i < es->pathCount; ++i) {
        es->paths[i] = AddPath(protobuf->paths(i));
    }
  }

  es->scriptCount = protobuf->scripts_size();
  if (es->scriptCount > 0) {
    es->scripts = new Script[es->scriptCount];
    for (int i = 0; i < es->scriptCount; ++i) {
        es->scripts[i] = AddScript(protobuf->scripts(i));
    }
  }

  es->shaderCount = protobuf->shaders_size();
  if (es->shaderCount > 0) {
    es->shaders = new Shader[es->shaderCount];
    for (int i = 0; i < es->shaderCount; ++i) {
        es->shaders[i] = AddShader(protobuf->shaders(i));
    }
  }

  es->fontCount = protobuf->fonts_size();
  if (es->fontCount > 0) {
    es->fonts = new Font[es->fontCount];
    for (int i = 0; i < es->fontCount; ++i) {
        es->fonts[i] = AddFont(protobuf->fonts(i));
    }
  }

  es->gmObjectCount = protobuf->objects_size();
  if (es->gmObjectCount > 0) {
    es->gmObjects = new GmObject[es->gmObjectCount];
    for (int i = 0; i < es->gmObjectCount; ++i) {
        es->gmObjects[i] = AddObject(protobuf->objects(i));
    }
  }

  es->roomCount = protobuf->rooms_size();
  if (es->roomCount > 0) {
    es->rooms = new Room[es->roomCount];
    for (int i = 0; i < es->roomCount; ++i) {
        es->rooms[i] = AddRoom(protobuf->rooms(i));
    }
  }

  return es;
}

Sprite AddSprite(const buffers::resources::Sprite& spr) {
  Sprite s = Sprite();

  s.name = spr.name().c_str();
  s.id = spr.id();

  s.transparent = spr.transparent();
  s.shape = spr.shape();
  s.alphaTolerance = spr.alpha_tolerance();
  s.separateMask = spr.separate_mask();
  s.smoothEdges = spr.smooth_edges();
  s.preload = false;
  s.originX = spr.origin_x();
  s.originY = spr.origin_y();

  s.bbMode = spr.bbox_mode();
  s.bbLeft = spr.bbox_left();
  s.bbRight = spr.bbox_right();
  s.bbTop = spr.bbox_top();
  s.bbBottom = spr.bbox_bottom();

  s.subImageCount = spr.subimages_size();
  if (s.subImageCount > 0) {
    s.subImages = new SubImage[s.subImageCount];
    for (int i=0; i < s.subImageCount; ++i) {
      s.subImages[i] = AddSubImage(spr.subimages(i));
    }
  }

  // Polygon_LOLWINDOWS *maskShapes; ???

  return s;
}

SubImage AddSubImage(const std::string fPath) {
  // Why do we have a Subimage class that contains only an image class?
  SubImage i;
  return i;
}

Sound AddSound(const buffers::resources::Sound& snd) {
  Sound s = Sound();

  s.name = snd.name().c_str();
  s.id = snd.id();

  s.kind = snd.kind();
  s.fileType = snd.file_extension().c_str();
  s.fileName = snd.file_name().c_str();
  s.volume = snd.volume();
  s.pan = snd.pan();
  s.preload = snd.preload();

  return s;
}

Background AddBackground(const buffers::resources::Background& bkg) {
  Background b = Background();

  b.name = bkg.name().c_str();
  b.id = bkg.id();

  b.transparent = bkg.transparent();
  b.smoothEdges = bkg.smooth_edges();
  b.preload = bkg.preload();
  b.useAsTileset = bkg.use_as_tileset();

  b.tileWidth = bkg.tile_width();
  b.tileHeight = bkg.tile_height();
  b.hOffset = bkg.horizontal_offset();
  b.vOffset = bkg.vertical_offset();
  b.hSep = bkg.horizontal_spacing();
  b.vSep = bkg.vertical_spacing();

  b.backgroundImage = Image();

  return b;
}

Path AddPath(const buffers::resources::Path& pth) {
  Path p = Path();

  p.name = pth.name().c_str();
  p.id = pth.id();

  p.smooth = pth.smooth();
  p.closed = pth.closed();
  p.precision = pth.precision();

  p.snapX = pth.snap_x();
  p.snapY = pth.snap_y();

  p.pointCount = pth.points_size();
  if (p.pointCount > 0) {
    p.points = new PathPoint[p.pointCount];
    for (int i=0; i < p.pointCount; ++i) {
      p.points[i] = AddPathPoint(pth.points(i));
    }
  }

  return p;
}

PathPoint AddPathPoint(const buffers::resources::Path::Point& pnt) {
  PathPoint p = PathPoint();

  p.x = pnt.x();
  p.y = pnt.y();
  p.speed = pnt.speed();

  return p;
}

Script AddScript(const buffers::resources::Script& scr) {
  Script s = Script();

  s.name = scr.name().c_str();
  s.id = scr.id();

  s.code = scr.code().c_str();

  return s;
}

Shader AddShader(const buffers::resources::Shader& shr) {
  Shader s = Shader();

  s.name = shr.name().c_str();
  s.id = shr.id();

  s.vertex = shr.vertex_code().c_str();
  s.fragment = shr.fragment_code().c_str();
  s.type = shr.type().c_str();
  s.precompile = shr.precompile();

  return s;
}

Font AddFont(const buffers::resources::Font& fnt) {
  Font f = Font();

  f.name = fnt.name().c_str();
  f.id = fnt.id();

  f.fontName = fnt.font_name().c_str();
  f.size = fnt.size();
  f.bold = fnt.bold();
  f.italic = fnt.italic();

  return f;
}

GmObject AddObject(const buffers::resources::Object& obj) {
  GmObject o = GmObject();

  o.name = obj.name().c_str();
  o.id = obj.id();

  o.spriteId = -1;
  o.solid = obj.solid();
  o.visible = obj.visible();
  o.depth = obj.depth();
  o.persistent = obj.persistent();
  o.parentId = -1;
  o.maskId = -1;

  o.mainEvents = nullptr;
  o.mainEventCount = 0;

/*
  o.mainEventCount = obj.instances_size();
  if (o.mainEventCount > 0) {
    r.mainEvents = new Instance[r.mainEventCount];
    for (int i = 0; i < r.mainEventCount; ++i) {
      r.mainEvents[i] = AddInstance(rmn.mainEvents(i));
    }
  }
*/

  return o;
}

Room AddRoom(const buffers::resources::Room& rmn) {
  Room r = Room();

  r.name = rmn.name().c_str();
  r.id = rmn.id();

  r.caption = rmn.caption().c_str();
  r.width = rmn.width();
  r.height = rmn.height();
  r.speed = rmn.speed();
  r.persistent = rmn.persistent();
  r.backgroundColor = 0x40C0FFFF;
  r.drawBackgroundColor = rmn.show_color();
  r.creationCode = rmn.code().c_str();
  r.enableViews = rmn.enable_views();

  r.backgroundDefs = nullptr;
  r.backgroundDefCount = 0;

  r.views = nullptr;
  r.viewCount = 0;

  r.instanceCount = rmn.instances_size();
  if (r.instanceCount > 0) {
    r.instances = new Instance[r.instanceCount];
    for (int i = 0; i < r.instanceCount; ++i) {
      r.instances[i] = AddInstance(rmn.instances(i));
    }
  }

  r.tiles = nullptr;
  r.tileCount = 0;

  return r;
}

Instance AddInstance(const buffers::resources::Room::Instance& inst) {
  Instance i = Instance();

  i.x = inst.x();
  i.y = inst.y();
  i.id = 0;
  i.locked = inst.locked();
  i.objectId = -1;
  i.creationCode = inst.code().c_str();
  i.preCreationCode = "";

  return i;
}
