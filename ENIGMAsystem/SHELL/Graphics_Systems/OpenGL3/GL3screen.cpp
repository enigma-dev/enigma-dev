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

#include "GL3profiler.h"
#include "Graphics_Systems/OpenGL/OpenGLHeaders.h"
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSbackground.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GScolors.h"

#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/background_internal.h"
#include "Universal_System/sprites_internal.h"
#include "Universal_System/roomsystem.h"
#include "Platforms/General/PFwindow.h"

#include <string>
#include <cstdio>

//WE SHOULDN'T DO THIS! Don't specify namespaces like this - Harijs
using namespace std;

using namespace enigma;
using namespace enigma_user;

namespace enigma {

extern GLuint msaa_fbo;
unsigned int bound_framebuffer = 0; //Shows the bound framebuffer, so glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo); don't need to be called (they are very slow)
int viewport_x, viewport_y, viewport_w, viewport_h; //These are used by surfaces, to set back the viewport

void scene_begin() {}

void scene_end() {
  gpuprof.end_frame();

  if (enigma::msaa_fbo != 0) {
    GLint fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::msaa_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    //TODO: Change the code below to fix this to size properly to views
    glBlitFramebuffer(0, 0, window_get_region_width_scaled(), window_get_region_height_scaled(), 0, 0, window_get_region_width_scaled(), window_get_region_height_scaled(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    // glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
  }
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

int screen_save(string filename) //Assumes native integers are little endian
{
  draw_batch_flush(batch_flush_deferred);

  unsigned int w=window_get_width(),h=window_get_height(),sz=w*h;
  string ext = enigma::image_get_format(filename);

  unsigned char *rgbdata = new unsigned char[sz*4];
  GLint prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  glReadPixels(0,0,w,h, GL_BGRA, GL_UNSIGNED_BYTE, rgbdata);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prevFbo);

  int ret = image_save(filename, rgbdata, w, h, w, h, false);

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
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  glReadPixels(x,window_get_region_height_scaled()-h-y,w,h, GL_BGRA, GL_UNSIGNED_BYTE, rgbdata);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prevFbo);

  int ret = image_save(filename, rgbdata, w, h, w, h, false);

  delete[] rgbdata;
  return ret;
}

// The following three leave a bad taste in my mouth because they depend on views, which should be removable.
// However, for now, they stay.

int background_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload) {
	draw_batch_flush(batch_flush_deferred);

  int full_width=enigma::nlpo2dc(w)+1, full_height=enigma::nlpo2dc(h)+1;
	int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	int patchSize = full_width*full_height;
	std::vector<unsigned char> rgbdata(4*patchSize);
	glReadPixels(x, enigma_user::window_get_region_height_scaled()-h-y,w,h,GL_BGRA, GL_UNSIGNED_BYTE, &rgbdata[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prevFbo);

	unsigned char* data = enigma::image_flip(&rgbdata[0], w, h, 4);

	enigma::backgroundstructarray_reallocate();
  int bckid=enigma::background_idmax;
	enigma::background_new(bckid, w, h, &data[0], removeback, smooth, preload, false, 0, 0, 0, 0, 0, 0);
  delete[] data;
	rgbdata.clear();
	enigma::background_idmax++;
  return bckid;
}

int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig) {
  draw_batch_flush(batch_flush_deferred);

  int full_width=enigma::nlpo2dc(w)+1, full_height=enigma::nlpo2dc(h)+1;
	int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
 	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	int patchSize = full_width*full_height;
	std::vector<unsigned char> rgbdata(4*patchSize);
	glReadPixels(x,enigma_user::window_get_region_height_scaled()-h-y,w,h,GL_BGRA, GL_UNSIGNED_BYTE, &rgbdata[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prevFbo);

	unsigned char* data = enigma::image_flip(&rgbdata[0], w, h, 4);

	enigma::spritestructarray_reallocate();
  int sprid=enigma::sprite_idmax;
  enigma::sprite_new_empty(sprid, 1, w, h, xorig, yorig, 0, h, 0, w, preload, smooth);
	enigma::sprite_set_subimage(sprid, 0, w, h, &data[0], &data[0], enigma::ct_precise); //TODO: Support toggling of precise.
  rgbdata.clear(); // Clear the temporary array
	delete[] data;
  return sprid;
}

int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig) {
	return sprite_create_from_screen(x, y, w, h, removeback, smooth, true, xorig, yorig);
}

void sprite_add_from_screen(int id, int x, int y, int w, int h, bool removeback, bool smooth) {
  draw_batch_flush(batch_flush_deferred);

  int full_width=enigma::nlpo2dc(w)+1, full_height=enigma::nlpo2dc(h)+1;
	int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
 	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	int patchSize = full_width*full_height;
	std::vector<unsigned char> rgbdata(4*patchSize);
	glReadPixels(x, enigma_user::window_get_region_height_scaled()-h-y,w,h,GL_BGRA, GL_UNSIGNED_BYTE, &rgbdata[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prevFbo);

	unsigned char* data = enigma::image_flip(&rgbdata[0], w, h, 4);

	enigma::sprite_add_subimage(id, w, h, &data[0], &data[0], enigma::ct_precise); //TODO: Support toggling of precise.
	delete[] data;
	rgbdata.clear();
}

} // namespace enigma_user
