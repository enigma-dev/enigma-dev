/** Copyright (C) 2008-2014 Josh Ventura, Harijs Grinbergs, Robert B. Colton
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

#include "Bridges/General/GLmanager.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSprimitives.h"

#include "Platforms/General/PFwindow.h"
#include "Universal_System/image_formats.h"

namespace enigma
{
extern GLuint msaa_fbo;
unsigned int bound_framebuffer = 0; //Shows the bound framebuffer, so glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo); don't need to be called (they are very slow)

void scene_begin() {
  //draw_batch_flush(batch_flush_deferred); //If called inside bound surface we need to finish drawing
  oglmgr->BeginScene();
}

void scene_end() {
  oglmgr->EndScene();

  if (enigma::msaa_fbo != 0) {
    GLint fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
    //glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::msaa_fbo); TODO: draw me
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //TODO: Change the code below to fix this to size properly to views
    //TODO: draw fullscreen quad
    //glBlitFramebuffer(0, 0, window_get_region_width_scaled(), window_get_region_height_scaled(), 0, 0, window_get_region_width_scaled(), window_get_region_height_scaled(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  }
}

}

namespace enigma_user
{

int screen_save(string filename) //Assumes native integers are little endian
{
  draw_batch_flush(batch_flush_deferred);
  unsigned int w=window_get_width(),h=window_get_height(),sz=w*h;

  string ext = enigma::image_get_format(filename);

  unsigned char *rgbdata = new unsigned char[sz*4];
  GLint prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glReadPixels(0,0,w,h, GL_BGRA, GL_UNSIGNED_BYTE, rgbdata);
  glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);

  int ret = enigma::image_save(filename, rgbdata, w, h, w, h, false);

  delete[] rgbdata;
  return ret;
}

int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h) //Assumes native integers are little endian
{
  draw_batch_flush(batch_flush_deferred);
  unsigned sz = w*h;

  string ext = enigma::image_get_format(filename);

  unsigned char *rgbdata = new unsigned char[sz*4];
  GLint prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glReadPixels(x,window_get_region_height_scaled()-h-y,w,h, GL_BGRA, GL_UNSIGNED_BYTE, rgbdata);
  glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);

  int ret = enigma::image_save(filename, rgbdata, w, h, w, h, false);

  delete[] rgbdata;
  return ret;
}

}
