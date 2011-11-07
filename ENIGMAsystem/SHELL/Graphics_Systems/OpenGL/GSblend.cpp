/** Copyright (C) 2008-2011 Josh Ventura
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

#include "OpenGLHeaders.h"
#include "GSblend.h"

int draw_set_blend_mode(int mode){
	switch (mode)
	{
    case bm_add:
        glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
      return 0;
    case bm_max:
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_COLOR);
      return 0;
    case bm_subtract:
        glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
      return 0;
    default:
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      return 0;
  }
}

int draw_set_blend_mode_ext(double src,double dest){
	const static GLenum blendequivs[11] = {
	  GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA,
	  GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_DST_COLOR,
	  GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA_SATURATE
  };
	glBlendFunc(blendequivs[((int)src-1)%10],blendequivs[((int)dest-1)%10]);
	return 0;
}
