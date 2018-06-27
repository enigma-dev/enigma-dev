/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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
#include "Graphics_Systems/General/OpenGLHeaders.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/sprites_internal.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"

#include "Collision_Systems/collision_types.h"

#include <cmath>
#include <cstdlib>
#include <string>
using std::string;

// These two leave a bad taste in my mouth because they depend on views, which should be removable.
// However, for now, they stay.

#include <string>
using std::string;
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"

namespace enigma_user {
  extern int window_get_region_height_scaled();
}

namespace enigma_user
{

int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig) {
	int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
 	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	int patchSize = w*h;
	std::vector<unsigned char> rgbdata(4*patchSize);
	glReadPixels(x,enigma_user::window_get_region_height_scaled()-h-y,w,h,GL_RGBA, GL_UNSIGNED_BYTE, &rgbdata[0]);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, prevFbo);

	unsigned char* data = enigma::image_flip(&rgbdata[0], w, h, 4);
  return enigma::sprite_add(data, w, h, 1, enigma::ct_precise, removeback, smooth, preload, xorig, yorig, false);
}

int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig) {
	return sprite_create_from_screen(x, y, w, h, removeback, smooth, true, xorig, yorig);
}

void sprite_add_from_screen(int id, int x, int y, int w, int h, bool removeback, bool smooth) {
	int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
 	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	int patchSize = w*h;
	std::vector<unsigned char> rgbdata(4*patchSize);
	glReadPixels(x,enigma_user::window_get_region_height_scaled()-h-y,w,h,GL_RGBA, GL_UNSIGNED_BYTE, &rgbdata[0]);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, prevFbo);

	unsigned char* data = enigma::image_flip(&rgbdata[0], w, h, 4);
  enigma::sprite_add_subimage(data, w, h, id, 1, enigma::ct_precise, removeback, smooth, x, y, false);
}

}
