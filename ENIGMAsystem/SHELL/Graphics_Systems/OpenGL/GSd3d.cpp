/********************************************************************************\
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
\********************************************************************************/

using namespace std;

#include "OpenGLHeaders.h"
#include "GSprmtvs.h"
#include <string>
#include "../../Universal_System/roomsystem.h"
#include <math.h>

/*void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
   GLdouble xmin, xmax, ymin, ymax;

   ymax = zNear * tan(fovy * M_PI / 360.0);
   ymin = -ymax;
   xmin = ymin * aspect;
   xmax = ymax * aspect;


   glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}//I need to include GL/glu.h*/
//I need it for gluLookAt()


int d3d_start()
{
    glEnable(GL_DEPTH_TEST);
    gluPerspective(110, view_wview[view_current]/view_hview[view_current], 5, 6);

    return 0;
}

int d3d_end()
{
    glDisable(GL_DEPTH_TEST);
    glOrtho(0,room_width,0,room_height,0,1);

    return 0;
}

int d3d_primitive_begin(int kind)
{
    glBegin(kind);
    return 0;
}

int d3d_vertex(double x, double y, double z)
{
    glVertex3d(x,y,z);
    return 0;
}

int d3d_primitive_end()
{
    glEnd();
    return 0;
}

int d3d_set_projection(double xfrom,double yfrom,double zfrom,double xto,
                       double yto,double zto,double xup,double yup,double zup)
{
    gluLookAt(xfrom, yfrom, zfrom, xto, yto, zto, xup, yup, zup);
    return 0;
}
