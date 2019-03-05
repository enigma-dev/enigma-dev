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
#include "Graphics_Systems/General/OpenGLHeaders.h"
#include "Graphics_Systems/General/GSd3d.h"
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

void d3d_light_update_positions(); // forward declare

bool d3dMode = false;
bool d3dHidden = false;
bool d3dZWriteEnable = true;
int d3dCulling = 0;
extern unsigned bound_shader;
extern vector<enigma::ShaderProgram*> shaderprograms;

void graphics_set_matrix(int type) {
  enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
  //Send transposed (done by GL because of "true" in the function below) matrices to shader
  switch(type) {
    case enigma_user::matrix_world:
      glsl_uniform_matrix4fv_internal(shaderprograms[bound_shader]->uni_modelMatrix,  1, glm::value_ptr(glm::transpose(world)));
      break;
    case enigma_user::matrix_view:
      glsl_uniform_matrix4fv_internal(shaderprograms[bound_shader]->uni_viewMatrix,  1, glm::value_ptr(glm::transpose(view)));
      break;
    case enigma_user::matrix_projection:
      glsl_uniform_matrix4fv_internal(shaderprograms[bound_shader]->uni_projectionMatrix,  1, glm::value_ptr(glm::transpose(projection)));
      break;
    default:
      #ifdef DEBUG_MODE
      show_error("Unknown matrix type " + std::to_string(type), false);
      #endif
      return;
  }

  glm::mat4 mv_matrix = view * world;
  switch (type) {
    case enigma_user::matrix_world:
    case enigma_user::matrix_view:
    glsl_uniform_matrix4fv_internal(shaderprograms[bound_shader]->uni_mvMatrix,  1, glm::value_ptr(glm::transpose(mv_matrix)));
    break;
  }

  glm::mat4 mvp_matrix = projection * mv_matrix;
  glsl_uniform_matrix4fv_internal(shaderprograms[bound_shader]->uni_mvpMatrix,  1, glm::value_ptr(glm::transpose(mvp_matrix)));
  glsl_uniform_matrix3fv_internal(shaderprograms[bound_shader]->uni_normalMatrix,  1, glm::value_ptr(glm::mat3(glm::inverse(mv_matrix))));

  enigma::d3d_light_update_positions();
}

}

GLenum renderstates[3] = {
  GL_NICEST, GL_FASTEST, GL_DONT_CARE
};

GLenum fogmodes[3] = {
  GL_EXP, GL_EXP2, GL_LINEAR
};

GLenum depthoperators[8] = {
  GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL,
  GL_GEQUAL, GL_ALWAYS
};

GLenum fillmodes[3] = {
  GL_POINT, GL_LINE, GL_FILL
};

GLenum windingstates[2] = {
  GL_CW, GL_CCW
};

GLenum cullingstates[3] = {
  GL_BACK, GL_FRONT, GL_FRONT_AND_BACK
};

GLenum stenciloperators[8] = {
  GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT
};

namespace enigma_user
{

void d3d_clear_depth(double value) {
  draw_batch_flush(batch_flush_deferred);
  glClearDepth(value);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void d3d_set_software_vertex_processing(bool software) {
	//Does nothing as GL doesn't have such an awful thing
  //TODO: When we seperate platform specific things, then this shouldn't even exist
}

void d3d_start()
{
  draw_batch_flush(batch_flush_deferred);

  // Enable depth buffering
  enigma::d3dMode = true;
  enigma::d3dPerspective = true;
  enigma::d3dHidden = true;
  enigma::d3dZWriteEnable = true;
  enigma::d3dCulling = rs_none;
  glDepthMask(true);
  glEnable(GL_DEPTH_TEST);

  // Set up projection matrix
  d3d_set_projection_perspective(0, 0, view_wview[view_current], view_hview[view_current], 0);

  // Set up modelview matrix
  d3d_transform_set_identity();
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void d3d_end()
{
  draw_batch_flush(batch_flush_deferred);

  enigma::d3dMode = false;
  enigma::d3dPerspective = false;
  enigma::d3dHidden = false;
  enigma::d3dZWriteEnable = false;
  enigma::d3dCulling = rs_none;
  glDepthMask(false);
  glDisable(GL_DEPTH_TEST);
  d3d_set_projection_ortho(0, 0, view_wview[view_current], view_hview[view_current], 0); //This should probably be changed not to use views
}

// disabling hidden surface removal in means there is no depth buffer
void d3d_set_hidden(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
  (enable?glEnable:glDisable)(GL_DEPTH_TEST);
  enigma::d3dHidden = enable;
}

// disabling zwriting can let you turn off testing for a single model, for instance
// to fix cutout billboards such as trees the alpha pixels on their edges may not depth sort
// properly particle effects are usually drawn with zwriting disabled because of this as well
void d3d_set_zwriteenable(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
	glDepthMask(enable);
	enigma::d3dZWriteEnable = enable;
}

void d3d_set_fog(bool enable, int color, double start, double end)
{
  d3d_set_fog_enabled(enable);
  d3d_set_fog_color(color);
  d3d_set_fog_start(start);
  d3d_set_fog_end(end);
  d3d_set_fog_hint(rs_nicest);
  d3d_set_fog_mode(rs_linear);
}//NOTE: fog can use vertex checks with less good graphic cards which screws up large textures (however this doesn't happen in directx)

void d3d_set_fog_enabled(bool enable)
{

}

void d3d_set_fog_mode(int mode)
{
}

void d3d_set_fog_hint(int mode) {

}

void d3d_set_fog_color(int color)
{

}

void d3d_set_fog_start(double start)
{

}

void d3d_set_fog_end(double end)
{

}

void d3d_set_fog_density(double density)
{

}

void d3d_set_culling(int mode)
{
  draw_batch_flush(batch_flush_deferred);
	enigma::d3dCulling = mode;
	(mode>0?glEnable:glDisable)(GL_CULL_FACE);
	if (mode > 0){
		glFrontFace(windingstates[mode-1]);
	}
}

void d3d_set_color_mask(bool r, bool g, bool b, bool a){
  draw_batch_flush(batch_flush_deferred);
  glColorMask(r,g,b,a);
}

bool d3d_get_mode()
{
  return enigma::d3dMode;
}

bool d3d_get_hidden() {
	return enigma::d3dHidden;
}

int d3d_get_culling() {
	return enigma::d3dCulling;
}

void d3d_set_fill_mode(int fill)
{
  draw_batch_flush(batch_flush_deferred);
  glPolygonMode(GL_FRONT_AND_BACK, fillmodes[fill]);
}

void d3d_set_line_width(float value) {
  draw_batch_flush(batch_flush_deferred);
  glLineWidth(value);
}

void d3d_set_point_size(float value) {
  draw_batch_flush(batch_flush_deferred);
  glPointSize(value);
}

void d3d_set_depth_operator(int mode) {
  draw_batch_flush(batch_flush_deferred);
  glDepthFunc(depthoperators[mode]);
}

void d3d_clear_depth(){
  draw_batch_flush(batch_flush_deferred);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void d3d_set_depth(double dep)
{

}//TODO: Write function

void d3d_set_shading(bool smooth)
{

}

void d3d_set_clip_plane(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
  (enable?glEnable:glDisable)(GL_CLIP_DISTANCE0);
}

}

#include <map>
#include <list>
#include "Universal_System/fileio.h"

namespace enigma
{

struct light3D {
  int type; //0 - directional, 1 - positional
  bool enabled;
  bool update;
  gs_scalar position[4];
  gs_scalar transformed_position[4];
  float diffuse[4];
  float specular[4];
  float ambient[4];
  float constant_attenuation;
  float linear_attenuation;
  float quadratic_attenuation;

  light3D(bool first = false)
  {
    type = 0;
    enabled = false;
    update = false;
    position[0]=0, position[1]=0, position[2]=1, position[3]=0;
    transformed_position[0]=0, transformed_position[1]=0, transformed_position[2]=0, transformed_position[3]=0;
    if (first == true){ //By GL1 spec, the first light is different
      diffuse[0]=1, diffuse[1]=1, diffuse[2]=1, diffuse[3]=1;
      specular[0]=1, specular[1]=1, specular[2]=1, specular[3]=1;
    }else{
      diffuse[0]=0, diffuse[1]=0, diffuse[2]=0, diffuse[3]=0;
      specular[0]=0, specular[1]=0, specular[2]=0, specular[3]=0;
    }
    ambient[0]=0, ambient[1]=0, ambient[2]=0, ambient[3]=1.0;
    constant_attenuation = 1.0;
    linear_attenuation = 0;
    quadratic_attenuation = 0;
  };
};

struct material3D {
  float ambient[4];
  float diffuse[4];
  float specular[4];
  float shininess;

  material3D()
  {
    //ambient[0] = 0.2, ambient[1] = 0.2, ambient[2] = 0.2, ambient[3] = 1.0; //This is default in GL1.1
    ambient[0] = 0.0, ambient[1] = 0.0, ambient[2] = 0.0, ambient[3] = 1.0; //This is default in GM
    diffuse[0] = 0.8, diffuse[1] = 0.8, diffuse[2] = 0.8, diffuse[3] = 1.0;
    specular[0] = 0.0, specular[1] = 0.0, specular[2] = 0.0, specular[3] = 0.0;
    shininess = 0.0;
  }
};

class d3d_lights
{
  vector<light3D> lights;
  material3D material;
  bool lights_enabled;

  public:
  float global_ambient_color[4];
  d3d_lights() {
    lights_enabled = false;
    global_ambient_color[0] = global_ambient_color[1] = global_ambient_color[2] = 0.2f;
    global_ambient_color[3] = 1.0f;
    lights.push_back(light3D(true));
    for (unsigned int i=0; i<7; ++i){
      lights.push_back(light3D());
    }
  }
  //~d3d_lights() {}

  void lights_enable(bool enable){
    lights_enabled = enable;
  }

  void light_update()
  {
    enigma_user::glsl_uniformi(enigma::shaderprograms[enigma::bound_shader]->uni_lightEnable, lights_enabled);
    if (lights_enabled == true){
      enigma_user::glsl_uniform4fv(enigma::shaderprograms[enigma::bound_shader]->uni_ambient_color, 1, global_ambient_color);
      enigma_user::glsl_uniform4fv(enigma::shaderprograms[enigma::bound_shader]->uni_material_ambient, 1, material.ambient);
      enigma_user::glsl_uniform4fv(enigma::shaderprograms[enigma::bound_shader]->uni_material_diffuse, 1, material.diffuse);
      enigma_user::glsl_uniform4fv(enigma::shaderprograms[enigma::bound_shader]->uni_material_specular, 1, material.specular);
      enigma_user::glsl_uniformf(enigma::shaderprograms[enigma::bound_shader]->uni_material_shininess, material.shininess);
    }
  }

  void lightsource_update()
  {
    if (lights_enabled == true){
      unsigned int al = 0; //Active lights
      for (unsigned int i=0; i<lights.size(); ++i){
        if (lights[i].enabled == true){
          if (lights[i].update == true){
            enigma_user::glsl_uniform4fv(enigma::shaderprograms[enigma::bound_shader]->uni_light_ambient[al], 1, lights[i].ambient);
            enigma_user::glsl_uniform4fv(enigma::shaderprograms[enigma::bound_shader]->uni_light_diffuse[al], 1, lights[i].diffuse);
            enigma_user::glsl_uniform4fv(enigma::shaderprograms[enigma::bound_shader]->uni_light_specular[al], 1, lights[i].specular);
            if (lights[i].position[3] != 0.0){ //Light is a point light
              enigma_user::glsl_uniformf(enigma::shaderprograms[enigma::bound_shader]->uni_light_cAttenuation[al], lights[i].constant_attenuation);
              enigma_user::glsl_uniformf(enigma::shaderprograms[enigma::bound_shader]->uni_light_lAttenuation[al], lights[i].linear_attenuation);
              enigma_user::glsl_uniformf(enigma::shaderprograms[enigma::bound_shader]->uni_light_qAttenuation[al], lights[i].quadratic_attenuation);
            }
            enigma_user::glsl_uniform4fv(enigma::shaderprograms[enigma::bound_shader]->uni_light_position[al], 1, lights[i].transformed_position);
            lights[i].update = false;
          }
          ++al;
        }
      }
      enigma_user::glsl_uniformi(enigma::shaderprograms[enigma::bound_shader]->uni_lights_active, al);
    }
  }

  void light_update_positions()
  {
    unsigned int al = 0; //Active lights
    for (unsigned int i=0; i<lights.size(); ++i){
      if (lights[i].type == 0){ //Directional light
        glm::vec3 lpos_eyespace = glm::vec3(lights[i].position[0],lights[i].position[1],lights[i].position[2]);
        lights[i].transformed_position[0] = lpos_eyespace.x, lights[i].transformed_position[1] = lpos_eyespace.y, lights[i].transformed_position[2] = lpos_eyespace.z, lights[i].transformed_position[3] = lights[i].position[3];
      }else{ //Point lights
        glm::vec4 lpos_eyespace = glm::vec4(lights[i].position[0],lights[i].position[1],lights[i].position[2],1.0);
        lights[i].transformed_position[0] = lpos_eyespace.x, lights[i].transformed_position[1] = lpos_eyespace.y, lights[i].transformed_position[2] = lpos_eyespace.z, lights[i].transformed_position[3] = lights[i].position[3];
      }
      if (lights[i].enabled == true){
        enigma_user::glsl_uniform4fv(enigma::shaderprograms[enigma::bound_shader]->uni_light_position[al], 1, lights[i].transformed_position);
        ++al;
      }
    }
  }

  bool light_define_direction(unsigned int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col)
  {
    if (id<lights.size()){
      lights[id].type = 0;
      lights[id].position[0] = -dx;
      lights[id].position[1] = -dy;
      lights[id].position[2] = -dz;
      lights[id].position[3] = 0.0;
      lights[id].diffuse[0] = COL_GET_Rf(col);
      lights[id].diffuse[1] = COL_GET_Gf(col);
      lights[id].diffuse[2] = COL_GET_Bf(col);
      lights[id].diffuse[3] = 1.0f;
      lights[id].update = true;
      lightsource_update();
      light_update_positions();
      return true;
    }
    return false;
  }

  bool light_define_point(unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar range, int col)
  {
    if (range <= 0.0) {
      return false;
    }
    if (id<lights.size()){
      lights[id].type = 1;
      lights[id].position[0] = x;
      lights[id].position[1] = y;
      lights[id].position[2] = z;
      lights[id].position[3] = range;
      lights[id].diffuse[0] = COL_GET_Rf(col);
      lights[id].diffuse[1] = COL_GET_Gf(col);
      lights[id].diffuse[2] = COL_GET_Bf(col);
      lights[id].diffuse[3] = 1.0f;
      lights[id].specular[0] = 0.0f;
      lights[id].specular[1] = 0.0f;
      lights[id].specular[2] = 0.0f;
      lights[id].specular[3] = 0.0f;
      lights[id].ambient[0] = 0.0f;
      lights[id].ambient[1] = 0.0f;
      lights[id].ambient[2] = 0.0f;
      lights[id].ambient[3] = 0.0f;
      lights[id].constant_attenuation = 1.0f;
      lights[id].linear_attenuation = 0.0f;
      lights[id].quadratic_attenuation = 8.0f/(range*range);
      lights[id].update = true;
      lightsource_update();
      light_update_positions();
      return true;
    }
    return false;
  }

  bool light_set_specularity(unsigned int id, gs_scalar r, gs_scalar g, gs_scalar b, gs_scalar a)
  {
    if (id<lights.size()){
      lights[id].specular[0] = r;
      lights[id].specular[1] = g;
      lights[id].specular[2] = b;
      lights[id].specular[3] = a;
      lights[id].update = true;
      lightsource_update();
      return true;
    }
    return false;
  }

  bool light_set_ambient(unsigned int id, gs_scalar r, gs_scalar g, gs_scalar b, gs_scalar a)
  {
    if (id<lights.size()){
      lights[id].ambient[0] = r;
      lights[id].ambient[1] = g;
      lights[id].ambient[2] = b;
      lights[id].ambient[3] = a;
      lights[id].update = true;
      lightsource_update();
      return true;
    }
    return false;
  }


  bool light_enable(unsigned int id)
  {
    if (id<lights.size()){
      lights[id].enabled = true;
      lights[id].update = true;
      lightsource_update();
      return true;
    }
    return false;
  }

  bool light_disable(unsigned int id)
  {
    if (id<lights.size()){
      lights[id].enabled = false;
      lightsource_update();
      return true;
    }
    return false;
  }
} d3d_lighting;

}

namespace enigma_user
{

bool d3d_light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col)
{
  draw_batch_flush(batch_flush_deferred);
  return enigma::d3d_lighting.light_define_direction(id, dx, dy, dz, col);
}

bool d3d_light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
{
  draw_batch_flush(batch_flush_deferred);
  return enigma::d3d_lighting.light_define_point(id, x, y, z, range, col);
}

bool d3d_light_set_specularity(int id, int r, int g, int b, double a)
{
  draw_batch_flush(batch_flush_deferred);
  return enigma::d3d_lighting.light_set_specularity(id, (gs_scalar)r/255.0, (gs_scalar)g/255.0, (gs_scalar)b/255.0, a);
}

bool d3d_light_set_ambient(int id, int r, int g, int b, double a)
{
  draw_batch_flush(batch_flush_deferred);
  return enigma::d3d_lighting.light_set_ambient(id, (gs_scalar)r/255.0, (gs_scalar)g/255.0, (gs_scalar)b/255.0, a);
}

void d3d_light_define_ambient(int col)
{
  draw_batch_flush(batch_flush_deferred);
  enigma::d3d_lighting.global_ambient_color[0] = COL_GET_Rf(col);
  enigma::d3d_lighting.global_ambient_color[1] = COL_GET_Gf(col);
  enigma::d3d_lighting.global_ambient_color[2] = COL_GET_Bf(col);
  enigma::d3d_lighting.light_update();
}

bool d3d_light_enable(int id, bool enable)
{
  draw_batch_flush(batch_flush_deferred);
  return enable?enigma::d3d_lighting.light_enable(id):enigma::d3d_lighting.light_disable(id);
}

void d3d_set_lighting(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
  enigma::d3d_lighting.lights_enable(enable);
  enigma::d3d_lighting.light_update();
  if (enable == true){
    enigma::d3d_lighting.lightsource_update();
  }
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

}

namespace enigma {
  void d3d_light_update_positions()
  {
    enigma::d3d_lighting.light_update_positions();
  }
}
