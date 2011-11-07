/** Copyright (C) 2008-2011 Josh Ventura
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
#include "GSenable.h"

void gs_enable_alpha(bool enable) {
  (enable?glEnable:glDisable)(GL_ALPHA_TEST);
}//If enabled, do alpha testing. See glAlphaFunc.

void gs_enable_blending(bool enable) {
  (enable?glEnable:glDisable)(GL_BLEND);
}//If enabled, blend the incoming RGBA color values with the values in the color buffers. See glBlendFunc.

void gs_enable_depthbuffer(bool enable) {
  (enable?glEnable:glDisable)(GL_DEPTH_TEST);
}//If enabled, do depth comparisons and update the depth buffer. See glDepthFunc and glDepthRange.

void gs_enable_dither(bool enable) {
  (enable?glEnable:glDisable)(GL_DITHER);
}//If enabled, dither color components or indexes before they are written to the color buffer.

void gs_enable_smooth_lines(bool enable) {
  (enable?glEnable:glDisable)(GL_LINE_SMOOTH);
}//If enabled, draw lines with correct filtering. If disabled, draw aliased lines. See glLineWidth.

void gs_enable_stipple(bool enable) {
  if(enable) {
    glEnable(GL_LINE_STIPPLE);
    glEnable(GL_POLYGON_STIPPLE);
  } else {
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_POLYGON_STIPPLE);
  }
}//If enabled, use the current polygon stipple pattern when rendering polygons. See glPolygonStipple.

void gs_enable_logical_op(bool enable) {
  (enable?glEnable:glDisable)(GL_LOGIC_OP);
}//If enabled, apply the currently selected logical operation to the incoming and color-buffer indexes. See glLogicOp.

void gs_enable_smooth_points(bool enable) {
  (enable?glEnable:glDisable)(GL_POINT_SMOOTH);
}//If enabled, draw points with proper filtering. If disabled, draw aliased points. See glPointSize.

void gs_enable_smooth_polygons(bool enable) {
  (enable?glEnable:glDisable)(GL_POLYGON_SMOOTH);
}//If enabled, draw polygons with proper filtering. If disabled, draw aliased polygons. See glPolygonMode.

void gs_enable_stencil(bool enable) {
  (enable?glEnable:glDisable)(GL_STENCIL_TEST);
}//If enabled, do stencil testing and update the stencil buffer. See glStencilFunc and glStencilOp.

void gs_enable_texture(bool enable) {
  (enable?glEnable:glDisable)(GL_TEXTURE_2D);
}//If enabled, textures
