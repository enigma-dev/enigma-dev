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

#include "DX9shapes.h"

gs_scalar* block_vertices(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2)
{
 //{ x2, y2, z1, x2, y2, z2, x2, y1, z1, x2, y1, z2}
  static gs_scalar verts[24] = {x2, y1, z1, x2, y1, z2};
  verts[0]=x1;verts[1]=y1;verts[2]=z1; verts[3]=x1;verts[4]=y1;verts[5]=z2; verts[6]=x1;verts[7]=y2;verts[8]=z1;
  verts[9]=x1;verts[10]=y2;verts[11]=z2; verts[12]=x2;verts[13]=y2;verts[14]=z1; verts[15]=x2;verts[16]=y2;verts[17]=z2;
  verts[18]=x2;verts[19]=y1;verts[20]=z1; verts[21]=x2;verts[22]=y1;verts[23]=z2; 
  return verts;
}
