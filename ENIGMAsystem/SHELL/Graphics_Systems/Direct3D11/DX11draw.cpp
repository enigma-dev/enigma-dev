/** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2014 Seth N. Hetu
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

#include "Direct3D11Headers.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Universal_System/roomsystem.h"

#include <vector>
#include <math.h>
#include <stdio.h>
using std::vector;

namespace enigma_user
{

int draw_get_msaa_maxlevel()
{
  return 0; //TODO: implement
}

bool draw_get_msaa_supported()
{
  return false; //TODO: implement
}

void draw_set_msaa_enabled(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
}

void draw_enable_alphablend(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
}

bool draw_get_alpha_test()
{
  return false; //TODO: implement
}

unsigned draw_get_alpha_test_ref_value()
{
  return 0; //TODO: implement
}

void draw_set_alpha_test(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
}

void draw_set_alpha_test_ref_value(unsigned val)
{
  draw_batch_flush(batch_flush_deferred);
}

void draw_set_line_pattern(int pattern, int scale)
{
  draw_batch_flush(batch_flush_deferred);
}

} // namespace enigma_user

//#include <endian.h>
//TODO: Though serprex, the author of the function below, never included endian.h,
//   // Doing so is necessary for the function to work at its peak.
//   // When ENIGMA generates configuration files, one should be included here.

namespace enigma_user
{

int draw_getpixel(int x, int y)
{
  draw_batch_flush(batch_flush_deferred);
  return 0; //TODO: implement
}

int draw_getpixel_ext(int x, int y)
{
  draw_batch_flush(batch_flush_deferred);
  return 0; //TODO: implement
}

} // namespace enigma_user

namespace enigma {

bool fill_complex_polygon(const std::list<PolyVertex>& vertices, int defaultColor, bool allowHoles)
{
  enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
  //TODO: Complex polygon supported only in OpenGL1 at the moment. By returning false here, we fall back
  //      on a convex-only polygon drawing routine that works on any platform.
  return false;
}

} // namespace enigma
