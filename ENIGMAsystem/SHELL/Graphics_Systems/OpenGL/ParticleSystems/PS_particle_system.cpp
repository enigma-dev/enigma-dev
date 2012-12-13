/********************************************************************************\
**                                                                              **
**  Copyright (C) 2012 forthevin                                                **
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

#include "PS_particle_system.h"
#include "PS_particle.h"
#include "PS_particle_type.h"
#include "PS_particle_instance.h"
#include "PS_particle_emitter.h"
#include "Universal_System/depth_draw.h"
#include "Graphics_Systems/OpenGL/GSstdraw.h"
#include "Graphics_Systems/OpenGL/binding.h"
#include <GL/gl.h> //TODO: Consider this.
#include <cmath>
#include <cstdlib>
#include <list>

using enigma::pt_manager;

namespace enigma
{
  struct particle_system
  {
    // Particles.
    bool oldtonew;
    double x_offset, y_offset;
    double depth; // Integer stored as double.
    std::list<particle_instance> pi_list;
    bool auto_update, auto_draw;
    void initialize();
    void update_particlesystem();
    void draw_particlesystem();
    void create_particles(double x, double y, particle_type* pt, int number);
    // Emitters.
    std::map<int,particle_emitter*> id_to_emitter;
    int emitter_max_id;
    int create_emitter();
    void set_emitter_region(int em_id, double xmin, double xmax, double ymin, double ymax, ps_shape shape, ps_distr distribution);
    void set_emitter_stream(int em_id, int particle_type_id, int number);

    // TODO: Write emitters, attractors, destroyers, deflectors and changers,
    // and create a map of each in the particle_system.
  };
  void particle_system::initialize()
  {
    oldtonew = true;
    auto_update = true, auto_draw = true;
    depth = 0.0;
    pi_list = std::list<particle_instance>();
    id_to_emitter = std::map<int,particle_emitter*>();
    emitter_max_id = 0;
  }
  void particle_system::update_particlesystem()
  {
    // Emitters.
    {
      std::map<int,particle_emitter*>::iterator end = id_to_emitter.end();
      for (std::map<int,particle_emitter*>::iterator it = id_to_emitter.begin(); it != end; it++)
      {
        particle_emitter* p_e = (*it).second;
        std::map<int,particle_type*>::iterator pt_it = pt_manager.id_to_particletype.find(p_e->particle_type_id);
        if (pt_it != pt_manager.id_to_particletype.end()) {
          particle_type* p_t = (*pt_it).second;
          const int number = p_e->get_step_number();
          for (int i = 1; i <= number; i++)
          {
            int x, y;
            p_e->get_point(x, y);
            create_particles(x, y, p_t, 1);
          }
        }
      }
    }
    // Handle life and death.
    for (std::list<particle_instance>::iterator it = pi_list.begin(); it !=pi_list.end(); it++)
    {
      // Decrease life.
      //TODO: When dying due to end of life (not due to destroyers), particles may be spawned.
      it->life_current--;
      if (it->life_current <= 0) {
        particle_type* pt = it->pt;
        pt->particle_count--;
        if (pt->particle_count <= 0 && !pt->alive) {
          // Particle type is no longer used, delete it.
          delete pt;
        }
        it = pi_list.erase(it);
      }
    }
    // Move particles.
    {
      std::list<particle_instance>::iterator end = pi_list.end();
      for (std::list<particle_instance>::iterator it = pi_list.begin(); it !=end; it++)
      {
        it->x += it->vx;
        it->y += it->vy;
        // TODO: Implement direction and speed change.
      }
    }
  }
  void particle_system::draw_particlesystem()
  {
    // TODO: Handle different particle shapes.
    // TODO: Draw the particle system either from oldest to youngest or reverse.

    const std::list<particle_instance>::iterator end = pi_list.end();
    for (std::list<particle_instance>::iterator it = pi_list.begin(); it != end; it++)
    {
      particle_type* pt = it->pt;
      // TODO: Use default shape if particle type not alive.
      if (pt->is_particle_sprite) {
        particle_sprite* ps = pt->part_sprite;
        bind_texture(ps->texture);

        glPushAttrib(GL_CURRENT_BIT); // Push 1.

        glColor4f(1,1,1,1); // TODO: Color may change.
        const float tbx = 1, tby = 1,
          xvert1 = it->x - ps->width/2, xvert2 = it->x + ps->width/2,
          yvert1 = it->y - ps->height/2, yvert2 = it->y + ps->height/2;

        glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex2f(xvert1,yvert1);
        glTexCoord2f(tbx,0);
        glVertex2f(xvert2,yvert1);
        glTexCoord2f(tbx,tby);
        glVertex2f(xvert2,yvert2);
        glTexCoord2f(0,tby);
        glVertex2f(xvert1,yvert2);
        glEnd();

	glPopAttrib(); // Pop 1.
      }
    }
  }
  void particle_system::create_particles(double x, double y, particle_type* pt, int number)
  {
    if (number > 0) {
      pt->particle_count += number;
    }
    for (int i = 0; i < number; i++)
    {
      particle_instance pi;
      pi.pt = pt;
      pi.life_current = pt->life_min == pt->life_max ? pt->life_min : pt->life_min + rand() % (	pt->life_max - pt->life_min);
      pi.x = x;
      pi.y = y;
      pi.vx = pt->speed_min*cos(pt->dir_min*M_PI/180.0); // TODO: Implement randomness.
      pi.vy = -pt->speed_min*sin(pt->dir_min*M_PI/180.0); // TODO: Implement randomness.
      pi_list.push_back(pi);
    }
  }
  int particle_system::create_emitter()
  {
    particle_emitter* p_e = new particle_emitter();
    p_e->initialize();

    emitter_max_id++;
    id_to_emitter.insert(std::pair<int,particle_emitter*>(emitter_max_id, p_e));

    return emitter_max_id;
  }
  void particle_system::set_emitter_region(int em_id, double xmin, double xmax, double ymin, double ymax, ps_shape shape, ps_distr distribution)
  {
    std::map<int,particle_emitter*>::iterator pe_it = id_to_emitter.find(em_id);
    if (pe_it != id_to_emitter.end()) {
      (*pe_it).second->set_region(xmin, xmax, ymin, ymax, shape, distribution);
    }
  }
  void particle_system::set_emitter_stream(int em_id, int particle_type_id, int number)
  {
    std::map<int,particle_emitter*>::iterator pe_it = id_to_emitter.find(em_id);
    if (pe_it != id_to_emitter.end()) {
      (*pe_it).second->set_stream(particle_type_id, number);
    }
  }
}

namespace enigma
{
  struct particle_system_manager
  {
    int max_id;
    std::map<int,particle_system*> id_to_particlesystem;
  };

  particle_system_manager ps_manager;

  void update_particlesystems()
  {
    std::map<int,particle_system*>::iterator end = ps_manager.id_to_particlesystem.end();
    for (std::map<int,particle_system*>::iterator it = ps_manager.id_to_particlesystem.begin(); it != end; it++)
    {
      if ((*it).second->auto_update) {
        (*it).second->update_particlesystem();
      }
    }
  }

  void draw_particlesystems(std::set<int>& particlesystem_ids)
  {
    std::set<int>::iterator end = particlesystem_ids.end();
    for (std::set<int>::iterator it = particlesystem_ids.begin(); it != end; it++)
    {
      std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(*it);
      if (ps_it != ps_manager.id_to_particlesystem.end()) {
        (*ps_it).second->draw_particlesystem();
      }
    }
  }
}

using enigma::particle_system;
using enigma::particle_type;
using enigma::ps_manager;
using enigma::ps_manager;
using enigma::particle_type_manager;

// General functions.

int part_system_create()
{
  particle_system* p_s = new particle_system();
  p_s->initialize();

  ps_manager.max_id++;
  ps_manager.id_to_particlesystem.insert(std::pair<int,particle_system*>(ps_manager.max_id, p_s));

  // Drawing is automatic, so register in depth.
  enigma::drawing_depths[p_s->depth].particlesystem_ids.insert(ps_manager.max_id);

  return ps_manager.max_id;
}
void part_system_destroy(int id);
void part_system_exists(int id);
void part_system_clear(int id);
void part_system_draw_order(int id, bool oldtonew);

// Update and draw.

void part_system_automatic_update(int id, bool automatic);
void part_system_automatic_draw(int id, bool automatic); // TODO: Remember to register/unregister with depth_draw.
void part_system_update(int id)
{
  std::map<int,particle_system*>::iterator it = ps_manager.id_to_particlesystem.find(id);
  if (it != ps_manager.id_to_particlesystem.end()) {
    (*it).second->update_particlesystem();
  }
}
void part_system_drawit(int id)
{
  std::map<int,particle_system*>::iterator it = ps_manager.id_to_particlesystem.find(id);
  if (it != ps_manager.id_to_particlesystem.end()) {
    (*it).second->draw_particlesystem();
  }
}

// Particles.

void part_particles_create(int id, double x, double y, int particle_type_id, int number)
{
  std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(id);
  if (ps_it != ps_manager.id_to_particlesystem.end()) {
    std::map<int,particle_type*>::iterator pt_it = pt_manager.id_to_particletype.find(id);
    if (pt_it != pt_manager.id_to_particletype.end()) {
      (*ps_it).second->create_particles(x, y, (*pt_it).second, number);
    }
  }
}
void part_particles_create_color(int id, double x, double y, int particle_type_id, int color, int number);
void part_particles_clear(int id);
int part_particles_count(int id);

// Emitters.

int part_emitter_create(int id)
{
  std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(id);
  if (ps_it != ps_manager.id_to_particlesystem.end()) {
    return (*ps_it).second->create_emitter();
  }
  return -1;
}
void part_emitter_destroy(int ps_id, int em_id);
void part_emitter_destroy_all(int ps_id);
void part_emitter_exists(int ps_id, int em_id);
void part_emitter_clear(int ps_id, int em_id);
void part_emitter_region(int ps_id, int em_id, double xmin, double xmax, double ymin, double ymax, ps_shape shape, ps_distr distribution)
{
  std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(ps_id);
  if (ps_it != ps_manager.id_to_particlesystem.end()) {
    (*ps_it).second->set_emitter_region(em_id, xmin, xmax, ymin, ymax, shape, distribution);
  }
}
void part_emitter_burst(int ps_id, int em_id, int particle_type_id, int number);
void part_emitter_stream(int ps_id, int em_id, int particle_type_id, int number)
{
  std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(ps_id);
  if (ps_it != ps_manager.id_to_particlesystem.end()) {
    (*ps_it).second->set_emitter_stream(em_id, particle_type_id, number);
  }
}

