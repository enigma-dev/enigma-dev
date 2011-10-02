/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Harijs Grînbergs                                         **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/
#include "pathstruct.h"
#include "path_functions.h"
//#include "../Graphics_Systems/OpenGL/GScurves.h" //This should probably be changed later on, because this is clearly GL only and when d3d is added then this will break
//#include "../Graphics_Systems/OpenGL/OpenGLHeaders.h" //For drawing straight lines
//#include "instance_system.h" //for path_start()
#include <algorithm>
#include <cmath>

namespace enigma {
	extern size_t path_idmax;
	extern unsigned bound_texture;
}

////////THIS NEEDS TO BE FIXED/FINISHED
bool path_start(unsigned pathid,double speed,unsigned endaction,bool absolute)
{
    //(pseudocode)
    /*enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    enigma::path_inst pathi={inst,pathid,endaction,0,speed,absolute};*/

    //Then in update it would be something like
    /*double speed;
    path_getspeed(pathi->path_index, speed, pathi->path_position);
    path_getXY(pathi->path_index,pathi->inst.x,pathi->inst.y,pathi->path_position);
    pathi->inst.direction = path_get_direction(pathi->path_index,pathi->path_position);
    pathi->path_position += pathi->path_speed/pathi->path_index.total_length*speed;*/
}
///////////////////////////////////////

bool path_exists(unsigned pathid)
{
    return (pathid < enigma::path_idmax && enigma::pathstructarray[pathid]);
}

void path_delete(unsigned pathid)
{
    delete enigma::pathstructarray[pathid];
}

int path_add()
{
    enigma::pathstructarray_reallocate();
    new enigma::path(enigma::path_idmax, false, false, 8, 0);
    return enigma::path_idmax-1;
}

int path_duplicate(unsigned pathid)
{
    enigma::pathstructarray_reallocate();
    enigma::pathstructarray[enigma::path_idmax] = new enigma::path(*enigma::pathstructarray[pathid]);
    return enigma::path_idmax++;
}

void path_copy(unsigned pathid,unsigned srcid)
{
    enigma::pathstructarray[pathid]->pointarray.clear();
    enigma::path *pa = enigma::pathstructarray[pathid];
    enigma::path *pa2 = enigma::pathstructarray[srcid];
    for (size_t i=0; i<pa2->pointarray.size(); i++){
        enigma::path_point point={pa2->pointarray[i].x,pa2->pointarray[i].y,pa2->pointarray[i].speed};
        pa->pointarray.push_back(point);
    }
    enigma::path_recalculate(pathid);
}

void path_assign(unsigned pathid,unsigned path)
{
    delete enigma::pathstructarray[pathid];
    enigma::pathstructarray[pathid] = new enigma::path(*enigma::pathstructarray[path]);
}

void path_append(unsigned pathid,unsigned path)
{
    enigma::path *pa = enigma::pathstructarray[pathid];
    enigma::path *pa2 = enigma::pathstructarray[path];
    for (size_t i=0; i<pa2->pointarray.size(); i++){
        enigma::path_point point={pa2->pointarray[i].x,pa2->pointarray[i].y,pa2->pointarray[i].speed};
        pa->pointarray.push_back(point);
    }
    enigma::path_recalculate(pathid);
}

void path_shift(unsigned pathid,double xshift,double yshift)
{
    enigma::path *pa = enigma::pathstructarray[pathid];
    for (vector<enigma::path_point>::iterator it = pa->pointarray.begin(); it!=pa->pointarray.end(); ++it)
        (*it).x = (*it).x + xshift, (*it).y = (*it).y + yshift;
}

void path_flip(unsigned pathid)
{
    enigma::path *pa = enigma::pathstructarray[pathid];
    for (size_t i=0; i<pa->pointarray.size(); i++){
        pa->pointarray[i].y = pa->centery*2-pa->pointarray[i].y;
    }
}

void path_mirror(unsigned pathid)
{
    enigma::path *pa = enigma::pathstructarray[pathid];
    for (size_t i=0; i<pa->pointarray.size(); i++){
        pa->pointarray[i].x = pa->centerx*2-pa->pointarray[i].x;
    }
}

void path_scale(unsigned pathid,double xscale,double yscale)
{
    enigma::path *pa = enigma::pathstructarray[pathid];
    for (size_t i=0; i<pa->pointarray.size(); i++){
        pa->pointarray[i].x = pa->centerx-(pa->centerx-pa->pointarray[i].x)*xscale;
        pa->pointarray[i].y = pa->centery-(pa->centery-pa->pointarray[i].y)*yscale;
    }
    enigma::path_recalculate(pathid);
}

void path_rotate(unsigned pathid,double angle)
{
    enigma::path *pa = enigma::pathstructarray[pathid];
    double tmpx, tmpy, a = (M_PI / 180) * -angle;
    for (size_t i=0; i<pa->pointarray.size(); i++){
        tmpx = pa->pointarray[i].x-pa->centerx;
        tmpy = pa->pointarray[i].y-pa->centery;
        pa->pointarray[i].x = tmpx*cos(a) - tmpy*sin(a) + pa->centerx;
        pa->pointarray[i].y = tmpx*sin(a) + tmpy*cos(a) + pa->centery;
    }
}

double path_get_x(unsigned pathid, double t)
{
    double x,y;
    path_getXY(enigma::pathstructarray[pathid], x, y, t);
    return x;
}

double path_get_y(unsigned pathid, double t)
{
    double x,y;
    path_getXY(enigma::pathstructarray[pathid], x, y, t);
    return y;
}

double path_get_speed(unsigned pathid, double t)
{
    double speed;
    path_getspeed(enigma::pathstructarray[pathid], speed, t);
    return speed*100;
}

double path_get_direction(unsigned pathid, double t) //this is clearly via ass kind of thing, but something like this is needed at the end anyway
{
    double x1,y1,x2,y2;
    path_getXY(enigma::pathstructarray[pathid], x1, y1, t);
    path_getXY(enigma::pathstructarray[pathid], x2, y2, fmin(1,fmax(0,t+0.005f)));
    if (t<1)
        return fmod((atan2(y1-y2,x2-x1)*(180/M_PI))+360,360);
    else
        return fmod((atan2(y2-y1,x1-x2)*(180/M_PI))+360,360);
}

double path_get_center_x(unsigned pathid)
{
    return enigma::pathstructarray[pathid]->centerx;
}

double path_get_center_y(unsigned pathid)
{
    return enigma::pathstructarray[pathid]->centery;
}

int path_get_number(unsigned pathid)
{
    return enigma::pathstructarray[pathid]->pointarray.size();
}

double path_get_point_x(unsigned pathid, unsigned n)
{
    return enigma::pathstructarray[pathid]->pointarray[n].x;
}

double path_get_point_y(unsigned pathid, unsigned n)
{
    return enigma::pathstructarray[pathid]->pointarray[n].y;
}

double path_get_point_length(unsigned pathid, unsigned n)
{
    return enigma::pathstructarray[pathid]->pointarray[n].length;
}

double path_get_point_speed(unsigned pathid, unsigned n)
{
    return enigma::pathstructarray[pathid]->pointarray[n].speed*100;
}

int path_get_precision(unsigned pathid)
{
    return enigma::pathstructarray[pathid]->precision;
}

double path_get_length(unsigned pathid)
{
    return enigma::pathstructarray[pathid]->total_length;
}

bool path_get_kind(unsigned pathid)
{
    return enigma::pathstructarray[pathid]->smooth;
}

bool path_get_closed(unsigned pathid)
{
    return enigma::pathstructarray[pathid]->closed;
}

void path_set_kind(unsigned pathid, bool val)
{
    if (enigma::pathstructarray[pathid]->smooth != val){
        enigma::pathstructarray[pathid]->smooth = val;
        enigma::path_recalculate(pathid);
    }
}

void path_set_closed(unsigned pathid, bool val)
{
    if (enigma::pathstructarray[pathid]->closed != val){
        enigma::pathstructarray[pathid]->closed = val;
        enigma::path_recalculate(pathid);
    }
}

void path_set_precision(unsigned pathid, int prec)
{
    if (enigma::pathstructarray[pathid]->precision != prec){
        enigma::pathstructarray[pathid]->precision = prec;
    }
}

void path_clear_points(unsigned pathid)
{
    enigma::pathstructarray[pathid]->pointarray.clear();
    enigma::path_recalculate(pathid);
}

void path_add_point(unsigned pathid, double x, double y, double speed)
{
    enigma::path_point point={x,y,speed/100};
    enigma::pathstructarray[pathid]->pointarray.push_back(point);
    enigma::path_recalculate(pathid);
}

void path_insert_point(unsigned pathid,unsigned n,double x,double y,double speed)
{
    enigma::path_point point={x,y,speed/100};
    enigma::pathstructarray[pathid]->pointarray.insert(enigma::pathstructarray[pathid]->pointarray.begin() + n,point);
    enigma::path_recalculate(pathid);
}

void path_change_point(unsigned pathid,unsigned n,double x,double y,double speed)
{
    enigma::pathstructarray[pathid]->pointarray[n].x = x;
    enigma::pathstructarray[pathid]->pointarray[n].y = y;
    enigma::pathstructarray[pathid]->pointarray[n].speed = speed/100;
    enigma::path_recalculate(pathid);
}

void path_reverse(unsigned pathid)
{
    reverse(enigma::pathstructarray[pathid]->pointarray.begin(),enigma::pathstructarray[pathid]->pointarray.end());
    enigma::path_recalculate(pathid);
}

//Declare drawing functions here, so it works no matter if GL, GLES or D3D is used
void draw_spline_begin(int mode);
int draw_spline_vertex(float x, float y);
void draw_bezier_quadratic_spline_end();
/*void glBegin(GLenum mode);
void glEnd(void);
void glVertex2f(GLfloat  x,  GLfloat  y);
void glPushAttrib(GLbitfield mask);
void glPopAttrib(void);*/
int draw_primitive_begin(int kind);
int draw_vertex(double x, double y);
int draw_primitive_end();

void draw_path(unsigned pathid,double x,double y,bool absolute)
{
    enigma::path *path = enigma::pathstructarray[pathid];
    if (!path->pointarray.size()){return;}
    if (absolute)
        x=0, y=0;
    else
        x=x-path->pointarray[0].x, y=y-path->pointarray[0].y;
    if (path->smooth) //Draw using quadratic bezier splines
    {
        draw_spline_begin(3);
        if (!path->closed)
            draw_spline_vertex(x+path->pointarray.front().x,y+path->pointarray.front().y);
        else
            draw_spline_vertex(x+path->pointarray.back().x,y+path->pointarray.back().y);

        for (int i=0; i<path->pointarray.size(); i++)
            draw_spline_vertex(x+path->pointarray[i].x,y+path->pointarray[i].y);

        if (!path->closed)
            draw_spline_vertex(x+path->pointarray.back().x,y+path->pointarray.back().y);
        else
            draw_spline_vertex(x+path->pointarray.front().x,y+path->pointarray.front().y);
        draw_bezier_quadratic_spline_end();
    }else{ //Draw using lines
        //if(enigma::bound_texture) glBindTexture(GL_TEXTURE_2D,enigma::bound_texture = 0);
        //glPushAttrib(GL_LINE_BIT);
        //glBegin(GL_LINE_STRIP);
        draw_primitive_begin(3);
        if (path->closed)
            //glVertex2f(x+path->pointarray.back().x,y+path->pointarray.back().y);
            draw_vertex(x+path->pointarray.back().x,y+path->pointarray.back().y);

        for (int i=0; i<path->pointarray.size(); i++)
            //glVertex2f(x+path->pointarray[i].x,y+path->pointarray[i].y);
            draw_vertex(x+path->pointarray[i].x,y+path->pointarray[i].y);

        //glEnd();
        //glPopAttrib();
        draw_primitive_end();
    }
}
