/** Copyright (C) 2008-2013 Josh Ventura, Harijs Grinbergs, Robert B. Colton
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

#include "Graphics_Systems/OpenGL/OpenGLHeaders.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSprimitives.h"

#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/sprites_internal.h"

#include "Platforms/General/PFwindow.h"

// These two leave a bad taste in my mouth because they depend on views, which should be removable.
// However, for now, they stay.

namespace enigma_user {

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
