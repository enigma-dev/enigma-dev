/*********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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
\*********************************************************************************/

/**Standard drawing functions**************************************************\

int draw_point(double x, double y)
int draw_point_color(double x, double y,double color)
int draw_line(double x1,double y1,double x2,double y2)
int draw_line_width(double x1,double y1,double x2,double y2,double width)
int draw_line_color(double x1,double y1,double x2,double y2,double color1,double color2)
int draw_line_width_color(double x1,double y1,double x2,double y2,double width,double color1,double color2)
int draw_rectangle(double x1,double y1,double x2,double y2,double outline)
int draw_rectangle_angle(double x1,double y1,double x2,double y2,double outline,double angle)
int draw_rectangle_color(double x1,double y1,double x2,double y2,double color1,double color2,double color3,double color4,double outline)
int draw_set_circle_precision(double precision)
int draw_circle(double x, double y, double radius, double outline)
int draw_circle_color(double x, double y, double radius, double color1, double color2, double outline)
int draw_circle_smooth(double x, double y, double radius, double outline)
int draw_circle_smooth_color(double x, double y, double radius, double outline, double color1, double color2)
int draw_ellipse(double x1,double y1,double x2,double y2,double outline)
int draw_triangle(double x1,double y1,double x2,double y2,double x3,double y3,double outline)
int draw_triangle_color(double x1,double y1,double x2,double y2,double x3,double y3,double outline)
int draw_roundrect(double x1,double y1,double x2,double y2,double outline)

\******************************************************************************/




int screen_redraw(int dontswap=0);

int draw_point(double x, double y)
{
    enigma::untexture();
    glBegin(GL_POINTS);
    glVertex2f(x,y);
    glEnd();
    return 0;
}

int draw_point_color(double x, double y,double color)
{
    enigma::untexture();
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(__GETR(color)/255.0,__GETG(color)/255.0,__GETB(color)/255.0,enigma::currentcolor[3]);
    glBegin(GL_POINTS);
    glVertex2f(x,y);
    glEnd();
    glPopAttrib();
    return 0;
}

int draw_line(double x1,double y1,double x2,double y2)
{
        enigma::untexture();

        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();

        return 0;
}

int draw_line_width(double x1,double y1,double x2,double y2,double width)
{
        enigma::untexture();
        double dir=point_direction(x1,y1,x2,y2)+90;

        double DLWldx= cos(dir/180.0*pi)*(width/2);
        double DLWldy=-sin(dir/180.0*pi)*(width/2);

        glBegin(GL_QUADS);
        glVertex2f(x1+DLWldx, y1+DLWldy);
        glVertex2f(x1-DLWldx, y1-DLWldy);
        glVertex2f(x2-DLWldx, y2-DLWldy);
        glVertex2f(x2+DLWldx, y2+DLWldy);
        glEnd();

        return 0;
}

int draw_line_color(double x1,double y1,double x2,double y2,double color1,double color2)
{
    enigma::untexture();
      glPushAttrib(GL_CURRENT_BIT);
        glBegin(GL_LINES);
        glColor4f(__GETR(color1)/255.0,__GETG(color1)/255.0,__GETB(color1)/255.0,enigma::colorArray[3]);
        glVertex2f(x1, y1);
        glColor4f(__GETR(color2)/255.0,__GETG(color2)/255.0,__GETB(color2)/255.0,enigma::colorArray[3]);
        glVertex2f(x2, y2);
      glEnd();
    glPopAttrib();
    return 0;
}

int draw_line_width_color(double x1,double y1,double x2,double y2,double width,double color1,double color2)
{
    enigma::untexture();
    double dir=point_direction(x1,y1,x2,y2)+90;
    double DLWldx= cos(dir/180.0*pi)*(width/2);
    double DLWldy=-sin(dir/180.0*pi)*(width/2);

    glPushAttrib(GL_CURRENT_COLOR);
      glBegin(GL_QUADS);
         glColor4f(__GETR(color1)/255.0,__GETG(color1)/255.0,__GETB(color1)/255.0,enigma::colorArray[3]);
          glVertex2f(x1+DLWldx, y1+DLWldy);
          glVertex2f(x1-DLWldx, y1-DLWldy);
         glColor4f(__GETR(color2)/255.0,__GETG(color2)/255.0,__GETB(color2)/255.0,enigma::colorArray[3]);
          glVertex2f(x2-DLWldx, y2-DLWldy);
          glVertex2f(x2+DLWldx, y2+DLWldy);
      glEnd();
    glPopAttrib();
    return 0;
}

int draw_rectangle(double x1,double y1,double x2,double y2,double outline)
{
   enigma::untexture();
   if (!outline)
   {
     glRectd(x1,y1,x2,y2);
     return 0;
   }
   glBegin(GL_LINE_LOOP);
     glVertex2f(x1,y1);
     glVertex2f(x1,y2);
     glVertex2f(x2,y2);
     glVertex2f(x2,y1);
   glEnd();
   return 0;
}

int draw_rectangle_angle(double x1,double y1,double x2,double y2,int outline,double angle)
{
   enigma::untexture();
   double len,dir,ldx,ldy,ldx2,ldy2,xm,ym;

   xm=(x2+x1)/2;
   ym=(y2+y1)/2;

   len=sqrt((xm-x1)*(xm-x1)+(ym-y1)*(ym-y1));
   dir=(int(180+180*(1-atan2((y1-ym),(x1-xm))/pi))) % 360;
     ldx=len*cos(pi/180*(dir+angle));
     ldy=len*-sin(pi/180*(dir+angle));
   dir=(int(180+180*(1-atan2((y2-ym),(x1-xm))/pi))) % 360;
     ldx2=len*cos(pi/180*(180-dir+angle));
     ldy2=len*-sin(pi/180*(180-dir+angle));

   glBegin((GL_LINE_LOOP*outline) + (GL_QUADS * !outline));
     glVertex2f(xm+ldx,ym+ldy);
     glVertex2f(xm+ldx2,ym+ldy2);
     glVertex2f(xm-ldx,ym-ldy);
     glVertex2f(xm-ldx2,ym-ldy2);
   glEnd();

   return 0;
}



int draw_rectangle_color(double x1,double y1,double x2,double y2,double color1,double color2,double color3,double color4,int outline)
{
        enigma::untexture();
        glPushAttrib(GL_CURRENT_COLOR);

        glBegin(GL_LINE_LOOP*outline | GL_QUADS * !outline);
          glColor4f(__GETR(color1)/255.0,__GETG(color1)/255.0,__GETB(color1)/255.0,enigma::currentcolor[3]);
            glVertex2f(x1,y1);
          glColor4f(__GETR(color2)/255.0,__GETG(color2)/255.0,__GETB(color2)/255.0,enigma::currentcolor[3]);
            glVertex2f(x2,y1);
          glColor4f(__GETR(color4)/255.0,__GETG(color4)/255.0,__GETB(color4)/255.0,enigma::currentcolor[3]);
            glVertex2f(x2,y2);
          glColor4f(__GETR(color3)/255.0,__GETG(color3)/255.0,__GETB(color3)/255.0,enigma::currentcolor[3]);
            glVertex2f(x1,y2);
        glEnd();

        glPopAttrib();
        return 0;
}



namespace enigma
{
  int circleprecision=24;
}

int draw_set_circle_precision(int precision)
{
    enigma::circleprecision=precision*(precision>=3)+3*(precision<3);
    return 0;
}

int draw_circle(double x, double y, double radius, double outline)
{
    enigma::untexture();
    if (!outline) {glBegin(GL_TRIANGLE_FAN); glVertex2f(x,y); }
    else          {glBegin(GL_LINE_STRIP);                    }
    for (int i=0; i<=360; i+=360/enigma::circleprecision)
    {
        glVertex2f(x+radius*cos(i/180.0*pi),y-sin(i/180.0*pi)*radius);
    }   glEnd();

    return 0;
}
int draw_circle_color(double x, double y, double radius, double color1, double color2, double outline)
{
    enigma::untexture();
    glPushAttrib(GL_CURRENT_COLOR);
    if (!outline) {glBegin(GL_TRIANGLE_FAN); glColor4f(__GETR(color1)/255.0,__GETG(color1)/255.0,__GETB(color1)/255.0,enigma::currentcolor[3]); glVertex2f(x,y); }
    else          {glBegin(GL_LINE_STRIP);   }
    glColor4f(__GETR(color2)/255.0,__GETG(color2)/255.0,__GETB(color2)/255.0,enigma::currentcolor[3]);
    for (int i=0; i<=360; i+=360/enigma::circleprecision)
    {
        glVertex2f(x+radius*cos(i/180.0*pi),y-sin(i/180.0*pi)*radius);
    }   glEnd();
    glPopAttrib();

    return 0;
}



int draw_circle_smooth(double x, double y, double radius, int outline)
{
    enigma::untexture();

    double radius2=radius*radius;

    if (!outline) { glBegin(GL_TRIANGLE_FAN); glVertex2f(x,y); }
    else          { glBegin(GL_LINE_STRIP);                    }
    for (double xcur=-radius; xcur<=radius; xcur++)
    {
        glVertex2f(x+xcur,y+sqrt(radius2-(xcur*xcur)));
    }   glEnd();

    if (!outline) { glBegin(GL_TRIANGLE_FAN); glVertex2f(x,y); }
    else          { glBegin(GL_LINE_STRIP);                    }
    for (double xcur=-radius; xcur<=radius; xcur++)
    {
        glVertex2f(x+xcur,y-sqrt(radius2-(xcur*xcur)));
    }   glEnd();

    return 0;
}
int draw_circle_smooth_color(double x, double y, double radius, double outline, double color1, double color2)
{
    enigma::untexture();
    glPushAttrib(GL_CURRENT_COLOR);

    double radius2=radius*radius;

    if (!outline) { glBegin(GL_TRIANGLE_FAN); glColor4f(__GETR(color1)/255.0,__GETG(color1)/255.0,__GETB(color1)/255.0,enigma::currentcolor[3]); glVertex2f(x,y); }
    else          { glBegin(GL_LINE_STRIP);                    }
    glColor4f(__GETR(color2)/255.0,__GETG(color2)/255.0,__GETB(color2)/255.0,enigma::currentcolor[3]);
    for (double xcur=-radius; xcur<=radius; xcur++)
    {
        glVertex2f(x+xcur,y+sqrt(radius2-(xcur*xcur)));
    }   glEnd();

    if (!outline) { glBegin(GL_TRIANGLE_FAN); glColor4f(__GETR(color1)/255.0,__GETG(color1)/255.0,__GETB(color1)/255.0,enigma::currentcolor[3]); glVertex2f(x,y); }
    else          { glBegin(GL_LINE_STRIP);                    }
    glColor4f(__GETR(color2)/255.0,__GETG(color2)/255.0,__GETB(color2)/255.0,enigma::currentcolor[3]);
    for (double xcur=-radius; xcur<=radius; xcur++)
    {
        glVertex2f(x+xcur,y-sqrt(radius2-(xcur*xcur)));
    }   glEnd();

    glPopAttrib();

    return 0;
}


int draw_ellipse(double x1,double y1,double x2,double y2,double outline)
{
  enigma::untexture();
  double x = (x1+x2)/2;
  double y = (y1+y2)/2;
  double hr = fabs(x2-x);
  double vr = fabs(y2-y);

  if (outline) { glBegin(GL_LINE_STRIP); }
  else { glBegin(GL_TRIANGLE_FAN); glVertex2f(x,y); }
  for (int i=0;i<=enigma::circleprecision;i++)
  {
   	glVertex2f(x+hr*cos(2*pi*i/enigma::circleprecision),y+vr*sin(2*pi*i/enigma::circleprecision));
  }
  glEnd();

  return 0;
}

int draw_triangle(double x1,double y1,double x2,double y2,double x3,double y3,double outline)
{
    enigma::untexture();
    bool ol=(bool)outline;
    glBegin(GL_TRIANGLES*(!ol)+GL_LINE_LOOP*ol);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glVertex2f(x3,y3);
    glEnd();

    return 0;
}


int draw_triangle_color(double x1,double y1,double x2,double y2,double x3,double y3,double col1,double col2,double col3,double outline)
{
    enigma::untexture();
    bool ol=(bool)outline;
    glPushAttrib(GL_CURRENT_BIT);
      glBegin(GL_TRIANGLES*(!ol)+GL_LINE_LOOP*ol);
       glColor4f(__GETR(col1)/255.0,__GETG(col1)/255.0,__GETB(col1)/255.0,enigma::currentcolor[3]);
        glVertex2f(x1,y1);
       glColor4f(__GETR(col2)/255.0,__GETG(col2)/255.0,__GETB(col2)/255.0,enigma::currentcolor[3]);
        glVertex2f(x2,y2);
       glColor4f(__GETR(col2)/255.0,__GETG(col2)/255.0,__GETB(col2)/255.0,enigma::currentcolor[3]);
        glVertex2f(x3,y3);
      glEnd();
    glPopAttrib();

    return 0;
}


int draw_roundrect(double x1,double y1,double x2,double y2,double outline)
{
    enigma::untexture();
    //double mx1 = min(x1,x2), mx2 = std::max(x1,x2);
    //double my1 = min(y1,y2), my2 = std::max(y1,y2);
    double hr = min(4.0,(x2-x1)/2.0);
    double vr = min(4.0,(y2-y1)/2.0);

    double precision = (int)(enigma::circleprecision/4.0+0.5);

    int i;
    double xx,yy;

    if (outline)
    {
       glBegin(GL_LINE_STRIP);
       //right side
       draw_vertex(x2,y2-vr);
       draw_vertex(x2,y1+vr);
       //topright corner
       for (i=1;i<precision;i+=1)
       {
           xx = x2-hr+hr*cos(-(i/precision)*0.5*pi);
           yy = y1+vr+vr*sin(-(i/precision)*0.5*pi);
           draw_vertex(xx,yy);
       }
      //top side
      glVertex2f(x2-hr,y1);
      glVertex2f(x1+hr,y1);
      //topleft corner
      for (i=1;i<precision;i+=1)
      {
          xx = x1+hr+hr*cos(-(i/precision+1)*0.5*pi);
          yy = y1+vr+vr*sin(-(i/precision+1)*0.5*pi);
          glVertex2f(xx,yy);
      }
      //left side
      glVertex2f(x1,y1+vr);
      glVertex2f(x1,y2-vr);
      //bottomleft corner
      for (i=1;i<precision;i+=1)
      {
          xx = x1+hr+hr*cos(-(i/precision+2)*0.5*pi);
          yy = y2-vr+vr*sin(-(i/precision+2)*0.5*pi);
          glVertex2f(xx,yy);
      }
      //bottom side
      glVertex2f(x1+hr,y2);
      glVertex2f(x2-hr,y2);
      //bottomright corner
      for (i=1;i<=precision;i+=1)
      {
          xx = x2-hr+hr*cos(-(i/precision+3)*0.5*pi);
          yy = y2-vr+vr*sin(-(i/precision+3)*0.5*pi);
          glVertex2f(xx,yy);
      }
      glEnd();
    }
    else
    {
        glBegin(GL_TRIANGLES);
        //middle part
        glVertex2f(x1+hr,y1+vr);
        glVertex2f(x2-hr,y1+vr);
        glVertex2f(x1+hr,y2-vr);

        glVertex2f(x2-hr,y1+vr);
        glVertex2f(x1+hr,y2-vr);
        glVertex2f(x2-hr,y2-vr);
        //the other parts
        glVertex2f(x1,y1+vr);
        glVertex2f(x1+hr,y1+vr);
        glVertex2f(x1,y2-vr);

        glVertex2f(x1+hr,y1+vr);
        glVertex2f(x1,y2-vr);
        glVertex2f(x1+hr,y2-vr);

        glVertex2f(x2,y1+vr);
        glVertex2f(x2-hr,y1+vr);
        glVertex2f(x2,y2-vr);

        glVertex2f(x2-hr,y1+vr);
        glVertex2f(x2,y2-vr);
        glVertex2f(x2-hr,y2-vr);

        glVertex2f(x1+hr,y1);
        glVertex2f(x1+hr,y1+vr);
        glVertex2f(x2-hr,y1);

        glVertex2f(x1+hr,y1+vr);
        glVertex2f(x2-hr,y1);
        glVertex2f(x2-hr,y1+vr);

        glVertex2f(x1+hr,y2);
        glVertex2f(x1+hr,y2-vr);
        glVertex2f(x2-hr,y2);

        glVertex2f(x1+hr,y2-vr);
        glVertex2f(x2-hr,y2);
        glVertex2f(x2-hr,y2-vr);
        //topright corner
        for (i=0;i<precision;i+=1)
        {
            glVertex2f(x2-hr,y1+vr);
            xx = x2-hr+hr*cos(-(i/precision)*0.5*pi);
            yy = y1+vr+vr*sin(-(i/precision)*0.5*pi);
            glVertex2f(xx,yy);
            xx = x2-hr+hr*cos(-((i+1)/precision)*0.5*pi);
            yy = y1+vr+vr*sin(-((i+1)/precision)*0.5*pi);
            glVertex2f(xx,yy);
        }
        //topleft corner
        for (i=0;i<precision;i+=1)
        {
            glVertex2f(x1+hr,y1+vr);
            xx = x1+hr+hr*cos(-(i/precision+1)*0.5*pi);
            yy = y1+vr+vr*sin(-(i/precision+1)*0.5*pi);
            glVertex2f(xx,yy);
            xx = x1+hr+hr*cos(-((i+1)/precision+1)*0.5*pi);
            yy = y1+vr+vr*sin(-((i+1)/precision+1)*0.5*pi);
            glVertex2f(xx,yy);
        }
        //bottomleft corner
        for (i=0;i<precision;i+=1)
        {
            glVertex2f(x1+hr,y2-vr);
            xx = x1+hr+hr*cos(-(i/precision+2)*0.5*pi);
            yy = y2-vr+vr*sin(-(i/precision+2)*0.5*pi);
            glVertex2f(xx,yy);
            xx = x1+hr+hr*cos(-((i+1)/precision+2)*0.5*pi);
            yy = y2-vr+vr*sin(-((i+1)/precision+2)*0.5*pi);
            glVertex2f(xx,yy);
        }
        //bottomright corner
        for (i=0;i<precision;i+=1)
        {
            glVertex2f(x2-hr,y2-vr);
            xx = x2-hr+hr*cos(-(i/precision+3)*0.5*pi);
            yy = y2-vr+vr*sin(-(i/precision+3)*0.5*pi);
            glVertex2f(xx,yy);
            xx = x2-hr+hr*cos(-((i+1)/precision+3)*0.5*pi);
            yy = y2-vr+vr*sin(-((i+1)/precision+3)*0.5*pi);
            glVertex2f(xx,yy);
        }
        glEnd();
    }

    return 0;
}

int draw_healthbar(double x1,double y1,double x2,double y2,double amount,double backcol,double mincol,double maxcol,int direction,int showback,int showborder)
{
	amount=amount>100?100:(amount<0?0:amount);
    if (!(direction > -1 && direction < 4)) { direction = 0; }
        enigma::untexture();
            glPushAttrib(GL_CURRENT_COLOR);
                glBegin(GL_LINE_LOOP*showborder | GL_QUADS * !showborder );
                  glColor4f(__GETR(0)/255.0,__GETG(0)/255.0,__GETB(0)/255.0,enigma::currentcolor[3]);
                    glVertex2f(x1,y1);
                    glVertex2f(x2,y1);
                    glVertex2f(x2,y2);
                    glVertex2f(x1,y2);
                glEnd();
        if (showborder>0)
        {
            x1+=1;
            y1+=1;
        }
        if (showback>0)
        {
            glBegin(GL_LINE_LOOP | GL_QUADS );
			glColor4f(__GETR(backcol)/255.0,__GETG(backcol)/255.0,__GETB(backcol)/255.0,enigma::currentcolor[3]);
			glVertex2f(x1,y1);
			glVertex2f(x2,y1);
			glVertex2f(x2,y2);
			glVertex2f(x1,y2);
            glEnd();
        }
        switch (direction)
        {
         case 0:x2 = x1+((x2-x1)*(amount/100));
         break;
         case 1:x1 = x2-((x2-x1)*(amount/100));
         break;
         case 2:y2 = y1+((y2-y1)*(amount/100));
         break;
         case 3:y1 = y1+((y2-y1)*((100-amount)/100));
         break;
        }
        double colDraw = mincol;
        int Rl,Gl,Bl,Rh,Gh,Bh;
        Rh = __GETR(maxcol);
        Gh = __GETG(maxcol);
        Bh = __GETB(maxcol);
        Rl = __GETR(mincol);
        Gl = __GETG(mincol);
        Bl = __GETB(mincol);
        Rl = (int)(Rl+((Rh-Rl)*(amount/100)));
        Gl = (int)(Gl+((Gh-Gl)*(amount/100)));
        Bl = (int)(Bl+((Bh-Bl)*(amount/100)));
        colDraw = ((Bl<<16)|(Gl<<8)|Rl);
            glBegin(GL_QUADS );
			glColor4f(__GETR(colDraw)/255.0,__GETG(colDraw)/255.0,__GETB(colDraw)/255.0,enigma::currentcolor[3]);
			glVertex2f(x1,y1);
			glVertex2f(x2,y1);
			glVertex2f(x2,y2);
			glVertex2f(x1,y2);
            glEnd();
        if (showborder>0){
            glBegin(GL_LINES);
              glColor3f(0,0,0);

                switch (direction)
                {
                 case 0:
                     glVertex2f(x2,y2);
                     glVertex2f(x2,y1);
                 break;
                 case 1:
                     glVertex2f(x1,y1);
                     glVertex2f(x1,y2);
                 break;
                 case 2:
                     glVertex2f(x2,y2);
                     glVertex2f(x1,y2);
                 break;
                 case 3:
                     glVertex2f(x1,y1);
                     glVertex2f(x2,y1);
                 break;
                }
            glEnd();
        }
        glPopAttrib();
    return 0;
}
