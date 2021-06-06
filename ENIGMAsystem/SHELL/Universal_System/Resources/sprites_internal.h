/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2015 Harijs Grinbergs
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

//FIXME: this should be ifdef shellmain but enigmas in a sorry state
#ifdef JUST_DEFINE_IT_RUN
#error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_SPRITESTRUCT
#define ENIGMA_SPRITESTRUCT

#include "AssetArray.h"
#include "Collision_Systems/collision_types.h"
#include "Universal_System/scalar.h"
#include "Universal_System/image_formats.h"

namespace enigma {

using BoundingBox = Rect<int>;

struct Subimage {
  
  Subimage() {}
  /// Copy constructor that duplicates the texture
  Subimage(const Subimage &s, bool duplicateTexture = false);
  void FreeTexture();
  
  // AssArray mandatory
  static const char* getAssetTypeName() { return "Subimage"; }
  bool isDestroyed() const { return _destroyed; }
  void destroy() { _destroyed = true; }
  bool _destroyed = false;
  
  int textureID = -1; 
  TexRect textureBounds;
  collision_type collisionType = enigma::ct_precise;
  void* collisionData = nullptr;
};

class Sprite {
public:
  Sprite() {}
  Sprite(const Sprite& s);
  Sprite(int width, int height, int xoffset = 0, int yoffset = 0) : width(width), height(height), xoffset(xoffset), yoffset(yoffset) 
    { bbox = {0, 0, width, height}; }
    
  size_t SubimageCount() const { return _subimages.size(); }
  
  void FreeTextures() { for (std::pair<int, Subimage&> s : _subimages) s.second.FreeTexture(); }
  const int& GetTexture(int subimg) const { return _subimages.get(subimg).textureID; }
  const int ModSubimage(int subimg) const;
  void SetTexture(int subimg, int textureID, TexRect texRect);
  const TexRect& GetTextureRect(int subimg) const { return _subimages.get(subimg).textureBounds; } 
  
  /// Add Subimage from existing texture
  int AddSubimage(int texid, TexRect texRect, collision_type ct = ct_precise, void* collisionData = nullptr, bool mipmap = false);
  /// Add Subimage from raw pixel data (creating a new texture)
  int AddSubimage(const RawImage& img, collision_type ct = ct_precise, void* collisionData = nullptr, bool mipmap = false);
  /// Copy an existing subimage into the sprite (duplicating the texture)
  void AddSubimage(const Subimage& s);
  
  const Subimage& GetSubimage(int index) const { return _subimages.get(index); }
  
  void SetBBox(int x, int y, int w, int h) { bbox = {x, y, w, h}; }
  
  int width = 0;
  int height = 0;
  int xoffset = 0;
  int yoffset = 0;
  BoundingBox bbox = {0, 0, 0, 0};
  int bbox_mode = 0;  //Default is automatic
  
  // AssArray mandatory
  static const char* getAssetTypeName() { return "Sprite"; }
  bool isDestroyed() const { return _destroyed; }
  void destroy() { _destroyed = true; }
  
protected:
  bool _destroyed = false;
  AssetArray<Subimage> _subimages;
};

extern AssetArray<Sprite> sprites;

const BoundingBox& sprite_get_bbox(int spr);
BoundingBox sprite_get_bbox_relative(int spr);
RawImage sprite_get_raw(int spr, unsigned subimg);
const int sprite_get_polygon(int spr);

}  //namespace enigma

#endif  // ENIGMA_SPRITESTRUCT
