/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Harijs Grînbergs                                         **
**  Modified 2013 by Josh Ventura                                               **
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
#include <algorithm>
#include <floatcomp.h>
#include <cmath>

#include "Graphics_Systems/General/GScurves.h" // for draw_path
#include "Graphics_Systems/General/GSprimitives.h"
#include "Universal_System/math_consts.h"

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
#endif


namespace enigma {
	extern size_t path_idmax;
	extern unsigned bound_texture;
}

#include "Universal_System/Object_Tiers/collisions_object.h"
#include "Universal_System/Instances/instance_system.h"
#include "implement.h"

namespace enigma {
  namespace extension_cast {
    extension_path *as_extension_path(object_basic*);
  }
}

namespace enigma_user
{

void path_start(unsigned pathid, cs_scalar speed, unsigned endaction, bool absolute)
{
    enigma::object_collisions* const inst = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    enigma::extension_path* const inst_paths = enigma::extension_cast::as_extension_path(enigma::instance_event_iterator->inst);
    inst_paths->path_index = pathid;
    inst_paths->path_speed = speed;
    inst_paths->path_endaction = endaction;

    cs_scalar sx, sy;
    path_getXY_scaled(enigma::pathstructarray[inst_paths->path_index], sx, sy, 0, inst_paths->path_scale);

    inst_paths->path_position = 0;

    if (absolute) {
      inst_paths->path_xstart = 0;
      inst_paths->path_ystart = 0;
      inst->x = sx;
      inst->y = sy;
    } else {
      inst_paths->path_xstart = inst->x - sx;
      inst_paths->path_ystart = inst->y - sy;
    }
}

void path_end()
{
    enigma::extension_path* const inst_paths = enigma::extension_cast::as_extension_path(enigma::instance_event_iterator->inst);
    inst_paths->path_index = -1;
}

void path_set_position(cs_scalar position, bool relative)
{
    enigma::object_collisions* const inst = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    enigma::extension_path* const inst_paths = enigma::extension_cast::as_extension_path(enigma::instance_event_iterator->inst);
    inst_paths->path_position = position;
    if (relative)
    {
        inst->x = path_get_x(inst_paths->path_index , inst_paths->path_position);
        inst->x = path_get_y(inst_paths->path_index , inst_paths->path_position);
    }
}

void path_set_speed(cs_scalar speed, bool relative)
{
    enigma::object_collisions* const inst = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    enigma::extension_path* const inst_paths = enigma::extension_cast::as_extension_path(enigma::instance_event_iterator->inst);
    inst_paths->path_speed = relative ? speed : double(inst->speed + speed);
}

bool path_update()
{
  enigma::object_planar*  const inst = (enigma::object_planar*)enigma::instance_event_iterator->inst;
  enigma::extension_path* const inst_paths = enigma::extension_cast::as_extension_path(inst);

  if (size_t(inst_paths->path_index) >= enigma::path_idmax || fzero(inst_paths->path_speed))
      return false;

  enigma::path *path = enigma::pathstructarray[inst_paths->path_index];
  if (!path)
    return false;

  bool at_end = false;
  cs_scalar pstep = inst_paths->path_speed / path->total_length;
  if (!inst_paths->path_orientation) {
    inst_paths->path_positionprevious = inst_paths->path_position;
    inst_paths->path_position += pstep;
    if ((at_end = inst_paths->path_position >= 1)) {
      inst_paths->path_position = 1;
    }
  } else {
    inst_paths->path_positionprevious = inst_paths->path_position;
    inst_paths->path_position -= pstep;
    if ((at_end = inst_paths->path_position <= 0)) {
      inst_paths->path_position = 0;
    }
  }

  cs_scalar ax, ay;
  path_getXY_scaled(path, ax, ay, inst_paths->path_position, inst_paths->path_scale);
  inst->x = inst_paths->path_xstart + ax;
  inst->y = inst_paths->path_ystart + ay;

  if (at_end) {
    //Give the user some time to intervene
    inst_paths->myevent_pathend();

    switch (inst_paths->path_endaction) {
      case 0: // Stop
          inst_paths->path_index = -1;
        return false;
      case 1: // Restart
          inst_paths->path_position = 0;
        break;
      case 2: { // Continue
          cs_scalar sx, sy;
          path_getXY_scaled(path, sx, sy, 0, inst_paths->path_scale);
          inst_paths->path_xstart = inst->x - sx;
          inst_paths->path_ystart = inst->y - sy;
          inst_paths->path_position = 0;
        break;
      }
      case 3: // Reverse
          inst_paths->path_orientation ^= true;
        break;
    }
  }

  return true;
}

bool path_exists(unsigned pathid)
{
    return (pathid < enigma::path_idmax && enigma::pathstructarray[pathid]);
}

void path_delete(unsigned pathid)
{
    #ifdef DEBUG_MODE
    if (!path_exists(pathid)){
        DEBUG_MESSAGE("Attempting to delete invalid path "+toString(pathid), MESSAGE_TYPE::M_ERROR);
        return;
    }
    #endif
    delete enigma::pathstructarray[pathid];
}

int path_add()
{
    enigma::pathstructarray_reallocate();
    return (new enigma::path(enigma::path_idmax, false, false, 8, 0))->id;
}

int path_duplicate(unsigned pathid)
{
    enigma::pathstructarray_reallocate();
    enigma::pathstructarray[enigma::path_idmax] = new enigma::path(*enigma::pathstructarray[pathid]);
    return enigma::path_idmax++;
}

void path_copy(unsigned pathid, unsigned srcid)
{
    enigma::pathstructarray[pathid]->pointarray.clear();
    enigma::path *pa = enigma::pathstructarray[pathid];
    enigma::path *pa2 = enigma::pathstructarray[srcid];
    for (size_t i=0; i<pa2->pointarray.size(); i++) {
        enigma::path_point point(pa2->pointarray[i].x,pa2->pointarray[i].y,pa2->pointarray[i].speed);
        pa->pointarray.push_back(point);
    }
    enigma::path_recalculate(pathid);
}

void path_assign(unsigned pathid, unsigned path)
{
    delete enigma::pathstructarray[pathid];
    enigma::pathstructarray[pathid] = new enigma::path(*enigma::pathstructarray[path]);
}

void path_append(unsigned pathid, unsigned path)
{
    enigma::path *pa = enigma::pathstructarray[pathid];
    enigma::path *pa2 = enigma::pathstructarray[path];
    for (size_t i=0; i<pa2->pointarray.size(); i++) {
        enigma::path_point point(pa2->pointarray[i].x,pa2->pointarray[i].y,pa2->pointarray[i].speed);
        pa->pointarray.push_back(point);
    }
    enigma::path_recalculate(pathid);
}

void path_shift(unsigned pathid, cs_scalar xshift, cs_scalar yshift)
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

void path_scale(unsigned pathid, cs_scalar xscale, cs_scalar yscale)
{
    enigma::path *pa = enigma::pathstructarray[pathid];
    for (size_t i=0; i<pa->pointarray.size(); i++){
        pa->pointarray[i].x = pa->centerx-(pa->centerx-pa->pointarray[i].x)*xscale;
        pa->pointarray[i].y = pa->centery-(pa->centery-pa->pointarray[i].y)*yscale;
    }
    enigma::path_recalculate(pathid);
}

void path_rotate(unsigned pathid, double angle)
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

cs_scalar path_get_x(unsigned pathid, double t)
{
    cs_scalar x,y;
    path_getXY(enigma::pathstructarray[pathid], x, y, t);
    return x;
}

cs_scalar path_get_y(unsigned pathid, double t)
{
    cs_scalar x,y;
    path_getXY(enigma::pathstructarray[pathid], x, y, t);
    return y;
}

cs_scalar path_get_speed(unsigned pathid, double t)
{
    cs_scalar speed;
    path_getspeed(enigma::pathstructarray[pathid], speed, t);
    return speed*100;
}

double path_get_direction(unsigned pathid, double t)
{
    cs_scalar x1,y1,x2,y2,p1,p2,precision;
    precision = 0.0005;

    p1 = t - precision;
    if (p1 < 0)
        p1 = 1 - fmod(-p1, 1);
    else
        p1 = fmod(p1, 1);

    p2 = t + precision;
    if (p2 < 0)
        p2 = 1 - fmod(-p2, 1);
    else
        p2 = fmod(p2, 1);

    path_getXY(enigma::pathstructarray[pathid], x1, y1, p1);
    path_getXY(enigma::pathstructarray[pathid], x2, y2, p2);
    return fmod((atan2(y1-y2,x2-x1)*(180/M_PI))+360,360);
}

cs_scalar path_get_center_x(unsigned pathid)
{
    return enigma::pathstructarray[pathid]->centerx;
}

cs_scalar path_get_center_y(unsigned pathid)
{
    return enigma::pathstructarray[pathid]->centery;
}

int path_get_number(unsigned pathid)
{
    return enigma::pathstructarray[pathid]->pointarray.size();
}

cs_scalar path_get_point_x(unsigned pathid, unsigned n)
{
    return enigma::pathstructarray[pathid]->pointarray[n].x;
}

cs_scalar path_get_point_y(unsigned pathid, unsigned n)
{
    return enigma::pathstructarray[pathid]->pointarray[n].y;
}

cs_scalar path_get_point_length(unsigned pathid, unsigned n)
{
    return enigma::pathstructarray[pathid]->pointarray[n].length;
}

cs_scalar path_get_point_speed(unsigned pathid, unsigned n)
{
    return enigma::pathstructarray[pathid]->pointarray[n].speed*100;
}

int path_get_precision(unsigned pathid)
{
    return enigma::pathstructarray[pathid]->precision;
}

cs_scalar path_get_length(unsigned pathid)
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

void path_add_point(unsigned pathid, cs_scalar x, cs_scalar y, cs_scalar speed)
{
    enigma::path_point point(x,y,speed/100);
    enigma::pathstructarray[pathid]->pointarray.push_back(point);
    enigma::path_recalculate(pathid);
}

void path_insert_point(unsigned pathid, unsigned n, cs_scalar x, cs_scalar y, cs_scalar speed)
{
    #ifdef DEBUG_MODE
    if (n>enigma::pathstructarray[pathid]->pointarray.size()){
        DEBUG_MESSAGE("Attempting to insert point in invalid place " + toString(n) + " for path "+ toString(pathid), MESSAGE_TYPE::M_ERROR);
        return;
    }
    #endif
    enigma::path_point point(x,y,speed/100);
    enigma::pathstructarray[pathid]->pointarray.insert(enigma::pathstructarray[pathid]->pointarray.begin() + n,point);
    enigma::path_recalculate(pathid);
}

void path_change_point(unsigned pathid, unsigned n, cs_scalar x, cs_scalar y, cs_scalar speed)
{
    #ifdef DEBUG_MODE
    if (n>enigma::pathstructarray[pathid]->pointarray.size()){
        DEBUG_MESSAGE("Attempting to change invalid point " + toString(n) + " for path "+toString(pathid), MESSAGE_TYPE::M_ERROR);
        return;
    }
    #endif
    enigma::pathstructarray[pathid]->pointarray[n].x = x;
    enigma::pathstructarray[pathid]->pointarray[n].y = y;
    enigma::pathstructarray[pathid]->pointarray[n].speed = speed/100;
    enigma::path_recalculate(pathid);
}

void path_delete_point(unsigned pathid, unsigned n)
{
    #ifdef DEBUG_MODE
    if (n>enigma::pathstructarray[pathid]->pointarray.size()){
        DEBUG_MESSAGE("Attempting to delete invalid point " + toString(n) + " for path "+toString(pathid), MESSAGE_TYPE::M_ERROR);
        return;
    }
    #endif
    enigma::pathstructarray[pathid]->pointarray.erase(enigma::pathstructarray[pathid]->pointarray.begin()+n);
    enigma::path_recalculate(pathid);
}

void path_reverse(unsigned pathid)
{
    reverse(enigma::pathstructarray[pathid]->pointarray.begin(),enigma::pathstructarray[pathid]->pointarray.end());
    enigma::path_recalculate(pathid);
}

void draw_path(unsigned pathid, gs_scalar x, gs_scalar y, bool absolute)
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

        for (unsigned i=0; i<path->pointarray.size(); i++)
            draw_spline_vertex(x+path->pointarray[i].x,y+path->pointarray[i].y);

        if (!path->closed)
            draw_spline_vertex(x+path->pointarray.back().x,y+path->pointarray.back().y);
        else
            draw_spline_vertex(x+path->pointarray.front().x,y+path->pointarray.front().y);
        draw_bezier_quadratic_spline_end();
    }else{ //Draw using lines
        draw_primitive_begin(3);
        if (path->closed)
            draw_vertex(x+path->pointarray.back().x,y+path->pointarray.back().y);

        for (unsigned i=0; i<path->pointarray.size(); i++)
            draw_vertex(x+path->pointarray[i].x,y+path->pointarray[i].y);

        draw_primitive_end();
    }
}

}
