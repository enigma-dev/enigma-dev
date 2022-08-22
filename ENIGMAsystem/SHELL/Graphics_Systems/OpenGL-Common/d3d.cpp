/** Copyright (C) 2008-2014 Josh Ventura, DatZach, Polygone, Harijs Grinbergs
*** Copyright (C) 2013-2014, 2019 Robert B. Colton
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
#include "shader.h"
#include "enums.h"
#include "textures_impl.h"
#include "version.h"
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

namespace enigma {

void graphics_state_flush_lighting(const glm::mat4& mv_matrix, const glm::mat3& normal_matrix) {
  const auto& current_shader = enigma::shaderprograms[enigma::bound_shader];

  const float glAmbientColor[4] = {COL_GET_Rf(d3dLightingAmbient),COL_GET_Gf(d3dLightingAmbient),COL_GET_Bf(d3dLightingAmbient),1.0f};
  enigma_user::glsl_uniform4fv(current_shader->uni_ambient_color, 1, glAmbientColor);

  // these 4 are harri's material properties
  float material_ambient[4] = {0.0,0.0,0.0,1.0}; //This is default in GM
  float material_diffuse[4] = {0.8,0.8,0.8,1.0};
  float material_specular[4] = {0.0,0.0,0.0,0.0};
  float material_shininess = 0.0;
  enigma_user::glsl_uniform4fv(current_shader->uni_material_ambient, 1, material_ambient);
  enigma_user::glsl_uniform4fv(current_shader->uni_material_diffuse, 1, material_diffuse);
  enigma_user::glsl_uniform4fv(current_shader->uni_material_specular, 1, material_specular);
  enigma_user::glsl_uniformf(current_shader->uni_material_shininess, material_shininess);

  for (int i = 0; i < d3dLightsActive; ++i) {
    const Light& light = get_active_light(i);

    const float posFactor = light.directional ? -1.0f : 1.0f;
    const float pos[4] = {posFactor * (float)light.x, posFactor * (float)light.y, posFactor * (float)light.z, light.directional ? 0.0f : 1.0f},
                diffuse[4] = {float(COL_GET_Rf(light.color)), float(COL_GET_Gf(light.color)), float(COL_GET_Bf(light.color)), 1.0f},
                specular[4] = {0.0,0.0,0.0,0.0},
                ambient[4] = {0.0,0.0,0.0,0.0};

    float tpos[4];
    if (light.directional) {
      glm::vec3 lpos_eyespace = normal_matrix * glm::vec3(pos[0],pos[1],pos[2]);
      tpos[0] = lpos_eyespace.x, tpos[1] = lpos_eyespace.y, tpos[2] = lpos_eyespace.z;
    } else {
      glm::vec4 lpos_eyespace = mv_matrix * glm::vec4(pos[0],pos[1],pos[2],1.0f);
      tpos[0] = lpos_eyespace.x, tpos[1] = lpos_eyespace.y, tpos[2] = lpos_eyespace.z;
    }
    tpos[3] = pos[3];

    enigma_user::glsl_uniform4fv(current_shader->uni_light_diffuse[i], 1, diffuse);
    enigma_user::glsl_uniform4fv(current_shader->uni_light_ambient[i], 1, ambient);
    enigma_user::glsl_uniform4fv(current_shader->uni_light_specular[i], 1, specular);
    enigma_user::glsl_uniform4fv(current_shader->uni_light_position[i], 1, tpos);

    if (light.directional) continue; // only point lights have range falloff and attenuation
    enigma_user::glsl_uniformf(current_shader->uni_light_cAttenuation[i], 1.0);
    enigma_user::glsl_uniformf(current_shader->uni_light_lAttenuation[i], 0.0);
    enigma_user::glsl_uniformf(current_shader->uni_light_qAttenuation[i], 8.0f/(light.range*light.range));
  }
  enigma_user::glsl_uniformi(current_shader->uni_lights_active, d3dLightsActive);
}

void graphics_state_flush_fog() {
  const auto& current_shader = enigma::shaderprograms[enigma::bound_shader];

  const float glFogColor[] = {COL_GET_Rf(d3dFogColor),COL_GET_Gf(d3dFogColor),COL_GET_Bf(d3dFogColor),1.0f};
  enigma_user::glsl_uniform4fv(current_shader->uni_fogColor, 1, glFogColor);
  enigma_user::glsl_uniformf(current_shader->uni_fogStart, d3dFogStart);
  enigma_user::glsl_uniformf(current_shader->uni_fogRange, d3dFogEnd);
}

void graphics_state_flush_stencil() {
  glStencilMask(d3dStencilMask);
  glStencilFunc(depthoperators[d3dStencilFunc], d3dStencilFuncRef, d3dStencilFuncMask);
  glStencilOp(stenciloperators[d3dStencilOpStencilFail], stenciloperators[d3dStencilOpDepthFail],
              stenciloperators[d3dStencilOpPass]);
}

void graphics_state_flush_samplers();
void graphics_state_flush() {
  const auto& current_shader = enigma::shaderprograms[enigma::bound_shader];
  
  graphics_flush_ext();
  
  glLineWidth(drawLineWidth);

  glDepthFunc(depthoperators[d3dDepthOperator]);
  glDepthMask(d3dZWriteEnable);
  (d3dCulling>0?glEnable:glDisable)(GL_CULL_FACE);
  if (d3dCulling > 0){
    glFrontFace(windingstates[d3dCulling-1]);
  }

  glColorMask(colorWriteEnable[0], colorWriteEnable[1], colorWriteEnable[2], colorWriteEnable[3]);
  glBlendFunc(blendequivs[(blendMode[0]-1)%11],blendequivs[(blendMode[1]-1)%11]);
  (alphaBlend?glEnable:glDisable)(GL_BLEND);
  enigma_user::glsl_uniformi(current_shader->uni_alphaTestEnable, alphaTest);
  enigma_user::glsl_uniformf(current_shader->uni_alphaTest, (gs_scalar)alphaTestRef/255.0);

  graphics_state_flush_samplers();

  (d3dStencilTest?glEnable:glDisable)(GL_STENCIL_TEST);
  if (d3dStencilTest) graphics_state_flush_stencil();

  //Send transposed (done by GL because of "true" in the function below) matrices to shader
  const glm::mat4 mv_matrix = view * world;
  const glm::mat4 mvp_matrix = projection * mv_matrix;
  const glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(mv_matrix)));
  glsl_uniform_matrix4fv_internal(current_shader->uni_modelMatrix,  1, glm::value_ptr(glm::transpose(world)));
  glsl_uniform_matrix4fv_internal(current_shader->uni_viewMatrix,  1, glm::value_ptr(glm::transpose(view)));
  glsl_uniform_matrix4fv_internal(current_shader->uni_projectionMatrix,  1, glm::value_ptr(glm::transpose(projection)));

  glsl_uniform_matrix4fv_internal(current_shader->uni_mvMatrix,  1, glm::value_ptr(glm::transpose(mv_matrix)));
  glsl_uniform_matrix4fv_internal(current_shader->uni_mvpMatrix,  1, glm::value_ptr(glm::transpose(mvp_matrix)));
  glsl_uniform_matrix3fv_internal(current_shader->uni_normalMatrix,  1, glm::value_ptr(normal_matrix));

  enigma_user::glsl_uniformi(current_shader->uni_lightEnable, d3dLighting);
  if (d3dLighting) graphics_state_flush_lighting(mv_matrix, normal_matrix);
  enigma_user::glsl_uniformi(current_shader->uni_fogPSEnable, d3dFogEnabled);
  if (d3dFogEnabled) graphics_state_flush_fog();
}

} // namespace enigma

namespace enigma_user {

void d3d_clear_depth(double value) {
  draw_batch_flush(batch_flush_deferred);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void d3d_clear_depth() {
  draw_batch_flush(batch_flush_deferred);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void d3d_stencil_clear_value(int value) {
  draw_batch_flush(batch_flush_deferred);
  glClearStencil(value);
  glClear(GL_STENCIL_BUFFER_BIT);
}

void d3d_stencil_clear() {
  draw_batch_flush(batch_flush_deferred);
  glClear(GL_STENCIL_BUFFER_BIT);
}

void d3d_set_software_vertex_processing(bool software) {
  //Does nothing as GL doesn't have such an awful thing
  //TODO: When we seperate platform specific things, then this shouldn't even exist
}

} // namespace enigma_user
