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
#include "../General/DirectXHeaders.h"
#include "DX10draw.h"
#include "../General/DXbinding.h"
#include <stdio.h>
#include "Universal_System/roomsystem.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma {
  float circleprecision=24;
  extern unsigned char currentcolor[4];
}

namespace enigma_user
{

int draw_get_msaa_maxlevel()
{

}

bool draw_get_msaa_supported()
{

}

void draw_set_msaa_enabled(bool enable)
{

}

void draw_set_line_pattern(unsigned short pattern, int scale)
{

}

void draw_point(float x,float y)
{

}

void draw_point_color(float x,float y,int col)
{

}

void draw_line(float x1,float y1,float x2,float y2)
{

}

void draw_line_width(float x1,float y1,float x2,float y2,float width)
{

}

void draw_line_color(float x1,float y1,float x2,float y2,int c1,int c2)
{

}

void draw_line_width_color(float x1,float y1,float x2,float y2,float width,int c1,int c2)
{

}

void draw_rectangle(float x1,float y1,float x2,float y2,bool outline)
{

}

void draw_rectangle_angle(float x1,float y1,float x2,float y2,float angle,bool outline)
{

}

void draw_rectangle_color(float x1,float y1,float x2,float y2,int c1,int c2,int c3,int c4,bool outline)
{

}

void draw_set_circle_precision(float pr) {
    enigma::circleprecision = pr<3 ? 3 : pr;
}
float draw_get_circle_precision() {
    return enigma::circleprecision;
}

void draw_circle(float x,float y,float r,bool outline)
{

}

void draw_circle_color(float x,float y,float r,int c1, int c2,bool outline)
{

}

void draw_circle_perfect(float x,float y,float r,bool outline)
{

}

void draw_circle_color_perfect(float x,float y,float r, int c1, int c2, bool outline)
{

}

void draw_ellipse(float x1,float y1,float x2,float y2,bool outline)
{

}

void draw_ellipse_color(float x1,float y1,float x2,float y2,int c1, int c2,bool outline)
{

}

void draw_ellipse_perfect(float x1,float y1,float x2,float y2,bool outline)
{

}

void draw_triangle(float x1,float y1,float x2,float y2,float x3,float y3,bool outline)
{

}

void draw_triangle_color(float x1,float y1,float x2,float y2,float x3,float y3,int col1,int col2,int col3,bool outline)
{

}

void draw_roundrect(float x1,float y1,float x2,float y2,float r, bool outline)
{

}

void draw_roundrect_color(float x1, float y1, float x2, float y2, float r, int col1, int col2, bool outline)
{

}

void draw_arrow(float x1,float y1,float x2,float y2, float arrow_size, float line_size, bool outline)
{

}

void draw_button(float x1,float y1,float x2,float y2,float border_width,bool up)
{

}

//Mind that health is 1-100
void draw_healthbar(float x1,float y1,float x2,float y2,float amount,int backcol,int mincol,int maxcol,int dir,bool showback,bool showborder)
{

}

}

//#include <endian.h>
//TODO: Though serprex, the author of the function below, never included endian.h,
//   // Doing so is necessary for the function to work at its peak.
//   // When ENIGMA generates configuration files, one should be included here.

namespace enigma_user
{

int draw_getpixel(int x,int y)
{

}

int draw_mandelbrot(int x,int y,float w,double Zx,double Zy,double Zw,unsigned iter)
{

}

}

