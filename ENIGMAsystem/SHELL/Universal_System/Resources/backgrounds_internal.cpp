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

std::vector<std::byte> Background::serialize() const {
  std::vector<std::byte> result{};
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize_many(len, result, width, height);
  unsigned texture_width = 0;
  unsigned texture_height = 0;
  auto texture = graphics_copy_texture_pixels(textureID, &texture_width, &texture_height);
  enigma::bytes_serialization::enigma_serialize_many(len, result, texture_width, texture_height);
  result.resize(result.size() + (texture_width * texture_height * 4));
  std::copy(texture, texture + (texture_width * texture_height * 4),
            reinterpret_cast<unsigned char*>(result.data() + len));
  len += texture_width * texture_height;
  enigma::bytes_serialization::enigma_serialize_many(len, result, textureBounds.x, textureBounds.y, textureBounds.h, textureBounds.w, isTileset,
                        tileWidth, tileHeight, hOffset, vOffset, hSep, vSep);

  result.shrink_to_fit();
  return result;
}

std::size_t Background::deserialize_self(std::byte* iter) {
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_deserialize_many(iter, len, width, height);
  unsigned texture_width = 0;
  unsigned texture_height = 0;
  enigma::bytes_serialization::enigma_deserialize_many(iter, len, texture_width, texture_height);
  RawImage img{new unsigned char[texture_width * texture_height * 4], texture_width, texture_height};
  std::copy(iter + len, iter + len + (texture_width * texture_height * 4), reinterpret_cast<std::byte*>(img.pxdata));
  textureID = graphics_create_texture(img, false);
  len += texture_width * texture_height * 4;
  enigma::bytes_serialization::enigma_deserialize_many(iter, len, textureBounds.x, textureBounds.y, textureBounds.h, textureBounds.w, isTileset,
                          tileWidth, tileHeight, hOffset, vOffset, hSep, vSep);
  _destroyed = false;

  return len;
}

std::pair<Background, std::size_t> Background::deserialize(std::byte* iter) {
  Background result;
  auto len = result.deserialize_self(iter);
  return {std::move(result), len};
}

std::string Background::json_serialize() const {
  std::string json = "{\"width\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(width);

  json += ",\"height\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(height);

  unsigned texture_width = 0;
  unsigned texture_height = 0;

  auto texture = graphics_copy_texture_pixels(textureID, &texture_width, &texture_height);
  json += ",\"texture_width\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(texture_width);

  json += ",\"texture_height\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(texture_height);

  json += ",\"texture\":";

  std::vector<std::string> hexStream(texture_width * texture_height * 4);
  for (unsigned int i = 0; i < texture_width * texture_height * 4; ++i) {
    std::stringstream hexStreamItem;
    hexStreamItem << std::hex << static_cast<int>(texture[i]);
    hexStream[i] = hexStreamItem.str();
  }

  json += enigma::JSON_serialization::internal_serialize_into_fn(hexStream);

  json += ",\"textureBounds.x\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(textureBounds.x);

  json += ",\"textureBounds.y\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(textureBounds.y);

  json += ",\"textureBounds.h\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(textureBounds.h);

  json += ",\"textureBounds.w\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(textureBounds.w);

  json += ",\"isTileset\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(isTileset);

  json += ",\"tileWidth\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(tileWidth);

  json += ",\"tileHeight\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(tileHeight);

  json += ",\"hOffset\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(hOffset);

  json += ",\"vOffset\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(vOffset);

  json += ",\"hSep\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(hSep);

  json += ",\"vSep\":";
  json += enigma::JSON_serialization::internal_serialize_into_fn(vSep);

  json += "}";

  return json;
}

void Background::json_deserialize_self(const std::string& json) {
  width = enigma::JSON_serialization::internal_deserialize_fn<unsigned int>(enigma::JSON_serialization::json_find_value(json,"width"));
  height = enigma::JSON_serialization::internal_deserialize_fn<unsigned int>(enigma::JSON_serialization::json_find_value(json,"height"));

  unsigned texture_width =
      enigma::JSON_serialization::internal_deserialize_fn<unsigned int>(enigma::JSON_serialization::json_find_value(json,"texture_width"));
  unsigned texture_height =
      enigma::JSON_serialization::internal_deserialize_fn<unsigned int>(enigma::JSON_serialization::json_find_value(json,"texture_height"));
  RawImage img{new unsigned char[texture_width * texture_height * 4], texture_width, texture_height};

  std::vector<std::string> hexStream =
      enigma::JSON_serialization::internal_deserialize_fn<std::vector<std::string>>(enigma::JSON_serialization::json_find_value(json,"texture"));

  for (unsigned int i = 0; i < texture_width * texture_height * 4; ++i) {
    std::istringstream hexStreamItem(hexStream[i]);
    int value;
    hexStreamItem >> std::hex >> value;
    img.pxdata[i] = static_cast<unsigned char>(value);
  }

  textureID = graphics_create_texture(img, false);

  textureBounds.x = enigma::JSON_serialization::internal_deserialize_fn<gs_scalar>(enigma::JSON_serialization::json_find_value(json,"textureBounds.x"));
  textureBounds.y = enigma::JSON_serialization::internal_deserialize_fn<gs_scalar>(enigma::JSON_serialization::json_find_value(json,"textureBounds.y"));
  textureBounds.h = enigma::JSON_serialization::internal_deserialize_fn<gs_scalar>(enigma::JSON_serialization::json_find_value(json,"textureBounds.h"));
  textureBounds.w = enigma::JSON_serialization::internal_deserialize_fn<gs_scalar>(enigma::JSON_serialization::json_find_value(json,"textureBounds.w"));

  isTileset = enigma::JSON_serialization::internal_deserialize_fn<bool>(enigma::JSON_serialization::json_find_value(json,"isTileset"));
  tileWidth = enigma::JSON_serialization::internal_deserialize_fn<unsigned int>(enigma::JSON_serialization::json_find_value(json,"tileWidth"));
  tileHeight = enigma::JSON_serialization::internal_deserialize_fn<unsigned int>(enigma::JSON_serialization::json_find_value(json,"tileHeight"));
  hOffset = enigma::JSON_serialization::internal_deserialize_fn<int>(enigma::JSON_serialization::json_find_value(json,"hOffset"));
  vOffset = enigma::JSON_serialization::internal_deserialize_fn<int>(enigma::JSON_serialization::json_find_value(json,"vOffset"));
  hSep = enigma::JSON_serialization::internal_deserialize_fn<int>(enigma::JSON_serialization::json_find_value(json,"hSep"));
  vSep = enigma::JSON_serialization::internal_deserialize_fn<int>(enigma::JSON_serialization::json_find_value(json,"vSep"));

  _destroyed = false;
}

Background Background::json_deserialize(const std::string& json) {
  Background result;
  result.json_deserialize_self(json);
  return result;
}

}  // namespace enigma
