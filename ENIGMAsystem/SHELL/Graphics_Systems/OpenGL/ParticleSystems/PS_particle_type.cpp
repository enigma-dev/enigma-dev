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

#include "PS_particle_type.h"
#include "PS_particle_sprites.h"
#include "PS_particle_enums.h"
#include "Graphics_Systems/OpenGL/GScolors.h"
#include "Graphics_Systems/OpenGL/GSsprite.h"

inline int bounds(int value, int low, int high)
{
  if (value < low) return low;
  if (value > high) return high;
  return value;
}

namespace enigma
{
  particle_type_manager pt_manager;
}

void initialize_particle_type(enigma::particle_type* pt)
{
  pt->is_particle_sprite = true;
  enigma::particle_sprite* sprite = enigma::get_particle_sprite(enigma::pt_sh_pixel);
  if (sprite != 0) {
    pt->part_sprite = sprite;
  }
  pt->sprite_id = -1;
  pt->sprite_animated = false, pt->sprite_stretched = false, pt->sprite_random = false;
  pt->size_min = 1.0, pt->size_max = 1.0;
  pt->size_incr = 0.0, pt->size_wiggle = 0.0;
  pt->xscale = 1.0, pt->yscale = 1.0;
  pt->ang_min = 0.0, pt->ang_max = 0.0;
  pt->ang_incr = 0.0, pt->ang_wiggle = 0.0;
  pt->ang_relative = false;
  pt->c_mode = enigma::one_color;
  pt->color1 = c_white;
  pt->color2 = c_white;
  pt->color3 = c_white;
  pt->rmin = 255, pt->rmax = 255, pt->gmin = 255, pt->gmax = 255, pt->bmin = 255, pt->bmax = 255;
  pt->hmin = 255, pt->hmax = 255, pt->smin = 255, pt->smax = 255, pt->vmin = 255, pt->vmax = 255;
  pt->a_mode = enigma::one_alpha;
  pt->alpha1 = 255;
  pt->alpha2 = 255;
  pt->alpha3 = 255;
  pt->blend_additive = false;
  pt->life_min = 100;
  pt->life_max = 100;
  pt->death_on = false, pt->death_particle_id = 0, pt->death_number = 0;
  pt->step_on = false, pt->step_particle_id = 0, pt->step_number = 0;
  pt->speed_min = 0.0, pt->speed_max = 0.0;
  pt->speed_incr = 0.0, pt->speed_wiggle = 0.0;
  pt->dir_min = 0.0, pt->dir_max = 0.0;
  pt->dir_incr = 0.0, pt->dir_wiggle = 0.0;
  pt->grav_amount = 0.0, pt->grav_dir = 0.0;
}
int part_type_create()
{ 
  enigma::particle_type* pt = new enigma::particle_type();
  pt->particle_count = 0;
  pt->alive = true;

  initialize_particle_type(pt);

  enigma::pt_manager.max_id++;
  pt->id = enigma::pt_manager.max_id;
  enigma::pt_manager.id_to_particletype.insert(std::pair<int,enigma::particle_type*>(enigma::pt_manager.max_id, pt));

  return enigma::pt_manager.max_id;
}
void part_type_destroy(int id)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    enigma::particle_type* pt = (*it).second;
    pt->alive = false;
    if (pt->particle_count <= 0) {
      delete (*it).second;
      enigma::pt_manager.id_to_particletype.erase(id);
    }
  }
}
bool part_type_exists(int id)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    return true;
  }
  return false;
}
void part_type_clear(int id)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    initialize_particle_type((*it).second);
  }
}
// Shape.
void part_type_shape(int id, int particle_shape)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {

    enigma::pt_shape part_shape = enigma::get_pt_shape(particle_shape);

    enigma::particle_sprite* sprite = enigma::get_particle_sprite(part_shape);
    if (sprite != 0) {
       (*it).second->is_particle_sprite = true;
       (*it).second->part_sprite = sprite;
    }
  }
}
void part_type_sprite(int id, int sprite, bool animat, bool stretch, bool random)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end() && sprite_exists(sprite)) {
       (*it).second->is_particle_sprite = false;
       (*it).second->sprite_id = sprite;
       (*it).second->sprite_animated = animat;
       (*it).second->sprite_stretched = stretch;
       (*it).second->sprite_random = random;
  }
}
void part_type_size(int id, double size_min, double size_max, double size_incr, double size_wiggle)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->size_min = size_min;
    (*it).second->size_max = std::max(size_min, size_max);
    (*it).second->size_incr = size_incr;
    (*it).second->size_wiggle = size_wiggle;
  }
}
void part_type_scale(int id, double xscale, double yscale)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->xscale = xscale;
    (*it).second->yscale = yscale;
  }
}
void part_type_orientation(int id, double ang_min, double ang_max, double ang_incr, double ang_wiggle, bool ang_relative)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    ang_min = std::max(ang_min, 0.0);
    ang_max = std::max(ang_min, ang_max);
    (*it).second->ang_min = ang_min;
    (*it).second->ang_max = ang_max;
    (*it).second->ang_incr = ang_incr;
    (*it).second->ang_wiggle = ang_wiggle;
    (*it).second->ang_relative = ang_relative;
  }
}
// Color and blending.
void part_type_color1(int id, int color1)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->c_mode = enigma::one_color;
    (*it).second->color1 = color1;
  }
}
void part_type_color2(int id, int color1, int color2)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->c_mode = enigma::two_color;
    (*it).second->color1 = color1;
    (*it).second->color2 = color2;
  }
}
void part_type_color3(int id, int color1, int color2, int color3)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->c_mode = enigma::three_color;
    (*it).second->color1 = color1;
    (*it).second->color2 = color2;
    (*it).second->color3 = color3;
  }
}
void part_type_color_mix(int id, int color1, int color2)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->c_mode = enigma::mix_color;
    unsigned char r1 = color_get_red(color1), g1 = color_get_green(color1), b1 = color_get_blue(color1);
    unsigned char r2 = color_get_red(color2), g2 = color_get_green(color2), b2 = color_get_blue(color2);
    enigma::particle_type* pt = (*it).second;
    pt->rmin = r1, pt->rmax = r2;
    pt->gmin = g1, pt->gmax = g2;
    pt->bmin = b1, pt->bmax = b2;
  }
}
void part_type_color_rgb(int id, int rmin, int rmax, int gmin, int gmax, int bmin, int bmax)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->c_mode = enigma::rgb_color;
    rmin = bounds(rmin, 0, 255);
    rmax = bounds(rmax, 0, 255);
    rmax = std::max(rmin, rmax);
    gmin = bounds(gmin, 0, 255);
    gmax = bounds(gmax, 0, 255);
    gmax = std::max(gmin, gmax);
    bmin = bounds(bmin, 0, 255);
    bmax = bounds(bmax, 0, 255);
    bmax = std::max(bmin, bmax);
    enigma::particle_type* pt = (*it).second;
    pt->rmin = rmin, pt->rmax = rmax;
    pt->gmin = gmin, pt->gmax = gmax;
    pt->bmin = bmin, pt->bmax = bmax;
  }
}
void part_type_color_hsv(int id, int hmin, int hmax, int smin, int smax, int vmin, int vmax)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->c_mode = enigma::hsv_color;
    hmin = bounds(hmin, 0, 255);
    hmax = bounds(hmax, 0, 255);
    hmax = std::max(hmin, hmax);
    vmin = bounds(vmin, 0, 255);
    vmax = bounds(vmax, 0, 255);
    vmax = std::max(vmin, vmax);
    smin = bounds(smin, 0, 255);
    smax = bounds(smax, 0, 255);
    smax = std::max(smin, smax);
    enigma::particle_type* pt = (*it).second;
    pt->hmin = hmin, pt->hmax = hmax;
    pt->vmin = vmin, pt->vmax = vmax;
    pt->smin = smin, pt->smax = smax;
  }
}
void part_type_alpha1(int id, double alpha1)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->a_mode = enigma::one_alpha;
    (*it).second->alpha1 = bounds(int(255*alpha1), 0, 255);
  }
}
void part_type_alpha2(int id, double alpha1, double alpha2)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->a_mode = enigma::two_alpha;
    (*it).second->alpha1 = bounds(int(255*alpha1), 0, 255);
    (*it).second->alpha2 = bounds(int(255*alpha2), 0, 255);
  }
}
void part_type_alpha3(int id, double alpha1, double alpha2, double alpha3)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->a_mode = enigma::three_alpha;
    (*it).second->alpha1 = bounds(int(255*alpha1), 0, 255);
    (*it).second->alpha2 = bounds(int(255*alpha2), 0, 255);
    (*it).second->alpha3 = bounds(int(255*alpha3), 0, 255);
  }
}
void part_type_blend(int id, bool additive)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->blend_additive = additive;
  }
}
// Life and death.
void part_type_life(int id, int life_min, int life_max)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    life_min = std::max(life_min, 1);
    life_max = std::max(life_max, 1);
    (*it).second->life_min = std::min(life_min, life_max);
    (*it).second->life_max = std::max(life_min, life_max);
  }
}
void part_type_step(int id, int step_number, int step_type)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->step_on = true;
    (*it).second->step_number = step_number;
    (*it).second->step_particle_id = step_type;
  }
}
void part_type_death(int id, int death_number, int death_type)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->death_on = true;
    (*it).second->death_number = death_number;
    (*it).second->death_particle_id = death_type;
  }
}
// Motion.
void part_type_speed(int id, double speed_min, double speed_max, double speed_incr, double speed_wiggle)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    speed_min = std::max(speed_min, 0.0);
    speed_max = std::max(speed_min, speed_max);
    (*it).second->speed_min = speed_min;
    (*it).second->speed_max = speed_max;
    (*it).second->speed_incr = speed_incr;
    (*it).second->speed_wiggle = speed_wiggle;
  }
}
void part_type_direction(int id, double dir_min, double dir_max, double dir_incr, double dir_wiggle)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    dir_min = std::max(dir_min, 0.0);
    dir_max = std::max(dir_min, dir_max);
    (*it).second->dir_min = dir_min;
    (*it).second->dir_max = dir_max;
    (*it).second->dir_incr = dir_incr;
    (*it).second->dir_wiggle = dir_wiggle;
  }
}
void part_type_gravity(int id, double grav_amount, double grav_dir)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->grav_amount = grav_amount;
    (*it).second->grav_dir = grav_dir;
  }
}

