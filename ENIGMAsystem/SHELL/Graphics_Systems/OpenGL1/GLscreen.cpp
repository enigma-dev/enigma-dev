/** Copyright (C) 2008-2014 Josh Ventura
*** Copyright (C) 2010-2013 Alasdair Morrison
*** Copyright (C) 2013-2014 Robert B. Colton
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

#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/OpenGL/GLscreen.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSprimitives.h"

#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include "Platforms/General/PFwindow.h"

using namespace enigma;

namespace enigma {

unsigned int bound_framebuffer = 0; //Shows the bound framebuffer, so glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo); don't need to be called (they are very slow)
int viewport_x, viewport_y, viewport_w, viewport_h; //These are used by surfaces, to set back the viewport

void scene_begin() {

}

void scene_end() {
	msaa_fbo_blit();
}

unsigned char* graphics_copy_screen_pixels(int x, int y, int width, int height, bool* flipped) {
  if (flipped) *flipped = true;

  const int bpp = 4; // bytes per pixel
  const int topY = enigma_user::window_get_region_height_scaled()-height-y;
  unsigned char* pxdata = new unsigned char[width*height*bpp];

  GLint prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
  glReadPixels(x,topY,width,height,GL_BGRA,GL_UNSIGNED_BYTE,pxdata);
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, prevFbo);
  return pxdata;
}

unsigned char* graphics_copy_screen_pixels(unsigned* fullwidth, unsigned* fullheight, bool* flipped) {
  if (flipped) *flipped = true;

  const int fw = enigma_user::window_get_region_width_scaled(),
            fh = enigma_user::window_get_region_height_scaled();

  *fullwidth = fw;
  *fullheight = fh;
  return graphics_copy_screen_pixels(0,0,fw,fh,flipped);
}

} // namespace enigma

namespace enigma_user {

void screen_set_viewport(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height) {
  draw_batch_flush(batch_flush_deferred);

  x = (x / window_get_region_width()) * window_get_region_width_scaled();
  y = (y / window_get_region_height()) * window_get_region_height_scaled();
  width = (width / window_get_region_width()) * window_get_region_width_scaled();
  height = (height / window_get_region_height()) * window_get_region_height_scaled();
  gs_scalar sx, sy;
  sx = (window_get_width() - window_get_region_width_scaled()) / 2;
  sy = (window_get_height() - window_get_region_height_scaled()) / 2;
  viewport_x = sx + x;
  viewport_y = window_get_height() - (sy + y) - height;
  viewport_w = width;
  viewport_h = height;

  //NOTE: OpenGL viewports are bottom left unlike Direct3D viewports which are top left
  glViewport(viewport_x, viewport_y, viewport_w, viewport_h);
  glScissor(viewport_x, viewport_y, viewport_w, viewport_h);
}

} // namespace enigma_user
