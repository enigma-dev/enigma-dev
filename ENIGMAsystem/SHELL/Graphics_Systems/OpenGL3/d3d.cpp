#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/OpenGL-Common/textures_impl.h"
#include "Graphics_Systems/OpenGL-Desktop/enums.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GSd3d.h"

namespace enigma {

void graphics_state_flush_samplers() {
  static bool samplers_generated = false;
  static GLuint sampler_ids[8];
  if (!samplers_generated) {
    glGenSamplers(8, sampler_ids);
    samplers_generated = true;
  }

  for (size_t i = 0; i < 8; i++) {
    const auto sampler = samplers[i];

    const GLuint gt = get_texture_peer(sampler.texture);
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, gt);

    if (gt == 0) continue; // texture doesn't exist skip updating the sampler

    const GLuint sampler_id = sampler_ids[i];
    glBindSampler(i, sampler_id);

    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_R, sampler.wrapu?GL_REPEAT:GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, sampler.wrapv?GL_REPEAT:GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, sampler.wrapw?GL_REPEAT:GL_CLAMP_TO_EDGE);
    // Default to interpolation disabled, for some reason textures do that by default but not samplers.
    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, sampler.interpolate?GL_LINEAR:GL_NEAREST);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, sampler.interpolate?GL_LINEAR:GL_NEAREST);
  }
}

void graphics_state_flush_extra() {
  glPolygonMode(GL_FRONT_AND_BACK, fillmodes[drawFillMode]);
  glPointSize(drawPointSize);
  (msaaEnabled?glEnable:glDisable)(GL_MULTISAMPLE);
  (d3dClipPlane?glEnable:glDisable)(GL_CLIP_DISTANCE0);
}

} // namespace enigma
