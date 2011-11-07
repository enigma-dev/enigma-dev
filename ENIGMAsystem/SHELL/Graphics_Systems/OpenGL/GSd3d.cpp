/** Copyright (C) 2008-2011 Josh Ventura
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

using namespace std;

#include "OpenGLHeaders.h"
#include "GSprmtvs.h"
#include "GSd3d.h"
#include <string>
#include "../../Universal_System/var4.h"
#include "../../Universal_System/roomsystem.h"
#include <math.h>

bool d3dMode = false;

int d3d_start()
{
    // Enable depth buffering
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

    // Reset projection and model matrices
	glMatrixMode(GL_PROJECTION);
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadIdentity();

	// Setup a projection
	glMatrixMode(GL_PROJECTION);
	gluPerspective(40, view_wview[view_current] / view_hview[view_current], 1, 10);

    // Start with disabled lighting
	glDisable(GL_LIGHTING);

    return 0;
}

int d3d_end()
{
    d3dMode = false;
    glDisable(GL_DEPTH_TEST);
    glOrtho(-1, room_width, -1, room_height, 0, 1);

    return 0;
}

int d3d_set_hidden(int enable)
{

    return 0;
}

int d3d_set_lighting(int enable)
{
    if (enable)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);

    return 0;
}

int d3d_set_fog(int enable, int color, int start, int end)
{
    if (enable)
    {
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, 1);
        glFogf(GL_FOG_DENSITY, 0.35f);
        glFogf(GL_FOG_START, start);
        glFogf(GL_FOG_END, end);
    } else
        glDisable(GL_FOG);

    return 0;
}

int d3d_set_culling(int enable)
{
    if (enable)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    return 0;
}

int d3d_set_perspective(int enable)
{
    if (enable)
    {
        glMatrixMode(GL_PROJECTION);
        gluPerspective(40, view_wview[view_current] / view_hview[view_current], 1, 100);
    }
    else
    {

    }

    return 0;
}

int d3d_primitive_begin(int kind)
{
    glBegin(kind);
    return 0;
}

int d3d_vertex(double x, double y, double z)
{
    glVertex3d(x,y,z);
    return 0;
}

int d3d_primitive_end()
{
    glEnd();
    return 0;
}

int d3d_set_projection(double xfrom,double yfrom,double zfrom,double xto,
                       double yto,double zto,double xup,double yup,double zup)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(xfrom, yfrom, zfrom, xto, yto, zto, xup, yup, zup);

    return 0;
}

int d3d_set_projection_ortho(int x, int y, int width, int height, int angle)
{
    //glOrtho(x,x + width,y,y + height,0,1);
    return 0;
}

int d3d_draw_wall(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep)
{
    glBegin(GL_QUADS);

    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x1, y2, z1);
    glVertex3f(x2, y2, z2);

    glEnd();

    return 0;
}

int d3d_draw_floor(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep)
{


    return 0;
}

void d3d_transform_set_identity()
{
    glLoadIdentity();
    glScalef(1,-1,1);
    glOrtho(-1,room_width,-1,room_height,0,1);
}

void d3d_transform_add_translation(float xt,float yt,float zt)
{
    glTranslatef(xt, yt, zt);
}

void d3d_transform_add_scaling(float xs,float ys,float zs)
{
    glScalef(xs, ys, zs);
}
void d3d_transform_add_rotation_x(float angle)
{
    glRotatef(-angle,1,0,0);
}

void d3d_transform_add_rotation_y(float angle)
{
    glRotatef(-angle,0,1,0);
}
void d3d_transform_add_rotation_z(float angle)
{
    glRotatef(-angle,0,0,1);
}

void d3d_transform_stack_push()
{
    glPushMatrix();
}

void d3d_transform_stack_pop()
{
    glPopMatrix();
}

void d3d_transform_set_translation(float xt,float yt,float zt)
{
    d3d_transform_set_identity();
    glTranslatef(xt, yt, zt);
}

void d3d_transform_set_scaling(float xs,float ys,float zs)
{
    d3d_transform_set_identity();
    glScalef(xs, ys, zs);
}
void d3d_transform_set_rotation_x(float angle)
{
    d3d_transform_set_identity();
    glRotatef(-angle,1,0,0);
}

void d3d_transform_set_rotation_y(float angle)
{
    d3d_transform_set_identity();
    glRotatef(-angle,0,1,0);
}
void d3d_transform_set_rotation_z(float angle)
{
    d3d_transform_set_identity();
    glRotatef(-angle,0,0,1);
}
