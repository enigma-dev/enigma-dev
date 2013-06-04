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

#ifndef __DX10SHAPESH_
#define __DX10SHAPESH_
#include "../General/DirectXHeaders.h"
GLfloat* block_vertices(double x1, double y1, double z1, double x2, double y2, double z2);
GLfloat* block_indices(double x1, double y1, double z1, double x2, double y2, double z2);
#endif
