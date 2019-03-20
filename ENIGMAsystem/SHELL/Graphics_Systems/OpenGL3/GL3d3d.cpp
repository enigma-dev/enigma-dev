/** Copyright (C) 2008-2014 Josh Ventura, Robert B. Colton, DatZach, Polygone, Harijs Grinbergs
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

#include "GLSLshader.h"
#include "GL3shader.h"
#include "GL3TextureStruct.h"
#include "Graphics_Systems/General/OpenGLHeaders.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GSblend.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GSmatrix_impl.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/roomsystem.h" // for view variables

#include <glm/gtc/type_ptr.hpp>

#include <math.h>
#include <floatcomp.h>

using namespace std;

namespace {

const GLenum renderstates[3] = {
  GL_NICEST, GL_FASTEST, GL_DONT_CARE
};

const GLenum fogmodes[3] = {
  GL_EXP, GL_EXP2, GL_LINEAR
};

const GLenum depthoperators[8] = {
  GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL,
  GL_GEQUAL, GL_ALWAYS
};

const GLenum fillmodes[3] = {
  GL_POINT, GL_LINE, GL_FILL
};

const GLenum windingstates[2] = {
  GL_CW, GL_CCW
};

const GLenum cullingstates[3] = {
  GL_BACK, GL_FRONT, GL_FRONT_AND_BACK
};

const GLenum stenciloperators[8] = {
  GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT
};

const GLenum blendequivs[11] = {
  GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA,
  GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_DST_COLOR,
  GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA_SATURATE
};

} // namespace anonymous

namespace enigma {

extern unsigned bound_shader;
extern vector<enigma::ShaderProgram*> shaderprograms;

void graphics_state_flush_samplers() {
  static bool samplers_generated = false;
  static GLuint sampler_ids[8];
  if (!samplers_generated) {
    glGenSamplers(8, sampler_ids);
    samplers_generated = true;
  }

  for (size_t i = 0; i < 8; i++) {
    const auto sampler = samplers[i];

    get_texture(gt,sampler.texture,);
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, gt);

    if (gt == 0) continue; // texture doesn't exist skip updating the sampler

    const GLuint sampler_id = sampler_ids[i];
    glBindSampler(i, sampler_id);

    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_R, sampler.wrapu?GL_REPEAT:GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, sampler.wrapv?GL_REPEAT:GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, sampler.wrapw?GL_REPEAT:GL_CLAMP_TO_EDGE);
    // Default to interpolation disabled, for some reason textures do that by default but not samplers.
    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }
}

void graphics_state_flush_lighting() {
  const auto current_shader = enigma::shaderprograms[enigma::bound_shader];

  enigma_user::glsl_uniform4fv(current_shader->uni_ambient_color, 1, d3dLightingAmbient);

  // these 4 are harri's material properties
  float material_ambient[4] = {0.0,0.0,0.0,1.0}; //This is default in GM
  float material_diffuse[4] = {0.8,0.8,0.8,1.0};
  float material_specular[4] = {0.0,0.0,0.0,0.0};
  float material_shininess = 0.0;
  enigma_user::glsl_uniform4fv(current_shader->uni_material_ambient, 1, material_ambient);
  enigma_user::glsl_uniform4fv(current_shader->uni_material_diffuse, 1, material_diffuse);
  enigma_user::glsl_uniform4fv(current_shader->uni_material_specular, 1, material_specular);
  enigma_user::glsl_uniformf(current_shader->uni_material_shininess, material_shininess);

  int activeLights = 0;
  for (int i = 0; i < 8; ++i) {
    if (!d3dLightEnabled[i]) continue; // don't bother updating disabled lights
    ++activeLights;

    const Light& light = d3dLights[i];

    const float posFactor = light.directional ? -1.0f : 1.0f;
    const float pos[4] = {posFactor * (float)light.x, posFactor * (float)light.y, posFactor * (float)light.z, light.directional ? 0.0f : 1.0f},
                diffuse[4] = {float(COL_GET_Rf(light.color)), float(COL_GET_Gf(light.color)), float(COL_GET_Bf(light.color)), 1.0f},
                specular[4] = {0.0,0.0,0.0,0.0},
                ambient[4] = {0.0,0.0,0.0,0.0};

    enigma_user::glsl_uniform4fv(current_shader->uni_light_diffuse[i], 1, diffuse);
    enigma_user::glsl_uniform4fv(current_shader->uni_light_ambient[i], 1, ambient);
    enigma_user::glsl_uniform4fv(current_shader->uni_light_specular[i], 1, specular);
    enigma_user::glsl_uniform4fv(current_shader->uni_light_position[i], 1, pos);

    if (light.directional) continue; // only point lights have range falloff and attenuation
    enigma_user::glsl_uniformf(current_shader->uni_light_cAttenuation[i], 1.0);
    enigma_user::glsl_uniformf(current_shader->uni_light_lAttenuation[i], 0.0);
    enigma_user::glsl_uniformf(current_shader->uni_light_qAttenuation[i], 8.0f/(light.range*light.range));
  }
  enigma_user::glsl_uniformi(current_shader->uni_lights_active, activeLights);
}

void graphics_state_flush() {
  const auto current_shader = enigma::shaderprograms[enigma::bound_shader];
  glPolygonMode(GL_FRONT_AND_BACK, fillmodes[drawFillMode]);
  glPointSize(drawPointSize);
  glLineWidth(drawLineWidth);

  (msaaEnabled?glEnable:glDisable)(GL_MULTISAMPLE);
  (d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);
  glDepthFunc(depthoperators[d3dDepthOperator]);
  glDepthMask(d3dZWriteEnable);
  (d3dCulling>0?glEnable:glDisable)(GL_CULL_FACE);
  if (d3dCulling > 0){
    glFrontFace(windingstates[d3dCulling-1]);
  }
  (d3dClipPlane?glEnable:glDisable)(GL_CLIP_DISTANCE0);

  glColorMask(colorWriteEnable[0], colorWriteEnable[1], colorWriteEnable[2], colorWriteEnable[3]);
  glBlendFunc(blendequivs[(blendMode[0]-1)%11],blendequivs[(blendMode[1]-1)%11]);
  (alphaBlend?glEnable:glDisable)(GL_BLEND);
  enigma_user::glsl_uniformi(current_shader->uni_alphaTestEnable, alphaTest);
  enigma_user::glsl_uniformf(current_shader->uni_alphaTest, (gs_scalar)alphaTestRef/255.0);

  graphics_state_flush_samplers();

  enigma_user::glsl_uniformi(current_shader->uni_lightEnable, d3dLighting);
  if (d3dLighting) graphics_state_flush_lighting();

  //Send transposed (done by GL because of "true" in the function below) matrices to shader
  glm::mat4 mv_matrix = view * world;
  glm::mat4 mvp_matrix = projection * mv_matrix;
  glsl_uniform_matrix4fv_internal(current_shader->uni_modelMatrix,  1, glm::value_ptr(glm::transpose(world)));
  glsl_uniform_matrix4fv_internal(current_shader->uni_viewMatrix,  1, glm::value_ptr(glm::transpose(view)));
  glsl_uniform_matrix4fv_internal(current_shader->uni_projectionMatrix,  1, glm::value_ptr(glm::transpose(projection)));

  glsl_uniform_matrix4fv_internal(current_shader->uni_mvMatrix,  1, glm::value_ptr(glm::transpose(mv_matrix)));
  glsl_uniform_matrix4fv_internal(current_shader->uni_mvpMatrix,  1, glm::value_ptr(glm::transpose(mvp_matrix)));
  glsl_uniform_matrix3fv_internal(current_shader->uni_normalMatrix,  1, glm::value_ptr(glm::mat3(glm::inverse(mv_matrix))));
}

} // namespace enigma

namespace enigma_user {

void d3d_clear_depth(double value) {
  draw_batch_flush(batch_flush_deferred);
  glClearDepth(value);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void d3d_clear_depth(){
  draw_batch_flush(batch_flush_deferred);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void d3d_set_software_vertex_processing(bool software) {
	//Does nothing as GL doesn't have such an awful thing
  //TODO: When we seperate platform specific things, then this shouldn't even exist
}

void d3d_stencil_start_mask(){
  draw_batch_flush(batch_flush_deferred);
  glEnable(GL_STENCIL_TEST);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  glStencilMask(0x1);
  glStencilFunc(GL_ALWAYS, 0x1, 0x1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glClear(GL_STENCIL_BUFFER_BIT);
}

void d3d_stencil_continue_mask(){
  draw_batch_flush(batch_flush_deferred);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  glStencilMask(0x1);
  glStencilFunc(GL_ALWAYS, 0x1, 0x1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void d3d_stencil_use_mask(){
  draw_batch_flush(batch_flush_deferred);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
  glStencilMask(0x0);
  glStencilFunc(GL_EQUAL, 0x1, 0x1);
}

void d3d_stencil_end_mask(){
  draw_batch_flush(batch_flush_deferred);
  glDisable(GL_STENCIL_TEST);
}

void d3d_stencil_enable(bool enable){
  draw_batch_flush(batch_flush_deferred);
  (enable?glEnable:glDisable)(GL_STENCIL_TEST);
}

void d3d_stencil_clear_value(int value){
  draw_batch_flush(batch_flush_deferred);
  glClearStencil(value);
  glClear(GL_STENCIL_BUFFER_BIT);
}

void d3d_stencil_mask(unsigned int mask){
  draw_batch_flush(batch_flush_deferred);
  glStencilMask(mask);
}

void d3d_stencil_clear(){
  draw_batch_flush(batch_flush_deferred);
  glClear(GL_STENCIL_BUFFER_BIT);
}

void d3d_stencil_function(int func, int ref, unsigned int mask){
  draw_batch_flush(batch_flush_deferred);
  glStencilFunc(depthoperators[func], ref, mask);
}

void d3d_stencil_operator(int sfail, int dpfail, int dppass){
  draw_batch_flush(batch_flush_deferred);
  glStencilOp(stenciloperators[sfail], stenciloperators[dpfail], stenciloperators[dppass]);
}

} // namespace enigma_user
