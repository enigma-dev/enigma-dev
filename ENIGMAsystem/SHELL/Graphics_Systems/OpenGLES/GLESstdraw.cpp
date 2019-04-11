/** Copyright (C) 2008-2013 Josh Ventura
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


#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include <math.h>
#include <stdio.h>

namespace enigma{
float circleprecision=24;
extern unsigned bound_texture;
extern unsigned char currentcolor[4];
}
#define untexture() if(enigma::bound_texture) glBindTexture(GL_TEXTURE_2D,enigma::bound_texture=0);

namespace enigma_user {

void draw_set_line_pattern(int pattern, int scale)
{
   /* if (pattern == -1){
        glDisable(GL_LINE_STIPPLE);
    }else{
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(scale,pattern);
    }*/
}

int draw_point(gs_scalar x, gs_scalar y)
{
	untexture();
/*	glBegin(GL_POINTS);
    glVertex2f(x,y);
	glEnd(); OPENGLES */
	return 0;
}

int draw_point_color(gs_scalar x, gs_scalar y, int col)
{
	untexture();
	/*glPushAttrib(GL_CURRENT_BIT);
	glColor4ub(COL_GET_R(col),COL_GET_G(col),COL_GET_B(col),enigma::currentcolor[3]);
	glBegin(GL_POINTS);
	glVertex2f(x,y);
	glEnd();
	glPopAttrib(); OPENGLES */
	return 0;
}

int draw_line(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2)
{
	untexture();
	/*glBegin(GL_LINES);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	glEnd(); OPENGLES */
	return 0;
}

int draw_line_width(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar width)
{
	untexture();
	/*glPushAttrib(GL_LINE_BIT);
	glLineWidth(width);
	glBegin(GL_LINES);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	glEnd();
	glPopAttrib(); OPENGLES */
	return 0;
}

int draw_line_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int c1, int c2)
{
	untexture();
	/*glBegin(GL_LINES);
    glColor4ub(COL_GET_R(c1),COL_GET_G(c1),COL_GET_B(c1),enigma::currentcolor[3]);
      glVertex2f(x1,y1);
    glColor4ub(COL_GET_R(c2),COL_GET_G(c2),COL_GET_B(c2),enigma::currentcolor[3]);
      glVertex2f(x2,y2);
	glEnd();
	glColor4ubv(enigma::currentcolor); OPENGLES */
	return 0;
}

int draw_line_width_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar width, int c1, int c2){
	untexture();
	/*glPushAttrib(GL_LINE_BIT);
    glLineWidth(width);
    glBegin(GL_LINES);
      glColor4ub(COL_GET_R(c1),COL_GET_G(c1),COL_GET_B(c1),enigma::currentcolor[3]);
      glVertex2f(x1,y1);
      glColor4ub(COL_GET_R(c2),COL_GET_G(c2),COL_GET_B(c2),enigma::currentcolor[3]);
      glVertex2f(x2,y2);
    glEnd();
	glPopAttrib();
	glColor4ubv(enigma::currentcolor); OPENGLES */
	return 0;
}

int draw_rectangle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, bool outline)
{
	untexture();
	/*if(outline)
	{
		glBegin(GL_LINE_LOOP);
      glVertex2f(x1,y1);
      glVertex2f(x1,y2);
      glVertex2f(x2,y2);
      glVertex2f(x2,y1);
		glEnd();
	}
	else glRectf(x1,y1,x2,y2); OPENGLES */
	return 0;
}

int draw_rectangle_angle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float angle, bool outline)
{
	untexture();
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

/*glBegin(outline ? GL_LINE_LOOP : GL_QUADS);
    glVertex2f(xm+ldx1,ym-ldy1);
    glVertex2f(xm+ldx2,ym-ldy2);
    glVertex2f(xm-ldx1,ym+ldy1);
    glVertex2f(xm-ldx2,ym+ldy2);
	glEnd(); OPENGLES*/
	return 0;
}

int draw_rectangle_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int c1, int c2, int c3, int c4, bool outline)
{
	untexture();
	/*glBegin(outline?GL_LINE_LOOP:GL_QUADS);
    glColor4ub(COL_GET_R(c1),COL_GET_G(c1),COL_GET_B(c1),enigma::currentcolor[3]);
      glVertex2f(x1,y1);
    glColor4ub(COL_GET_R(c2),COL_GET_G(c2),COL_GET_B(c2),enigma::currentcolor[3]);
      glVertex2f(x2,y1);
    glColor4ub(COL_GET_R(c4),COL_GET_G(c4),COL_GET_B(c4),enigma::currentcolor[3]);
      glVertex2f(x2,y2);
    glColor4ub(COL_GET_R(c3),COL_GET_G(c3),COL_GET_B(c3),enigma::currentcolor[3]);
      glVertex2f(x1,y2);
	glEnd();
	glColor4ubv(enigma::currentcolor); OPENGLES*/
	return 0;
}

int draw_set_circle_precision(float pr) {
	enigma::circleprecision = pr<3 ? 3 : pr;
	return 0;
}
float draw_get_circle_precision() {
	return enigma::circleprecision;
}

int draw_circle(gs_scalar x, gs_scalar y, float rad, bool outline)
{
	untexture();
	gs_scalar pr=2*M_PI/enigma::circleprecision;
	/*if(outline)
	{
		glBegin(GL_LINES);
		for(gs_scalar i=pr;i<M_PI/2;i+=pr)
		{
			float xc1=cos(i)*rad,yc1=sin(i)*rad;
			i+=pr;
			float xc2=cos(i)*rad,yc2=sin(i)*rad;
			glVertex2f(x+xc1,y+yc1); glVertex2f(x+xc2,y+yc2);
			glVertex2f(x-xc1,y+yc1); glVertex2f(x-xc2,y+yc2);
			glVertex2f(x+xc1,y-yc1); glVertex2f(x+xc2,y-yc2);
			glVertex2f(x-xc1,y-yc1); glVertex2f(x-xc2,y-yc2);
			glVertex2f(x+yc1,y+xc1); glVertex2f(x+yc2,y+xc2);
			glVertex2f(x-yc1,y+xc1); glVertex2f(x-yc2,y+xc2);
			glVertex2f(x+yc1,y-xc1); glVertex2f(x+yc2,y-xc2);
			glVertex2f(x-yc1,y-xc1); glVertex2f(x-yc2,y-xc2);
		}
	}
	else
	{
		glBegin(GL_QUADS);
		for(gs_scalar i=pr;i<M_PI/2;i+=pr){
			gs_scalar xc1=cos(i)*rad,yc1=sin(i)*rad;
			i+=pr;
			gs_scalar xc2=cos(i)*rad,yc2=sin(i)*rad;
			glVertex2f(x-xc1,y+yc1); glVertex2f(x+xc1,y+yc1); glVertex2f(x+xc2,y+yc2); glVertex2f(x-xc2,y+yc2);
			glVertex2f(x-xc1,y-yc1); glVertex2f(x+xc1,y-yc1); glVertex2f(x+xc2,y-yc2); glVertex2f(x-xc2,y-yc2);
			glVertex2f(x-yc1,y+xc1); glVertex2f(x+yc1,y+xc1); glVertex2f(x+yc2,y+xc2); glVertex2f(x-yc2,y+xc2);
			glVertex2f(x-yc1,y-xc1); glVertex2f(x+yc1,y-xc1); glVertex2f(x+yc2,y-xc2); glVertex2f(x-yc2,y-xc2);
		}
	}
	glEnd(); OPENGLES*/
	return 0;
}

int draw_circle_color(gs_scalar x, gs_scalar y, float rad, int c1, int c2, bool outline)
{
  untexture();
	/*if(outline)
	  glBegin(GL_LINE_STRIP);
	else
	{
		glBegin(GL_TRIANGLE_FAN);
      glColor4ub(COL_GET_R(c1),COL_GET_G(c1),COL_GET_B(c1),enigma::currentcolor[3]);
        glVertex2f(x,y);
	}
	//Bagan above
    glColor4ub(COL_GET_R(c2),COL_GET_G(c2),COL_GET_B(c2),enigma::currentcolor[3]);
      float pr=2*M_PI/enigma::circleprecision;
      glVertex2f(x+rad,y);
      for(float i=pr;i<2*M_PI;i+=pr)
        glVertex2f(x+rad*cos(i),y-rad*sin(i));
      glVertex2f(x+rad,y);
	glEnd();
	glColor4ubv(enigma::currentcolor); OPENGLES*/
	return 0;
}

int draw_circle_perfect(gs_scalar x, gs_scalar y, float rad, bool outline)
{
	untexture();
	gs_scalar r2=rad*rad,r12=rad*M_SQRT1_2;
	/*glBegin(outline?GL_POINTS:GL_LINES);
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
	glEnd(); */
    return 0;
}

int draw_circle_color_perfect(gs_scalar x, gs_scalar y, float rad, int c1, int c2, bool outline)
{
	untexture();
	gs_scalar r2=rad*rad;
	/*if(outline)
	{
		glBegin(GL_POINTS);
      glColor4ub(COL_GET_R(c2),COL_GET_G(c2),COL_GET_B(c2),enigma::currentcolor[3]);
        gs_scalar r12=rad*M_SQRT1_2;
        for(float xc=0,yc=rad;xc<=r12;xc++)
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
      glColor4ub(COL_GET_R(c1),COL_GET_G(c1),COL_GET_B(c1),enigma::currentcolor[3]);
        glVertex2f(x,y);
      glColor4ub(COL_GET_R(c2),COL_GET_G(c2),COL_GET_B(c2),enigma::currentcolor[3]);
        glVertex2f(x-rad,y);
        for(float xc=-rad+1;xc<r;xc++)
          glVertex2f(x+xc,y+sqrt(r2-(xc*xc)));
        for(float xc=rad;xc>-r;xc--)
          glVertex2f(x+xc,y-sqrt(r2-(xc*xc)));
        glVertex2f(x-rad,y);
	}
	glEnd();
	glColor4ubv(enigma::currentcolor); OPENGLES */
	return 0;
}

int draw_ellipse(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, bool outline)
{
	untexture();
	gs_scalar
		x=(x1+x2)/2,y=(y1+y2)/2,
		hr=fabs(x2-x),vr=fabs(y2-y),
		pr=2*M_PI/enigma::circleprecision;
	/*if(outline)
	{
		glBegin(GL_LINES);
		for(float i=pr;i<M_PI;i+=pr)
		{
			float xc1=cos(i)*hr,yc1=sin(i)*vr;
			i+=pr;
			float xc2=cos(i)*hr,yc2=sin(i)*vr;
			glVertex2f(x+xc1,y+yc1);glVertex2f(x+xc2,y+yc2);
			glVertex2f(x-xc1,y+yc1);glVertex2f(x-xc2,y+yc2);
			glVertex2f(x+xc1,y-yc1);glVertex2f(x+xc2,y-yc2);
			glVertex2f(x-xc1,y-yc1);glVertex2f(x-xc2,y-yc2);
		}
	}
	else
	{
		glBegin(GL_QUADS);
		for(float i=pr;i<M_PI;i+=pr)
		{
			float xc1=cos(i)*hr,yc1=sin(i)*vr;
			i+=pr;
			float xc2=cos(i)*hr,yc2=sin(i)*vr;
			glVertex2f(x-xc1,y+yc1);glVertex2f(x+xc1,y+yc1);glVertex2f(x+xc2,y+yc2);glVertex2f(x-xc2,y+yc2);
			glVertex2f(x-xc1,y-yc1);glVertex2f(x+xc1,y-yc1);glVertex2f(x+xc2,y-yc2);glVertex2f(x-xc2,y-yc2);
		}
	}
	glEnd(); OPENGLES*/
	return 0;
}

int draw_ellipse_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int c1, int c2, bool outline)
{
  untexture();
	float
		x=(x1+x2)/2,y=(y1+y2)/2,
		hr=fabs(x2-x),vr=fabs(y2-y),
		pr=2*M_PI/enigma::circleprecision;

	/*if(outline)
	  glBegin(GL_LINE_STRIP);
	else
	{
		glBegin(GL_TRIANGLE_FAN);
		glColor4ub(COL_GET_R(c1),COL_GET_G(c1),COL_GET_B(c1),enigma::currentcolor[3]);
		glVertex2f(x,y);
	}

    glColor4ub(COL_GET_R(c2),COL_GET_G(c2),COL_GET_B(c2),enigma::currentcolor[3]);
      glVertex2f(x+hr,0);
    for(float i=pr;i<2*M_PI;i+=pr)
      glVertex2f(x+hr*cos(i),y+vr*sin(i));
    glVertex2f(x+hr,0);
	glEnd();

	glColor4ubv(enigma::currentcolor); OPENGLES*/
	return 0;
}

int draw_ellipse_perfect(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, bool outline)
{
	untexture();
	float
		x=(x1+x2)/2,y=(y1+y2)/2,
		hr=fabs(x2-x),vr=fabs(y2-y);
	/*glBegin(outline?GL_POINTS:GL_LINES);
	for(float xc=0;xc<hr;xc++)
	{
		float yc=vr*cos((M_PI/2)/hr*xc);
		glVertex2f(x+xc,y+yc);
		glVertex2f(x+xc,y-yc);
		glVertex2f(x-xc,y+yc);
		glVertex2f(x-xc,y-yc);
	}
	glEnd(); OPENGLES*/
	return 0;
}

int draw_triangle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, bool outline)
{
	untexture();
	/*glBegin(outline?GL_LINE_LOOP:GL_TRIANGLES);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	glVertex2f(x3,y3);
	glEnd(); OPENGLES*/
	return 0;
}

int draw_triangle_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, int col1, int col2, int col3, bool outline)
{
	untexture();
	/*glBegin(outline?GL_LINE_LOOP:GL_TRIANGLES);
	glColor4ub(COL_GET_R(col1),COL_GET_G(col1),COL_GET_B(col1),enigma::currentcolor[3]);
	glVertex2f(x1,y1);
	glColor4ub(COL_GET_R(col2),COL_GET_G(col2),COL_GET_B(col2),enigma::currentcolor[3]);
	glVertex2f(x2,y2);
	glColor4ub(COL_GET_R(col3),COL_GET_G(col3),COL_GET_B(col3),enigma::currentcolor[3]);
	glVertex2f(x3,y3);
	glEnd();
	glColor4ubv(enigma::currentcolor); OPENGLES*/
	return 0;
}

int draw_roundrect(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float rad, bool outline)
{
    untexture();
    if(x1>x2)
    {
        float t=x2;
        x2=x1;
        x1=t;
    }
    if(y1>y2)
    {
        float t=y2;
        y2=y1;
        y1=t;
    }
    if (x2-x1<r*2){r=(x2-x1)/2;}
    if (y2-y1<r*2){r=(y2-y1)/2;}
    if (r<0){r=0;}
    gs_scalar r2=rad*rad,r12=r*M_SQRT1_2,
    bx1=x1+rad,by1=y1+rad,
    bx2=x2-rad,by2=y2-rad;
   /* glBegin(GL_LINES);
    if(outline)
    {
        glVertex2f(x1,by1);glVertex2f(x1,by2);
        glVertex2f(x2,by1);glVertex2f(x2,by2);
        glVertex2f(bx1,y1);glVertex2f(bx2,y1);
        glVertex2f(bx1,y2);glVertex2f(bx2,y2);
        glEnd();
        glBegin(GL_POINTS);
        for(float xc=0,yc=rad;xc<=r12;xc++)
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
        for(float xc=0,yc=rad;xc<=r12;xc++)
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
    }*/
    return 0;
}

int draw_roundrect_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float rad, int col1, int col2, bool outline)
{
    untexture();
    if(x1>x2)
    {
        float t=x2;
        x2=x1;
        x1=t;
    }
    if(y1>y2)
    {
        float t=y2;
        y2=y1;
        y1=t;
    }
    if (x2-x1<r*2){r=(x2-x1)/2;}
    if (y2-y1<r*2){r=(y2-y1)/2;}
    if (r<0){r=0;}
    gs_scalar r2=rad*rad,r12=r*M_SQRT1_2,
    bx1=x1+rad,by1=y1+rad,
    bx2=x2-rad,by2=y2-rad;
   /* glBegin(GL_LINES);
    if(outline)
    {
        glColor4ub(COL_GET_R(col2),COL_GET_G(col2),COL_GET_B(col2),enigma::currentcolor[3]);
        glVertex2f(x1,by1);glVertex2f(x1,by2);
        glVertex2f(x2,by1);glVertex2f(x2,by2);
        glVertex2f(bx1,y1);glVertex2f(bx2,y1);
        glVertex2f(bx1,y2);glVertex2f(bx2,y2);
        glEnd();
        glBegin(GL_POINTS);
        for(float xc=0,yc=rad;xc<=r12;xc++)
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
        glColor4ub(COL_GET_R(col2),COL_GET_G(col2),COL_GET_B(col2),enigma::currentcolor[3]);
        for(float xc=0,yc=rad;xc<=r12;xc++)
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
        glColor4ub(COL_GET_R(col1),COL_GET_G(col1),COL_GET_B(col1),enigma::currentcolor[3]);
        glVertex2f(x1+(x2-x1)/2,y1+(y2-y1)/2);
        glColor4ub(COL_GET_R(col2),COL_GET_G(col2),COL_GET_B(col2),enigma::currentcolor[3]);
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
    */ return 0;
}

int draw_arrow(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, gs_scalar arrow_size, gs_scalar line_size, bool outline)
{
    untexture();
    double dir = atan2(y2-y1,x2-x1);
    float tc = cos(dir), ts = sin(dir),
    xs = x2-tc*arrow_size, ys = y2-ts*arrow_size,
    lw = ts*(line_size/2), lh = tc*(line_size/2);
    double at = atan2(ys-y1,xs-x1);
  /*  if (fabs((dir<0?dir+2*M_PI:dir)-(at<0?at+2*M_PI:at)) < 0.01){
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
    */ return 0;
}

void draw_button(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar border_width, bool up)
{
    untexture();
    if(x1>x2)
    {
        float t=x2;
        x2=x1;
        x1=t;
    }
    if(y1>y2)
    {
        float t=y2;
        y2=y1;
        y1=t;
    }
    if (x2-x1<border_width*2){border_width=(x2-x1)/2;}
    if (y2-y1<border_width*2){border_width=(y2-y1)/2;}
   /* glBegin(GL_QUADS);
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
    glColor4ubv(enigma::currentcolor);*/
}

//Mind that health is 1-100
int draw_healthbar(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float amount, int backcol, int mincol, int maxcol, int dir, bool showback, bool showborder)
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

	untexture();
	/*if(showborder)
	{
		glColor4ub(COL_GET_R(backcol),COL_GET_G(backcol),COL_GET_B(backcol),enigma::currentcolor[3]);
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
		glColor4ub(COL_GET_R(backcol),COL_GET_G(backcol),COL_GET_B(backcol),enigma::currentcolor[3]);
		showback_yes: glRectf(x1,y1,x2,y2);
	} showback_no:

	switch(dir) {
    case 1:x1=x2-(x2-x1)*amount;
    break;case 2:y2=y1+(y2-y1)*amount;
    break;case 3:y1=y2-(y2-y1)*amount;
    default:x2=x1+(x2-x1)*amount;
	}

	const int
		R = COL_GET_R(mincol),
		G = COL_GET_G(mincol),
		B = COL_GET_B(mincol);

	glColor4ub(R+(unsigned char)((COL_GET_R(maxcol)-R)*amount),G+(unsigned char)((COL_GET_G(maxcol)-G)*amount),B+(unsigned char)((COL_GET_B(maxcol)-B)*amount),enigma::currentcolor[3]);
	printf("%d\n",mincol);
	glRectf(x1,y1,x2,y2);
	glColor4ubv(enigma::currentcolor);OPENGLES */
	return 0;
}

int draw_getpixel(int x, int y)
{
  #ifdef __BIG_ENDIAN__
    glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,&x);
    return x;
  #elif defined __LITTLE_ENDIAN__
   // glReadPixels(x,y,1,1,GL_BGR,GL_UNSIGNED_BYTE,&x); //OPENGLES
    return x>>8;
  #else
    char r,g,b;
    //glReadPixels(x,y,1,1,GL_RED,GL_UNSIGNED_BYTE,&r);
    //glReadPixels(x,y,1,1,GL_GREEN,GL_UNSIGNED_BYTE,&g);
    //glReadPixels(x,y,1,1,GL_BLUE,GL_UNSIGNED_BYTE,&b);
    return r|(g<<8)|(b<<16);
  #endif
}

int draw_mandelbrot(int x, int y, float w, double Zx, double Zy, double Zw, unsigned iter)
{
	int c=0;
	/*glBegin(GL_POINTS);
	for(int i=y; i<y+w; i++)
		for(int j=x;j<x+w;j++){
			double zx=Zx+(j-x)*(Zw/w),zy=Zy+(i-y)*(Zw/w),cx=zx,cy=zy;
			for(unsigned k=0;k<iter;k++)
				if(zx*zx+zy*zy>=4) goto UNMANLY;
				else{
					zx=zx*zx-zy*zy+cx;
					zy=2*zx*zy+cy;
				}
			glVertex2i(i,j);
			c++;
			UNMANLY:;
		}
	glEnd(); OPENGLES*/
	return c;
}
}
