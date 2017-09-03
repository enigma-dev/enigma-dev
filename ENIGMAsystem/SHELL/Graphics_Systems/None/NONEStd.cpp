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

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>     /* malloc, free, rand */
#include <unordered_map>
#include <math.h>

using namespace std;
#include "OPENGLStd.h"
#include "Universal_System/shaderstruct.h"
#include "Universal_System/scalar.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h" // Room dimensions.
#include "Universal_System/depth_draw.h"
#include "Universal_System/backgroundstruct.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/spritestruct.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/fileio.h"
#include "Universal_System/estring.h"
#include "Collision_Systems/collision_types.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"
#include <algorithm>
#include "Graphics_Systems/graphics_mandatory.h" // Room dimensions.
#include "Graphics_Systems/General/GSprimitives.h"
#include "../General/GSmath.h"
#include "../General/GSbackground.h"
#include "../General/GStextures.h"
#include "../General/GStiles.h"
#include "../General/GSvertex.h"
#include "../General/GStextures.h"
#include "../General/GSsurface.h"
#include "../General/GSstdraw.h"
#include "../General/GSsprite.h"
#include "../General/GStextures.h"
#include "../General/GLSurfaceStruct.h"
#include "../General/GStextures.h"
#include "../General/GSscreen.h"
#include "../General/GSd3d.h"
#include "../General/GSblend.h"
#include "../General/GSmatrix.h"
#include "../General/GScolors.h"
#include "Platforms/platforms_mandatory.h"
#include <floatcomp.h>
#include <vector>
#include <limits>
#include <map>
#include <list>
#include <stack>
#include "Universal_System/estring.h"
#include "fillin.h"

#define degtorad(x) x*(M_PI/180.0)
unsigned get_texture(int texid){return -1;}

using namespace enigma;
using namespace enigma_user;

namespace enigma {
	unsigned char currentcolor[4] = {0,0,0,255};
	int currentblendmode[2] = {0,0};
	int currentblendtype = 0;
	bool glew_isgo;
	bool pbo_isgo;

	unordered_map<unsigned int, surface> surface_array;
	size_t surface_max=0;

	extern unsigned char currentcolor[4];
}

namespace enigma_user {
	extern int room_width, room_height/*, sprite_idmax*/;
}
