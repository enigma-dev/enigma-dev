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

#include "DirectX10Headers.h"
#include "DX10d3d.h"
#include "DX10vertexbuffer.h"
#include "DX10textures.h"
#include "DX10mesh.h"
#include "DX10shapes.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>
#include "binding.h"

using namespace std;

#define __GETR(x) ((x & 0x0000FF))/255.0
#define __GETG(x) ((x & 0x00FF00)>>8)/255.0
#define __GETB(x) ((x & 0xFF0000)>>16)/255.0

bool d3dMode = false;
bool d3dHidden = false;
bool d3dZWriteEnable = true;
double projection_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}, transformation_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

enum renderstates
{
	
};

void d3d_start()
{

}

void d3d_end()
{

}

bool d3d_get_mode()
{

}

void d3d_set_hidden(bool enable)
{

}  

void d3d_set_zwriteenable(bool enable)
{

}

void d3d_set_lighting(bool enable)
{

}

void d3d_set_fog(bool enable, int color, double start, double end)
{

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

}//TODO: Write function

void d3d_set_shading(bool smooth)
{

}

//extern enum ptypes_by_id;
namespace enigma {
  extern unsigned char currentcolor[4];
}

void d3d_primitive_begin(int kind)
{

}
void d3d_primitive_begin_texture(int kind, int texId)
{

}

void d3d_primitive_end()
{
}

void d3d_vertex(double x, double y, double z)
{

}
void d3d_vertex_color(double x, double y, double z, int color, double alpha)
{

}
void d3d_vertex_texture(double x, double y, double z, double tx, double ty)
{

}
void d3d_vertex_texture_color(double x, double y, double z, double tx, double ty, int color, double alpha)
{

}

void d3d_vertex_normal(double x, double y, double z, double nx, double ny, double nz)
{

}
void d3d_vertex_normal_color(double x, double y, double z, double nx, double ny, double nz, int color, double alpha)
{

}
void d3d_vertex_normal_texture(double x, double y, double z, double nx, double ny, double nz, double tx, double ty)
{

}
void d3d_vertex_normal_texture_color(double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int color, double alpha)
{

}

void d3d_set_projection(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup)
{

}

void d3d_set_projection_ext(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup,double angle,double aspect,double znear,double zfar)
{

}

void d3d_set_projection_ortho(double x, double y, double width, double height, double angle)
{

}

void d3d_set_projection_perspective(double x, double y, double width, double height, double angle)
{

}

void d3d_draw_floor(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep)
{

}

void d3d_draw_block(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, bool closed)
{

}

void d3d_draw_cylinder(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, bool closed, int steps)
{

}

void d3d_draw_cone(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, bool closed, int steps)
{

}

void d3d_draw_ellipsoid(double x1, double y1, double z1, double x2, double y2, double z2, int texId, double hrep, double vrep, int steps)
{

}

void d3d_draw_icosahedron(int texId) {

}

void d3d_draw_torus(double x1, double y1, double z1, int texId, int hrep, int vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI) {

}

// ***** TRANSFORMATIONS BEGIN *****
void d3d_transform_set_identity()
{

}

void d3d_transform_add_translation(double xt,double yt,double zt)
{

}
void d3d_transform_add_scaling(double xs,double ys,double zs)
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
void d3d_transform_add_rotation_axis(double x, double y, double z, double angle)
{

}

void d3d_transform_set_translation(double xt,double yt,double zt)
{

}
void d3d_transform_set_scaling(double xs,double ys,double zs)
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
void d3d_transform_set_rotation_axis(double x, double y, double z, double angle)
{

}

#include <stack>
stack<bool> trans_stack;
int trans_stack_size = 0;

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

}

bool d3d_transform_stack_top()
{

}

bool d3d_transform_stack_disgard()
{

}
// ***** TRANSFORMATIONS END *****

// ***** LIGHTS BEGIN *****
#include <map>
#include <list>
#include "Universal_System/fileio.h"

struct posi { // Homogenous point.
    double x;
    double y;
    double z;
    double w;
    posi(double x1, double y1, double z1, double w1) : x(x1), y(y1), z(z1), w(w1){}
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

    bool light_define_direction(int id, double dx, double dy, double dz, int col)
    {

    }

    bool light_define_point(int id, double x, double y, double z, double range, int col)
    {

    }

    void light_define_specularity(int id, int r, int g, int b, double a) 
    {

    }

    bool light_enable(int id)
    {

    }

    bool light_disable(int id)
    {

    }
} d3d_lighting;

bool d3d_light_define_direction(int id, double dx, double dy, double dz, int col)
{

}

bool d3d_light_define_point(int id, double x, double y, double z, double range, int col)
{

}

void d3d_light_define_specularity(int id, int r, int g, int b, double a) 
{

}

void d3d_light_specularity(int facemode, int r, int g, int b, double a)
{

}

void d3d_light_shininess(int facemode, int shine) 
{

}

void d3d_light_define_ambient(int col)
{

}

bool d3d_light_enable(int id, bool enable)
{

}

namespace enigma {
    void d3d_light_update_positions()
    {
        d3d_lighting.light_update_positions();
    }
}

// ***** LIGHTS END *****
