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

#include <stack>
#include <vector>
#include <math.h>

#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GScurves.h"
#include "DX9binding.h"

#define __GETR(x) (((x & 0x0000FF))/255.0)
#define __GETG(x) (((x & 0x00FF00)>>8)/255.0)
#define __GETB(x) (((x & 0xFF0000)>>16)/255.0)


namespace enigma{
    extern unsigned char currentcolor[4];
}

int pr_curve_detail = 20;
int pr_spline_points = 0;
int pr_curve_width = 1;

struct splinePoint {
    float x,y,al;
    int col;
};
typedef std::vector< splinePoint > spline;
static std::stack< spline, std::vector<spline*> > startedSplines;
static std::stack< int > startedSplinesMode;

namespace enigma_user
{

void draw_set_curve_width(int width)
{
    pr_curve_width=width;
}

void draw_set_curve_mode(int mode)
{

}

void draw_set_curve_detail(int detail)
{
    pr_curve_detail = detail;
}

void draw_bezier_quadratic(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3)
{

}

void draw_bezier_quadratic_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, int c1, int c2, float al1, float al2)
{

}

void draw_bezier_cubic(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4)
{

}

void draw_bezier_cubic_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, float al1, float al2)
{

}

//NOTICE:
//Some of the arguments are not used, but I left them in so it would be easier to use these functions together with drawing functions
float draw_bezier_quadratic_x(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, float t)
{
    float a = 1.0 - t, b = t;
    return x1*a*a + x2*2*a*b + x3*b*b;
}

float draw_bezier_quadratic_y(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, float t)
{
    float a = 1.0 - t, b = t;
    return y1*a*a + y2*2*a*b + y3*b*b;
}

float draw_bezier_cubic_x(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, float t)
{
    float a = 1.0 - t, b = t, a2, b2;
    a2 = a*a; b2 = b*b;
    return x1*a2*a + x2*3*a2*b + x3*3*a*b2 + x4*b2*b;
}

float draw_bezier_cubic_y(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, float t)
{
    float a = 1.0 - t, b = t, a2, b2;
    a2 = a*a; b2 = b*b;
    return y1*a2*a + y2*3*a2*b + y3*3*a*b2 + y4*b2*b;
}

//The following function is used in other drawing functions for all splines
//it takes 4 points. Two control points which are at the beggining and the end, and the two points which it actually draws through
//in the middle
void draw_spline_part(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4)
{

}

void draw_spline_part_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, float a1, float a2)
{

}

void draw_spline2c(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4)
{

}

void draw_spline2c_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, float a1, float a2)
{

}

void draw_spline3(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3)
{

}

void draw_spline3_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, int c1, int c2, float a1, float a2)
{

}


void draw_spline3c(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5)
{

}

void draw_spline3c_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5, int c1, int c2, float a1, float a2)
{

}

void draw_spline4(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4)
{

}

void draw_spline4_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, float a1, float a2)
{

}

void draw_spline4c(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5, gs_scalar x6, gs_scalar y6)
{

}

void draw_spline4c_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5, gs_scalar x6, gs_scalar y6, int c1, int c2, float a1, float a2)
{

}

void draw_spline_begin(int mode)
{
    startedSplinesMode.push(mode);
    startedSplines.push(new spline);
}

int draw_spline_vertex(float x, float y)
{
    splinePoint point={x,y,enigma::currentcolor[3],enigma::currentcolor[0] + (enigma::currentcolor[1] << 8) + (enigma::currentcolor[2] << 16)};
    startedSplines.top()->push_back(point);
    return startedSplines.top()->size()-2;
}

int draw_spline_vertex_color(gs_scalar x, gs_scalar y, int col, float alpha)
{
    splinePoint point={x,y,alpha,col};
    startedSplines.top()->push_back(point);
    return startedSplines.top()->size()-2;
}


void draw_spline_end()
{

}

void draw_bezier_quadratic_spline_part(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, int c1, int c2, float a1, float a2)
{

}

//first and last point is used as control points, so they will not be drawn
void draw_bezier_quadratic_spline_end()
{

}

int draw_spline_optimized_end()
{

}

}

