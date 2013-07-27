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

#include <string>
#include <cstdio>
#include "../General/DirectXHeaders.h"
#include "../General/GSbackground.h"
#include "../General/GSscreen.h"
#include "../General/GSd3d.h"
#include "../General/DXbinding.h"

using namespace std;

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

//Fuck whoever did this to the spec
#ifndef GL_BGR
  #define GL_BGR 0x80E0
#endif

using namespace enigma;

namespace enigma_user {
  extern int window_get_width();
  extern int window_get_height();
}

static inline void draw_back()
{
    using enigma_user::background_x;
    using enigma_user::background_y;
    using enigma_user::background_visible;
    using enigma_user::background_alpha;
    using enigma_user::background_xscale;
    using enigma_user::background_yscale;
    using enigma_user::background_htiled;
    using enigma_user::background_vtiled;
    using enigma_user::background_index;
    using enigma_user::background_coloring;
    using enigma_user::draw_background_tiled_ext;
    using enigma_user::draw_background_ext;
    //Draw backgrounds
    for (int back_current=0; back_current<7; back_current++)
    {
        if (background_visible[back_current] == 1)
        {
            if (background_htiled[back_current] || background_vtiled[back_current])
                draw_background_tiled_ext(background_index[back_current], background_x[back_current], background_y[back_current], background_xscale[back_current], background_xscale[back_current], background_coloring[back_current], background_alpha[back_current]);
            else
                draw_background_ext(background_index[back_current], background_x[back_current], background_y[back_current], background_xscale[back_current], background_xscale[back_current], 0, background_coloring[back_current], background_alpha[back_current]);
        }
    }
}

namespace enigma
{
    extern bool d3dHidden;
    extern std::map<int,roomstruct*> roomdata;
    particles_implementation* particles_impl;
    void set_particles_implementation(particles_implementation* part_impl)
    {
        particles_impl = part_impl;
    }
}

namespace enigma_user
{

void screen_redraw()
{
    
}

void screen_init()
{
    
}

int screen_save(string filename) //Assumes native integers are little endian
{
	
}

int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h) //Assumes native integers are little endian
{
	
}

}

