/** Copyright (C) 2010-2013 Harijs Grinbergs, Robert B. Colton
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
#include "GScolors.h"
#include "GScurves.h"
#include "GSprimitives.h"

#include <stack>
#include <vector>
#include <algorithm> // min/max
#include <math.h>

int pr_curve_detail = 20;
int pr_curve_mode = enigma_user::pr_linestrip;
int pr_spline_points = 0;
gs_scalar pr_curve_width = 1;

struct splinePoint {
    gs_scalar x,y,al;
    int col;
};
typedef std::vector< splinePoint > spline;
static std::stack< spline*, std::vector<spline*> > startedSplines;
static std::stack< int > startedSplinesMode;

namespace enigma_user
{

void draw_set_curve_width(gs_scalar width)
{
    pr_curve_width = (width<1.0?1.0:width);
}

void draw_set_curve_mode(int mode)
{
    pr_curve_mode = mode;
}

void draw_set_curve_detail(int detail)
{
    pr_curve_detail = (detail<1?1:detail);
}

void draw_bezier_quadratic(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3)
{
    gs_scalar x, y, a2, b2;
    float a = 1.0, b = 0.0, det = 1.0/(gs_scalar)pr_curve_detail;
    draw_primitive_begin(pr_curve_mode);
    for(int i = 0; i <= pr_curve_detail; ++i)
    {
        a2 = a*a; b2 = b*b;
        x = x1*a2 + x2*2*a*b + x3*b2;
        y = y1*a2 + y2*2*a*b + y3*b2;
        draw_vertex(x, y);

        a -= det;
        b = 1.0 - a;
    }
    draw_primitive_end();
}

void draw_bezier_quadratic_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, int c1, int c2, gs_scalar al1, gs_scalar al2)
{
    int col;
    gs_scalar x, y, al, a2, b2;
    float a = 1.0, b = 0.0, det = 1.0/(gs_scalar)pr_curve_detail;
    draw_primitive_begin(pr_curve_mode);
    for(int i = 0; i <= pr_curve_detail; ++i)
    {
        a2 = a*a; b2 = b*b;
        x = x1*a2 + x2*2*a*b + x3*b2;
        y = y1*a2 + y2*2*a*b + y3*b2;
        al = al1 + (al2-al1)*b;
        col = merge_color(c1, c2, b);
        draw_vertex_color(x, y,col,al);

        a -= det;
        b = 1.0 - a;
    }
    draw_primitive_end();
}

void draw_bezier_cubic(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4)
{
    gs_scalar x, y, a2, b2;
    float a = 1.0, b = 0.0, det = 1.0/(gs_scalar)pr_curve_detail;
    draw_primitive_begin(pr_curve_mode);
    for(int i = 0; i <= pr_curve_detail; ++i)
    {
        a2 = a*a; b2 = b*b;
        x = x1*a2*a + x2*3*a2*b + x3*3*a*b2 + x4*b2*b;
        y = y1*a2*a + y2*3*a2*b + y3*3*a*b2 + y4*b2*b;
        draw_vertex(x, y);

        a -= det;
        b = 1.0 - a;
    }
    draw_primitive_end();
}

void draw_bezier_cubic_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, gs_scalar al1, gs_scalar al2)
{
    int col;
    gs_scalar x, y, al;
    float a2, b2, a = 1.0, b = 0.0, det = 1.0/(gs_scalar)pr_curve_detail;

    if (pr_curve_width == 1){
      draw_primitive_begin(pr_curve_mode);
      for(int i = 0; i <= pr_curve_detail; ++i)
      {
          a2 = a*a; b2 = b*b;
          x = x1*a2*a + x2*3*a2*b + x3*3*a*b2 + x4*b2*b;
          y = y1*a2*a + y2*3*a2*b + y3*3*a*b2 + y4*b2*b;
          al = al1 + (al2-al1)*b;
          col = merge_color(c1, c2, b);
          draw_vertex_color(x, y,col,al);

          a -= det;
          b = 1.0 - a;
      }
      draw_primitive_end();
    }else{ //Thick line
      gs_scalar dw = pr_curve_width/2.0;
      gs_scalar px, py;
      x = x1, y = y1;
      a -= det;
      b = 1.0 - a;
      al = al1, col = c1;

      draw_primitive_begin(pr_trianglestrip);
      for(int i = 1; i <= pr_curve_detail; ++i)
      {
          a2 = a*a; b2 = b*b;
          px = x, py = y;
          x = x1*a2*a + x2*3*a2*b + x3*3*a*b2 + x4*b2*b;
          y = y1*a2*a + y2*3*a2*b + y3*3*a*b2 + y4*b2*b;
          double dir = fmod(atan2(py-y,x-px)+2*M_PI,2*M_PI);
          double cv = cos(dir-M_PI/2.0), sv = -sin(dir-M_PI/2.0);
          draw_vertex_color(px+dw*cv, py+dw*sv, col, al);
          draw_vertex_color(px-dw*cv, py-dw*sv, col, al);

          al = al1 + (al2-al1)*b;
          col = merge_color(c1, c2, b);

          draw_vertex_color(x+dw*cv, y+dw*sv, col, al);
          draw_vertex_color(x-dw*cv, y-dw*sv, col, al);

          a -= det;
          b = 1.0 - a;
      }
      draw_primitive_end();
    }
}

//NOTICE:
//Some of the arguments are not used, but I left them in so it would be easier to use these functions together with drawing functions
gs_scalar draw_bezier_quadratic_x(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, float t)
{
    float a = 1.0 - t, b = t;
    return x1*a*a + x2*2*a*b + x3*b*b;
}

gs_scalar draw_bezier_quadratic_y(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, float t)
{
    float a = 1.0 - t, b = t;
    return y1*a*a + y2*2*a*b + y3*b*b;
}

gs_scalar draw_bezier_cubic_x(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, float t)
{
    float a = 1.0 - t, b = t, a2, b2;
    a2 = a*a; b2 = b*b;
    return x1*a2*a + x2*3*a2*b + x3*3*a*b2 + x4*b2*b;
}

gs_scalar draw_bezier_cubic_y(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, float t)
{
    float a = 1.0 - t, b = t, a2, b2;
    a2 = a*a; b2 = b*b;
    return y1*a2*a + y2*3*a2*b + y3*3*a*b2 + y4*b2*b;
}

//The following function is used in other drawing functions for all splines
//it takes 4 points. Two control points which are at the beginning and the end, and the two points which it actually draws through
//in the middle
void draw_spline_part(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4)
{
    float t = 0.0, det = 1.0/(gs_scalar)pr_curve_detail;
    gs_scalar x, y;
    for (int i=0; i<=pr_curve_detail; ++i){
        gs_scalar t2 = t * t, t3 = t2 * t;
        x = 0.5 * ( ( 2.0 * x2 ) + ( -x1 + x3 ) * t + ( 2.0 * x1 - 5.0 * x2 + 4 * x3 - x4 ) * t2 +
        ( -x1 + 3.0 * x2 - 3.0 * x3 + x4 ) * t3 );
        y = 0.5 * ( ( 2.0 * y2 ) + ( -y1 + y3 ) * t + ( 2.0 * y1 - 5.0 * y2 + 4 * y3 - y4 ) * t2 +
        ( -y1 + 3.0 * y2 - 3.0 * y3 + y4 ) * t3 );
        draw_vertex(x, y);
        t += det;
    }
}

void draw_spline_part_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, gs_scalar a1, gs_scalar a2)
{
    int col;
    float t = 0.0, det = 1.0/(gs_scalar)pr_curve_detail;
    gs_scalar x, y, al;
    for (int i=0; i<=pr_curve_detail; ++i){
        al = a1 + (a2-a1)*t;
        col = merge_color(c1, c2, t);

        float t2 = t * t, t3 = t2 * t;
        x = 0.5 * ( ( 2.0 * x2 ) + ( -x1 + x3 ) * t + ( 2.0 * x1 - 5.0 * x2 + 4 * x3 - x4 ) * t2 +
        ( -x1 + 3.0 * x2 - 3.0 * x3 + x4 ) * t3 );
        y = 0.5 * ( ( 2.0 * y2 ) + ( -y1 + y3 ) * t + ( 2.0 * y1 - 5.0 * y2 + 4 * y3 - y4 ) * t2 +
        ( -y1 + 3.0 * y2 - 3.0 * y3 + y4 ) * t3 );
        draw_vertex_color(x, y,col,al);
        t += det;
    }
}

void draw_spline2c(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4)
{
    draw_primitive_begin(pr_curve_mode);
    draw_spline_part(x1, y1, x2, y2, x3, y3, x4, y4);
    draw_primitive_end();
}

void draw_spline2c_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, gs_scalar a1, gs_scalar a2)
{
    draw_primitive_begin(pr_curve_mode);
    draw_spline_part_color(x1, y1, x2, y2, x3, y3, x4, y4, c1, c2, a1, a2);
    draw_primitive_end();
}

void draw_spline3(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3)
{
    draw_primitive_begin(pr_curve_mode);
    //As I need 4 points to draw 1 line, then I will just take 2 control points from the existing ones
    draw_spline_part(x1, y1, x1, y1, x2, y2, x3, y3);
    draw_spline_part(x1, y1, x2, y2, x3, y3, x3, y3);
    draw_primitive_end();
}

void draw_spline3_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, int c1, int c2, gs_scalar a1, gs_scalar a2)
{
    int col1;
    gs_scalar al1;
    col1 = merge_color(c1, c2, 0.5);
    al1 = a1 + (a2-a1)*0.5;
    draw_primitive_begin(pr_curve_mode);
    //As I need 4 points to draw 1 line, then I will just take 2 control points from the existing ones
    //Color and alpha is interpolated in the middle for now (so it doesn't take into account length of each separate segment)
    draw_spline_part_color(x1, y1, x1, y1, x2, y2, x3, y3, c1, col1, a1, al1);
    draw_spline_part_color(x1, y1, x2, y2, x3, y3, x3, y3, col1, c2, al1, a2);
    draw_primitive_end();
}


void draw_spline3c(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5)
{
    draw_primitive_begin(pr_curve_mode);
    //As I need 4 points to draw 1 line, then I will just take 2 control points from the existing ones
    draw_spline_part(x1, y1, x2, y2, x3, y3, x4, y4);
    draw_spline_part(x2, y2, x3, y3, x4, y4, x5, y5);
    draw_primitive_end();
}

void draw_spline3c_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5, int c1, int c2, gs_scalar a1, gs_scalar a2)
{
    int col1;
    gs_scalar al1;
    col1 = merge_color(c1, c2, 0.5);
    al1 = a1 + (a2-a1)*0.5;
    draw_primitive_begin(pr_curve_mode);
    //As I need 4 points to draw 1 line, then I will just take 2 control points from the existing ones
    //Color and alpha is interpolated in the middle for now (so it doesn't take into account length of each separate segment)
    draw_spline_part_color(x1, y1, x2, y2, x3, y3, x4, y4, c1, col1, a1, al1);
    draw_spline_part_color(x2, y2, x3, y3, x4, y4, x5, y5, col1, c2, al1, a2);
    draw_primitive_end();
}

void draw_spline4(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4)
{
    draw_primitive_begin(pr_curve_mode);
    draw_spline_part(x1, y1, x1, y1, x2, y2, x3, y3);
    draw_spline_part(x1, y1, x2, y2, x3, y3, x4, y4);
    draw_spline_part(x2, y2, x3, y3, x4, y4, x4, y4);
    draw_primitive_end();
}

void draw_spline4_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, gs_scalar a1, gs_scalar a2)
{
    int col1, col2;
    gs_scalar al1, al2;
    col1 = merge_color(c1, c2, 0.33);
    al1 = a1 + (a2-a1)*0.33;
    col2 = merge_color(c1, c2, 0.66);
    al2 = a1 + (a2-a1)*0.66;
    draw_primitive_begin(pr_curve_mode);
    draw_spline_part_color(x1, y1, x1, y1, x2, y2, x3, y3, c1, col1, a1, al1);
    draw_spline_part_color(x1, y1, x2, y2, x3, y3, x4, y4, col1, col2, al1, al2);
    draw_spline_part_color(x2, y2, x3, y3, x4, y4, x4, y4, col2, c2, al2, a2);
    draw_primitive_end();
}

void draw_spline4c(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5, gs_scalar x6, gs_scalar y6)
{
    draw_primitive_begin(pr_curve_mode);
    draw_spline_part(x1, y1, x2, y2, x3, y3, x4, y4);
    draw_spline_part(x2, y2, x3, y3, x4, y4, x5, y5);
    draw_spline_part(x3, y3, x4, y4, x5, y5, x6, y6);
    draw_primitive_end();
}

void draw_spline4c_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5, gs_scalar x6, gs_scalar y6, int c1, int c2, gs_scalar a1, gs_scalar a2)
{
    int col1, col2;
    gs_scalar al1, al2;
    col1 = merge_color(c1, c2, 0.33);
    al1 = a1 + (a2-a1)*0.33;
    col2 = merge_color(c1, c2, 0.66);
    al2 = a1 + (a2-a1)*0.66;

    draw_primitive_begin(pr_curve_mode);
    draw_spline_part_color(x1, y1, x2, y2, x3, y3, x4, y4, c1, col1, a1, al1);
    draw_spline_part_color(x2, y2, x3, y3, x4, y4, x5, y5, col1, col2, al1, al2);
    draw_spline_part_color(x3, y3, x4, y4, x5, y5, x6, y6, col2, c2, al2, a2);
    draw_primitive_end();
}

void draw_spline_begin(int mode)
{
    startedSplinesMode.push(mode);
    startedSplines.push(new spline);
}

void draw_spline_vertex(gs_scalar x, gs_scalar y)
{
    splinePoint point={x,y,gs_scalar(enigma::currentcolor[3]), int(enigma::currentcolor[0] + (enigma::currentcolor[1] << 8) + (enigma::currentcolor[2] << 16))};
    startedSplines.top()->push_back(point);
}

void draw_spline_vertex_color(gs_scalar x, gs_scalar y, int col, gs_scalar alpha)
{
    splinePoint point={x,y,alpha,col};
    startedSplines.top()->push_back(point);
}


void draw_spline_end()
{
    draw_primitive_begin(startedSplinesMode.top());
    spline &arr = *startedSplines.top();
    if (arr.size()>4)
        for (unsigned i = 3; i < arr.size(); ++i)
            draw_spline_part_color(arr[i-3].x, arr[i-3].y, arr[i-2].x, arr[i-2].y, arr[i-1].x, arr[i-1].y, arr[i].x, arr[i].y, arr[i-2].col, arr[i-1].col, arr[i-2].al, arr[i-1].al);
    draw_primitive_end();
    delete &arr;
    startedSplines.pop();
    startedSplinesMode.pop();
}

void draw_bezier_quadratic_spline_part(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, int c1, int c2, gs_scalar a1, gs_scalar a2)
{
    int col;
    float t = 0.0, det = 1.0/(gs_scalar)pr_curve_detail;
    gs_scalar x, y, al;
    for (int i=0; i<=pr_curve_detail; ++i){
        al = a1 + (a2-a1)*t;
        col = merge_color(c1, c2, t);

        x = 0.5 * (((x1 - 2 * x2 + x3) * t + 2 * x2 - 2 * x1) * t + x1 + x2);
        y = 0.5 * (((y1 - 2 * y2 + y3) * t + 2 * y2 - 2 * y1) * t + y1 + y2);
        draw_vertex_color(x, y, col, al);
        t += det;
    }
}

//first and last point is used as control points, so they will not be drawn
void draw_bezier_quadratic_spline_end()
{
    draw_primitive_begin(startedSplinesMode.top());
    spline &arr = *startedSplines.top();
    if (arr.size()>3)
        for (unsigned i = 2; i < arr.size(); ++i)
            draw_bezier_quadratic_spline_part(arr[i-2].x, arr[i-2].y, arr[i-1].x, arr[i-1].y, arr[i].x, arr[i].y, arr[i-2].col, arr[i-1].col, arr[i-2].al, arr[i-1].al);
    draw_primitive_end();
    delete &arr;
    startedSplines.pop();
    startedSplinesMode.pop();
}

int draw_spline_optimized_end()
{
    float tmp_detail = (gs_scalar)pr_curve_detail;
    int tot_det = 0;
    draw_primitive_begin(startedSplinesMode.top());
    spline &arr = *startedSplines.top();
    if (arr.size()>4){
        for (unsigned i = 3; i < arr.size(); ++i)
        {
            pr_curve_detail = std::min((float)(sqrt((arr[i-2].x-arr[i-1].x)*(arr[i-2].x-arr[i-1].x)+(arr[i-2].y-arr[i-1].y)*(arr[i-2].y-arr[i-1].y))/5.0),tmp_detail);
            tot_det+=pr_curve_detail;
            draw_spline_part_color(arr[i-3].x, arr[i-3].y, arr[i-2].x, arr[i-2].y, arr[i-1].x, arr[i-1].y, arr[i].x, arr[i].y, arr[i-2].col, arr[i-1].col, arr[i-2].al, arr[i-1].al);
        }
    }
    draw_primitive_end();
    delete &arr;
    startedSplines.pop();
    startedSplinesMode.pop();
    pr_curve_detail = (int)tmp_detail;
    return tot_det;
}

}
