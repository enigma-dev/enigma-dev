/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
*** Copyright (C) 2014 Robert B. Colton
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

/* These functions are primarily for use of the engine. Experienced users
 * who are familiar with C++ can make use of these, but they were made to
 * use at load time with data read from the executable. These both expect
 * RAW format, RGB only.
 */

#ifndef ENIGMA_GSTEXTURES_H
#define ENIGMA_GSTEXTURES_H

#include "Universal_System/scalar.h"

#include <string>
using std::string;

namespace enigma
{
  extern bool interpolate_textures;
}

namespace enigma_user {
  enum {
    tx_none,
    tx_nearest,
    tx_bilinear,
    tx_trilinear
  };
}

namespace enigma_user
{
  int texture_add(string fname, bool mipmap=false);
  void texture_save(int texid, string fname);
  void texture_delete(int texid);
  bool texture_exists(int texid);
  void texture_preload(int texid);
  gs_scalar texture_get_width(int texid);
  gs_scalar texture_get_height(int texid);
  gs_scalar texture_get_texel_width(int texid);
  gs_scalar texture_get_texel_height(int texid);
  void texture_set_priority(int texid, double prio);
  void texture_set_enabled(bool enable);
  void texture_set(int texid);
  void texture_set_stage(int stage, int texid);
  #define texture_set(texid) texture_set_stage(0, texid)
  void texture_reset();
  void texture_set_blending(bool enable);

  void texture_set_repeat(bool repeat);
  void texture_set_repeat_ext(int sampler, bool repeat);
  #define texture_set_repeat(repeat) texture_set_repeat_ext(0, repeat)
  void texture_set_wrap(bool wrapu, bool wrapv, bool wrapw);
  void texture_set_wrap_ext(int sampler, bool wrapu, bool wrapv, bool wrapw);
  #define texture_set_wrap(wrapu, wrapv, wrapw) texture_set_wrap_ext(0, wrapu, wrapv, wrapw)
  void texture_set_border(int r, int g, int b, double a);
  void texture_set_border_ext(int sampler, int r, int g, int b, double a);
  #define texture_set_border(r, g, b, a) texture_set_border_ext(0, r, g, b, a)
  void texture_set_filter(int filter);
  void texture_set_filter_ext(int sampler, int filter);
  #define texture_set_filter(filter) texture_set_filter_ext(0, filter)
  void texture_set_lod(gs_scalar minlod, gs_scalar maxlod, int maxlevel);
  void texture_set_lod_ext(int sampler, gs_scalar minlod, gs_scalar maxlod, int maxlevel);
  #define texture_set_lod(minlod, maxlod, maxlevel) texture_set_lod_ext(0, minlod, maxlod, maxlevel)

  bool texture_mipmapping_supported();
  bool  texture_anisotropy_supported();
  float texture_anisotropy_maxlevel();
  void  texture_anisotropy_filter(int samplerid, gs_scalar level);
}

#endif //ENIGMA_GSTEXTURES_H
