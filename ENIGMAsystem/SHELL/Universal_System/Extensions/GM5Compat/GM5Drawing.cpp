//
// Copyright (C) 2014 Seth N. Hetu
//
// This file is a part of the ENIGMA Development Environment.
//
// ENIGMA is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, version 3 of the license or any later version.
//
// This application and its source code is distributed AS-IS, WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along
// with this code. If not, see <http://www.gnu.org/licenses/>
//

#include "GM5Drawing.h"

#include <cmath>
#include <vector>
#include <list>
#include <algorithm>
#include <utility>
#include <cstdio>

#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSprimitives.h"

using std::vector;
using std::swap;

namespace enigma {

///Simple container class for a Vertex in a Polygon.
///A color of -1 means "the color of the previous vertex", and defaults to the current draw_color.
struct PolyVertex {
  PolyVertex(gs_scalar x, gs_scalar y, int color) : x(x),y(y),color(color) {}
  gs_scalar x;
  gs_scalar y;
  int color;
};

//List of vertices we are buffering to draw.
std::list<PolyVertex> currComplexPoly;

} // namespace enigma

namespace enigma_user {

//GM5 draw state.
gs_scalar pen_size = 1;
int brush_style = enigma_user::bs_solid;
int pen_color = enigma_user::c_black;
enigma::BindPropRW brush_color(&draw_get_color, &draw_set_color);


void draw_rectangle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2)
{
  //lwid/hwid are the width and half-width, respectively. shft is used to nudge odd-width penned shapes to the right.
  int lwid = std::max(1,(int)round(pen_size));
  int hwid = lwid/2;
  int shft = (lwid%2==0)?0:1;

  //Ensure (x1,y1) are the smallest components, and (x2,y2) are the largest.
  if (x2<x1) { swap(x1,x2); }
  if (y2<y1) { swap(y1,y2); }

  //Nudge the shape into position.
  x1 = int(x1) + shft;
  x2 = int(x2) + shft - 1 ;
  y1 = int(y1);
  y2 = int(y2) - 1 ;

  //Fill the shape, if we have the correct brush style.
  //Note: At the moment, we treat all unsupported brush styles as "solid"; only "hollow" avoids drawing.
  if (brush_style != bs_hollow) {
    draw_rectangle(x1,y1,x2,y2,false);
  }

  //Draw the line (this always happens; a pen_size of 0 still draws a 1px line).
  //Note: So many off-by-one errors worries me; will have to confirm on Windows that this
  //      is not a bug in the OpenGL code.
  int old_color = draw_get_color();
  draw_set_color(pen_color);
  draw_line_width(x1,           y1-hwid, x1,      y2+hwid, lwid);
  draw_line_width(x1-hwid-shft, y2,      x2+hwid, y2,      lwid);
  draw_line_width(x2,           y2+hwid, x2,      y1-hwid, lwid);
  draw_line_width(x2+hwid,      y1,      x1-hwid, y1,      lwid);
  draw_set_color(old_color);
}


void draw_triangle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3)
{
  const int lwid = std::max(1,(int)round(pen_size));

  //Fill the shape, if we have the correct brush style.
  //Note: At the moment, we treat all unsupported brush styles as "solid"; only "hollow" avoids drawing.
  if (brush_style != bs_hollow) {
    draw_triangle(x1,y1,x2,y2,x3,y3,false);
  }

  //Draw the outline.
  int old_color = draw_get_color();
  draw_set_color(pen_color);
  draw_line_width(x1,y1, x2,y2, lwid);
  draw_line_width(x2,y2, x3,y3, lwid);
  draw_line_width(x3,y3, x1,y1, lwid);

  //We fake triangle joints with circles.
  //TODO: Triangle coords are still off slightly, not just on the joints.
  draw_circle(x1,y1,lwid/2.0, false);
  draw_circle(x2,y2,lwid/2.0, false);
  draw_circle(x3,y3,lwid/2.0, false);
  draw_set_color(old_color);
}


void draw_ellipse(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2)
{
  const int lwid = std::max(1,(int)round(pen_size));

  //Ensure (x1,y1) are the smallest components, and (x2,y2) are the largest.
  if (x2<x1) { swap(x1,x2); }
  if (y2<y1) { swap(y1,y2); }

  //Nudge the shape into position.
  gs_scalar shft = (lwid%2 == 0) ? 1 : 0.5;
  x1 += shft;
  x2 -= shft;
  y1 += shft;
  y2 -= shft;

  //Fill the shape, if we have the correct brush style.
  //Note: At the moment, we treat all unsupported brush styles as "solid"; only "hollow" avoids drawing.
  if (brush_style != bs_hollow) {
    draw_ellipse(x1,y1,x2,y2,false);
  }

  //This is borrowed from General drawing code.
  int old_color = draw_get_color();
  draw_set_color(pen_color);
  gs_scalar x = (x1+x2)/2,y=(y1+y2)/2;
  gs_scalar hr = fabs(x2-x),vr=fabs(y2-y);
  gs_scalar pr = 2*M_PI/draw_get_circle_precision();
  for(gs_scalar i=pr;i<M_PI;i+=pr) {
    gs_scalar xc1 = cos(i)*hr;
    gs_scalar yc1 = sin(i)*vr;
    i += pr;
    gs_scalar xc2=cos(i)*hr;
    gs_scalar yc2=sin(i)*vr;
    draw_line_width(x+xc1,y+yc1  ,  x+xc2,y+yc2  , lwid);
    draw_line_width(x-xc1,y+yc1  ,  x-xc2,y+yc2  , lwid);
    draw_line_width(x+xc1,y-yc1  ,  x+xc2,y-yc2  , lwid);
    draw_line_width(x-xc1,y-yc1  ,  x-xc2,y-yc2  , lwid);
  }

  //These are needed to prevent the top half-width of the line from overlapping for wide lines.
  //This is clearly massively inefficient; currently it rotates the entire shape through each half-width
  //  in an attempt to cover blank spots.
  for(gs_scalar i=pr;i<M_PI;i+=pr) {
    gs_scalar xc1 = cos(i+pr/2)*hr;
    gs_scalar yc1 = sin(i+pr/2)*vr;
    i += pr;
    gs_scalar xc2=cos(i+pr/2)*hr;
    gs_scalar yc2=sin(i+pr/2)*vr;
    draw_line_width(x+xc1,y+yc1  ,  x+xc2,y+yc2  , lwid);
    draw_line_width(x-xc1,y+yc1  ,  x-xc2,y+yc2  , lwid);
    draw_line_width(x+xc1,y-yc1  ,  x+xc2,y-yc2  , lwid);
    draw_line_width(x-xc1,y-yc1  ,  x-xc2,y-yc2  , lwid);
  }
  for(gs_scalar i=pr;i<M_PI;i+=pr) {
    gs_scalar xc1 = cos(i-pr/2)*hr;
    gs_scalar yc1 = sin(i-pr/2)*vr;
    i += pr;
    gs_scalar xc2=cos(i-pr/2)*hr;
    gs_scalar yc2=sin(i-pr/2)*vr;
    draw_line_width(x+xc1,y+yc1  ,  x+xc2,y+yc2  , lwid);
    draw_line_width(x-xc1,y+yc1  ,  x-xc2,y+yc2  , lwid);
    draw_line_width(x+xc1,y-yc1  ,  x+xc2,y-yc2  , lwid);
    draw_line_width(x-xc1,y-yc1  ,  x-xc2,y-yc2  , lwid);
  }
  draw_set_color(old_color);
}

void draw_circle(gs_scalar x, gs_scalar y, float radius)
{
  const int lwid = std::max(1,(int)round(pen_size));

  //Shuffle the radius a bit.
  if (lwid%2 == 0) {
    radius -= 1;
  }

  //Fill the shape, if we have the correct brush style.
  //Note: At the moment, we treat all unsupported brush styles as "solid"; only "hollow" avoids drawing.
  if (brush_style != bs_hollow) {
    draw_circle(x,y,radius,false);
  }

  //This is borrowed from General drawing code.
  int old_color = draw_get_color();
  draw_set_color(pen_color);
  double pr = 2 * M_PI / draw_get_circle_precision();
  double oldX = 0.0;
  double oldY = 0.0;
  for (double i = 0; i <= 2*M_PI; i += pr) {
    double xc1=cos(i)*radius;
    double yc1=sin(i)*radius;
    double newX = x+xc1;
    double newY = y+yc1;
    if (i>0) {
      draw_line_width(oldX,oldY, newX, newY, lwid);
    }
    oldX = newX;
    oldY = newY;
  }

  //Similar to the ellipse drawing code, this overlays patches to the circle's shape.
  //It is also inefficient, but necessary for large pen widths.
  oldX = oldY = 0.0;
  for (double i = 0; i <= 2*M_PI; i += pr) {
    double xc1=cos(i+pr/2)*radius;
    double yc1=sin(i+pr/2)*radius;
    double newX = x+xc1;
    double newY = y+yc1;
    if (i>0) {
      draw_line_width(oldX,oldY, newX, newY, lwid);
    }
    oldX = newX;
    oldY = newY;
  }
  draw_set_color(old_color);
}

void draw_polygon_begin()
{
  enigma::currComplexPoly.clear();
}

void draw_polygon_vertex(gs_scalar x, gs_scalar y, int color)
{
  //-1 means "the color of the previous vertex.
  //The default color (first vertex) is the current draw color.
  //This conforms to GM5's treatment of draw colors.
  enigma::currComplexPoly.push_back(enigma::PolyVertex(x, y, color));
}

void draw_polygon_end(bool outline, bool allowHoles)
{
  std::list<enigma::PolyVertex>& currPoly = enigma::currComplexPoly;
  if (outline) {
    if (currPoly.size() >= 2) {
      int color = draw_get_color();
      gs_scalar alpha = draw_get_alpha();

      //Close it, ensure the correct color.
      currPoly.push_back(currPoly.front());
      if (currPoly.back().color==-1) { currPoly.back().color = color; }

      //Draw it.
      draw_primitive_begin(pr_linestrip);
      for (const auto& v : currPoly) {
        color = (v.color!=-1 ? v.color : color);
        draw_vertex_color(v.x, v.y, color, alpha);
      }

      //Close it.
      draw_primitive_end();
    }
  } else {
    if (currPoly.size() >= 3) {
      //Self-intersecting polygons makes this much harder than "outline" mode
      //Use a triangle fan as a backup for now. This will work for concave polygons only.

      int color = draw_get_color();
      gs_scalar alpha = draw_get_alpha();

      //Draw it.
      draw_primitive_begin(pr_trianglefan);
      for (const auto& v : currPoly) {
        color = (v.color!=-1 ? v.color : color);
        draw_vertex_color(v.x, v.y, color, alpha);
      }

      //Close it.
      draw_primitive_end();
    }
  }

  currPoly.clear();
}

void draw_polygon_end()
{
  const int lwid = std::max(1,(int)round(pen_size));

  //We need a copy of currPoly since draw_polygon_end() will clear it.
  std::list<enigma::PolyVertex> cachedPoly = enigma::currComplexPoly;

  //Fill the shape, if we have the correct brush style.
  //Note: At the moment, we treat all unsupported brush styles as "solid"; only "hollow" avoids drawing.
  //TODO: Test if GM5 actually respects brush_style for polygons.
  if (brush_style != bs_hollow) {
    draw_polygon_end(false); //Dispatch to GSstdraw
  } else {
    enigma::currComplexPoly.clear(); //Just clear it; we're only drawing a line.
  }

  //Draw the line around it.
  int old_color = draw_get_color();
  draw_set_color(pen_color);
  enigma::PolyVertex lastPt = *(--cachedPoly.end());
  for (const auto& v : cachedPoly) {
    draw_line_width(lastPt.x,lastPt.y, v.x,v.y , lwid);
    lastPt = v;
  }
  draw_set_color(old_color);
}

} // namespace enigma_user
