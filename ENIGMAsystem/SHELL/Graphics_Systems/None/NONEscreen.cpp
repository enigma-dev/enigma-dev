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
#include <string>
#include <cstdio>
#include "../General/GStextures.h"
#include "../General/GSbackground.h"
#include "../General/GSscreen.h"
#include "../General/GSd3d.h"
#include "../General/GSmatrix.h"
#include "../General/GScolors.h"

using namespace std;

#include "Universal_System/image_formats.h"
#include "Universal_System/background_internal.h"
#include "Universal_System/var4.h"
#include "Universal_System/estring.h"

#define __GETR(x) (((unsigned int)x & 0x0000FF))
#define __GETG(x) (((unsigned int)x & 0x00FF00) >> 8)
#define __GETB(x) (((unsigned int)x & 0xFF0000) >> 16)

#include "Universal_System/roomsystem.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"
#include "Universal_System/depth_draw.h"
#include "Platforms/platforms_mandatory.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include <limits>

using namespace enigma;
using namespace enigma_user;
namespace enigma_user {
  extern int window_get_width();
  extern int window_get_height();
  extern int window_get_region_width();
  extern int window_get_region_height();
}

namespace enigma
{
	extern bool d3dMode;
	extern int d3dCulling;
	particles_implementation* particles_impl;
	void set_particles_implementation(particles_implementation* part_impl)
	{
			particles_impl = part_impl;
	}
}


static inline void draw_back(){}
static inline void draw_insts(){}
static inline int draw_tiles(){return 0;}
void clear_view(float x, float y, float w, float h, float angle, bool showcolor){}
static inline void draw_gui(){}
