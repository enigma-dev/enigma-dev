/** Copyright (C) 2013 Robert B. Colton
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

#include "../General/DirectXHeaders.h"
#include "../General/GSd3d.h"
#include "DX10vertexbuffer.h"
#include "../General/GStextures.h"
#include "DX10model.h"
#include "DX10shapes.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>
#include "../General/DXbinding.h"

using namespace std;

#define __GETR(x) ((x & 0x0000FF))/255.0
#define __GETG(x) ((x & 0x00FF00)>>8)/255.0
#define __GETB(x) ((x & 0xFF0000)>>16)/255.0

bool d3dMode = false;
bool d3dHidden = false;
bool d3dZWriteEnable = true;
double projection_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}, transformation_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

namespace enigma_user
{

void d3d_start()
{

}

void d3d_end()
{

}

bool d3d_get_mode()
{
    return d3dMode;
}

void d3d_set_hidden(bool enable)
{
    d3dHidden = enable;
}   // TODO: Write function

void d3d_set_zwriteenable(bool enable)
{

}

void d3d_set_lighting(bool enable)
{

}

void d3d_set_fog(bool enable, int color, double start, double end)
{
  d3d_set_fog_enabled(enable);
  d3d_set_fog_color(color);
  d3d_set_fog_start(start);
  d3d_set_fog_end(end);
  d3d_set_fog_hint(rs_nicest);
  d3d_set_fog_mode(rs_linear);
}///NOTE: fog can use vertex checks with less good graphic cards which screws up large textures (however this doesn't happen in directx)

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

void d3d_set_culling(bool enable)
{

}

void d3d_set_culling_mode(int mode) {

}

void d3d_set_culling_orientation(int mode) {

}

void d3d_set_render_mode(int face, int fill)
{

}
void d3d_set_line_width(float value) {

}

void d3d_set_point_size(float value) {

}

void d3d_depth_clear() {
  d3d_depth_clear_value(1.0f);
}

void d3d_depth_clear_value(float value) {

}

void d3d_depth_operator(int mode) {

}

void d3d_set_perspective(bool enable)
{

}

void d3d_set_depth(double dep)
{

}

void d3d_set_shading(bool smooth)
{

}

}

namespace enigma {
  extern unsigned char currentcolor[4];
}

namespace enigma_user
{

void d3d_set_projection(float xfrom, float yfrom, float zfrom,float xto, float yto, float zto,float xup, float yup, float zup)
{

}

void d3d_set_projection_ext(float xfrom, float yfrom, float zfrom,float xto, float yto, float zto,float xup, float yup, float zup,double angle,double aspect,double znear,double zfar)
{

}

void d3d_set_projection_ortho(float x, float y, float width, float height, double angle)
{

}

void d3d_set_projection_perspective(float x, float y, float width, float height, double angle)
{

}

void d3d_draw_wall(float x1, float y1, float z1, float x2, float y2, float z2, int texId, float hrep, float vrep)
{

}

void d3d_draw_floor(float x1, float y1, float z1, float x2, float y2, float z2, int texId, float hrep, float vrep)
{

}

void d3d_draw_block(float x1, float y1, float z1, float x2, float y2, float z2, int texId, float hrep, float vrep, bool closed)
{

}

void d3d_draw_cylinder(float x1, float y1, float z1, float x2, float y2, float z2, int texId, float hrep, float vrep, bool closed, int steps)
{

}

void d3d_draw_cone(float x1, float y1, float z1, float x2, float y2, float z2, int texId, float hrep, float vrep, bool closed, int steps)
{

}

void d3d_draw_ellipsoid(float x1, float y1, float z1, float x2, float y2, float z2, int texId, float hrep, float vrep, int steps)
{

}

void d3d_draw_icosahedron(int texId) {
    texture_use(get_texture(texId));
}

void d3d_draw_torus(float x1, float y1, float z1, int texId, float hrep, float vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI) {

}

// ***** TRANSFORMATIONS BEGIN *****
void d3d_transform_set_identity()
{

}

void d3d_transform_add_translation(float xt, float yt, float zt)
{

}
void d3d_transform_add_scaling(float xs, float ys, float zs)
{

}
void d3d_transform_add_rotation_x(double angle)
{

}
void d3d_transform_add_rotation_y(double angle)
{

}
void d3d_transform_add_rotation_z(double angle)
{

}
void d3d_transform_add_rotation_axis(float x, float y, float z, double angle)
{

}

void d3d_transform_set_translation(float xt, float yt, float zt)
{

}
void d3d_transform_set_scaling(float xs, float ys, float zs)
{

}
void d3d_transform_set_rotation_x(double angle)
{

}
void d3d_transform_set_rotation_y(double angle)
{

}
void d3d_transform_set_rotation_z(double angle)
{

}
void d3d_transform_set_rotation_axis(float x, float y, float z, double angle)
{

}

}

#include <stack>
stack<bool> trans_stack;
int trans_stack_size = 0;

namespace enigma_user
{

bool d3d_transform_stack_push()
{

}

bool d3d_transform_stack_pop()
{

}

void d3d_transform_stack_clear()
{

}

bool d3d_transform_stack_empty()
{
    return (trans_stack_size == 0);
}

bool d3d_transform_stack_top()
{

}

bool d3d_transform_stack_disgard()
{
    if (trans_stack_size == 0) return false;
    trans_stack.push(0);
    trans_stack_size--;
    return true;
}

}

// ***** TRANSFORMATIONS END *****

// ***** LIGHTS BEGIN *****
#include <map>
#include <list>
#include "Universal_System/fileio.h"

struct posi { // Homogenous point.
    float x;
    float y;
    float z;
    float w;
    posi(float x1, float y1, float z1, float w1) : x(x1), y(y1), z(z1), w(w1){}
};

class d3d_lights
{
    map<int,int> light_ind;
    map<int,posi> ind_pos; // Internal index to position.

    public:
    d3d_lights() {}
    ~d3d_lights() {}

    void light_update_positions()
    {

    }

    bool light_define_direction(int id, float dx, float dy, float dz, int col)
    {

    }

    bool light_define_point(int id, float x, float y, double z, double range, int col)
    {

    }

    bool light_define_specularity(int id, int r, int g, int b, double a) 
    {

    }

    bool light_enable(int id)
    {

    }

    bool light_disable(int id)
    {

    }
} d3d_lighting;

namespace enigma_user
{

bool d3d_light_define_direction(int id, float dx, float dy, float dz, int col)
{
    return d3d_lighting.light_define_direction(id, dx, dy, dz, col);
}

bool d3d_light_define_point(int id, float x, float y, double z, double range, int col)
{
    return d3d_lighting.light_define_point(id, x, y, z, range, col);
}

bool d3d_light_define_specularity(int id, int r, int g, int b, double a) 
{
    return d3d_lighting.light_define_specularity(id, r, g, b, a);
}

void d3d_light_specularity(int facemode, int r, int g, int b, double a)
{
  float specular[4] = {r, g, b, a};

}

void d3d_light_shininess(int facemode, int shine)
{

}

void d3d_light_define_ambient(int col)
{
    const float color[4] = {__GETR(col), __GETG(col), __GETB(col), 1};

}

bool d3d_light_enable(int id, bool enable)
{
    return enable?d3d_lighting.light_enable(id):d3d_lighting.light_disable(id);
}

}

namespace enigma {
    void d3d_light_update_positions()
    {
        d3d_lighting.light_update_positions();
    }
}

// ***** LIGHTS END *****
