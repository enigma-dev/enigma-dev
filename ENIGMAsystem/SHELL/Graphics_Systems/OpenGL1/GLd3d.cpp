/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton, DatZach, Polygone
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

void graphics_set_matrix(int type) {
  enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
  glm::mat4 matrix;
  switch(type) {
    case enigma_user::matrix_world:
    case enigma_user::matrix_view:
      if (type == enigma_user::matrix_view)
        enigma::d3d_light_update_positions();
      matrix = enigma::view * enigma::world;
      glMatrixMode(GL_MODELVIEW);
      break;
    case enigma_user::matrix_projection:
      matrix = enigma::projection;
      glMatrixMode(GL_PROJECTION);
      break;
    default:
      #ifdef DEBUG_MODE
      show_error("Unknown matrix type " + std::to_string(type), false);
      #endif
      return;
  }
  glLoadMatrixf(glm::value_ptr(matrix));
}

} // namespace enigma

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

  // Set global ambient lighting to nothing.
  float global_ambient[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

  // Enable depth buffering
  enigma::d3dMode = true;
  enigma::d3dPerspective = true;
  enigma::d3dHidden = true;
  enigma::d3dZWriteEnable = true;
  enigma::d3dCulling = rs_none;
  glDepthMask(true);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);

  // Set up projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, -view_wview[view_current] / (double)view_hview[view_current], 1, 32000);

  // Set up modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
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
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_NORMALIZE);
  glDisable(GL_COLOR_MATERIAL);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(0, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
}

// disabling hidden surface removal in means there is no depth buffer
void d3d_set_hidden(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
  (enable?glEnable:glDisable)(GL_DEPTH_TEST);
  enigma::d3dHidden = enable;
  //d3d_set_zwriteenable(enable);
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

void d3d_set_lighting(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
  (enable?glEnable:glDisable)(GL_LIGHTING);
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
  draw_batch_flush(batch_flush_deferred);
  (enable?glEnable:glDisable)(GL_FOG);
}

void d3d_set_fog_mode(int mode)
{
  draw_batch_flush(batch_flush_deferred);
  glFogi(GL_FOG_MODE, fogmodes[mode]);
}

void d3d_set_fog_hint(int mode) {
  draw_batch_flush(batch_flush_deferred);
  glHint(GL_FOG_HINT, mode);
}

void d3d_set_fog_color(int color)
{
  draw_batch_flush(batch_flush_deferred);
  GLfloat fog_color[3];
  fog_color[0] = COL_GET_Rf(color);
  fog_color[1] = COL_GET_Gf(color);
  fog_color[2] = COL_GET_Bf(color);
  glFogfv(GL_FOG_COLOR, fog_color);
}

void d3d_set_fog_start(double start)
{
  draw_batch_flush(batch_flush_deferred);
  glFogf(GL_FOG_START, start);
}

void d3d_set_fog_end(double end)
{
  draw_batch_flush(batch_flush_deferred);
  glFogf(GL_FOG_END, end);
}

void d3d_set_fog_density(double density)
{
  draw_batch_flush(batch_flush_deferred);
  glFogf(GL_FOG_DENSITY, density);
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

void d3d_set_depth(double dep)
{
  //TODO: Write function
}

void d3d_set_shading(bool smooth)
{
  draw_batch_flush(batch_flush_deferred);
  glShadeModel(smooth?GL_SMOOTH:GL_FLAT);
}

void d3d_set_clip_plane(bool enable)
{
  //GL1 doesn't really support clip planes. There could be ways around that though
  //printf not declared
  //printf("warning: d3d_set_clip_plane(bool enable) called even though GL1 doesn't support this!\n");
}

}

#include <map>
#include <list>
#include "Universal_System/fileio.h"

struct posi { // Homogenous point.
    gs_scalar x;
    gs_scalar y;
    gs_scalar z;
    gs_scalar w;
    posi(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar w1) : x(x1), y(y1), z(z1), w(w1){}
};

class d3d_lights
{
    map<int,int> light_ind;
    map<int,posi> ind_pos; // Internal index to position.

    public:
    d3d_lights() {}
    ~d3d_lights() {}

    void light_set_position(int id, const float* pos) {
      // this is done for compatibility with D3D/GM
      // make sure to call this anywhere you set a light's position
      // instead of calling glLightfv directly
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix(); // save present matrix
      // define lights with respect to view matrix but not world
      glLoadMatrixf(glm::value_ptr(enigma::view));
      glLightfv(GL_LIGHT0+id, GL_POSITION, pos);
      glPopMatrix(); // restore original matrix
    }

    void light_update_positions()
    {
        // this logic is repeated from light_set_position for efficiency
        // so that we don't keep pushing/popping for all 8 hardware
        // lights that are supported by GM
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix(); // save present matrix
        // define lights with respect to view matrix but not world
        glLoadMatrixf(glm::value_ptr(enigma::view));

        map<int, posi>::iterator end = ind_pos.end();
        for (map<int, posi>::iterator it = ind_pos.begin(); it != end; it++) {
            const posi pos1 = (*it).second;
            const float pos[4] = {pos1.x, pos1.y, pos1.z, pos1.w};
            glLightfv(GL_LIGHT0+(*it).first, GL_POSITION, pos);
        }

        glPopMatrix(); // restore original matrix
    }

    bool light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col)
    {
        int ms;
        if (light_ind.find(id) != light_ind.end())
        {
            ms = (*light_ind.find(id)).second;
            multimap<int,posi>::iterator it = ind_pos.find(ms);
            if (it != ind_pos.end())
                ind_pos.erase(it);
            ind_pos.insert(pair<int,posi>(ms, posi(-dx, -dy, -dz, 0.0f)));
        }
        else
        {
            ms = light_ind.size();
            int MAX_LIGHTS;
            glGetIntegerv(GL_MAX_LIGHTS, &MAX_LIGHTS);
            if (ms >= MAX_LIGHTS)
                return false;

            light_ind.insert(pair<int,int>(id, ms));
            ind_pos.insert(pair<int,posi>(ms, posi(-dx, -dy, -dz, 0.0f)));
        }

        const float dir[4] = {float(-dx), float(-dy), float(-dz), 0.0f}, color[4] = {float(COL_GET_Rf(col)), float(COL_GET_Gf(col)), float(COL_GET_Bf(col)), 1.0f};
        light_set_position(ms, dir);
        glLightfv(GL_LIGHT0+ms, GL_DIFFUSE, color);

        return true;
    }

    bool light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
    {
        if (range <= 0.0) {
            return false;
        }
        int ms;
        if (light_ind.find(id) != light_ind.end())
        {
            ms = (*light_ind.find(id)).second;
            multimap<int,posi>::iterator it = ind_pos.find(ms);
            if (it != ind_pos.end())
                ind_pos.erase(it);
            ind_pos.insert(pair<int,posi>(ms, posi(x, y, z, 1)));
        }
        else
        {
            ms = light_ind.size();
            int MAX_LIGHTS;
            glGetIntegerv(GL_MAX_LIGHTS, &MAX_LIGHTS);
            if (ms >= MAX_LIGHTS)
                return false;

            light_ind.insert(pair<int,int>(id, ms));
            ind_pos.insert(pair<int,posi>(ms, posi(x, y, z, 1)));
        }
        const float pos[4] = {(float)x, (float)y, (float)z, 1.0f}, color[4] = {float(COL_GET_Rf(col)), float(COL_GET_Gf(col)), float(COL_GET_Bf(col)), 1.0f},
            specular[4] = {0.0f, 0.0f, 0.0f, 0.0f}, ambient[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        light_set_position(ms, pos);
        glLightfv(GL_LIGHT0+ms, GL_DIFFUSE, color);
        glLightfv(GL_LIGHT0+ms, GL_SPECULAR, specular);
        glLightfv(GL_LIGHT0+ms, GL_AMBIENT, ambient);
        // Limit the range of the light through attenuation.
        glLightf(GL_LIGHT0+ms, GL_CONSTANT_ATTENUATION, 1.0);
        glLightf(GL_LIGHT0+ms, GL_LINEAR_ATTENUATION, 0.0);
        // 48 is a number gotten through manual calibration. Make it lower to increase the light power.
        const float attenuation_calibration = 8.0;
        glLightf(GL_LIGHT0+ms, GL_QUADRATIC_ATTENUATION, attenuation_calibration/(range*range));

        return true;
    }

    bool light_define_specularity(int id, int r, int g, int b, double a)
    {
        int ms;
        if (light_ind.find(id) != light_ind.end())
        {
            ms = (*light_ind.find(id)).second;
        }
        else
        {
            ms = light_ind.size();
            int MAX_LIGHTS;
            glGetIntegerv(GL_MAX_LIGHTS, &MAX_LIGHTS);
            if (ms >= MAX_LIGHTS)
                return false;
        }
        float specular[4] = {(float)r, (float)g, (float)b, (float)a};
        glLightfv(GL_LIGHT0+ms, GL_SPECULAR, specular);

        return true;
    }

    bool light_set_ambient(int id, int r, int g, int b, double a)
    {
        int ms;
        if (light_ind.find(id) != light_ind.end())
        {
            ms = (*light_ind.find(id)).second;
        }
        else
        {
            ms = light_ind.size();
            int MAX_LIGHTS;
            glGetIntegerv(GL_MAX_LIGHTS, &MAX_LIGHTS);
            if (ms >= MAX_LIGHTS)
                return false;
        }
        float specular[4] = {(float)r, (float)g, (float)b, (float)a};
        glLightfv(GL_LIGHT0+ms, GL_AMBIENT, specular);

        return true;
    }

    bool light_set_specular(int id, int r, int g, int b, double a)
    {
        int ms;
        if (light_ind.find(id) != light_ind.end())
        {
            ms = (*light_ind.find(id)).second;
        }
        else
        {
            ms = light_ind.size();
            int MAX_LIGHTS;
            glGetIntegerv(GL_MAX_LIGHTS, &MAX_LIGHTS);
            if (ms >= MAX_LIGHTS)
                return false;
        }
        float specular[4] = {(float)r, (float)g, (float)b, (float)a};
        glLightfv(GL_LIGHT0+ms, GL_SPECULAR, specular);

        return true;
    }

    bool light_enable(int id)
    {
        map<int, int>::iterator it = light_ind.find(id);
        if (it == light_ind.end())
        {
            const int ms = light_ind.size();
            int MAX_LIGHTS;
            glGetIntegerv(GL_MAX_LIGHTS, &MAX_LIGHTS);
            if (ms >= MAX_LIGHTS)
                return false;
            light_ind.insert(pair<int,int>(id, ms));
            glEnable(GL_LIGHT0+ms);
        }
        else
        {
            glEnable(GL_LIGHT0+(*it).second);
        }

        return true;
    }

    bool light_disable(int id)
    {
        map<int, int>::iterator it = light_ind.find(id);
        if (it == light_ind.end())
        {
            return false;
        }
        else
        {
            glDisable(GL_LIGHT0+(*it).second);
        }
        return true;
    }
} d3d_lighting;

namespace enigma_user
{

bool d3d_light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col)
{
  draw_batch_flush(batch_flush_deferred);
  return d3d_lighting.light_define_direction(id, dx, dy, dz, col);
}

bool d3d_light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
{
  draw_batch_flush(batch_flush_deferred);
  return d3d_lighting.light_define_point(id, x, y, z, range, col);
}

bool d3d_light_define_specularity(int id, int r, int g, int b, double a)
{
  draw_batch_flush(batch_flush_deferred);
  return d3d_lighting.light_define_specularity(id, r, g, b, a);
}

void d3d_light_specularity(int facemode, int r, int g, int b, double a)
{
  draw_batch_flush(batch_flush_deferred);
  double specular[4] = {(double)r, (double)g, (double)b, a};
  glMaterialfv(renderstates[facemode], GL_SPECULAR, (float*)specular);
}

bool d3d_light_set_ambient(int id, int r, int g, int b, double a)
{
  draw_batch_flush(batch_flush_deferred);
  return d3d_lighting.light_set_ambient(id, r, g, b, a);
}

bool d3d_light_set_specularity(int id, int r, int g, int b, double a)
{
  draw_batch_flush(batch_flush_deferred);
  return d3d_lighting.light_set_specular(id, r, g, b, a);
}

void d3d_light_shininess(int facemode, int shine)
{
  draw_batch_flush(batch_flush_deferred);
  glMateriali(renderstates[facemode], GL_SHININESS, shine);
}

void d3d_light_define_ambient(int col)
{
  draw_batch_flush(batch_flush_deferred);
  float color[4] = {float(COL_GET_Rf(col)), float(COL_GET_Gf(col)), float(COL_GET_Bf(col)), 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color);
}

bool d3d_light_enable(int id, bool enable)
{
  draw_batch_flush(batch_flush_deferred);
  return enable?d3d_lighting.light_enable(id):d3d_lighting.light_disable(id);
}

void d3d_stencil_start_mask(){
  glEnable(GL_STENCIL_TEST);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  glStencilMask(0x1);
  glStencilFunc(GL_ALWAYS, 0x1, 0x1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glClear(GL_STENCIL_BUFFER_BIT);
}

void d3d_stencil_continue_mask(){
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  glStencilMask(0x1);
  glStencilFunc(GL_ALWAYS, 0x1, 0x1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void d3d_stencil_use_mask(){
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
  glStencilMask(0x0);
  glStencilFunc(GL_EQUAL, 0x1, 0x1);
}

void d3d_stencil_end_mask(){
  glDisable(GL_STENCIL_TEST);
}

}

namespace enigma {
    void d3d_light_update_positions()
    {
      d3d_lighting.light_update_positions();
    }
}
