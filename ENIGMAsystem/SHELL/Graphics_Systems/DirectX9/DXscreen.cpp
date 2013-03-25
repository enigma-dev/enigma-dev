/** Copyright (C) 2008-2013 Robert B. Colton
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
#include "DirectXHeaders.h"
#include "DXbackground.h"
#include "DXscreen.h"
#include "DXd3d.h"

using namespace std;

#include "Universal_System/var4.h"

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

static inline void draw_back()
{
    //Draw backgrounds
    for (int back_current=0; back_current<7; back_current++)
    {
        if (background_visible[back_current] == 1)
        {
            // if (background_stretched) draw_background_stretched(back, x, y, w, h);
            draw_background_tiled(background_index[back_current], background_x[back_current], background_y[back_current]);
        }
        // background_foreground, background_index, background_x, background_y, background_htiled,
        // background_vtiled, background_hspeed, background_vspeed;
    }
}

namespace enigma
{
    extern std::map<int,roomstruct*> roomdata;
    particles_implementation* particles_impl;
    void set_particles_implementation(particles_implementation* part_impl)
    {
        particles_impl = part_impl;
    };
    void update_particlesystems()
    {
        if (particles_impl != NULL) {
            (particles_impl->update_particlesystems)();
        }
    }
    void graphics_clean_up_roomend()
    {
        if (particles_impl != NULL) {
            (particles_impl->clear_effects)();
        }
    }
}

void screen_redraw()
{

}

void screen_init()
{

}
