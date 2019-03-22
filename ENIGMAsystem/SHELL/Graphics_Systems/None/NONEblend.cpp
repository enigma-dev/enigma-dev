/** Copyright (C) 2017 Faissal I. Bensefia
*** Copyright (C) 2008-2013 Robert B. Colton, Adriano Tumminelli
*** Copyright (C) 2014 Seth N. Hetu
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

#include "Graphics_Systems/General/GSblend.h"

namespace enigma_user
{
	int draw_set_blend_mode(int mode){
			if (enigma::currentblendmode[0] == mode && enigma::currentblendtype == 0) return 0;
			enigma::currentblendmode[0] = mode;
			enigma::currentblendtype = 0;
			return 0;
	}

	int draw_set_blend_mode_ext(int src,int dest){
			if (enigma::currentblendmode[0] == src && enigma::currentblendmode[1] == dest && enigma::currentblendtype == 1) return 0;
			enigma::currentblendtype = 1;
			enigma::currentblendmode[0] = src;
			enigma::currentblendmode[1] = dest;
			return 0;
	}
}
