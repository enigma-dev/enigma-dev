/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

// ***** Information BEGIN *****
/*
   Everything must use Vertex Buffer Object's in the new graphics system's, the old one also needs to use similiar
   shape functions for obtaining data for Vertex Array pointer's. Nothing can use immediate mode anymore.
   These are the basic shappes...

   Plane : Walls/Floors/Sprites
   Cube
   Uvsphere
   Icosahedron
   Cone
   Cylinder
   Torus
*/
// ***** Information END *****

#ifndef __GLSHAPESH_
#define __GLSHAPESH_
#include "Universal_System/scalar.h"
#include "../General/OpenGLHeaders.h"

//No need for additional parameters, as a plane will always have 4 vertices and 8 attributes (x,y,tx,ty,r,g,b,a) for each (so the size is 4*8)
//Indices will be automatically filled in order - top left, top right, bottom right, bottom right, bottom left, top left
void plane2D_rotated(const gs_scalar data[]);

//GLfloat* block_vertices(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2);
//GLfloat* block_indices(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2);
#endif
