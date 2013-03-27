/** Copyright (C) 2008-2013 forthevin, Robert B. Colton
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

#include "DX10draw.h"
#include "DX10sprite.h"
#include "binding.h"
#include "DX10textures.h"
#include "DX10colors.h"
#include "Universal_System/Extensions/ParticleSystems/PS_particle_instance.h"
#include "Universal_System/Extensions/ParticleSystems/PS_particle_sprites.h"
#include <vector>
#include <cmath>
#include <algorithm>
//#include <DX/dx.h>

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma
{
    double wiggle;
    int subimage_index;
    double x_offset;
    double y_offset;
    particle_sprite* (*draw_get_particle_sprite)(pt_shape particle_shape);
    double get_wiggle_result(double wiggle_offset, double wiggle)
    {
        double result_wiggle = wiggle + wiggle_offset;
        result_wiggle = result_wiggle > 1.0 ? result_wiggle - 1.0 : result_wiggle;
        if (result_wiggle < 0.5) return -1.0 + 4*result_wiggle;
        else return 3.0 - 4.0*result_wiggle;
    }
    void draw_particle(particle_instance* it)
    {

    }
    void draw_particles(std::vector<particle_instance>& pi_list, bool oldtonew, double wiggle, int subimage_index,
        double x_offset, double y_offset, particle_sprite* (*get_particle_sprite)(pt_shape particle_shape))
    {    

    }
}

