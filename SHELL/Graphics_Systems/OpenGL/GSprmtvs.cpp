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

/**Primitive functions*********************************************************\

pr_pointlist, pr_linelist, pr_linestrip, pr_trianglelist, pr_trianglestrip, 
pr_trianglefan, pr_lineloop, pr_quadlist, pr_quadstrip, pr_polygon

int draw_primitive_begin(double kind)
int draw_vertex(double x, double y)
int draw_vertex_color(double x, double y, double color, double alpha)
int draw_primitive_end()

\******************************************************************************/

#include <GL/gl.h>

#if PRIMBUFFER
unsigned char __primitivetype[PRIMDEPTH2];
int __primitivelength[PRIMDEPTH2];
unsigned char __primitivecolor[PRIMBUFFER][PRIMDEPTH2][3];
unsigned char __primitivealpha[PRIMBUFFER][PRIMDEPTH2];
int __primitivex[PRIMBUFFER][PRIMDEPTH2];
int __primitivey[PRIMBUFFER][PRIMDEPTH2];
int __currentpcount[PRIMDEPTH2];
int __currentpdepth;
#endif

namespace enigma
{ extern void untexture(); }

int draw_primitive_begin(double kind)
{
     enigma::untexture();
     switch ((int)kind)
     {
        case 1:
             kind=GL_POINTS;
             break;
        case 2:
             kind=GL_LINES;
             break;
        case 3:
             kind=GL_LINE_STRIP;
             break;
        case 4:
             kind=GL_TRIANGLES;
             break;
        case 5:
             kind=GL_TRIANGLE_STRIP;
             break;
        case 6:
             kind=GL_TRIANGLE_FAN;
             break;
        case 7:
             kind=GL_LINE_LOOP;
             break;
        case 8:
             kind=GL_QUADS;
             break;
        case 9:
             kind=GL_QUAD_STRIP;
             break;
        case 10:
             kind=GL_POLYGON;
             break;
        default:
             kind=GL_POINTS;
             break;
     }
     #if !PRIMBUFFER
     glBegin((int)kind);
     #else
     __currentpdepth++;
     
     if (__currentpdepth>PRIMDEPTH2)
     {
         #if SHOWERRORS
         show_error("Max number of open primitives exceeded. You can disable the limit by unchecking the buffer option in settings, or increasing the depth to buffer.",0);
         #endif
         return -1;
     }
     
     __currentpcount[__currentpdepth]=0;
     __primitivetype[__currentpdepth]=kind;
     #endif
     return 0;
}
int draw_vertex(double x, double y)
{
     #if !PRIMBUFFER
     glVertex2f(x, y);
     #else
     float DVcol[4];
     glGetFloatv(GL_CURRENT_COLOR,DVcol);
     enigma::untexture();
     
     __primitivecolor[__currentpcount[__currentpdepth]][__currentpdepth][0]=(unsigned char)(DVcol[0]*255);
     __primitivecolor[__currentpcount[__currentpdepth]][__currentpdepth][1]=(unsigned char)(DVcol[1]*255);
     __primitivecolor[__currentpcount[__currentpdepth]][__currentpdepth][2]=(unsigned char)(DVcol[2]*255);
     __primitivealpha[__currentpcount[__currentpdepth]][__currentpdepth]=(unsigned char) (DVcol[3]*255);
     __primitivex[__currentpcount[__currentpdepth]][__currentpdepth]=(int) x;
     __primitivey[__currentpcount[__currentpdepth]][__currentpdepth]=(int) y;
     __currentpcount[__currentpdepth]++;
     
     if (__currentpcount[__currentpdepth]>PRIMBUFFER)
     {
         #if SHOWERRORS
         show_error("Max point count exceeded.",0);
         #endif
         return 0;
     }
     #endif
     return 0;
}
int draw_vertex_color(double x, double y, double color, double alpha)
{
     #if !PRIMBUFFER
     glPushAttrib(GL_CURRENT_BIT);
     enigma::untexture();
     
     color=(int)color&0xFFFFFF;
     
     glColor4f(
               ((int)color^(((int)((int)color >> 8)) << 8))/255.0,
               (((int)((int)color>>8))^(((int)((int)color>>16))<<8))/255.0,
               (((int)((int)color>>16)))/255.0,
               alpha
              );   
     glVertex2f(x, y); 
     glPopAttrib();
     #else
     __primitivecolor[__currentpcount[__currentpdepth]][__currentpdepth][0]=(unsigned char) ((int)color^(((int)((int)color >> 8)) << 8));
     __primitivecolor[__currentpcount[__currentpdepth]][__currentpdepth][1]=(unsigned char) (((int)((int)color >> 8))^(((int)((int)color>>16))<<8));
     __primitivecolor[__currentpcount[__currentpdepth]][__currentpdepth][2]=(unsigned char) (((int)((int)color >> 16)));
     __primitivealpha[__currentpcount[__currentpdepth]][__currentpdepth]=(unsigned char) (alpha*255);
     __primitivex[__currentpcount[__currentpdepth]][__currentpdepth]=(int) x;
     __primitivey[__currentpcount[__currentpdepth]][__currentpdepth]=(int) y;
     
     __currentpcount[__currentpdepth]++;
     
     if (__currentpcount[__currentpdepth]>PRIMBUFFER)
     {
         #if SHOWERRORS
         show_error("Max point count exceeded.",0);
         #endif
         return 0;
     }
     #endif
     
     return 0;
}
int draw_primitive_end()
{
     enigma::untexture();
     
     #if PRIMBUFFER
     glPushAttrib(GL_CURRENT_BIT);
     
     glBegin(__primitivetype[__currentpdepth]);
     int DPEinx;
     for (DPEinx=0;DPEinx<__currentpcount[__currentpdepth];DPEinx++)
     {
         glColor4f(
                  __primitivecolor[DPEinx][__currentpdepth][0]/255.0,
                  __primitivecolor[DPEinx][__currentpdepth][1]/255.0,
                  __primitivecolor[DPEinx][__currentpdepth][2]/255.0,
                   ((int)__primitivealpha[DPEinx][__currentpdepth])/255.0
                   );
         glVertex2f(__primitivex[DPEinx][__currentpdepth],__primitivey[DPEinx][__currentpdepth]);
         
         
         __primitivecolor[DPEinx][__currentpdepth][0]=0;
         __primitivecolor[DPEinx][__currentpdepth][1]=0;
         __primitivecolor[DPEinx][__currentpdepth][2]=0;
         __primitivealpha[DPEinx][__currentpdepth]=0;
         __primitivex[DPEinx][__currentpdepth]=0;
         __primitivey[DPEinx][__currentpdepth]=0;
     }
     __currentpcount[__currentpdepth]=0;
     __primitivetype[__currentpdepth]=0;
     __primitivelength[__currentpdepth]=0;
     __currentpdepth-=1;
     
     glPopAttrib();
     #endif
     glEnd();
     
     return 0;
}
