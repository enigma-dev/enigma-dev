#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/OpenGL-Common/textures_impl.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/OpenGL-Common/shader.h"
#include <string>

namespace enigma {
  std::string texColorString = "TexColor.bgra = texture( en_TexSampler, v_TextureCoord.st ).rgba * v_Color;";
void graphics_state_flush_samplers() {
  for (int i = 0; i < 8; ++i) {
    const Sampler& sampler = samplers[i];
    const auto tex = get_texture_peer(sampler.texture);
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, tex);
    if (tex == 0) continue; // texture doesn't exist skip updating the sampler
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrapv?GL_REPEAT:GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrapw?GL_REPEAT:GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler.interpolate?GL_LINEAR:GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler.interpolate?GL_LINEAR:GL_NEAREST);
  }
}

}
