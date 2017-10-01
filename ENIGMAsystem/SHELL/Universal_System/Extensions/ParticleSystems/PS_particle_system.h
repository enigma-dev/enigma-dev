/********************************************************************************\
**                                                                              **
**  Copyright (C) 2012-2013 forthevin                                           **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#ifndef ENIGMA_PS_PARTICLESYSTEM
#define ENIGMA_PS_PARTICLESYSTEM

#include "PS_particle_emitter.h"
#include "PS_particle_attractor.h"
#include "PS_particle_destroyer.h"
#include "PS_particle_deflector.h"
#include "PS_particle_changer.h"
#include "PS_particle_instance.h"
#include "PS_particle_enums.h"
#include "Graphics_Systems/General/GScolors.h"
#include <list>
#include <vector>
#include <map>

using namespace enigma_user;

namespace enigma
{
  namespace particle_bridge {
    // Initialization
    void initialize_particle_bridge();
    // Drawing
    void draw_particles(std::vector<particle_instance>& pi_list, bool oldtonew, double wiggle, int subimage_index,
        double x_offset, double y_offset);
  }
  
  struct particle_system
  {
    // Wiggling.
    double wiggle;
    int wiggle_frequency; // Number of steps for a full cycle. Domain: [1;[.
    double get_wiggle_result(double wiggle_offset);
    static double get_wiggle_result(double wiggle_offset, double wiggle_amount);
    // Subimage index.
    int subimage_index;
    // Particles.
    int id;
    bool oldtonew;
    double x_offset, y_offset;
    double depth; // Integer stored as double.
    std::vector<particle_instance> pi_list;
    bool auto_update, auto_draw;
    void initialize();
    void update_particlesystem();
    void draw_particlesystem();
    void create_particles(double x, double y, particle_type* pt, int number, bool use_color=false, int given_color=c_white);
    // Emitters.
    std::map<int,particle_emitter*> id_to_emitter;
    int emitter_max_id;
    int create_emitter();
    void set_emitter_region(int em_id, double xmin, double xmax, double ymin, double ymax, ps_shape shape, ps_distr distribution);
    void set_emitter_stream(int em_id, int particle_type_id, int number);
    // Attractors.
    std::map<int,particle_attractor*> id_to_attractor;
    int attractor_max_id;
    int create_attractor();
    // Destroyers.
    std::map<int,particle_destroyer*> id_to_destroyer;
    int destroyer_max_id;
    int create_destroyer();
    // Deflectors.
    std::map<int,particle_deflector*> id_to_deflector;
    int deflector_max_id;
    int create_deflector();
    // Changers.
    std::map<int,particle_changer*> id_to_changer;
    int changer_max_id;
    int create_changer();
    // Protection.
    bool hidden;
  };
}

#endif // ENIGMA_PS_PARTICLESYSTEM

