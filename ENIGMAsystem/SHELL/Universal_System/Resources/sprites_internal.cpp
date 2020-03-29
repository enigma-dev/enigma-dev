#include "Graphics_Systems/graphics_mandatory.h"
#include "Collision_Systems/collision_mandatory.h"
#include "Universal_System/nlpo2.h"
#include "sprites_internal.h"

namespace enigma {

AssetArray<Sprite> sprites;

Subimage::Subimage(const Subimage &s) {
  // FIXME: instead of duplicating the texture we should probably use a ref counter
  // especially when using an atlas
  textureID = graphics_duplicate_texture(s.textureID);
  textureBounds = s.textureBounds;
  collisionType = s.collisionType;
  collisionData  = s.collisionData;
}

void Subimage::FreeTexture() {
  if (textureID != -1) enigma::graphics_delete_texture(textureID);
  textureID = -1;
}

void Sprite::SetTexture(int subimg, int textureID, TexRect texRect) { 
  Subimage& s = _subimages.get(subimg);
  s.textureID = textureID;
  s.textureBounds = texRect;
}

int Sprite::AddSubimage(int texid, TexRect texRect, collision_type ct, void* collisionData) {
  Subimage subimg;
  subimg.textureID = texid;
  subimg.textureBounds = texRect;
  subimg.collisionData = get_collision_mask(*this, collisionData, ct);
  return _subimages.add(std::move(subimg));
}

int Sprite::AddSubimage(unsigned char* pxdata, int w, int h, collision_type ct, void* collisionData) {
  unsigned fullwidth = nlpo2dc(w)+1, fullheight = nlpo2dc(h)+1;
  int texID = graphics_create_texture(w, h, fullwidth, fullheight, pxdata, false);
  return AddSubimage(texID, TexRect(0, 0, static_cast<gs_scalar>(w) / fullwidth, static_cast<gs_scalar>(h) / fullheight), ct, collisionData);
}

void Sprite::AddSubimage(const Subimage& s) {
  Subimage copy = s;
  _subimages.add(std::move(copy));
}

BBox_t sprite_get_bbox(int ind) {
  return sprites.get(ind).bbox;
}

BBox_t sprite_get_bbox_relative(int ind) {
  const Sprite& spr = sprites.get(ind);
  BBox_t bbox = spr.bbox;
  bbox.x -= spr.xoffset;
  bbox.y -= spr.yoffset;
  return bbox;
}


} // namespace enigma
