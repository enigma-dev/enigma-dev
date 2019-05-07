/** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2014 Seth N. Hetu
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

#include "GSprimitives.h"
#include "GSstdraw.h"
#include "GScolors.h"

#include "Universal_System/roomsystem.h"
#include "Universal_System/math_consts.h"

#include <list>
#include <math.h>
#include <stdio.h>

namespace {

// please keep drawStateDirty private to this translation unit!
bool drawStateDirty=false;

} // namespace anonymous

namespace enigma {

bool lineStippleEnable=false, msaaEnabled=true, alphaBlend=true, alphaTest=false;
unsigned short lineStipplePattern=0xFFFF;
unsigned char alphaTestRef=0;
float circleprecision=24, drawPointSize=1.0f, drawLineWidth=1.0f;
int drawFillMode=enigma_user::rs_solid, lineStippleScale=1;

// handler for when a generic rendering state has changed
void draw_set_state_dirty(bool dirty) { drawStateDirty = dirty; }
bool draw_get_state_dirty() { return drawStateDirty; }

} // namespace enigma

namespace enigma_user {

void draw_state_flush() {
  // track whether we are already flushing the state
  static bool flushing = false;

  // prevent state flush triggered by batch flush of another state flush
  if (flushing) return;

  // flush the batch even if state is not dirty because this function
  // means we are about to draw something anyway
  enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);

  // if the state isn't dirty, we don't have to do anything
  if (!drawStateDirty) return;

  flushing = true; // we are now flushing the state

  enigma::graphics_state_flush();
  drawStateDirty = false; // state is not dirty now

  flushing = false; // done flushing state
}

void draw_set_msaa_enabled(bool enable) {
  enigma::draw_set_state_dirty();
  enigma::msaaEnabled = enable;
}

void draw_enable_alphablend(bool enable) {
  enigma::draw_set_state_dirty();
  enigma::alphaBlend = enable;
}

void draw_set_alpha_test(bool enable) {
  enigma::draw_set_state_dirty();
  enigma::alphaTest = enable;
}

void draw_set_alpha_test_ref_value(unsigned val) {
  enigma::draw_set_state_dirty();
  enigma::alphaTestRef = val;
}

void draw_set_point_size(float value) {
  enigma::draw_set_state_dirty();
  enigma::drawPointSize = value;
}

void draw_set_fill_mode(int fill) {
  enigma::draw_set_state_dirty();
  enigma::drawFillMode = fill;
}

void draw_set_line_width(float value) {
  enigma::draw_set_state_dirty();
  enigma::drawLineWidth = value;
}

void draw_set_line_stipple(bool enable) {
  enigma::draw_set_state_dirty();
  enigma::lineStippleEnable = enable;
}

void draw_set_line_pattern(int scale, unsigned short pattern) {
  enigma::draw_set_state_dirty();
  enigma::lineStippleScale = scale;
  enigma::lineStipplePattern = pattern;
}

void draw_set_circle_precision(float pr) {
  enigma::draw_set_state_dirty();
  enigma::circleprecision = pr < 3 ? 3 : pr;
}

bool draw_get_alpha_test() { return enigma::alphaTest; }
unsigned draw_get_alpha_test_ref_value() { return enigma::alphaTestRef; }
float draw_get_circle_precision() { return enigma::circleprecision; }

void draw_point(gs_scalar x, gs_scalar y)
{
	draw_primitive_begin(pr_pointlist);
	draw_vertex(x, y);
	draw_primitive_end();
}

void draw_point_color(gs_scalar x, gs_scalar y, int col)
{
	draw_primitive_begin(pr_pointlist);
	draw_vertex_color(x, y, col, draw_get_alpha());
	draw_primitive_end();
}

void draw_line(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2)
{
	draw_primitive_begin(pr_linestrip);
	draw_vertex(x1, y1);
	draw_vertex(x2, y2);
	draw_primitive_end();
}

void draw_line_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, int c1, int c2)
{
  gs_scalar alpha = draw_get_alpha();
	draw_primitive_begin(pr_linestrip);
	draw_vertex_color(x1, y1, c1, alpha);
	draw_vertex_color(x2, y2, c2, alpha);
	draw_primitive_end();
}

void draw_line_width(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float width)
{
  double dir = fmod(atan2(y1-y2,x2-x1)+2*M_PI,2*M_PI);
  double cv = cos(dir-M_PI/2.0), sv = -sin(dir-M_PI/2.0);
  double dw = width/2.0;
  draw_primitive_begin(pr_trianglestrip);
	draw_vertex(x1+dw*cv, y1+dw*sv);
	draw_vertex(x1-dw*cv, y1-dw*sv);
  draw_vertex(x2+dw*cv, y2+dw*sv);
  draw_vertex(x2-dw*cv, y2-dw*sv);
	draw_primitive_end();
}

void draw_line_width_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float width, int c1, int c2)
{
  gs_scalar alpha = draw_get_alpha();
  double dir = fmod(atan2(y1-y2,x2-x1)+2*M_PI,2*M_PI);
  double cv = cos(dir-M_PI/2.0), sv = -sin(dir-M_PI/2.0);
  double dw = width/2.0;
  draw_primitive_begin(pr_trianglestrip);
  draw_vertex_color(x1+dw*cv, y1+dw*sv, c1, alpha);
  draw_vertex_color(x1-dw*cv, y1-dw*sv, c1, alpha);
  draw_vertex_color(x2+dw*cv, y2+dw*sv, c2, alpha);
  draw_vertex_color(x2-dw*cv, y2-dw*sv, c2, alpha);
  draw_primitive_end();
}

void draw_rectangle(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, bool outline)
{
  if (outline) {
    draw_primitive_begin(pr_linestrip);
    draw_vertex(x1, y1);
    draw_vertex(x2, y1);
    draw_vertex(x2, y2);
    draw_vertex(x1, y2);
    draw_vertex(x1, y1);
    draw_primitive_end();
  } else {
    draw_primitive_begin(pr_trianglestrip);
    draw_vertex(x1, y1);
    draw_vertex(x2, y1);
    draw_vertex(x1, y2);
    draw_vertex(x2, y2);
    draw_primitive_end();
  }
}

void draw_rectangle_angle(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float angle, bool outline)
{
  // TODO(JoshDreamland): Replace with settings macro to get from preferred unit to radians
  angle *= M_PI/180;

  float
    xm = (x2+x1)/2,
    ym = (y2+y1)/2;

  float
    len = hypot(xm-x1,ym-y1),
    dir = atan2(y1-ym,x1-xm)+angle;

  float
    ldx1 = len*cos(dir),
    ldy1 = len*sin(dir);

  dir = atan2(y2-ym,x1-xm)+angle;
  float
    ldx2 = len*cos(dir),
    ldy2 = len*sin(dir);

  if (outline) {
    draw_primitive_begin(pr_linestrip);
    draw_vertex(xm+ldx1, ym-ldy1);
    draw_vertex(xm+ldx2, ym-ldy2);
    draw_vertex(xm-ldx1, ym+ldy1);
    draw_vertex(xm-ldx2, ym+ldy2);
    draw_vertex(xm+ldx1, ym-ldy1);
    draw_primitive_end();
  } else {
    draw_primitive_begin(pr_trianglestrip);
    draw_vertex(xm+ldx2, ym-ldy2);
    draw_vertex(xm+ldx1, ym-ldy1);
    draw_vertex(xm-ldx1, ym+ldy1);
    draw_vertex(xm-ldx2, ym+ldy2);
    draw_primitive_end();
  }
}

void draw_rectangle_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, int c1, int c2, int c3, int c4, bool outline)
{
  gs_scalar alpha = draw_get_alpha();
  if (outline) {
    draw_primitive_begin(pr_linestrip);
    draw_vertex_color(x1, y1, c1, alpha);
    draw_vertex_color(x2, y1, c2, alpha);
    draw_vertex_color(x2, y2, c3, alpha);
    draw_vertex_color(x1, y2, c4, alpha);
    draw_vertex_color(x1, y1, c1, alpha);
    draw_primitive_end();
  } else {
    draw_primitive_begin(pr_trianglestrip);
    draw_vertex_color(x2, y1, c2, alpha);
    draw_vertex_color(x2, y2, c3, alpha);
    draw_vertex_color(x1, y1, c1, alpha);
    draw_vertex_color(x1, y2, c4, alpha);
    draw_primitive_end();
  }
}

void draw_circle(gs_scalar x, gs_scalar y, float rad, bool outline)
{
  gs_scalar pr = 2 * M_PI / enigma::circleprecision;
  if (outline) {
    draw_primitive_begin(pr_linestrip);
  }else{
    draw_primitive_begin(pr_trianglefan);
    draw_vertex(x, y);
  }
  for (gs_scalar i = 0; i <= 2*M_PI; i += pr) {
    gs_scalar xc1=cos(i)*rad,yc1=sin(i)*rad;
    draw_vertex(x+xc1, y+yc1);
  }
  draw_primitive_end();
}

void draw_circle_color(gs_scalar x, gs_scalar y, float rad,int c1, int c2,bool outline)
{
  gs_scalar alpha = draw_get_alpha();
  gs_scalar pr=2*M_PI/enigma::circleprecision;
  if (outline) {
	  draw_primitive_begin(pr_linestrip);
  } else {
    draw_primitive_begin(pr_trianglefan);
	  draw_vertex_color(x, y, c1, alpha);
  }
  for(gs_scalar i=0;i<=2*M_PI;i+=pr){
	  draw_vertex_color(x+rad*cos(i),y+rad*sin(i), c2, alpha);
  }
  draw_primitive_end();
}

void draw_circle_perfect(gs_scalar x, gs_scalar y, float rad, bool outline)
{
  const gs_scalar r2 = rad*rad, r12 = rad*M_SQRT1_2;
  draw_primitive_begin(outline ? pr_pointlist : pr_linelist);
  for(gs_scalar xc=0, yc=rad; xc <= r12; xc++)
  {
    if(xc*xc + yc*yc > r2) yc--;
    draw_vertex(x+xc, y+yc);
    draw_vertex(x+xc, y-yc);
    draw_vertex(x-xc, y+yc);
    draw_vertex(x-xc, y-yc);
    draw_vertex(x+xc, y+yc);
    draw_vertex(x+xc, y-yc);
    draw_vertex(x-xc, y+yc);
    draw_vertex(x-xc, y-yc);
  }
  draw_primitive_end();
}

void draw_circle_color_perfect(gs_scalar x, gs_scalar y, float rad, int c1, int c2, bool outline)
{
  gs_scalar r2=rad*rad;
  gs_scalar alpha = draw_get_alpha();
  if (outline)
  {
    draw_primitive_begin(pr_pointlist);
    gs_scalar r12=rad*M_SQRT1_2;
    for(gs_scalar xc=0,yc=rad;xc<=r12;xc++)
    {
      if(xc*xc+yc*yc>r2) yc--;
  	  draw_vertex_color(x+xc, y+yc, c2, alpha);
  	  draw_vertex_color(x+xc, y-yc, c2, alpha);
  	  draw_vertex_color(x-xc, y+yc, c2, alpha);
  	  draw_vertex_color(x-xc, y-yc, c2, alpha);
  	  draw_vertex_color(x+xc, y+yc, c2, alpha);
  	  draw_vertex_color(x+xc, y-yc, c2, alpha);
  	  draw_vertex_color(x-xc, y+yc, c2, alpha);
  	  draw_vertex_color(x-xc, y-yc, c2, alpha);
    }
  } else {
    draw_primitive_begin(pr_trianglefan);

    draw_vertex_color(x,y,c1,alpha);
    draw_vertex_color(x-rad,y,c2,alpha);
    for(gs_scalar xc=-rad+1;xc<rad;xc++)
  	   draw_vertex_color(x+xc,y+sqrt(r2-(xc*xc)),c2,alpha);
    for(gs_scalar xc=rad;xc>-rad;xc--)
       draw_vertex_color(x+xc,y-sqrt(r2-(xc*xc)),c2,alpha);
    draw_vertex_color(x-rad,y,c2,alpha);
  }
  draw_primitive_end();
}

void draw_ellipse(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, bool outline)
{
  gs_scalar
      x=(x1+x2)/2,y=(y1+y2)/2,
      hr=fabs(x2-x),vr=fabs(y2-y),
      pr=2*M_PI/enigma::circleprecision;
  if (outline)
  {
	  draw_primitive_begin(pr_linelist);
    for(gs_scalar i=pr;i<M_PI;i+=pr)
    {
      gs_scalar xc1 = cos(i)*hr, yc1 = sin(i)*vr;
      i += pr;
      gs_scalar xc2=cos(i)*hr,yc2=sin(i)*vr;
      draw_vertex(x+xc1,y+yc1);draw_vertex(x+xc2,y+yc2);
      draw_vertex(x-xc1,y+yc1);draw_vertex(x-xc2,y+yc2);
      draw_vertex(x+xc1,y-yc1);draw_vertex(x+xc2,y-yc2);
      draw_vertex(x-xc1,y-yc1);draw_vertex(x-xc2,y-yc2);
    }
	  draw_primitive_end();
  } else {
    for(gs_scalar i = pr; i < M_PI; i += pr)
    {
      gs_scalar xc1=cos(i)*hr,yc1=sin(i)*vr;
      i+=pr;
      gs_scalar xc2=cos(i)*hr,yc2=sin(i)*vr;
	    draw_primitive_begin(pr_trianglestrip);
      draw_vertex(x-xc1,y+yc1);draw_vertex(x+xc1,y+yc1);draw_vertex(x-xc2,y+yc2);draw_vertex(x+xc2,y+yc2);
	    draw_primitive_end();
	    draw_primitive_begin(pr_trianglestrip);
      draw_vertex(x-xc1,y-yc1);draw_vertex(x+xc1,y-yc1);draw_vertex(x-xc2,y-yc2);draw_vertex(x+xc2,y-yc2);
	    draw_primitive_end();
    }
  }
}

void draw_ellipse_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int c1, int c2, bool outline)
{
  gs_scalar
      x=(x1+x2)/2,y=(y1+y2)/2,
      hr=fabs(x2-x),vr=fabs(y2-y),
      pr=2*M_PI/enigma::circleprecision;

	gs_scalar alpha = draw_get_alpha();
  if (outline) {
	  draw_primitive_begin(pr_linestrip);
  } else {
		draw_primitive_begin(pr_trianglefan);
    draw_vertex_color(x,y,c1,alpha);
  }

  gs_scalar i;
  for(i = pr; i < 2*M_PI; i += pr)
    draw_vertex_color(x+hr*cos(i),y+vr*sin(i),c2,alpha);

  draw_vertex_color(x+hr*cos(i),y+vr*sin(i),c2,alpha);
  draw_primitive_end();
}

void draw_ellipse_perfect(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, bool outline)
{
  gs_scalar
    x=(x1+x2)/2,y=(y1+y2)/2,
    hr=fabs(x2-x),vr=fabs(y2-y);
  draw_primitive_begin(outline ? pr_pointlist : pr_linelist);
  for (gs_scalar xc=0;xc<hr;xc++)
  {
    gs_scalar yc=vr*cos((M_PI/2)/hr*xc);
    draw_vertex(x+xc,y+yc);
    draw_vertex(x+xc,y-yc);
    draw_vertex(x-xc,y+yc);
    draw_vertex(x-xc,y-yc);
  }
  draw_primitive_end();
}

void draw_sector(gs_scalar x, gs_scalar y, gs_scalar r, float a1, float a2, bool outline) {
  return draw_sector(x, y, r, r, a1, a2, outline);
}

void draw_sector(gs_scalar x, gs_scalar y, gs_scalar rx, gs_scalar ry, float a1, float a2, bool outline)
{
  // TODO(JoshDreamland): Replace with settings macro to get from preferred unit to radians
  a1 *= M_PI/180;
  a2 *= M_PI/180;

  gs_scalar pr = 2*M_PI/enigma::circleprecision;

  if (outline) {
    draw_primitive_begin(pr_linestrip);
    draw_vertex(x, y);
    for (gs_scalar a = a1; a < a2; a += pr) {
      draw_vertex(x + rx * cos(a), y - ry * sin(a));
    }
    draw_vertex(x + rx * cos(a2), y - ry * sin(a2));
    draw_vertex(x, y);
    draw_primitive_end();
  } else {
    draw_primitive_begin(pr_trianglefan);
    draw_vertex(x, y);
    for (gs_scalar a = a1; a < a2; a += pr) {
      draw_vertex(x + rx * cos(a), y - ry * sin(a));
    }
    draw_primitive_end();
  }
}

void draw_triangle(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, bool outline)
{
  if (outline) {
    draw_primitive_begin(pr_linestrip);
    draw_vertex(x1, y1);
    draw_vertex(x2, y2);
    draw_vertex(x3, y3);
    draw_vertex(x1, y1);
    draw_primitive_end();
  } else {
    draw_primitive_begin(pr_trianglestrip);
    draw_vertex(x1, y1);
    draw_vertex(x2, y2);
    draw_vertex(x3, y3);
    draw_primitive_end();
  }
}

void draw_triangle_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, int col1, int col2, int col3, bool outline)
{
    gs_scalar alpha = draw_get_alpha();
    if (outline) {
		draw_primitive_begin(pr_linestrip);
		draw_vertex_color(x1, y1, col1, alpha);
		draw_vertex_color(x2, y2, col2, alpha);
		draw_vertex_color(x3, y3, col3, alpha);
		draw_vertex_color(x1, y1, col1, alpha);
		draw_primitive_end();
    } else {
		draw_primitive_begin(pr_trianglestrip);
		draw_vertex_color(x1, y1, col1, alpha);
		draw_vertex_color(x2, y2, col2, alpha);
		draw_vertex_color(x3, y3, col3, alpha);
		draw_primitive_end();
    }
}

void draw_roundrect(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float rad, bool outline) {
	if (x1>x2) {
    gs_scalar t=x2;
    x2=x1;
    x1=t;
  }
  if (y1>y2) {
    gs_scalar t=y2;
    y2=y1;
    y1=t;
  }
  x1++, y1++; //This fixes an off-by-one error when drawing over a regular draw_rectangle
  gs_scalar pr = 2 * M_PI / enigma::circleprecision;
  if (outline) {
    draw_primitive_begin(pr_linestrip);
  }else{
    draw_primitive_begin(pr_trianglefan);
  }
  //Draw left 1/4 circle
  for (gs_scalar i = M_PI; i > M_PI_2; i -= pr) {
    gs_scalar xc1=cos(i)*rad,yc1=-sin(i)*rad;
    draw_vertex(x1+rad+xc1, y1+rad+yc1);
  }
  draw_vertex(x1+rad, y1);
  //Draw right 1/4 circle
  for (gs_scalar i = M_PI_2; i > 0; i -= pr) {
    gs_scalar xc1=cos(i)*rad,yc1=-sin(i)*rad;
    draw_vertex(x2-rad+xc1, y1+rad+yc1);
  }
  draw_vertex(x2, y1+rad);
  //Bottom right 1/4 circle
  for (gs_scalar i = 2 * M_PI; i > M_PI+M_PI_2; i -= pr) {
    gs_scalar xc1=cos(i)*rad,yc1=-sin(i)*rad;
    draw_vertex(x2-rad+xc1, y2-rad+yc1);
  }
  draw_vertex(x2-rad, y2);
  //Bottom left 1/4 circle
  for (gs_scalar i = M_PI+M_PI_2; i > M_PI; i -= pr) {
    gs_scalar xc1=cos(i)*rad,yc1=-sin(i)*rad;
    draw_vertex(x1+rad+xc1, y2-rad+yc1);
  }
  draw_vertex(x1, y2-rad);
  if (outline) {
    draw_vertex(x1, y1+rad);
  }
  draw_primitive_end();
}

void draw_roundrect_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float rad, int col1, int col2, bool outline) {
  if (x1>x2) {
    gs_scalar t=x2;
    x2=x1;
    x1=t;
  }
  if (y1>y2) {
    gs_scalar t=y2;
    y2=y1;
    y1=t;
  }
  x1++, y1++; //This fixes an off-by-one error when drawing over a regular draw_rectangle
  gs_scalar alpha = draw_get_alpha();
  gs_scalar pr = 2 * M_PI / enigma::circleprecision;
  if (outline) {
    draw_primitive_begin(pr_linestrip);
  }else{
    draw_primitive_begin(pr_trianglefan);
    draw_vertex_color(x1+(x2-x1)/2.0, y1+(y2-y1)/2.0, col1, alpha);
  }
  //Draw left 1/4 circle
  for (gs_scalar i = M_PI; i > M_PI_2; i -= pr) {
    gs_scalar xc1=cos(i)*rad,yc1=-sin(i)*rad;
    draw_vertex_color(x1+rad+xc1, y1+rad+yc1, col2, alpha);
  }
  draw_vertex_color(x1+rad, y1, col2, alpha);
  //Draw right 1/4 circle
  for (gs_scalar i = M_PI_2; i > 0; i -= pr) {
    gs_scalar xc1=cos(i)*rad,yc1=-sin(i)*rad;
    draw_vertex_color(x2-rad+xc1, y1+rad+yc1, col2, alpha);
  }
  draw_vertex_color(x2, y1+rad, col2, alpha);
  //Bottom right 1/4 circle
  for (gs_scalar i = 2 * M_PI; i > M_PI+M_PI_2; i -= pr) {
    gs_scalar xc1=cos(i)*rad,yc1=-sin(i)*rad;
    draw_vertex_color(x2-rad+xc1, y2-rad+yc1, col2, alpha);
  }
  draw_vertex_color(x2-rad, y2, col2, alpha);
  //Bottom left 1/4 circle
  for (gs_scalar i = M_PI+M_PI_2; i > M_PI; i -= pr) {
    gs_scalar xc1=cos(i)*rad,yc1=-sin(i)*rad;
    draw_vertex_color(x1+rad+xc1, y2-rad+yc1, col2, alpha);
  }
  draw_vertex_color(x1, y2-rad, col2, alpha);
  draw_vertex_color(x1, y1+rad, col2, alpha);
  draw_primitive_end();
}

void draw_roundrect_ext(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float xrad, float yrad, bool outline) {
  if (x1>x2) {
    gs_scalar t=x2;
    x2=x1;
    x1=t;
  }
  if (y1>y2) {
    gs_scalar t=y2;
    y2=y1;
    y1=t;
  }
  x1++, y1++; //This fixes an off-by-one error when drawing over a regular draw_rectangle
  gs_scalar pr = 2 * M_PI / enigma::circleprecision;
  if (outline) {
    draw_primitive_begin(pr_linestrip);
  }else{
    draw_primitive_begin(pr_trianglefan);
  }
  //Draw left 1/4 circle
  for (gs_scalar i = M_PI; i > M_PI_2; i -= pr) {
    gs_scalar xc1=cos(i)*xrad,yc1=-sin(i)*yrad;
    draw_vertex(x1+xrad+xc1, y1+yrad+yc1);
  }
  draw_vertex(x1+xrad, y1);
  //Draw right 1/4 circle
  for (gs_scalar i = M_PI_2; i > 0; i -= pr) {
    gs_scalar xc1=cos(i)*xrad,yc1=-sin(i)*yrad;
    draw_vertex(x2-xrad+xc1, y1+yrad+yc1);
  }
  draw_vertex(x2, y1+yrad);
  //Bottom right 1/4 circle
  for (gs_scalar i = 2 * M_PI; i > M_PI+M_PI_2; i -= pr) {
    gs_scalar xc1=cos(i)*xrad,yc1=-sin(i)*yrad;
    draw_vertex(x2-xrad+xc1, y2-yrad+yc1);
  }
  draw_vertex(x2-xrad, y2);
  //Bottom left 1/4 circle
  for (gs_scalar i = M_PI+M_PI_2; i > M_PI; i -= pr) {
    gs_scalar xc1=cos(i)*xrad,yc1=-sin(i)*yrad;
    draw_vertex(x1+xrad+xc1, y2-yrad+yc1);
  }
  draw_vertex(x1, y2-yrad);
  if (outline) {
    draw_vertex(x1, y1+yrad);
  }
  draw_primitive_end();
}

void draw_roundrect_color_ext(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float xrad, float yrad, int col1, int col2, bool outline) {
  if (x1>x2) {
    gs_scalar t=x2;
    x2=x1;
    x1=t;
  }
  if (y1>y2) {
    gs_scalar t=y2;
    y2=y1;
    y1=t;
  }
  x1++, y1++; //This fixes an off-by-one error when drawing over a regular draw_rectangle
  gs_scalar alpha = draw_get_alpha();
  gs_scalar pr = 2 * M_PI / enigma::circleprecision;
  if (outline) {
    draw_primitive_begin(pr_linestrip);
  }else{
    draw_primitive_begin(pr_trianglefan);
    draw_vertex_color(x1+(x2-x1)/2.0, y1+(y2-y1)/2.0, col1, alpha);
  }
  //Draw left 1/4 circle
  for (gs_scalar i = M_PI; i > M_PI_2; i -= pr) {
    gs_scalar xc1=cos(i)*xrad,yc1=-sin(i)*yrad;
    draw_vertex_color(x1+xrad+xc1, y1+yrad+yc1, col2, alpha);
  }
  draw_vertex_color(x1+xrad, y1, col2, alpha);
  //Draw right 1/4 circle
  for (gs_scalar i = M_PI_2; i > 0; i -= pr) {
    gs_scalar xc1=cos(i)*xrad,yc1=-sin(i)*yrad;
    draw_vertex_color(x2-xrad+xc1, y1+yrad+yc1, col2, alpha);
  }
  draw_vertex_color(x2, y1+yrad, col2, alpha);
  //Bottom right 1/4 circle
  for (gs_scalar i = 2 * M_PI; i > M_PI+M_PI_2; i -= pr) {
    gs_scalar xc1=cos(i)*xrad,yc1=-sin(i)*yrad;
    draw_vertex_color(x2-xrad+xc1, y2-yrad+yc1, col2, alpha);
  }
  draw_vertex_color(x2-xrad, y2, col2, alpha);
  //Bottom left 1/4 circle
  for (gs_scalar i = M_PI+M_PI_2; i > M_PI; i -= pr) {
    gs_scalar xc1=cos(i)*xrad,yc1=-sin(i)*yrad;
    draw_vertex_color(x1+xrad+xc1, y2-yrad+yc1, col2, alpha);
  }
  draw_vertex_color(x1, y2-yrad, col2, alpha);
  draw_vertex_color(x1, y1+yrad, col2, alpha);
  draw_primitive_end();
}

void draw_roundrect_precise(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float rad, bool outline)
{
  if (x1>x2) {
    float t=x2;
    x2=x1;
    x1=t;
  }
  if (y1>y2) {
    float t=y2;
    y2=y1;
    y1=t;
  }
  if (x2-x1<rad*2){rad=(x2-x1)/2;}
  if (y2-y1<rad*2){rad=(y2-y1)/2;}
  if (rad<0){rad=0;}
  float r2=rad*rad,r12=rad*M_SQRT1_2,
      bx1=x1+rad,by1=y1+rad,
      bx2=x2-rad,by2=y2-rad;
  if (outline) {
	  draw_primitive_begin(pr_linelist);
    draw_vertex(x1,by1);draw_vertex(x1,by2);
    draw_vertex(x2,by1);draw_vertex(x2,by2);
    draw_vertex(bx1,y1);draw_vertex(bx2,y1);
    draw_vertex(bx1,y2);draw_vertex(bx2,y2);
    draw_primitive_end();
	  draw_primitive_begin(pr_pointlist);
    for(float xc=0,yc=rad;xc<=r12;xc++) {
        if (xc*xc+yc*yc>r2) yc--;
        draw_vertex(bx2+xc,by2+yc);
        draw_vertex(bx2+xc,by1-yc);
        draw_vertex(bx1-xc,by2+yc);
        draw_vertex(bx1-xc,by1-yc);
        draw_vertex(bx2+yc,by2+xc);
        draw_vertex(bx2+yc,by1-xc);
        draw_vertex(bx1-yc,by2+xc);
        draw_vertex(bx1-yc,by1-xc);
    }
    draw_primitive_end();
  } else {
	  draw_primitive_begin(pr_linelist);
    for(float xc=0,yc=rad;xc<=r12;xc++) {
      if (xc*xc+yc*yc>r2) yc--;
      draw_vertex(bx2+xc,by2+yc);
      draw_vertex(bx2+xc,by1-yc);
      draw_vertex(bx1-xc,by2+yc);
      draw_vertex(bx1-xc,by1-yc);
      draw_vertex(bx2+yc,by2+xc);
      draw_vertex(bx2+yc,by1-xc);
      draw_vertex(bx1-yc,by2+xc);
      draw_vertex(bx1-yc,by1-xc);
    }
    draw_primitive_end();
    draw_rectangle(bx1,y1,bx2,y2,false);
  }
}

void draw_roundrect_precise_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float rad, int col1, int col2, bool outline)
{
  if (x1>x2) {
    gs_scalar t=x2;
    x2=x1;
    x1=t;
  }
  if (y1>y2) {
    gs_scalar t=y2;
    y2=y1;
    y1=t;
  }
  if (x2-x1<rad*2){rad=(x2-x1)/2;}
  if (y2-y1<rad*2){rad=(y2-y1)/2;}
  if (rad<0){rad=0;}
  gs_scalar alpha = draw_get_alpha();
  gs_scalar r2=rad*rad,r12=rad*M_SQRT1_2,
        bx1=x1+rad,by1=y1+rad,
        bx2=x2-rad,by2=y2-rad;
  draw_primitive_begin(pr_linelist);
  if (outline)
  {
    draw_vertex_color(x1,by1,col2,alpha);draw_vertex_color(x1,by2,col2,alpha);
    draw_vertex_color(x2,by1,col2,alpha);draw_vertex_color(x2,by2,col2,alpha);
    draw_vertex_color(bx1,y1,col2,alpha);draw_vertex_color(bx2,y1,col2,alpha);
    draw_vertex_color(bx1,y2,col2,alpha);draw_vertex_color(bx2,y2,col2,alpha);
    draw_primitive_end();
	  draw_primitive_begin(pr_pointlist);
    for (gs_scalar xc=0,yc=rad;xc<=r12;xc++)
    {
      if (xc*xc+yc*yc>r2) yc--;
      draw_vertex_color(bx2+xc,by2+yc,col2,alpha);
      draw_vertex_color(bx2+xc,by1-yc,col2,alpha);
      draw_vertex_color(bx1-xc,by2+yc,col2,alpha);
      draw_vertex_color(bx1-xc,by1-yc,col2,alpha);
      draw_vertex_color(bx2+yc,by2+xc,col2,alpha);
      draw_vertex_color(bx2+yc,by1-xc,col2,alpha);
      draw_vertex_color(bx1-yc,by2+xc,col2,alpha);
      draw_vertex_color(bx1-yc,by1-xc,col2,alpha);
    }
    draw_primitive_end();
  } else {
    for (gs_scalar xc=0,yc=rad;xc<=r12;xc++) {
      if (xc*xc+yc*yc>r2) yc--;
      draw_vertex_color(bx2+xc,by2+yc,col2,alpha);
      draw_vertex_color(bx2+xc,by1-yc,col2,alpha);
      draw_vertex_color(bx1-xc,by2+yc,col2,alpha);
      draw_vertex_color(bx1-xc,by1-yc,col2,alpha);
      draw_vertex_color(bx2+yc,by2+xc,col2,alpha);
      draw_vertex_color(bx2+yc,by1-xc,col2,alpha);
      draw_vertex_color(bx1-yc,by2+xc,col2,alpha);
      draw_vertex_color(bx1-yc,by1-xc,col2,alpha);
    }
    draw_primitive_end();
	  draw_primitive_begin(pr_trianglefan);
    draw_vertex_color(x1+(x2-x1)/2,y1+(y2-y1)/2,col1,alpha);
    draw_vertex_color(x1,by1,col2,alpha);
    draw_vertex_color(bx1,y1,col2,alpha);
    draw_vertex_color(bx2,y1,col2,alpha);
    draw_vertex_color(x2,by1,col2,alpha);
    draw_vertex_color(x2,by2,col2,alpha);
    draw_vertex_color(bx2,y2,col2,alpha);
    draw_vertex_color(bx1,y2,col2,alpha);
    draw_vertex_color(x1,by2,col2,alpha);
    draw_vertex_color(x1,by1,col2,alpha);
    draw_primitive_end();
  }
}

void draw_arrow(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, gs_scalar arrow_size, gs_scalar line_size, bool outline)
{
	double dir = atan2(y2-y1,x2-x1);
	gs_scalar tc = cos(dir), ts = sin(dir),
	xs = x2-tc*arrow_size, ys = y2-ts*arrow_size,
	lw = ts*(line_size/2), lh = tc*(line_size/2);
	double at = atan2(ys-y1,xs-x1);
	if (fabs((dir<0?dir+2*M_PI:dir)-(at<0?at+2*M_PI:at)) < 0.01){
		draw_primitive_begin(outline?pr_linestrip:pr_trianglestrip);
		draw_vertex(x1+lw,y1-lh);
		draw_vertex(x1-lw,y1+lh);
		if (!outline) { draw_vertex(xs+lw,ys-lh); }
    draw_vertex(xs-lw,ys+lh);
		if (outline) { draw_vertex(xs+lw,ys-lh); draw_vertex(x1+lw,y1-lh); }
		draw_primitive_end();
	}

	draw_primitive_begin(outline?pr_linestrip:pr_trianglestrip);
	draw_vertex(x2,y2);
	draw_vertex(xs-ts*(arrow_size/3),ys+tc*(arrow_size/3));
	draw_vertex(xs+ts*(arrow_size/3),ys-tc*(arrow_size/3));
	if (outline) { draw_vertex(x2,y2); }
	draw_primitive_end();
}

void draw_button(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, gs_scalar border_width, bool up)
{
  if(x1>x2) {
    gs_scalar t=x2;
    x2=x1;
    x1=t;
  }
  if(y1>y2) {
    gs_scalar t=y2;
    y2=y1;
    y1=t;
  }
  if (x2-x1<border_width*2){border_width=(x2-x1)/2;}
  if (y2-y1<border_width*2){border_width=(y2-y1)/2;}

	draw_primitive_begin(pr_trianglestrip);
	draw_vertex(x1,y1);
	draw_vertex(x2,y1);
	draw_vertex(x1,y2);
	draw_vertex(x2,y2);
	draw_primitive_end();

	int color;
	float alpha = 0.5;
  if (up == true){ color = make_color_rgb(127,127,127); } else { color = make_color_rgb(255,255,255); }

	draw_primitive_begin(pr_trianglestrip);
	draw_vertex_color(x1+border_width,y2-border_width,color,alpha);
	draw_vertex_color(x2-border_width,y1-border_width,color,alpha);
	draw_vertex_color(x1,y2,color,alpha);
	draw_vertex_color(x2,y2,color,alpha);
	draw_primitive_end();

	draw_primitive_begin(pr_trianglestrip);
	draw_vertex_color(x2-border_width,y1+border_width,color,alpha);
	draw_vertex_color(x2,y1,color,alpha);
	draw_vertex_color(x1-border_width,y1-border_width,color,alpha);
	draw_vertex_color(x2,y2,color,alpha);
	draw_primitive_end();

	if (up == true){ color = make_color_rgb(255,255,255); } else { color = make_color_rgb(127,127,127); }
	draw_primitive_begin(pr_trianglestrip);
	draw_vertex_color(x1,y1,color,alpha);
	draw_vertex_color(x2,y1,color,alpha);
	draw_vertex_color(x1+border_width,y1+border_width,color,alpha);
	draw_vertex_color(x2-border_width,y2+border_width,color,alpha);
	draw_primitive_end();


	draw_primitive_begin(pr_trianglestrip);
	draw_vertex_color(x1,y1,color,alpha);
	draw_vertex_color(x1+border_width,y1+border_width,color,alpha);
	draw_vertex_color(x1,y2,color,alpha);
	draw_vertex_color(x1+border_width,y2-border_width,color,alpha);
	draw_primitive_end();
}

void draw_button(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, bool up){
  draw_button(x1,y1,x2,y2,2,up);
}

//Mind that health is 1-100
void draw_healthbar(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float amount, int backcol, int mincol, int maxcol, int dir, bool showback, bool showborder)
{
  if (x1>x2) { // Swap them
    gs_scalar t = x2;
    x2 = x1, x1 = t;
  }
  if (y1>y2) { // Swap them
    gs_scalar t = y2;
    y2 = y1, y1 = t;
  }
    amount = amount>=100 ? 1 : (amount<=0 ? 0 : amount/100);
  gs_scalar alpha = draw_get_alpha();

  if (showback) {
    draw_primitive_begin(pr_trianglestrip);
    draw_vertex_color(x1,y1,backcol,alpha);
    draw_vertex_color(x2,y1,backcol,alpha);
    draw_vertex_color(x1,y2,backcol,alpha);
    draw_vertex_color(x2,y2,backcol,alpha);
    draw_primitive_end();

    if (showborder) {
      draw_primitive_begin(pr_linestrip);
      draw_vertex_color(x1,y1,0,alpha);
      draw_vertex_color(x2,y1,0,alpha);
      draw_vertex_color(x2,y2,0,alpha);
      draw_vertex_color(x1,y2,0,alpha);
      draw_vertex_color(x1,y1,0,alpha);
      draw_primitive_end();
    }
  }

  switch (dir) {
    case 1:x1=x2-(x2-x1)*amount;break;
    case 2:y2=y1+(y2-y1)*amount;break;
    case 3:y1=y2-(y2-y1)*amount;break;
    default:x2=x1+(x2-x1)*amount;
  }

  int col = merge_color(mincol, maxcol, amount);
  draw_primitive_begin(pr_trianglestrip);
  draw_vertex_color(x1,y1,col,alpha);
  draw_vertex_color(x2,y1,col,alpha);
  draw_vertex_color(x1,y2,col,alpha);
  draw_vertex_color(x2,y2,col,alpha);
  draw_primitive_end();

  if (showborder) {
    draw_primitive_begin(pr_linestrip);
    draw_vertex_color(x1,y1,0,alpha);
    draw_vertex_color(x2,y1,0,alpha);
    draw_vertex_color(x2,y2,0,alpha);
    draw_vertex_color(x1,y2,0,alpha);
    draw_vertex_color(x1,y1,0,alpha);
    draw_primitive_end();
  }
}

int draw_mandelbrot(int x,int y,float w,double Zx,double Zy,double Zw,unsigned iter)
{
  int c=0;
  draw_primitive_begin(pr_pointlist);
    for(int i=y; i<y+w; i++)
      for(int j=x;j<x+w;j++) {
        double zx=Zx+(j-x)*(Zw/w),zy=Zy+(i-y)*(Zw/w),cx=zx,cy=zy;
        for(unsigned k=0;k<iter;k++)
          if(zx*zx+zy*zy>=4) goto UNMANLY;
          else {
            zx = zx*zx - zy*zy + cx;
            zy = 2*zx*zy + cy;
          }
		draw_vertex(i,j);
        c++;
        UNMANLY:;
      }
    draw_primitive_end();
    return c;
}

}
