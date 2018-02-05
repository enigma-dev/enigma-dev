#include "Proto2ES.h"

#include "resources/Sprite.pb.h"

Sprite AddSprite(const buffers::resources::Sprite& spr);
SubImage AddSubImage(const std::string fPath);
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
