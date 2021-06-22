/** Copyright (C) 2008-2014 Josh Ventura, Harijs Grinbergs
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

#include "profiler.h"
#include "screen.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GSscreen.h"

#include "Platforms/General/PFwindow.h"

using namespace enigma;

namespace enigma {

unsigned int bound_framebuffer = 0; //Shows the bound framebuffer, so glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo); don't need to be called (they are very slow)

void graphics_set_viewport(float x, float y, float width, float height) {
  //NOTE: OpenGL viewports are bottom left unlike Direct3D viewports which are top left 		
  glViewport(x,y,width,height);
  glScissor(x,y,width,height);
}

void scene_begin() {}

void scene_end() {
  gpuprof.end_frame();
  msaa_fbo_blit();
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
