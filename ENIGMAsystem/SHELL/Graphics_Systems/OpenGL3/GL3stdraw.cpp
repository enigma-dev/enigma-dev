/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton, Serpex
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
#include "../General/OpenGLHeaders.h"
#include "../General/GSstdraw.h"
#include "../General/GLbinding.h"
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
  int maximumMSAA;
  glGetIntegerv(GL_MAX_SAMPLES, &maximumMSAA);
  return maximumMSAA;
}

bool draw_get_msaa_supported()
{
    return GLEW_EXT_multisample;
}

void draw_set_msaa_enabled(bool enable)
{
  (enable?glEnable:glDisable)(GL_MULTISAMPLE);
}

void draw_set_line_pattern(unsigned short pattern, int scale)
{
  if (pattern == -1)
      glDisable(GL_LINE_STIPPLE);
  else
    glEnable(GL_LINE_STIPPLE),
    glLineStipple(scale,pattern);
}

void draw_point(float x,float y)
{
  texture_reset();
  glBegin(GL_POINTS);
    glVertex2f(x,y);
  glEnd();
}

void draw_point_color(float x,float y,int col)
{
  texture_reset();
  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(col),__GETG(col),__GETB(col),enigma::currentcolor[3]);
    glBegin(GL_POINTS);
      glVertex2f(x,y);
    glEnd();
  glPopAttrib();
}

void draw_line(float x1,float y1,float x2,float y2)
{
  texture_reset();
  glBegin(GL_LINES);
    glVertex2f(x1+0.5,y1+0.5);
    glVertex2f(x2+0.5,y2+0.5);
  glEnd();
}

void draw_line_width(float x1,float y1,float x2,float y2,float width)
{
  texture_reset();
  glPushAttrib(GL_LINE_BIT);
    glLineWidth(width);
    glBegin(GL_LINES);
      glVertex2f(x1+0.5,y1+0.5);
      glVertex2f(x2+0.5,y2+0.5);
    glEnd();
  glPopAttrib();
}

void draw_line_color(float x1,float y1,float x2,float y2,int c1,int c2)
{
  texture_reset();
  glBegin(GL_LINES);
    glColor4ub(__GETR(c1),__GETG(c1),__GETB(c1),enigma::currentcolor[3]);
      glVertex2f(x1+0.5,y1+0.5);
    glColor4ub(__GETR(c2),__GETG(c2),__GETB(c2),enigma::currentcolor[3]);
      glVertex2f(x2+0.5,y2+0.5);
  glEnd();
  glColor4ubv(enigma::currentcolor);
}

void draw_line_width_color(float x1,float y1,float x2,float y2,float width,int c1,int c2)
{
  texture_reset();
  glPushAttrib(GL_LINE_BIT);
    glLineWidth(width);
    glBegin(GL_LINES);
      glColor4ub(__GETR(c1),__GETG(c1),__GETB(c1),enigma::currentcolor[3]);
      glVertex2f(x1+0.5,y1+0.5);
      glColor4ub(__GETR(c2),__GETG(c2),__GETB(c2),enigma::currentcolor[3]);
      glVertex2f(x2+0.5,y2+0.5);
    glEnd();
  glPopAttrib();
  glColor4ubv(enigma::currentcolor);
}

void draw_rectangle(float x1,float y1,float x2,float y2,bool outline)
{
  x1 -= 1;
  y1 -= 1;
  texture_reset();
  if(outline)
  {
    glBegin(GL_LINE_LOOP);
      glVertex2f(x1+0.5,y1+0.5);
      glVertex2f(x1+0.5,y2+0.5);
      glVertex2f(x2+0.5,y2+0.5);
      glVertex2f(x2+0.5,y1+0.5);
    glEnd();
  }
  else glRectf(x1,y1,x2,y2);
}

void draw_rectangle_angle(float x1,float y1,float x2,float y2,float angle,bool outline)
{
  texture_reset();
  angle *= M_PI/180;

  float
    xm = (x2+x1)/2,
    ym = (y2+y1)/2;

  float
    len = hypot(xm-x1,ym-y1),
    dir = atan2(y1-ym,x1-xm)+angle;

  float
    ldx1 = len*cos(dir)+(outline?0.5:0),
    ldy1 = len*sin(dir)+(outline?0.5:0);

  dir = atan2(y2-ym,x1-xm)+angle;
  float
    ldx2 = len*cos(dir)+(outline?0.5:0),
    ldy2 = len*sin(dir)+(outline?0.5:0);

  glBegin(outline ? GL_LINE_LOOP : GL_QUADS);
    glVertex2f(xm+ldx1,ym-ldy1);
    glVertex2f(xm+ldx2,ym-ldy2);
    glVertex2f(xm-ldx1,ym+ldy1);
    glVertex2f(xm-ldx2,ym+ldy2);
  glEnd();
}

void draw_rectangle_color(float x1,float y1,float x2,float y2,int c1,int c2,int c3,int c4,bool outline)
{
    texture_reset();
    if (outline == true){
        glBegin(GL_LINE_LOOP);
          glColor4ub(__GETR(c1),__GETG(c1),__GETB(c1),enigma::currentcolor[3]);
            glVertex2f(x1+0.5,y1+0.5);
          glColor4ub(__GETR(c2),__GETG(c2),__GETB(c2),enigma::currentcolor[3]);
            glVertex2f(x2+0.5,y1+0.5);
          glColor4ub(__GETR(c4),__GETG(c4),__GETB(c4),enigma::currentcolor[3]);
            glVertex2f(x2+0.5,y2+0.5);
          glColor4ub(__GETR(c3),__GETG(c3),__GETB(c3),enigma::currentcolor[3]);
            glVertex2f(x1+0.5,y2+0.5);
        glEnd();
    }else{
        glBegin(GL_QUADS);
          glColor4ub(__GETR(c1),__GETG(c1),__GETB(c1),enigma::currentcolor[3]);
            glVertex2f(x1,y1);
          glColor4ub(__GETR(c2),__GETG(c2),__GETB(c2),enigma::currentcolor[3]);
            glVertex2f(x2,y1);
          glColor4ub(__GETR(c4),__GETG(c4),__GETB(c4),enigma::currentcolor[3]);
            glVertex2f(x2,y2);
          glColor4ub(__GETR(c3),__GETG(c3),__GETB(c3),enigma::currentcolor[3]);
            glVertex2f(x1,y2);
        glEnd();
    }
    glColor4ubv(enigma::currentcolor);
}

void draw_set_circle_precision(float pr) {
    enigma::circleprecision = pr<3 ? 3 : pr;
}
float draw_get_circle_precision() {
    return enigma::circleprecision;
}

void draw_circle(float x,float y,float r,bool outline)
{
    texture_reset();
    double pr=2*M_PI/enigma::circleprecision;
    if(outline)
    {
        glBegin(GL_LINE_STRIP);
        for(double i=0;i<=2*M_PI; i+=pr)
        {
            double xc1=cos(i)*r,yc1=sin(i)*r;
            glVertex2f(x+xc1+0.5,y+yc1+0.5);
        }
    }
    else
    {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x,y);
        for(double i=0;i<=2*M_PI; i+=pr)
        {
            double xc1=cos(i)*r,yc1=sin(i)*r;
            glVertex2f(x+xc1,y+yc1);
        }
    }
    glEnd();
}

void draw_circle_color(float x,float y,float r,int c1, int c2,bool outline)
{
  texture_reset();
    if(outline)
      glBegin(GL_LINE_STRIP);
    else
    {
        glBegin(GL_TRIANGLE_FAN);
      glColor4ub(__GETR(c1),__GETG(c1),__GETB(c1),enigma::currentcolor[3]);
        glVertex2f(x,y);
    }
    //Bagan above
    //This maybe needs to be split in outline and filled, because now that 0.5 could break it (but by testing it doesn't)
    glColor4ub(__GETR(c2),__GETG(c2),__GETB(c2),enigma::currentcolor[3]);
      float pr=2*M_PI/enigma::circleprecision;
      glVertex2f(x+0.5+r,y+0.5);
      for(float i=pr;i<2*M_PI;i+=pr)
        glVertex2f(x+r*cos(i)+0.5,y-r*sin(i)+0.5);
      glVertex2f(x+0.5+r,y+0.5);
    glEnd();
    glColor4ubv(enigma::currentcolor);
}

void draw_circle_perfect(float x,float y,float r,bool outline)
{
    ///TODO: This is broken. Has some random empty lines when drawn. +0.5 offset doesn't fix it
    texture_reset();
    const float r2 = r*r, r12 = r*M_SQRT1_2;
    glBegin(outline?GL_POINTS:GL_LINES);
    for(float xc=0, yc=r; xc <= r12; xc++)
    {
      if(xc*xc + yc*yc > r2) yc--;
      glVertex2f(x+xc, y+yc);
      glVertex2f(x+xc, y-yc);
      glVertex2f(x-xc, y+yc);
      glVertex2f(x-xc, y-yc);
      glVertex2f(x+yc, y+xc);
      glVertex2f(x+yc, y-xc);
      glVertex2f(x-yc, y+xc);
      glVertex2f(x-yc, y-xc);
    }
    glEnd();
}

void draw_circle_color_perfect(float x,float y,float r, int c1, int c2, bool outline)
{
    texture_reset();
    float r2=r*r;
    if(outline)
    {
      glBegin(GL_POINTS);

      glColor4ub(__GETR(c2),__GETG(c2),__GETB(c2),enigma::currentcolor[3]);
        float r12=r*M_SQRT1_2;
        for(float xc=0,yc=r;xc<=r12;xc++)
        {
          if(xc*xc+yc*yc>r2) yc--;
          glVertex2f(x+xc,y+yc);
          glVertex2f(x+xc,y-yc);
          glVertex2f(x-xc,y+yc);
          glVertex2f(x-xc,y-yc);
          glVertex2f(x+yc,y+xc);
          glVertex2f(x+yc,y-xc);
          glVertex2f(x-yc,y+xc);
          glVertex2f(x-yc,y-xc);
        }
    }
    else
    {
      glBegin(GL_TRIANGLE_FAN);

      glColor4ub(__GETR(c1),__GETG(c1),__GETB(c1),enigma::currentcolor[3]);
        glVertex2f(x,y);
      glColor4ub(__GETR(c2),__GETG(c2),__GETB(c2),enigma::currentcolor[3]);
        glVertex2f(x-r,y);
      for(float xc=-r+1;xc<r;xc++)
        glVertex2f(x+xc,y+sqrt(r2-(xc*xc)));
      for(float xc=r;xc>-r;xc--)
        glVertex2f(x+xc,y-sqrt(r2-(xc*xc)));
      glVertex2f(x-r,y);
    }
    glEnd();
    glColor4ubv(enigma::currentcolor);
}

void draw_ellipse(float x1,float y1,float x2,float y2,bool outline)
{
  texture_reset();
  float
      x=(x1+x2)/2,y=(y1+y2)/2,
      hr=fabs(x2-x),vr=fabs(y2-y),
      pr=2*M_PI/enigma::circleprecision;
  if(outline)
  {
    glBegin(GL_LINE_STRIP);
    glVertex2f(x+0.5+hr,y+0.5);
    for(float i=pr;i<2*M_PI;i+=pr)
    {
      glVertex2f(x+hr*cos(i)+0.5,y-vr*sin(i)+0.5);
    }
    glVertex2f(x+0.5+hr,y+0.5);
  }
  else
  {
    glBegin(GL_QUADS);
    for(float i = pr; i < M_PI; i += pr)
    {
      float xc1=cos(i)*hr,yc1=sin(i)*vr;
      i+=pr;
      float xc2=cos(i)*hr,yc2=sin(i)*vr;
      glVertex2f(x-xc1,y+yc1);glVertex2f(x+xc1,y+yc1);glVertex2f(x+xc2,y+yc2);glVertex2f(x-xc2,y+yc2);
      glVertex2f(x-xc1,y-yc1);glVertex2f(x+xc1,y-yc1);glVertex2f(x+xc2,y-yc2);glVertex2f(x-xc2,y-yc2);
    }
  }
  glEnd();
}

void draw_ellipse_color(float x1,float y1,float x2,float y2,int c1, int c2,bool outline)
{
  texture_reset();
    float
        x=(x1+x2)/2,y=(y1+y2)/2,
        hr=fabs(x2-x),vr=fabs(y2-y),
        pr=2*M_PI/enigma::circleprecision;

    if(outline)
      glBegin(GL_LINE_STRIP);
    else
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor4ub(__GETR(c1),__GETG(c1),__GETB(c1),enigma::currentcolor[3]);
        glVertex2f(x,y);
    }
    //This maybe needs to be split in outline and filled, because now that 0.5 could break it (but by testing it doesn't)
    glColor4ub(__GETR(c2),__GETG(c2),__GETB(c2),enigma::currentcolor[3]);
    float i;
    for(i = pr; i < 2*M_PI; i += pr)
      glVertex2f(x+hr*cos(i)+0.5,y+vr*sin(i)+0.5);
    glVertex2f(x+hr*cos(i)+0.5,y+vr*sin(i)+0.5);
    glEnd();

    glColor4ubv(enigma::currentcolor);
}

void draw_ellipse_perfect(float x1,float y1,float x2,float y2,bool outline)
{
    ///TODO: Doesn't work at all. Draws some kind of lip shape
    texture_reset();
    float
    x=(x1+x2)/2,y=(y1+y2)/2,
    hr=fabs(x2-x),vr=fabs(y2-y);
    glBegin(outline?GL_POINTS:GL_LINES);
    for(float xc=0;xc<hr;xc++)
    {
    float yc=vr*cos((M_PI/2)/hr*xc);
        glVertex2f(x+xc,y+yc);
        glVertex2f(x+xc,y-yc);
        glVertex2f(x-xc,y+yc);
        glVertex2f(x-xc,y-yc);
    }
    glEnd();
}

void draw_triangle(float x1,float y1,float x2,float y2,float x3,float y3,bool outline)
{
  texture_reset();
  glBegin(outline?GL_LINE_LOOP:GL_TRIANGLES);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glVertex2f(x3,y3);
  glEnd();
}

void draw_triangle_color(float x1,float y1,float x2,float y2,float x3,float y3,int col1,int col2,int col3,bool outline)
{
  texture_reset();
  glBegin(outline?GL_LINE_LOOP:GL_TRIANGLES);
    glColor4ub(__GETR(col1),__GETG(col1),__GETB(col1),enigma::currentcolor[3]);
      glVertex2f(x1,y1);
    glColor4ub(__GETR(col2),__GETG(col2),__GETB(col2),enigma::currentcolor[3]);
      glVertex2f(x2,y2);
    glColor4ub(__GETR(col3),__GETG(col3),__GETB(col3),enigma::currentcolor[3]);
      glVertex2f(x3,y3);
  glEnd();
  glColor4ubv(enigma::currentcolor);
}

void draw_roundrect(float x1,float y1,float x2,float y2,float r, bool outline)
{
    ///TODO: Draws buggy when outline=false and the size is changed (an empty line sometimes appears)
    texture_reset();
    if(x1>x2) {
        float t=x2;
        x2=x1;
        x1=t;
    }
    if(y1>y2) {
        float t=y2;
        y2=y1;
        y1=t;
    }
    if (x2-x1<r*2){r=(x2-x1)/2;}
    if (y2-y1<r*2){r=(y2-y1)/2;}
    if (r<0){r=0;}
    float r2=r*r,r12=r*M_SQRT1_2,
        bx1=x1+r,by1=y1+r,
        bx2=x2-r,by2=y2-r;
    glBegin(GL_LINES);
    if(outline)
    {
        glVertex2f(x1,by1);glVertex2f(x1,by2);
        glVertex2f(x2,by1);glVertex2f(x2,by2);
        glVertex2f(bx1,y1);glVertex2f(bx2,y1);
        glVertex2f(bx1,y2);glVertex2f(bx2,y2);
        glEnd();
        glBegin(GL_POINTS);
        for(float xc=0,yc=r;xc<=r12;xc++)
        {
            if(xc*xc+yc*yc>r2) yc--;
            glVertex2f(bx2+xc,by2+yc);
            glVertex2f(bx2+xc,by1-yc);
            glVertex2f(bx1-xc,by2+yc);
            glVertex2f(bx1-xc,by1-yc);
            glVertex2f(bx2+yc,by2+xc);
            glVertex2f(bx2+yc,by1-xc);
            glVertex2f(bx1-yc,by2+xc);
            glVertex2f(bx1-yc,by1-xc);
        }
        glEnd();
    }
    else
    {
        for(float xc=0,yc=r;xc<=r12;xc++)
        {
            if(xc*xc+yc*yc>r2) yc--;
            glVertex2f(bx2+xc,by2+yc);
            glVertex2f(bx2+xc,by1-yc);
            glVertex2f(bx1-xc,by2+yc);
            glVertex2f(bx1-xc,by1-yc);
            glVertex2f(bx2+yc,by2+xc);
            glVertex2f(bx2+yc,by1-xc);
            glVertex2f(bx1-yc,by2+xc);
            glVertex2f(bx1-yc,by1-xc);
        }
        glEnd();
        glRectf(bx1,y1,bx2,y2);
    }
}

void draw_roundrect_color(float x1, float y1, float x2, float y2, float r, int col1, int col2, bool outline)
{
  texture_reset();
  if(x1>x2) {
    float t=x2;
    x2=x1;
    x1=t;
  }
  if(y1>y2) {
    float t=y2;
    y2=y1;
    y1=t;
  }
  if (x2-x1<r*2){r=(x2-x1)/2;}
  if (y2-y1<r*2){r=(y2-y1)/2;}
  if (r<0){r=0;}
  float r2=r*r,r12=r*M_SQRT1_2,
      bx1=x1+r,by1=y1+r,
      bx2=x2-r,by2=y2-r;
  glBegin(GL_LINES);
  if(outline)
  {
    glColor4ub(__GETR(col2),__GETG(col2),__GETB(col2),enigma::currentcolor[3]);
    glVertex2f(x1,by1);glVertex2f(x1,by2);
    glVertex2f(x2,by1);glVertex2f(x2,by2);
    glVertex2f(bx1,y1);glVertex2f(bx2,y1);
    glVertex2f(bx1,y2);glVertex2f(bx2,y2);
    glEnd();
    glBegin(GL_POINTS);
    for(float xc=0,yc=r;xc<=r12;xc++)
    {
      if(xc*xc+yc*yc>r2) yc--;
      glVertex2f(bx2+xc,by2+yc);
      glVertex2f(bx2+xc,by1-yc);
      glVertex2f(bx1-xc,by2+yc);
      glVertex2f(bx1-xc,by1-yc);
      glVertex2f(bx2+yc,by2+xc);
      glVertex2f(bx2+yc,by1-xc);
      glVertex2f(bx1-yc,by2+xc);
      glVertex2f(bx1-yc,by1-xc);
    }
    glEnd();
  }
  else
  {
    glColor4ub(__GETR(col2),__GETG(col2),__GETB(col2),enigma::currentcolor[3]);
    for(float xc=0,yc=r;xc<=r12;xc++)
    {
      if(xc*xc+yc*yc>r2) yc--;
      glVertex2f(bx2+xc,by2+yc);
      glVertex2f(bx2+xc,by1-yc);
      glVertex2f(bx1-xc,by2+yc);
      glVertex2f(bx1-xc,by1-yc);
      glVertex2f(bx2+yc,by2+xc);
      glVertex2f(bx2+yc,by1-xc);
      glVertex2f(bx1-yc,by2+xc);
      glVertex2f(bx1-yc,by1-xc);
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor4ub(__GETR(col1),__GETG(col1),__GETB(col1),enigma::currentcolor[3]);
    glVertex2f(x1+(x2-x1)/2,y1+(y2-y1)/2);
    glColor4ub(__GETR(col2),__GETG(col2),__GETB(col2),enigma::currentcolor[3]);
    glVertex2f(x1,by1);
    glVertex2f(bx1,y1);
    glVertex2f(bx2,y1);
    glVertex2f(x2,by1);
    glVertex2f(x2,by2);
    glVertex2f(bx2,y2);
    glVertex2f(bx1,y2);
    glVertex2f(x1,by2);
    glVertex2f(x1,by1);
    glEnd();
  }
  glColor4ubv(enigma::currentcolor);
}

void draw_arrow(float x1,float y1,float x2,float y2, float arrow_size, float line_size, bool outline)
{
  texture_reset();
  double dir = atan2(y2-y1,x2-x1);
  float tc = cos(dir), ts = sin(dir),
  xs = x2-tc*arrow_size, ys = y2-ts*arrow_size,
  lw = ts*(line_size/2), lh = tc*(line_size/2);
  double at = atan2(ys-y1,xs-x1);
  if (fabs((dir<0?dir+2*M_PI:dir)-(at<0?at+2*M_PI:at)) < 0.01){
      glBegin(outline?GL_LINE_LOOP:GL_QUADS);
      glVertex2f(x1+lw,y1-lh);
      glVertex2f(x1-lw,y1+lh);
      glVertex2f(xs-lw,ys+lh);
      glVertex2f(xs+lw,ys-lh);
      glEnd();
  }
  glBegin(outline?GL_LINE_LOOP:GL_TRIANGLES);
  glVertex2f(x2,y2);
  glVertex2f(xs-ts*(arrow_size/3),ys+tc*(arrow_size/3));
  glVertex2f(xs+ts*(arrow_size/3),ys-tc*(arrow_size/3));
  glEnd();
}

void draw_button(float x1,float y1,float x2,float y2,float border_width,bool up)
{
  texture_reset();
  if(x1>x2) {
    float t=x2;
    x2=x1;
    x1=t;
  }
  if(y1>y2) {
    float t=y2;
    y2=y1;
    y1=t;
  }
  if (x2-x1<border_width*2){border_width=(x2-x1)/2;}
  if (y2-y1<border_width*2){border_width=(y2-y1)/2;}
  glBegin(GL_QUADS);
    glVertex2f(x1,y1);
    glVertex2f(x2,y1);
    glVertex2f(x2,y2);
    glVertex2f(x1,y2);

    if (up == true){glColor4f(0.5,0.5,0.5,0.5);}else{glColor4f(1,1,1,0.5);}
    glVertex2f(x1+border_width,y2-border_width);
    glVertex2f(x2-border_width,y2-border_width);
    glVertex2f(x2,y2);
    glVertex2f(x1,y2);

    glVertex2f(x2-border_width,y1+border_width);
    glVertex2f(x2,y1);
    glVertex2f(x2,y2);
    glVertex2f(x2-border_width,y2-border_width);

    if (up == true){glColor4f(1,1,1,0.5);}else{glColor4f(0.5,0.5,0.5,0.5);}
    glVertex2f(x1,y1);
    glVertex2f(x2,y1);
    glVertex2f(x2-border_width,y1+border_width);
    glVertex2f(x1+border_width,y1+border_width);

    glVertex2f(x1,y1);
    glVertex2f(x1+border_width,y1+border_width);
    glVertex2f(x1+border_width,y2-border_width);
    glVertex2f(x1,y2);

  glEnd();
  glColor4ubv(enigma::currentcolor);
}

//Mind that health is 1-100
void draw_healthbar(float x1,float y1,float x2,float y2,float amount,int backcol,int mincol,int maxcol,int dir,bool showback,bool showborder)
{
  if(x1>x2) { // Swap them
    float t = x2;
    x2 = x1, x1 = t;
  }
  if(y1>y2) { // Swap them
    float t = y2;
    y2 = y1, y1 = t;
  }
  amount = amount>=100 ? 1 : (amount<=0 ? 0 : amount/100);

  texture_reset();
  if(showborder)
  {
    glColor4ub(__GETR(backcol),__GETG(backcol),__GETB(backcol),enigma::currentcolor[3]);
    glBegin(GL_LINE_LOOP);
      glVertex2f(x1-1,y1-1);
      glVertex2f(x1-1,y2+1);
      glVertex2f(x2+1,y2+1);
      glVertex2f(x2+1,y1-1);
    glEnd();
    if (showback)
      goto showback_yes;
    goto showback_no;
  }
  if(showback) {
      glColor4ub(__GETR(backcol),__GETG(backcol),__GETB(backcol),enigma::currentcolor[3]);
      showback_yes: glRectf(x1,y1,x2,y2);
  } showback_no:

  switch(dir) {
  case 1:x1=x2-(x2-x1)*amount;
  break;case 2:y2=y1+(y2-y1)*amount;
  break;case 3:y1=y2-(y2-y1)*amount;
  default:x2=x1+(x2-x1)*amount;
  }

  const int
      R = __GETR(mincol),
      G = __GETG(mincol),
      B = __GETB(mincol);

  glColor4ub(R+(unsigned char)((__GETR(maxcol)-R)*amount),G+(unsigned char)((__GETG(maxcol)-G)*amount),B+(unsigned char)((__GETB(maxcol)-B)*amount),enigma::currentcolor[3]);
  //printf("%d\n",mincol);
  glRectf(x1,y1,x2,y2);
  glColor4ubv(enigma::currentcolor);
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
    if (view_enabled)
    {
        x = x - view_xview[view_current];
        y = view_hview[view_current] - (y - view_yview[view_current]) - 1;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > view_wview[view_current] || y > view_hview[view_current]) return 0;
    }
    else
    {
        y = room_height - y - 1;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > room_width || y > room_height) return 0;
    }
  #if defined __BIG_ENDIAN__ || defined __BIG_ENDIAN
    int ret;
    glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,&ret);
    return ret;
  #elif defined __LITTLE_ENDIAN__ || defined __LITTLE_ENDIAN
    int ret;
    glReadPixels(x,y,1,1,GL_BGR,GL_UNSIGNED_BYTE,&ret);
    return ret>>8;
  #else
    unsigned char rgb[3];
    glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,&rgb);
    return rgb[0] | rgb[1] << 8 | rgb[2] << 16;
  #endif
}

int draw_mandelbrot(int x,int y,float w,double Zx,double Zy,double Zw,unsigned iter)
{
  int c=0;
  glBegin(GL_POINTS);
    for(int i=y; i<y+w; i++)
      for(int j=x;j<x+w;j++) {
        double zx=Zx+(j-x)*(Zw/w),zy=Zy+(i-y)*(Zw/w),cx=zx,cy=zy;
        for(unsigned k=0;k<iter;k++)
          if(zx*zx+zy*zy>=4) goto UNMANLY;
          else {
            zx = zx*zx - zy*zy + cx;
            zy = 2*zx*zy + cy;
          }
        glVertex2i(i,j);
        c++;
        UNMANLY:;
      }
    glEnd();
    return c;
}

}

