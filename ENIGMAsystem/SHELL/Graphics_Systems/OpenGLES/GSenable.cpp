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

#include "OpenGLHeaders.h"

int gs_enable_alpha(bool enable){
	(enable?glEnable:glDisable)(GL_ALPHA_TEST);
	return 0;
}//If enabled, do alpha testing. See glAlphaFunc.

int gs_enable_blending(bool enable){
	(enable?glEnable:glDisable)(GL_BLEND);
	return 0;
}//If enabled, blend the incoming RGBA color values with the values in the color buffers. See glBlendFunc.

int gs_enable_depthbuffer(bool enable){
	(enable?glEnable:glDisable)(GL_DEPTH_TEST);
	return 0;
}//If enabled, do depth comparisons and update the depth buffer. See glDepthFunc and glDepthRange.

int gs_enable_dither(bool enable){
	(enable?glEnable:glDisable)(GL_DITHER);
	return 0;
}//If enabled, dither color components or indexes before they are written to the color buffer.

int gs_enable_smooth_lines(bool enable){
	(enable?glEnable:glDisable)(GL_LINE_SMOOTH);
	return 0;
}//If enabled, draw lines with correct filtering. If disabled, draw aliased lines. See glLineWidth.

int gs_enable_stipple(bool enable){
	//Opengles does not have line stipple
	/*if(enable){
		glEnable(GL_LINE_STIPPLE);
		glEnable(GL_POLYGON_STIPPLE);
	}else{
		glDisable(GL_LINE_STIPPLE);
		glDisable(GL_POLYGON_STIPPLE);
	}*/
	return 0;
}//If enabled, use the current polygon stipple pattern when rendering polygons. See glPolygonStipple.

int gs_enable_logical_op(bool enable){
	//(enable?glEnable:glDisable)(GL_LOGIC_OP); OPENGLES
	return 0;
}//If enabled, apply the currently selected logical operation to the incoming and color-buffer indexes. See glLogicOp.

int gs_enable_smooth_points(bool enable){
	(enable?glEnable:glDisable)(GL_POINT_SMOOTH);
	return 0;
}//If enabled, draw points with proper filtering. If disabled, draw aliased points. See glPointSize.

int gs_enable_smooth_polygons(bool enable){
	//(enable?glEnable:glDisable)(GL_POLYGON_SMOOTH);
	return 0;
}//If enabled, draw polygons with proper filtering. If disabled, draw aliased polygons. See glPolygonMode.

int gs_enable_stencil(bool enable){
	(enable?glEnable:glDisable)(GL_STENCIL_TEST);
	return 0;
}//If enabled, do stencil testing and update the stencil buffer. See glStencilFunc and glStencilOp.

int gs_enable_texture(bool enable){
	(enable?glEnable:glDisable)(GL_TEXTURE_2D);
	return 0;
}//If enabled, textures
