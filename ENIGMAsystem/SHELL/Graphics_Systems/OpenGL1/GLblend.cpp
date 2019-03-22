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
#include "Graphics_Systems/General/GSblend.h"
#include "Graphics_Systems/General/GSprimitives.h"

namespace enigma_user
{

int draw_set_blend_mode(int mode){
    if (enigma::currentblendmode[0] == mode && enigma::currentblendtype == 0) return 0;
    draw_batch_flush(batch_flush_deferred);
    enigma::currentblendmode[0] = mode;
    enigma::currentblendtype = 0;
	switch (mode)
	{
    case bm_add:
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
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

int draw_set_blend_mode_ext(int src,int dest){
    if (enigma::currentblendmode[0] == src && enigma::currentblendmode[1] == dest && enigma::currentblendtype == 1) return 0;
    draw_batch_flush(batch_flush_deferred);
	const static GLenum blendequivs[11] = {
	  GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA,
	  GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_DST_COLOR,
	  GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA_SATURATE
    };
    enigma::currentblendtype = 1;
    enigma::currentblendmode[0] = src;
    enigma::currentblendmode[1] = dest;
	glBlendFunc(blendequivs[(src-1)%10],blendequivs[(dest-1)%10]);
	return 0;
}

}
