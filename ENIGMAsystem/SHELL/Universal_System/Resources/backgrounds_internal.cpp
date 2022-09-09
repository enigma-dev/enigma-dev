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

std::size_t Background::byte_size() const noexcept {
  unsigned texture_width = 0;
  unsigned texture_height = 0;
  auto texture = graphics_copy_texture_pixels(textureID, &texture_width, &texture_height);
  delete[] texture;
  return sizeof(width) + sizeof(height) + sizeof(textureBounds) + sizeof(isTileset) + (2 * sizeof(unsigned)) +
         (texture_width * texture_height * 4) + sizeof(tileWidth) + sizeof(tileHeight) + sizeof(hOffset) +
         sizeof(vOffset) + sizeof(hSep) + sizeof(vSep);
}

std::vector<std::byte> Background::serialize() {
  std::vector<std::byte> result{};
  std::size_t len = 0;

  enigma_internal_serialize_many(len, result, width, height);
  unsigned texture_width = 0;
  unsigned texture_height = 0;
  auto texture = graphics_copy_texture_pixels(textureID, &texture_width, &texture_height);
  enigma_internal_serialize_many(len, result, texture_width, texture_height);
  result.resize(result.size() + (texture_width * texture_height * 4));
  std::copy(texture, texture + (texture_width * texture_height * 4),
            reinterpret_cast<unsigned char *>(result.data() + len));
  len += texture_width * texture_height;
  enigma_internal_serialize_many(len, result, textureBounds.x, textureBounds.y, textureBounds.h, textureBounds.w,
                                 isTileset, tileWidth, tileHeight, hOffset, vOffset, hSep, vSep);

  result.shrink_to_fit();
  return result;
}

std::size_t Background::deserialize_self(std::byte* iter) {
  std::size_t len = 0;

  enigma_internal_deserialize_many(iter, len, width, height);
  unsigned texture_width = 0;
  unsigned texture_height = 0;
  enigma_internal_deserialize_many(iter, len, texture_width, texture_height);
  RawImage img{new unsigned char[texture_width * texture_height * 4], texture_width, texture_height};
  std::copy(iter + len, iter + len + (texture_width * texture_height * 4), reinterpret_cast<std::byte *>(img.pxdata));
  textureID = graphics_create_texture(img, false);
  len += texture_width * texture_height * 4;
  enigma_internal_deserialize_many(iter, len, textureBounds.x, textureBounds.y, textureBounds.h, textureBounds.w,
                                 isTileset, tileWidth, tileHeight, hOffset, vOffset, hSep, vSep);
  _destroyed = false;

  return len;
}

std::pair<Background, std::size_t> Background::deserialize(std::byte* iter) {
  Background result;
  auto len = result.deserialize_self(iter);
  return {std::move(result), len};
}

}
