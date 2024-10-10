/** Copyright (C) 2008-2013 Josh Ventura, forthevin, Robert B. Colton
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

#ifndef ENIGMA_PS_PARTICLE_BRIDGE_FALLBACK_H
#define ENIGMA_PS_PARTICLE_BRIDGE_FALLBACK_H

#include "PS_particle_instance.h"
#include "PS_particle_sprites.h"
#include <vector>
#include <cmath>
#include <algorithm>
//#include <GL/gl.h>

#include "PS_particle_system.h"
#include "PS_particle.h"

#include "Graphics_Systems/General/GSblend.h"

namespace enigma {
  namespace particle_bridge {
    void initialize_particle_bridge() {}; // Do nothing, nothing to initialize.
    double wiggle;
    int subimage_index;
    double x_offset;
    double y_offset;
    double get_wiggle_result(double wiggle_offset, double wiggle)
    {
      double result_wiggle = wiggle + wiggle_offset;
      result_wiggle = result_wiggle > 1.0 ? result_wiggle - 1.0 : result_wiggle;
      if (result_wiggle < 0.5) return -1.0 + 4*result_wiggle;
      else return 3.0 - 4.0*result_wiggle;
    }
    static void draw_particle(particle_instance* it)
    {
      int color = it->color;
      int alpha = it->alpha;
      if (it->pt->alive) {
        particle_type* pt = it->pt;

        double size;
        double rot_degrees;
        size = std::max(0.0, it->size + pt->size_wiggle*get_wiggle_result(it->size_wiggle_offset, wiggle));
        rot_degrees = it->angle + pt->ang_wiggle*get_wiggle_result(it->ang_wiggle_offset, wiggle);
        if (pt->ang_relative) {
          rot_degrees += it->direction;
        }

        if (size <= 0) return; // NOTE: Skip to next particle.

        if (!pt->is_particle_sprite) { // Draw sprite.
          int sprite_id = pt->sprite_id;
          if (!enigma_user::sprite_exists(sprite_id)) return; // NOTE: Skip to next particle.
          int subimg;
          if (!pt->sprite_animated) {
            subimg = it->sprite_subimageindex_initial;
          }
          else {
            const Sprite& spr = sprites.get(pt->sprite_id);
            const int subimage_count = spr.SubimageCount();
            if (pt->sprite_stretched) {
              subimg = int(subimage_count*(1.0 - 1.0*it->life_current/it->life_start));
              subimg = subimg >= subimage_count ? subimage_count - 1 : subimg;
              subimg = subimg % subimage_count;
            }
            else {
              subimg = (subimage_index + it->sprite_subimageindex_initial) % subimage_count;
            }
          }
          const double x = it->x, y = it->y;
          const double xscale = pt->xscale*size, yscale = pt->yscale*size;

          if (pt->blend_additive != enigma::blendMode[0]){
              if (pt->blend_additive) {
                enigma_user::draw_set_blend_mode(enigma_user::bm_add);
              }
              else {
                enigma_user::draw_set_blend_mode(enigma_user::bm_normal);
              }
          }

          enigma_user::draw_sprite_ext(sprite_id, subimg, x + x_offset, y + y_offset, xscale, yscale, rot_degrees, color, (double)alpha/255.0);
        }
        else { // Draw particle sprite.

          particle_sprite* ps = pt->part_sprite;

          const double x = it->x, y = it->y;
          const double xscale = pt->xscale*size, yscale = pt->yscale*size;

          int sprite_id = get_particle_actual_sprite(ps->shape);

          if (pt->blend_additive != enigma::blendMode[0]){
              if (pt->blend_additive) {
                draw_set_blend_mode(enigma_user::bm_add);
              } else {
                draw_set_blend_mode(enigma_user::bm_normal);
              }
          }

          enigma_user::draw_sprite_ext(sprite_id, 0, x + x_offset, y + y_offset, xscale, yscale, rot_degrees, color, (double)alpha/255.0);
        }
      }
      else { // Draw particle in a limited way if particle type not alive.
        double size = it->size;
        double rot_degrees = it->angle;
        if (size <= 0) return; // NOTE: Skip to next particle.

        particle_sprite* ps = get_particle_sprite(pt_sh_pixel);
        if (ps == NULL) return; // NOTE: Skip to next particle.

        if (enigma::blendMode[0] != 0){
            draw_set_blend_mode(enigma_user::bm_normal);
        }

        const double x = round(it->x), y = round(it->y);
        const double xscale = size, yscale = size;

        int sprite_id = get_particle_actual_sprite(ps->shape);

        enigma_user::draw_sprite_ext(sprite_id, 0, x + x_offset, y + y_offset, xscale, yscale, rot_degrees, color, (double)alpha/255.0);
      }
    }
    void draw_particles(std::vector<particle_instance>& pi_list, bool oldtonew, double a_wiggle, int a_subimage_index,
      double a_x_offset, double a_y_offset)
    {
        using namespace enigma::particle_bridge;
        wiggle = a_wiggle;
        subimage_index = a_subimage_index;
        x_offset = a_x_offset;
        y_offset = a_y_offset;

        // Draw the particle system either from oldest to youngest or reverse.
        int blend_src  = enigma::blendMode[0];
        int blend_dest = enigma::blendMode[1];

        if (oldtonew) {
          const std::vector<particle_instance>::iterator end = pi_list.end();
          for (std::vector<particle_instance>::iterator it = pi_list.begin(); it != end; it++)
          {
            draw_particle(&(*it));
          }
        } else {
          const std::vector<particle_instance>::reverse_iterator rend = pi_list.rend();
          for (std::vector<particle_instance>::reverse_iterator it = pi_list.rbegin(); it != rend; it++)
          {
            draw_particle(&(*it));
          }
        }

        if (enigma::blendMode[0] != blend_src || enigma::blendMode[1] != blend_dest){
          enigma_user::draw_set_blend_mode_ext(blend_src, blend_dest);
        }
    }
  }
}

#endif // ENIGMA_PS_PARTICLE_BRIDGE_FALLBACK_H
