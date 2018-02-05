#include "game.pb.h"

#include "backend/EnigmaStruct.h"

Sprite AddSprite(const buffers::resources::Sprite& spr);
SubImage AddSubImage(const std::string fPath);

EnigmaStruct* ProtoBuf2ES(buffers::Project* protobuf) {
  EnigmaStruct* es = new EnigmaStruct();

  es->spriteCount = protobuf->sprites_size();

  if (es->spriteCount > 0) {
    es->sprites = new Sprite[es->spriteCount];
    for (int i = 0; i < es->spriteCount; i++) {
      es->sprites[i] = AddSprite(protobuf->sprites(i));
    }
  }
  
  return es;
}

Sprite AddSprite(const buffers::resources::Sprite& spr) {
    Sprite s = Sprite();

    s.name = spr.name().c_str();
    s.id = -1;
    
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
        for (int i=0; i < s.subImageCount; i++) {
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
