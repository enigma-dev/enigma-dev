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

#ifndef ENIGMA_PS_PARTICLE_BRIDGE_OPENGL1_H
#define ENIGMA_PS_PARTICLE_BRIDGE_OPENGL1_H

#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GLbinding.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GLTextureStruct.h"
#include "Graphics_Systems/General/GScolors.h"
#include "PS_particle_instance.h"
#include "PS_particle_sprites.h"
#include <vector>
#include <cmath>
#include <algorithm>
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

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
      int color = it->color;
      int alpha = it->alpha;
      if (it->pt->alive) {
        particle_type* pt = it->pt;

        double size;
        double rot_degrees;
        size = std::max(0.0, it->size + pt->size_wiggle * particle_system::get_wiggle_result(it->size_wiggle_offset, wiggle));
        rot_degrees = it->angle + pt->ang_wiggle * particle_system::get_wiggle_result(it->ang_wiggle_offset, wiggle);
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
            const sprite *const spr = spritestructarray[pt->sprite_id];
            const int subimage_count = spr->subcount;
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

          if (pt->blend_additive) {
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);
          }
          else {
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
          }
          enigma_user::draw_sprite_ext(sprite_id, subimg, x, y, xscale, yscale, rot_degrees, color, alpha/255.0);
        }
        else { // Draw particle sprite.

          particle_sprite* ps = pt->part_sprite;
          texture_use(GmTextures[ps->texture]->gltex);

          if (pt->blend_additive) {
             glBlendFunc(GL_SRC_ALPHA,GL_ONE);
          }
          else {
              glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
          }

          glColor4ub(__GETR(color),__GETG(color),__GETB(color), alpha);

          const double rot = rot_degrees*M_PI/180.0;

          const double x = it->x, y = it->y;
          const double xscale = pt->xscale*size, yscale = pt->yscale*size;

          const float
          w = ps->width*xscale, h = ps->height*yscale,
          tbx = 1, tby = 1,
          wsinrot = w*sin(rot), wcosrot = w*cos(rot);

          glBegin(GL_TRIANGLE_STRIP);

          float
          ulcx = x - xscale * (ps->width/2.0) * cos(rot) + yscale * (ps->height/2.0) * cos(M_PI/2+rot),
          ulcy = y + xscale * (ps->width/2.0) * sin(rot) - yscale * (ps->height/2.0) * sin(M_PI/2+rot);
          glTexCoord2f(0,0);
          glVertex2f(ulcx,ulcy);
          glTexCoord2f(tbx,0);
          glVertex2f(ulcx + wcosrot, ulcy - wsinrot);

          const double mpr = 3*M_PI/2 + rot;
          ulcx += h * cos(mpr);
          ulcy -= h * sin(mpr);
          glTexCoord2f(0,tby);
          glVertex2f(ulcx,ulcy);
          glTexCoord2f(tbx,tby);
          glVertex2f(ulcx + wcosrot, ulcy - wsinrot);

          glEnd();
        }
      }
      else { // Draw particle in a limited way if particle type not alive.
        double size = it->size;
        double rot_degrees = it->angle;
        if (size <= 0) return; // NOTE: Skip to next particle.

        particle_sprite* ps = get_particle_sprite(pt_sh_pixel);
        if (ps == NULL) return; // NOTE: Skip to next particle.
        texture_use(GmTextures[ps->texture]->gltex);

        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glColor4ub(__GETR(color),__GETG(color),__GETB(color), alpha);

        const double rot = rot_degrees*M_PI/180.0;

        const double x = round(it->x), y = round(it->y);
        const double xscale = size, yscale = size;

        const float
        w = ps->width*xscale, h = ps->height*yscale,
        tbx = 1, tby = 1,
        wsinrot = w*sin(rot), wcosrot = w*cos(rot);

        glBegin(GL_TRIANGLE_STRIP);

        float
        ulcx = x - xscale * (ps->width/2.0) * cos(rot) + yscale * (ps->height/2.0) * cos(M_PI/2+rot),
        ulcy = y + xscale * (ps->width/2.0) * sin(rot) - yscale * (ps->height/2.0) * sin(M_PI/2+rot);
        glTexCoord2f(0,0);
        glVertex2f(ulcx,ulcy);
        glTexCoord2f(tbx,0);
        glVertex2f(ulcx + wcosrot, ulcy - wsinrot);

        const double mpr = 3*M_PI/2 + rot;
        ulcx += h * cos(mpr);
        ulcy -= h * sin(mpr);
        glTexCoord2f(0,tby);
        glVertex2f(ulcx,ulcy);
        glTexCoord2f(tbx,tby);
        glVertex2f(ulcx + wcosrot, ulcy - wsinrot);

        glEnd();
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

      glPushMatrix(); // Matrix push 1.

      glTranslated(x_offset, y_offset, 0.0);

      glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT); // Attrib push 1.

      // Draw the particle system either from oldest to youngest or reverse.
      if (oldtonew) {
        const std::vector<particle_instance>::iterator end = pi_list.end();
        for (std::vector<particle_instance>::iterator it = pi_list.begin(); it != end; it++)
        {
          draw_particle(&(*it));
        }
      }
      else {
        const std::vector<particle_instance>::reverse_iterator rend = pi_list.rend();
        for (std::vector<particle_instance>::reverse_iterator it = pi_list.rbegin(); it != rend; it++)
        {
          draw_particle(&(*it));
        }
      }

      glPopAttrib(); // Attrib pop 1.

      glPopMatrix(); // Matrix pop 1.
    }
  }
}

#endif // ENIGMA_PS_PARTICLE_BRIDGE_OPENGL1_H

