/********************************************************************************\
**                                                                              **
**  Copyright (C) 2010 Harijs Grînbergs                                         **
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

#include "OpenGLHeaders.h"
#include "GScolors.h"
#include <stack>
#include <vector>
#include <math.h>

#define __GETR(x) (((x & 0x0000FF))/255.0)
#define __GETG(x) (((x & 0x00FF00)>>8)/255.0)
#define __GETB(x) (((x & 0xFF0000)>>16)/255.0)


namespace enigma{
    extern unsigned bound_texture;
    extern unsigned char currentcolor[4];
}
#define untexture() if(enigma::bound_texture) glBindTexture(GL_TEXTURE_2D,enigma::bound_texture=0);

int pr_curve_detail = 20;
float pr_curve_alpha1 = 1;
float pr_curve_alpha2 = 1;
int pr_curve_color1 = 0;
int pr_curve_color2 = 0;
int pr_curve_mode = GL_LINE_STRIP;
int pr_spline_points = 0;
int pr_curve_width = 1;

struct splinePoint {
    float x,y,al;
    int col;
};
typedef std::vector< splinePoint > spline;
static std::stack< spline, std::vector<spline*> > startedSplines;
static std::stack< int > startedSplinesMode;

void draw_set_curve_width(int width)
{
    pr_curve_width=width;
}

void draw_set_curve_color(int c1, int c2)
{
    pr_curve_color1 = c1;
    pr_curve_color2 = c2;
}

void draw_set_curve_alpha(float a1, float a2)
{
    pr_curve_alpha1 = a1;
    pr_curve_alpha2 = a2;
}

void draw_set_curve_mode(int mode)
{
    pr_curve_mode = mode;
}

void draw_set_curve_detail(int detail)
{
    pr_curve_detail = detail;
}

void draw_bezier_quadratic(float x1,float y1,float x2,float y2,float x3,float y3)
{
    untexture();
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(pr_curve_width);
    glBegin(pr_curve_mode);
    int col;
    float x, y, al, a2, b2;
    float a = 1.0, b = 0, det = 1/(float)pr_curve_detail;
    for(int i = 0; i <= pr_curve_detail; i++)
    {
        a2 = a*a; b2 = b*b;
        x = x1*a2 + x2*2*a*b + x3*b2;
        y = y1*a2 + y2*2*a*b + y3*b2;
        al = pr_curve_alpha1 + (pr_curve_alpha2-pr_curve_alpha1)*b;
        col = merge_color(pr_curve_color1, pr_curve_color2, b);

        glColor4f(__GETR(col),__GETG(col),__GETB(col),al);
        glVertex2f(x, y);

        a -= det;
        b = 1.0 - a;
    }
    glEnd();
    glPopAttrib();
    glColor4ubv(enigma::currentcolor);
}

void draw_bezier_cubic(float x1,float y1,float x2,float y2,float x3,float y3, float x4, float y4)
{
    untexture();
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(pr_curve_width);
    glBegin(pr_curve_mode);
    int col;
    float x, y, al, a2, b2;
    float a = 1.0, b = 0, det = 1/(float)pr_curve_detail;
    for(int i = 0; i <= pr_curve_detail; i++)
    {
        a2 = a*a; b2 = b*b;
        x = x1*a2*a + x2*3*a2*b + x3*3*a*b2 + x4*b2*b;
        y = y1*a2*a + y2*3*a2*b + y3*3*a*b2 + y4*b2*b;
        al = pr_curve_alpha1 + (pr_curve_alpha2-pr_curve_alpha1)*b;
        col = merge_color(pr_curve_color1, pr_curve_color2, b);

        glColor4f(__GETR(col),__GETG(col),__GETB(col),al);
        glVertex2f(x, y);

        a -= det;
        b = 1.0 - a;
    }
    glEnd();
    glPopAttrib();
    glColor4ubv(enigma::currentcolor);
}

//NOTICE:
//Some of the arguments are not used, but I left them in so it would be easier to use these functions together with drawing functions
float draw_bezier_quadratic_x( float x1, float y1, float x2, float y2, float x3, float y3, float t)
{
    float a = 1.0 - t, b = t;
    return x1*a*a + x2*2*a*b + x3*b*b;
}

float draw_bezier_quadratic_y(float x1, float y1, float x2, float y2, float x3, float y3, float t)
{
    float a = 1.0 - t, b = t;
    return y1*a*a + y2*2*a*b + y3*b*b;
}

float draw_bezier_cubic_x(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float t)
{
    float a = 1.0 - t, b = t, a2, b2;
    a2 = a*a; b2 = b*b;
    return x1*a2*a + x2*3*a2*b + x3*3*a*b2 + x4*b2*b;
}

float draw_bezier_cubic_y(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float t)
{
    float a = 1.0 - t, b = t, a2, b2;
    a2 = a*a; b2 = b*b;
    return y1*a2*a + y2*3*a2*b + y3*3*a*b2 + y4*b2*b;
}

//The following function is used in other drawing functions for all splines
//it takes 4 points. Two control points which are at the beggining and the end, and the two points which it actually draws through
//in the middle
void draw_spline_part(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2)
{
    int col;
    float x, y, al, t = 0, det = 1/(float)pr_curve_detail;
    for (int i=0; i<=pr_curve_detail; i++){

        al = a1 + (a2-a1)*t;

        col = merge_color(c1, c2, t);

        float t2 = t * t, t3 = t2 * t;
        x = 0.5 * ( ( 2.0 * x2 ) + ( -x1 + x3 ) * t + ( 2.0 * x1 - 5.0 * x2 + 4 * x3 - x4 ) * t2 +
        ( -x1 + 3.0 * x2 - 3.0 * x3 + x4 ) * t3 );
        y = 0.5 * ( ( 2.0 * y2 ) + ( -y1 + y3 ) * t + ( 2.0 * y1 - 5.0 * y2 + 4 * y3 - y4 ) * t2 +
        ( -y1 + 3.0 * y2 - 3.0 * y3 + y4 ) * t3 );
        glColor4f(__GETR(col),__GETG(col),__GETB(col),al);
        glVertex2f(x, y);
        t += det;
    }
}

void draw_spline2c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    untexture();
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(pr_curve_width);
    glBegin(pr_curve_mode);
    draw_spline_part(x1, y1, x2, y2, x3, y3, x4, y4, pr_curve_color1, pr_curve_color2, pr_curve_alpha1, pr_curve_alpha2);
    glEnd();
    glPopAttrib();
    glColor4ubv(enigma::currentcolor);
}

void draw_spline3(float x1, float y1, float x2, float y2, float x3, float y3)
{
    untexture();
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(pr_curve_width);
    glBegin(pr_curve_mode);
    int col1;
    float al1;
    col1 = merge_color(pr_curve_color1, pr_curve_color2, 0.5);
    al1 = pr_curve_alpha1 + (pr_curve_alpha2-pr_curve_alpha1)*0.5;
    //As I need 4 points to draw 1 line, then I will just take 2 control points from the existing ones
    //Color and alpha is interpolated in the middle for now (so it doesn't take into account length of each seperate segment)
    draw_spline_part(x1, y1, x1, y1, x2, y2, x3, y3, pr_curve_color1, col1, pr_curve_alpha1, al1);
    draw_spline_part(x1, y1, x2, y2, x3, y3, x3, y3, col1, pr_curve_color2, al1, pr_curve_alpha2);
    glEnd();
    glPopAttrib();
    glColor4ubv(enigma::currentcolor);
}

void draw_spline3c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5)
{
    untexture();
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(pr_curve_width);
    glBegin(pr_curve_mode);
    int col1;
    float al1;
    col1 = merge_color(pr_curve_color1, pr_curve_color2, 0.5);
    al1 = pr_curve_alpha1 + (pr_curve_alpha2-pr_curve_alpha1)*0.5;
    //As I need 4 points to draw 1 line, then I will just take 2 control points from the existing ones
    //Color and alpha is interpolated in the middle for now (so it doesn't take into account length of each seperate segment)
    draw_spline_part(x1, y1, x2, y2, x3, y3, x4, y4, pr_curve_color1, col1, pr_curve_alpha1, al1);
    draw_spline_part(x2, y2, x3, y3, x4, y4, x5, y5, col1, pr_curve_color2, al1, pr_curve_alpha2);
    glEnd();
    glPopAttrib();
    glColor4ubv(enigma::currentcolor);
}

void draw_spline4(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    untexture();
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(pr_curve_width);
    glBegin(pr_curve_mode);
    int col1, col2;
    float al1, al2;
    col1 = merge_color(pr_curve_color1, pr_curve_color2, 0.33);
    al1 = pr_curve_alpha1 + (pr_curve_alpha2-pr_curve_alpha1)*0.33;

    col2 = merge_color(pr_curve_color1, pr_curve_color2, 0.66);
    al2 = pr_curve_alpha1 + (pr_curve_alpha2-pr_curve_alpha1)*0.66;
    draw_spline_part(x1, y1, x1, y1, x2, y2, x3, y3, pr_curve_color1, col1, pr_curve_alpha1, al1);
    draw_spline_part(x1, y1, x2, y2, x3, y3, x4, y4, col1, col2, al1, al2);
    draw_spline_part(x2, y2, x3, y3, x4, y4, x4, y4, col2, pr_curve_color2, al2, pr_curve_alpha2);
    glEnd();
    glPopAttrib();
    glColor4ubv(enigma::currentcolor);
}

void draw_spline4c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6)
{
    untexture();
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(pr_curve_width);
    glBegin(pr_curve_mode);
    int col1, col2;
    float al1, al2;
    col1 = merge_color(pr_curve_color1, pr_curve_color2, 0.33);
    al1 = pr_curve_alpha1 + (pr_curve_alpha2-pr_curve_alpha1)*0.33;

    col2 = merge_color(pr_curve_color1, pr_curve_color2, 0.66);
    al2 = pr_curve_alpha1 + (pr_curve_alpha2-pr_curve_alpha1)*0.66;
    draw_spline_part(x1, y1, x2, y2, x3, y3, x4, y4, pr_curve_color1, col1, pr_curve_alpha1, al1);
    draw_spline_part(x2, y2, x3, y3, x4, y4, x5, y5, col1, col2, al1, al2);
    draw_spline_part(x3, y3, x4, y4, x5, y5, x6, y6, col2, pr_curve_color2, al2, pr_curve_alpha2);
    glEnd();
    glPopAttrib();
    glColor4ubv(enigma::currentcolor);
}

void draw_spline_begin(int mode)
{
    startedSplinesMode.push(mode);
    startedSplines.push(new spline);
}

int draw_spline_vertex(float x, float y)
{
    splinePoint point={x,y,pr_curve_alpha1,pr_curve_color1};
    startedSplines.top()->push_back(point);
    return startedSplines.top()->size()-2;
}

int draw_spline_vertex_color(float x, float y, int col, float alpha)
{
    splinePoint point={x,y,alpha,col};
    startedSplines.top()->push_back(point);
    return startedSplines.top()->size()-2;
}


void draw_spline_end()
{
    untexture();
    glPushAttrib(GL_LINE_BIT);
      glLineWidth(pr_curve_width);
      glBegin(startedSplinesMode.top());
        spline &arr = *startedSplines.top();
        if (arr.size()>4)
          for (int i = 3; i < arr.size(); i++)
              draw_spline_part(arr[i-3].x, arr[i-3].y, arr[i-2].x, arr[i-2].y, arr[i-1].x, arr[i-1].y, arr[i].x, arr[i].y, arr[i-2].col, arr[i-1].col, arr[i-2].al, arr[i-1].al);
      glEnd();
    glPopAttrib();
    glColor4ubv(enigma::currentcolor);
    delete &arr;
    startedSplines.pop();
    startedSplinesMode.pop();
}

int draw_spline_optimized_end()
{
    double tmp_detail = (double)pr_curve_detail;
    int tot_det = 0;
    untexture();
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(pr_curve_width);
    glBegin(startedSplinesMode.top());
    spline &arr = *startedSplines.top();
    if (arr.size()>4){
        for (int i = 3; i < arr.size(); i++)
        {
            pr_curve_detail = std::min(sqrt((arr[i-2].x-arr[i-1].x)*(arr[i-2].x-arr[i-1].x)+(arr[i-2].y-arr[i-1].y)*(arr[i-2].y-arr[i-1].y))/5,tmp_detail);
            tot_det+=pr_curve_detail;
            draw_spline_part(arr[i-3].x, arr[i-3].y, arr[i-2].x, arr[i-2].y, arr[i-1].x, arr[i-1].y, arr[i].x, arr[i].y, arr[i-2].col, arr[i-1].col, arr[i-2].al, arr[i-1].al);
        }
    }
    glEnd();
    glPopAttrib();
    glColor4ubv(enigma::currentcolor);
    delete &arr;
    startedSplines.pop();
    startedSplinesMode.pop();
    pr_curve_detail = (int)tmp_detail;
    return tot_det;
}
