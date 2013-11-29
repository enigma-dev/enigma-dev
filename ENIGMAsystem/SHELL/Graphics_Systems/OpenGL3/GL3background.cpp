/** Copyright (C) 2010-2013 Alasdair Morrison, Harijs Grinbergs, Robert B. Colton
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

#include <cstddef>
#include <math.h>
#include "../General/OpenGLHeaders.h"
#include "../General/GSbackground.h"
#include "../General/GLTextureStruct.h"

#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/backgroundstruct.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Universal_System/spritestruct.h"

#include "Universal_System/roomsystem.h"

#include "GL3shapes.h"

#define __GETR(x) (gs_scalar)(((x & 0x0000FF))/255.0)
#define __GETG(x) (gs_scalar)(((x & 0x00FF00) >> 8)/255.0)
#define __GETB(x) (gs_scalar)(((x & 0xFF0000) >> 16)/255.0)

#include "../General/GStextures.h"
#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_background(bck2d,back)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return;\
    }\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return r;\
    }\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#else
  #define get_background(bck2d,back)\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#endif

namespace enigma {
  extern size_t background_idmax;
}

//#include <string.h> // needed for querying ARB extensions

namespace enigma_user
{

int background_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload) {
    int full_width=nlpo2dc(w)+1, full_height=nlpo2dc(h)+1;
	int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	int patchSize = full_width*full_height;
	std::vector<unsigned char> rgbdata(4*patchSize);
	glReadPixels(x, h-y,w,h,GL_RGBA, GL_UNSIGNED_BYTE, &rgbdata[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prevFbo);
	
	unsigned char* data = enigma::image_reverse_scanlines(&rgbdata[0], w, h, 4);
	
	enigma::backgroundstructarray_reallocate();
    int bckid=enigma::background_idmax;
	enigma::background_new(bckid, w, h, &data[0], removeback, smooth, preload, false, 0, 0, 0, 0, 0, 0);
    delete[] data;
	rgbdata.clear();
	enigma::background_idmax++;
    return bckid;
}

}

