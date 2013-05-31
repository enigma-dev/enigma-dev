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

/**Enabling functions**********************************************************\

int enable_alpha(double enable)
int enable_blending(double enable)
int enable_depthbuffer(double enable)
int enable_dither(double enable)
int enable_smooth_lines(double enable)
int enable_stipple(double enable)
int enable_logical_op(double enable)
int enable_smooth_points(double enable)
int enable_smooth_polygons(double enable)
int enable_stencil(double enable)
int enable_texture(double enable)

\******************************************************************************/

namespace enigma_user
{

int enable_alpha(double enable)
{

}
//If enabled, do alpha testing. See glAlphaFunc. 

int enable_blending(double enable)
{
 
}
//If enabled, blend the incoming RGBA color values with the values in the color buffers. See glBlendFunc. 

int enable_depthbuffer(double enable)
{

}
//If enabled, do depth comparisons and update the depth buffer. See glDepthFunc and glDepthRange. 

int enable_dither(double enable)
{

}
//If enabled, dither color components or indexes before they are written to the color buffer. 

int enable_smooth_lines(double enable)
{

}
//If enabled, draw lines with correct filtering. If disabled, draw aliased lines. See glLineWidth. 

int enable_stipple(double enable)
{

}
//If enabled, use the current polygon stipple pattern when rendering polygons. See glPolygonStipple. 

int enable_logical_op(double enable)
{

}
//If enabled, apply the currently selected logical operation to the incoming and color-buffer indexes. See glLogicOp. 

int enable_smooth_points(double enable)
{

}
//If enabled, draw points with proper filtering. If disabled, draw aliased points. See glPointSize. 

int enable_smooth_polygons(double enable)
{

}
//If enabled, draw polygons with proper filtering. If disabled, draw aliased polygons. See glPolygonMode. 


int enable_stencil(double enable)
{

}
//If enabled, do stencil testing and update the stencil buffer. See glStencilFunc and glStencilOp. 

int enable_texture(double enable)
{

}

}

