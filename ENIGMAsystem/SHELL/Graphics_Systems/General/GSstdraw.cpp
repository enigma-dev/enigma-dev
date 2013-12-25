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

#include <math.h>
#include "GSprimitives.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GScolors.h"

#include <stdio.h>
#include "Universal_System/roomsystem.h"

#include <vector>
using std::vector;

namespace enigma {
  float circleprecision=24;
}

namespace enigma_user
{

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
		draw_vertex_color(x1, y1, c1, alpha);
		draw_vertex_color(x2, y1, c2, alpha);
		draw_vertex_color(x1, y2, c3, alpha);
		draw_vertex_color(x2, y2, c4, alpha);
		draw_primitive_end();
    }
}

void draw_set_circle_precision(float pr) {
    enigma::circleprecision = pr<3 ? 3 : pr;
}
float draw_get_circle_precision() {
    return enigma::circleprecision;
}

void draw_circle(gs_scalar x, gs_scalar y, float rad, bool outline)
{
    double pr = 2 * M_PI / enigma::circleprecision;
    if (outline) {
		draw_primitive_begin(pr_linestrip);
        for (double i = 0; i <= 2*M_PI; i += pr) {
            double xc1=cos(i)*rad,yc1=sin(i)*rad;
			draw_vertex(x+xc1, y+yc1);
        }
		draw_primitive_end();
    } else {
		draw_primitive_begin(pr_trianglefan);
        for (double i = 0; i <= 2*M_PI; i += pr) {
            double xc1=cos(i)*rad,yc1=sin(i)*rad;
			draw_vertex(x+xc1, y+yc1);
        }
		draw_primitive_end();
    }
}

void draw_circle_color(gs_scalar x, gs_scalar y, float rad,int c1, int c2,bool outline)
{
    gs_scalar alpha = draw_get_alpha();
    if(outline) {
		draw_primitive_begin(pr_linestrip);
    } else {
        draw_primitive_begin(pr_trianglefan);
		draw_vertex_color(x, y, c1, alpha);
    }
	float pr=2*M_PI/enigma::circleprecision;
	for(float i=0;i<=2*M_PI;i+=pr)
		draw_vertex_color(x+rad*cos(i),y-rad*sin(i), c2, alpha);
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

void draw_ellipse(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, bool outline)
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
    for(float i = pr; i < M_PI; i += pr)
    {
      float xc1=cos(i)*hr,yc1=sin(i)*vr;
      i+=pr;
      float xc2=cos(i)*hr,yc2=sin(i)*vr;
	  draw_primitive_begin(pr_trianglestrip);
      draw_vertex(x-xc1,y+yc1);draw_vertex(x+xc1,y+yc1);draw_vertex(x-xc2,y+yc2);draw_vertex(x+xc2,y+yc2);
	  draw_primitive_end();
	  draw_primitive_begin(pr_trianglestrip);
      draw_vertex(x-xc1,y-yc1);draw_vertex(x+xc1,y-yc1);draw_vertex(x-xc2,y-yc2);draw_vertex(x+xc2,y-yc2);
	  draw_primitive_end();
    }
  }
}

void draw_ellipse_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, int c1, int c2, bool outline)
{
    float
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

    float i;
    for(i = pr; i < 2*M_PI; i += pr)
      draw_vertex_color(x+hr*cos(i),y+vr*sin(i),c2,alpha);
    draw_vertex_color(x+hr*cos(i),y+vr*sin(i),c2,alpha);
    draw_primitive_end();
}

void draw_ellipse_perfect(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, bool outline)
{
  float
    x=(x1+x2)/2,y=(y1+y2)/2,
    hr=fabs(x2-x),vr=fabs(y2-y);
  draw_primitive_begin(outline ? pr_pointlist : pr_linelist);
  for (float xc=0;xc<hr;xc++)
  {
    float yc=vr*cos((M_PI/2)/hr*xc);
    draw_vertex(x+xc,y+yc);
    draw_vertex(x+xc,y-yc);
    draw_vertex(x-xc,y+yc);
    draw_vertex(x-xc,y-yc);
  }
  draw_primitive_end();
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

void draw_roundrect(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float rad, bool outline)
{
	//TODO: Needs rewritten to use circle precision for the corners
}

void draw_roundrect_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float rad, int col1, int col2, bool outline)
{
	//TODO: Needs rewritten to use circle precision for the corners
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
    x2=x1;
    x1=x2;
  }
  if(y1>y2) {
    y2=y1;
    y1=y2;
  }
  if (x2-x1<border_width*2){border_width=(x2-x1)/2;}
  if (y2-y1<border_width*2){border_width=(y2-y1)/2;}

	draw_primitive_begin(pr_trianglestrip);
	draw_vertex(x1,y1);
	draw_vertex(x2,y1);
	draw_vertex(x1,y2);
	draw_vertex(x2,y2);
	draw_primitive_end();

	int color, alpha;
	alpha = 0.5;
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

