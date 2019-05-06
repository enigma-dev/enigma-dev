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

#include <cmath>
#include <cstdlib>
#include <algorithm>

#include "Universal_System/math_consts.h"
#include <Widget_Systems/widgets_mandatory.h> // show_error

#include "PS_particle_emitter.h"
#include "PS_particle_system.h"
#include "PS_particle_system_manager.h"
#include "PS_particle.h"

namespace enigma
{
  void particle_emitter::initialize()
  {
    xmin = 0, xmax = 0, ymin = 0, ymax = 0;
    shape = ps_sh_rectangle;
    distribution = ps_di_linear;
    particle_type_id = -1;
    number = 0;
  }
  particle_emitter* create_particle_emitter()
  {
    particle_emitter* pe = new particle_emitter();
    pe->initialize();
    return pe;
  }
  void particle_emitter::clear_particle_emitter()
  {
    initialize();
  }
  void particle_emitter::set_region(double p_xmin, double p_xmax, double p_ymin, double p_ymax, ps_shape p_shape, ps_distr p_distribution)
  {
    // "min" and "max" is somewhat weird in these cases. For instance, by having p_xmin > p_xmax and p_ymin < p_ymax,
    // the line region gives a line with a positive slope instead of the usual negative slope.
    // This is consistent with GM.
    xmin = p_xmin;
    xmax = p_xmax;
    ymin = p_ymin;
    ymax = p_ymax;

    shape = p_shape;
    distribution = p_distribution;
  }
  void particle_emitter::set_stream(int pt_id, int p_number)
  {
    particle_type_id = pt_id;
    number = p_number;
  }
  // Returns value in range [0, 1] with the given distribution approximated.
  // Note that since the normal distribution does not fit inside [0, 1],
  // and cannot be scaled correctly to [0, 1] due to being ]-inf, inf[,
  // the values returned are not really normal-distributed.
  static double get_random_value(ps_distr distribution)
  {
    switch (distribution) {
    case ps_di_linear: {
      double value = 1.0 * rand() / (RAND_MAX-1);
      return value;
    }
    case ps_di_gaussian: {
      double normal_value = 0.0;
      const double range = 2.6; // Heuristic value found through testing.
      // Iterate until value in range is found or max tries has been reached.
      const int max_sampling_tries = 10;
      for (int i = 0; i < max_sampling_tries; i++)
      {
        // Box-Muller method.
        // The uniform values are in the range ]0, 1] to avoid calculating log(0).
        double uniform_value1 = 1.0 * (rand()+1) / RAND_MAX;
        double uniform_value2 = 1.0 * (rand()+1) / RAND_MAX;
        normal_value = sqrt(-2.0*log(uniform_value1))*cos(2.0*M_PI*uniform_value2);
        if (normal_value >= -range && normal_value <= range) {
          return fabs(normal_value/range);
        }
      }
      return 1.0 * rand() / (RAND_MAX-1);
    }
    case ps_di_invgaussian: {
      double normal_value = 0.0;
      const double range = 2.6; // Heuristic value found through testing.
      // Iterate until value in range is found or max tries has been reached.
      const int max_sampling_tries = 10;
      for (int i = 0; i < max_sampling_tries; i++)
      {
        // Box-Muller method.
        // The uniform values are in the range ]0, 1] to avoid calculating log(0).
        double uniform_value1 = 1.0 * (rand()+1) / RAND_MAX;
        double uniform_value2 = 1.0 * (rand()+1) / RAND_MAX;
        normal_value = sqrt(-2.0*log(uniform_value1))*cos(2.0*M_PI*uniform_value2);
        if (normal_value >= -range && normal_value <= range) {
          return 1.0 - fabs(normal_value/range);
        }
      }
      return 1.0 * rand() / (RAND_MAX-1);
    }
    default: {
      double value = 1.0 * rand() / (RAND_MAX-1);
      return value;
    }
    }
  }
  using std::abs;
  inline double sqr(double x) {return x*x;}
  void particle_emitter::get_point(int& x, int& y)
  {
    switch (shape) {
    case ps_shape_rectangle: {
      const double a = abs(xmin-xmax)/2.0;
      const double b = abs(ymin-ymax)/2.0;
      const double center_x = std::min(xmin, xmax) + a;
      const double center_y = std::min(ymin, ymax) + b;
      const int sign1 = rand() % 2 == 0 ? 1 : -1;
      const int on1 = rand() % 2 == 0 ? 1 : 0;
      const int on2 = 1 - on1;
      const double v1x = a*sign1;
      const double v1y = -b*sign1;
      const double v2x = -2.0*a*on1*sign1;
      const double v2y = 2.0*b*on2*sign1;
      double d1 = get_random_value(distribution);
      double d2 = 1.0 * rand() / (RAND_MAX-1);
      // The sqrt ensures that more points are put further out from the center,
      // since there is more area further out from the center.
      x = center_x + v1x*sqrt(d1) + sqrt(d1)*d2*v2x;
      y = center_y + v1y*sqrt(d1) + sqrt(d1)*d2*v2y;
      return;
    }
    case ps_shape_ellipse: {
      const double a = abs(xmin-xmax)/2.0;
      const double b = abs(ymin-ymax)/2.0;
      double center_x = std::min(xmin, xmax) + a;
      double center_y = std::min(ymin, ymax) + b;
      double angle_radians = 2.0*M_PI*1.0 * rand() / (RAND_MAX-1);
      double random_value = get_random_value(distribution);
      // The sqrt ensures that more points are put further out from the center,
      // since there is more area further out from the center.
      x = center_x + sqrt(random_value)*a*cos(angle_radians);
      y = center_y + sqrt(random_value)*b*sin(angle_radians);
      return;
    }
    case ps_shape_diamond: {
      const double a = abs(xmin-xmax)/2.0;
      const double b = abs(ymin-ymax)/2.0;
      const double center_x = std::min(xmin, xmax) + a;
      const double center_y = std::min(ymin, ymax) + b;
      const int sign1 = rand() % 2 == 0 ? 1 : -1;
      const int sign2 = rand() % 2 == 0 ? 1 : -1;
      const double v1x = 0.0;
      const double v1y = -b*sign1;
      const double v2x = -a*sign2;
      const double v2y = b*sign1;
      double d1 = get_random_value(distribution);
      double d2 = 1.0 * rand() / (RAND_MAX-1);
      // The sqrt ensures that more points are put further out from the center,
      // since there is more area further out from the center.
      x = center_x + v1x*sqrt(d1) + sqrt(d1)*d2*v2x;
      y = center_y + v1y*sqrt(d1) + sqrt(d1)*d2*v2y;
      return;
    }
    case ps_shape_line: {
      const double a = (xmax-xmin)/2.0;
      const double b = (ymax-ymin)/2.0;
      const double origin_x = xmin + a;
      const double origin_y = ymin + b;
      const bool r1 = rand() % 2 == 0;
      const double v1x = r1 ? a : -a;
      const double v1y = r1 ? b : -b;
      const double rand1 = get_random_value(distribution);
      x = origin_x + rand1*v1x;
      y = origin_y + rand1*v1y;
      return;
    }
    default:
      #if DEBUG_MODE
      show_error("Internal error: invalid particle shape", false)
      #endif
      ;
    }
  }
  int particle_emitter::get_step_number()
  {
    if (number >= 0) {
      return number;
    }
    else {
      return rand() % (-number) < 1 ? 1 : 0; // Create particle with probability -1/number.
    }
  }
}

using enigma::particle_system;
using enigma::particle_type;
using enigma::particle_emitter;

namespace enigma_user {
  int part_emitter_create(int id)
  {
    particle_system* p_s = enigma::get_particlesystem(id);
    if (p_s != NULL) {
      return p_s->create_emitter();
    }
    return -1;
  }
  void part_emitter_destroy(int ps_id, int em_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_emitter*>::iterator em_it = p_s->id_to_emitter.find(em_id);
      if (em_it != p_s->id_to_emitter.end()) {
        delete (*em_it).second;
        p_s->id_to_emitter.erase(em_it);
      }
    }
  }
  void part_emitter_destroy_all(int ps_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      for (std::map<int,particle_emitter*>::iterator it = p_s->id_to_emitter.begin(); it != p_s->id_to_emitter.end(); it++)
      {
        delete (*it).second;
      }
      p_s->id_to_emitter.clear();
    }
  }
  bool part_emitter_exists(int ps_id, int em_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_emitter*>::iterator em_it = p_s->id_to_emitter.find(em_id);
      if (em_it != p_s->id_to_emitter.end()) {
        return true;
      }
    }
    return false;
  }
  void part_emitter_clear(int ps_id, int em_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_emitter*>::iterator em_it = p_s->id_to_emitter.find(em_id);
      if (em_it != p_s->id_to_emitter.end()) {
        (*em_it).second->initialize();
      }
    }
  }
  void part_emitter_region(int ps_id, int em_id, double xmin, double xmax, double ymin, double ymax, int shape, int distribution)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      p_s->set_emitter_region(em_id, xmin, xmax, ymin, ymax, enigma::get_ps_shape(shape), enigma::get_ps_distr(distribution));
    }
  }
  void part_emitter_burst(int ps_id, int em_id, int particle_type_id, int number)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_emitter*>::iterator em_it = p_s->id_to_emitter.find(em_id);
      if (em_it != p_s->id_to_emitter.end()) {
        particle_emitter* p_e = (*em_it).second;
        particle_type* p_t = enigma::get_particletype(particle_type_id);
        if (p_t != NULL) {
          for (int i = 1; i <= number; i++)
          {
            int x = 0, y = 0;
            p_e->get_point(x, y);
            p_s->create_particles(x, y, p_t, 1);
          }
        }
      }
    }
  }
  void part_emitter_stream(int ps_id, int em_id, int particle_type_id, int number)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL && enigma::get_particletype(particle_type_id) != NULL) {
      p_s->set_emitter_stream(em_id, particle_type_id, number);
    }
  }
}

