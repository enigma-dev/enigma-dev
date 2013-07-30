/** Copyright (C) 2013 Robert B. Colton
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

#ifndef ENIGMA_PS_PARTICLE_BRIDGE_DIRECTX9_H
#define ENIGMA_PS_PARTICLE_BRIDGE_DIRECTX9_H

#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GLbinding.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GScolors.h"
#include "PS_particle_instance.h"
#include "PS_particle_sprites.h"
#include <vector>
#include <cmath>
#include <algorithm>

#include "PS_particle_system.h"
#include "PS_particle.h"

namespace enigma {
  namespace particle_bridge {
    void initialize_particle_bridge() {} // Do nothing, nothing to initialize.
    inline int __GETR(int x) {return x & 0x0000FF;}
    inline int __GETG(int x) {return (x & 0x00FF00) >> 8;}
    inline int __GETB(int x) {return (x & 0xFF0000) >> 16;}
    double wiggle;
    int subimage_index;
    double x_offset;
    double y_offset;
    
    static void draw_particle(particle_instance* it)
    {
     
    }
    
    void draw_particles(std::vector<particle_instance>& pi_list, bool oldtonew, double a_wiggle, int a_subimage_index,
        double a_x_offset, double a_y_offset)
    {
     
    }
  }
}

#endif // ENIGMA_PS_PARTICLE_BRIDGE_OPENGL1_H

