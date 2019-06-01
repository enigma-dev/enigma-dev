/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
*** Copyright (C) 2015 Harijs Grinbergs
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

#ifndef ENIGMA_GSD3D_H
#define ENIGMA_GSD3D_H

#include "GSblend.h"
#include "GScolors.h"
#include "Universal_System/scalar.h"
#include <string>

namespace enigma_user {

// ***** RENDER STATE CONSTANTS *****
enum {
  rs_nicest,
  rs_fastest,
  rs_dontcare,
};

enum {
  rs_exp,    // D3DFOG_EXP
  rs_exp2,   // D3DFOG_EXP2
  rs_linear, // D3DFOG_LINEAR
};

enum {
  rs_never, 	// Always False            D3DCMP_NEVER            GL_NEVER
  rs_less, 	    // source Z < depth Z      D3DCMP_LESS             GL_LESS
  rs_equal,	    // source Z = depth Z      D3DCMP_EQUAL            GL_EQUAL
  rs_lequal, 	// source Z <= depth Z     D3DCMP_LESSEQUAL        GL_LEQUAL
  rs_greater,	// source Z > depth Z      D3DCMP_GREATER          GL_GREATER
  rs_notequal, 	// source Z != depth Z     D3DCMP_NOTEQUAL         GL_NOTEQUAL
  rs_gequal, 	// source Z >= depth Z     D3DCMP_GREATEREQUAL     GL_GEQUAL
  rs_always     // Always True             D3DCMP_ALWAYS           GL_ALWAYS
};

enum {
  rs_keep,   // GL_KEEP
  rs_zero,      //  GL_ZERO
  rs_replace,     //  GL_REPLACE
  rs_incr,  // GL_INCR
  rs_incr_wrap, // GL_INCR_WRAP
  rs_decr,  // GL_DECR
  rs_decr_wrap,  // GL_DECR_WRAP
  rs_invert     // GL_INVERT
};

// NOTE: Game Maker uses clockwise culling to define the front face,
// OpenGL's mode sets what defines the front face, Direct3D's mode sets what defines the back face
enum {
  rs_none, // No culling
  rs_cw, // Clockwise culling
  rs_ccw // Counter-clockwise culling
};

enum {
  rs_back,
  rs_front,
  rs_front_back
};

enum {
  rs_point, // Render vertices as points
  rs_line,  // Render in wireframe mode
  rs_solid  // Normal render mode
};

} // namespace enigma_user

namespace enigma {

struct Sampler {
  int texture=-1; // GML texture id, NOT GL texture id!
  bool wrapu=false, wrapv=false, wrapw=false;
  bool interpolate=false;
};

struct Light {
  gs_scalar x=0, y=0, z=0, range=0;
  bool directional=false;
  int color=0;
};

struct RenderState {
  unsigned char currentcolor[4] = {255,255,255,255};
  bool colorWriteEnable[4] = {true,true,true,true};
  bool lineStippleEnable=false, msaaEnabled=true, alphaBlend=true, alphaTest=false;
  unsigned short lineStipplePattern=0xFFFF;
  unsigned char alphaTestRef=0;
  float circleprecision=24, drawPointSize=1.0f, drawLineWidth=1.0f;
  int drawFillMode=enigma_user::rs_solid, lineStippleScale=1;
  int blendMode[2]={enigma_user::bm_src_alpha,enigma_user::bm_inv_src_alpha};

  Sampler samplers[8];

  bool d3dMode=false, d3dHidden=false, d3dClipPlane=false, d3dZWriteEnable=true,
      d3dPerspective=true, d3dLighting=false, d3dShading=true;
  int d3dCulling=0, d3dDepthOperator=enigma_user::rs_lequal;

  bool d3dFogEnabled=false;
  int d3dFogColor=enigma_user::c_gray, d3dFogMode=enigma_user::rs_linear, d3dFogHint=enigma_user::rs_nicest;
  float d3dFogStart=0.0f, d3dFogEnd=0.0f, d3dFogDensity=0.0f;

  int d3dLightsActive=0, d3dLightingAmbient=enigma_user::c_black;

  bool d3dStencilTest = false;
  unsigned int d3dStencilMask = 0x0;
  int d3dStencilFunc = enigma_user::rs_always, d3dStencilFuncRef = 0, d3dStencilFuncMask = -1,
      d3dStencilOpStencilFail = enigma_user::rs_keep, d3dStencilOpDepthFail = enigma_user::rs_keep,
      d3dStencilOpPass = enigma_user::rs_keep;
};

extern RenderState render_state;
const Light& get_active_light(int id);

} // namespace enigma

namespace enigma_user {
  void d3d_clear_depth(double value=1.0L);
  void d3d_start();
  void d3d_end();
  void d3d_set_software_vertex_processing(bool software);
  void d3d_set_perspective(bool enable);
  void d3d_set_hidden(bool enable);
  void d3d_set_depth(double dep);
  void d3d_set_depth_operator(int mode);
  void d3d_set_zwriteenable(bool enable);
  void d3d_set_culling(int mode);
  void d3d_set_clip_plane(bool enable);
  void d3d_set_lighting(bool enable);
  void d3d_set_shading(bool smooth);

  bool d3d_get_mode();
  bool d3d_get_perspective();
  int d3d_get_culling();
  bool d3d_get_hidden();

  // Fog
  void d3d_set_fog(bool enable, int color, double start, double end);
  void d3d_set_fog_enabled(bool enable);
  void d3d_set_fog_hint(int mode);
  void d3d_set_fog_mode(int mode);
  void d3d_set_fog_color(int color);
  void d3d_set_fog_start(double start);
  void d3d_set_fog_end(double end);
  void d3d_set_fog_density(double density);

  // Lighting
  void d3d_light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col);
  void d3d_light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col);
  void d3d_light_specularity(int facemode, int r, int g, int b, double a);
  void d3d_light_set_ambient(int id, int r, int g, int b, double a);
  void d3d_light_set_specularity(int id, int r, int g, int b, double a);
  void d3d_light_shininess(int facemode, int shine);
  void d3d_light_define_ambient(int col);
  void d3d_light_enable(int id, bool enable);

  // Stencil stuff
  void d3d_stencil_start_mask();
  void d3d_stencil_continue_mask();
  void d3d_stencil_use_mask();
  void d3d_stencil_end_mask();

  void d3d_stencil_enable(bool enable);
  void d3d_stencil_clear_value(int value);
  void d3d_stencil_clear();
  void d3d_stencil_mask(unsigned int mask);
  void d3d_stencil_function(int func, int ref, unsigned int mask);
  void d3d_stencil_operator(int sfail, int dpfail, int dppass);
}

#endif // ENIGMA_GL3D3D_H
