#include "Graphics_Systems/graphics_mandatory.h"
#include "Collision_Systems/collision_mandatory.h"
#include "Universal_System/nlpo2.h"
#include "sprites_internal.h"

namespace enigma {

AssetArray<Sprite> sprites;

Subimage::Subimage(const Subimage &s, bool duplicateTexture) {
  // FIXME: instead of duplicating the texture we should probably use a ref counter
  // especially when using an atlas
  if (duplicateTexture && s.textureID != -1) {
    textureID = graphics_duplicate_texture(s.textureID);
  } else {
    textureID = s.textureID;
  }
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

Sprite::Sprite(const Sprite& s) {
  width = s.width;
  height = s.height;
  xoffset = s.xoffset;
  yoffset = s.yoffset;
  bbox = s.bbox;
  bbox_mode = s.bbox_mode;
  
  for (size_t i = 0; i < s.SubimageCount(); ++i) {
    Subimage copy(s.GetSubimage(i), true);
    _subimages.add(std::move(copy));
  }
}

int Sprite::AddSubimage(int texid, TexRect texRect, collision_type ct, void* collisionData, bool mipmap) {
  Subimage subimg;
  subimg.textureID = texid;
  subimg.textureBounds = texRect;
  subimg.collisionData = get_collision_mask(*this, collisionData, ct);
  return _subimages.add(std::move(subimg));
}

int Sprite::AddSubimage(unsigned char* pxdata, int w, int h, collision_type ct, void* collisionData, bool mipmap) {
  unsigned fullwidth = nlpo2dc(w)+1, fullheight = nlpo2dc(h)+1;
  RawImage img = image_pad(pxdata, w, h, fullwidth, fullheight);
  int texID = graphics_create_texture(w, h, fullwidth, fullheight, img.pxdata, mipmap);
  return AddSubimage(texID, TexRect(0, 0, static_cast<gs_scalar>(w) / fullwidth, static_cast<gs_scalar>(h) / fullheight), ct, collisionData);
}

void Sprite::AddSubimage(const Subimage& s) {
  Subimage copy(s, true);
  _subimages.add(std::move(copy));
}

BoundingBox sprite_get_bbox(int ind) {
  return sprites.get(ind).bbox;
}

BoundingBox sprite_get_bbox_relative(int ind) {
  const Sprite& spr = sprites.get(ind);
  BoundingBox bbox = spr.bbox;
  bbox.x -= spr.xoffset;
  bbox.y -= spr.yoffset;
  return bbox;
}

RawImage sprite_get_raw(int ind, unsigned subimg) {
  RawImage img;
  if (!sprites.exists(ind)) return img;
  const Sprite& spr = sprites.get(ind);
  if (subimg >= spr.SubimageCount()) return img;
  img.pxdata = graphics_copy_texture_pixels(spr.GetTexture(subimg), &img.w, &img.h);
  return img;
}


} // namespace enigma
