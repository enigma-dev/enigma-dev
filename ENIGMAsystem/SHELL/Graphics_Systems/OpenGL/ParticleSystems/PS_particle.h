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

#include "PS_particle_constants.h"

// Particle systems.

// General.
int part_system_create();
void part_system_destroy(int id);
void part_system_exists(int id);
void part_system_clear(int id);
void part_system_draw_order(int id, bool oldtonew);
void part_system_depth(int id, double depth);
// Update and draw.
void part_system_automatic_update(int id, bool automatic);
void part_system_automatic_draw(int id, bool automatic);
void part_system_update(int id);
void part_system_drawit(int id);
// Particles.
void part_particles_create(int id, double x, double y, int particle_type_id, int number);
void part_particles_create_color(int id, double x, double y, int particle_type_id, int color, int number);
void part_particles_clear(int id);
int part_particles_count(int id);

// Emitters.

int part_emitter_create(int id);
void part_emitter_destroy(int ps_id, int em_id);
void part_emitter_destroy_all(int ps_id);
void part_emitter_exists(int ps_id, int em_id);
void part_emitter_clear(int ps_id, int em_id);
void part_emitter_region(int ps_id, int em_id, double xmin, double xmax, double ymin, double ymax, int shape, int distribution);
void part_emitter_burst(int ps_id, int em_id, int particle_type_id, int number);
void part_emitter_stream(int ps_id, int em_id, int particle_type_id, int number);

// Particle types.

// General.
int part_type_create();
void part_type_destroy(int id);
bool part_type_exists(int id);
void part_type_clear(int id);
// Shape.
void part_type_shape(int id, int particle_shape);
void part_type_sprite(int id, int sprite, bool animat, bool stretch, bool random);
void part_type_size(int id, double size_min, double size_max, double size_incr, double size_wiggle);
void part_type_scale(int id, double xscale, double yscale);
void part_type_orientation(int id, double ang_min, double ang_max, double ang_incr, double ang_wiggle, bool ang_relative);
// Color and blending.
void part_type_color1(int id, int color1);
void part_type_color2(int id, int color1, int color2);
void part_type_color3(int id, int color1, int color2, int color3);
void part_type_color_mix(int id, int color1, int color2);
void part_type_color_rgb(int id, int rmin, int rmax, int gmin, int gmax, int bmin, int bmax);
void part_type_color_hsv(int id, int hmin, int hmax, int smin, int smax, int vmin, int vmax);
void part_type_alpha1(int id, double alpha1);
void part_type_alpha2(int id, double alpha1, double alpha2);
void part_type_alpha3(int id, double alpha1, double alpha2, double alpha3);
void part_type_blend(int id, bool additive);
// Life and death.
void part_type_life(int id, int life_min, int life_max);
void part_type_step(int id, int step_number, int step_type);
void part_type_death(int id, int death_number, int death_type);
// Motion.
void part_type_speed(int id, double speed_min, double speed_max, double speed_incr, double speed_wiggle);
void part_type_direction(int id, double dir_min, double dir_max, double dir_incr, double dir_wiggle);
void part_type_gravity(int id, double grav_amount, double grav_dir);

#endif // ENIGMA_PS_PARTICLESYSTEM

