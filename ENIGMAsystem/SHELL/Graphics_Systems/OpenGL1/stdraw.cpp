/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton, Serpex
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
#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GScolor_macros.h"

namespace enigma_user
{

void draw_clear_alpha(int col,float alpha)
{
  draw_batch_flush(batch_flush_deferred);
  //Unfortunately, we lack a 255-based method for setting ClearColor.
  glClearColor(COL_GET_Rf(col),COL_GET_Gf(col),COL_GET_Bf(col),alpha);
  glClear(GL_COLOR_BUFFER_BIT);
}
void draw_clear(int col)
{
  draw_batch_flush(batch_flush_deferred);
  glClearColor(COL_GET_Rf(col),COL_GET_Gf(col),COL_GET_Bf(col),1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

int draw_get_msaa_maxlevel()
{
  int maximumMSAA;
  glGetIntegerv(GL_MAX_SAMPLES, &maximumMSAA);
  return maximumMSAA;
}

bool draw_get_msaa_supported()
{
  return GLEW_EXT_multisample;
}

} // namespace enigma_user
