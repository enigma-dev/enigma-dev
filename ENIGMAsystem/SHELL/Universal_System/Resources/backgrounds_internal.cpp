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
  // NOTE: This doesn't compute the texture size
  return sizeof(width) + sizeof(height) + sizeof(textureBounds) + sizeof(isTileset) +
         sizeof(tileWidth) + sizeof(tileHeight) + sizeof(hOffset) + sizeof(vOffset) + sizeof(hSep) + sizeof(vSep);
}

std::vector<std::byte> Background::serialize() {
  std::vector<std::byte> result{};
  std::size_t len = 0;

  enigma_internal_serialize(width, len, result);
  enigma_internal_serialize(height, len, result);
  unsigned texture_width = 0;
  unsigned texture_height = 0;
  auto texture = graphics_copy_texture_pixels(textureID, &texture_width, &texture_height);
  enigma_internal_serialize(texture_width, len, result);
  enigma_internal_serialize(texture_height, len, result);
  result.resize(result.size() + (texture_width * texture_height * 4));
  std::copy(texture, texture + (texture_width * texture_height * 4),
            reinterpret_cast<unsigned char *>(result.data() + len));
  len += texture_width * texture_height;
  enigma_internal_serialize(textureBounds.x, len, result);
  enigma_internal_serialize(textureBounds.y, len, result);
  enigma_internal_serialize(textureBounds.h, len, result);
  enigma_internal_serialize(textureBounds.w, len, result);
  enigma_internal_serialize(isTileset, len, result);
  enigma_internal_serialize(tileWidth, len, result);
  enigma_internal_serialize(tileHeight, len, result);
  enigma_internal_serialize(hOffset, len, result);
  enigma_internal_serialize(vOffset, len, result);
  enigma_internal_serialize(hSep, len, result);
  enigma_internal_serialize(vSep, len, result);


  result.shrink_to_fit();
  return result;
}

std::size_t Background::deserialize_self(std::byte* iter) {
  std::size_t len = 0;

  enigma_internal_deserialize(width, iter, len);
  enigma_internal_deserialize(height, iter, len);
  unsigned texture_width = 0;
  unsigned texture_height = 0;
  enigma_internal_deserialize(texture_width, iter, len);
  enigma_internal_deserialize(texture_height, iter, len);
  RawImage img{new unsigned char[texture_width * texture_height * 4], texture_width, texture_height};
  std::copy(iter + len, iter + len + (texture_width * texture_height * 4), reinterpret_cast<std::byte *>(img.pxdata));
  textureID = graphics_create_texture(img, false);
  len += texture_width * texture_height * 4;
  enigma_internal_deserialize(textureBounds.x, iter, len);
  enigma_internal_deserialize(textureBounds.y, iter, len);
  enigma_internal_deserialize(textureBounds.h, iter, len);
  enigma_internal_deserialize(textureBounds.w, iter, len);
  enigma_internal_deserialize(isTileset, iter, len);
  enigma_internal_deserialize(tileWidth, iter, len);
  enigma_internal_deserialize(tileHeight, iter, len);
  enigma_internal_deserialize(hOffset, iter, len);
  enigma_internal_deserialize(vOffset, iter, len);
  enigma_internal_deserialize(hSep, iter, len);
  enigma_internal_deserialize(vSep, iter, len);

  return len;
}

std::pair<Background, std::size_t> Background::deserialize(std::byte* iter) {
  Background result;
  auto len = result.deserialize_self(iter);
  return {std::move(result), len};
}

}
