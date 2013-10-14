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

#include "Bridges/General/DX9Device.h"
#include "Direct3D9Headers.h"
#include "DX9enable.h"

namespace enigma_user
{

void gs_enable_alpha(bool enable) {
	d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, enable);
}///If enabled, do alpha testing. See glAlphaFunc.

void gs_enable_blending(bool enable) {

}///If enabled, blend the incoming RGBA color values with the values in the color buffers. See glBlendFunc.

void gs_enable_depthbuffer(bool enable) {
	d3ddev->SetRenderState(D3DRS_ZENABLE, enable);
}///If enabled, do depth comparisons and update the depth buffer. See glDepthFunc and glDepthRange.

void gs_enable_dither(bool enable) {

}///If enabled, dither color components or indexes before they are written to the color buffer.

void gs_enable_smooth_lines(bool enable) {

}///If enabled, draw lines with correct filtering. If disabled, draw aliased lines. See glLineWidth.

void gs_enable_stipple(bool enable) {

}///If enabled, use the current polygon stipple pattern when rendering polygons. See glPolygonStipple.

void gs_enable_logical_op(bool enable) {

}///If enabled, apply the currently selected logical operation to the incoming and color-buffer indexes. See glLogicOp.

void gs_enable_smooth_points(bool enable) {

}///If enabled, draw points with proper filtering. If disabled, draw aliased points. See glPointSize.

void gs_enable_smooth_polygons(bool enable) {

}///If enabled, draw polygons with proper filtering. If disabled, draw aliased polygons. See glPolygonMode.

void gs_enable_stencil(bool enable) {
	d3ddev->SetRenderState(D3DRS_STENCILENABLE, enable);
}///If enabled, do stencil testing and update the stencil buffer. See glStencilFunc and glStencilOp.

void gs_enable_texture(bool enable) {

}///If enabled, textures

}

