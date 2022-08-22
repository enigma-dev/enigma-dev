#include "backgrounds_internal.h"
#include "Graphics_Systems/graphics_mandatory.h"

namespace enigma {

AssetArray<Background> backgrounds;

Background::Background(const Background& b, bool duplicateTexture) {
  width = b.width;
  height = b.height;
  
  if (duplicateTexture && b.textureID != -1) {
    textureID = enigma::graphics_duplicate_texture(b.textureID);
  } else {
    textureID = b.textureID;
  }
  
  textureBounds = b.textureBounds;
  isTileset = b.isTileset;
  tileWidth = b.tileWidth;
  tileHeight = b.tileHeight;
  hOffset = b.hOffset;
  vOffset = b.vOffset;
  hSep = b.hSep;
  vSep = b.vSep;
}

void Background::FreeTexture() {
  enigma::graphics_delete_texture(textureID);
  textureID = -1;
}

}
