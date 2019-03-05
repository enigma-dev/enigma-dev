/** Copyright (C) 2010-2013 Alasdair Morrison, Robert B. Colton
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
#include "Graphics_Systems/General/GSbackground.h"
#include "Graphics_Systems/General/GSprimitives.h"

#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/background_internal.h"

#include "Platforms/General/PFwindow.h"

namespace enigma_user {

int background_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload)
{
	draw_batch_flush(batch_flush_deferred);

  int full_width=enigma::nlpo2dc(w)+1, full_height=enigma::nlpo2dc(h)+1;
	int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
 	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	int patchSize = full_width*full_height;
	std::vector<unsigned char> rgbdata(4*patchSize);
	glReadPixels(x, enigma_user::window_get_region_height_scaled()-h-y,w,h,GL_BGRA, GL_UNSIGNED_BYTE, &rgbdata[0]);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);

	unsigned char* data = enigma::image_flip(&rgbdata[0], w, h, 4);

	enigma::backgroundstructarray_reallocate();
  int bckid=enigma::background_idmax;
	enigma::background_new(bckid, w, h, &data[0], removeback, smooth, preload, false, 0, 0, 0, 0, 0, 0);
  delete[] data;
	rgbdata.clear();
	enigma::background_idmax++;
  return bckid;
}

} // namespace enigma_user
