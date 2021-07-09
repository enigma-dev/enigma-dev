/** Copyright (C) 2008-2013 Josh Ventura, Dave "biggoron", Harijs Grinbergs
*** Copyright (C) 2013-2014 Robert B. Colton, Harijs Grinbergs
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

#include "surface_impl.h"
#include "textures_impl.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GSsurface.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GStextures_impl.h"
#include "Graphics_Systems/graphics_mandatory.h"

#ifdef DEBUG_MODE
#include "Widget_Systems/widgets_mandatory.h" // for show_error
#endif

using namespace std;

namespace enigma_user {



int surface_get_target()
{
  return enigma::bound_framebuffer;
}


int surface_get_depth_texture(int id)
{
  get_surfacev(surf,id,-1);
  #ifdef DEBUG_MODE
  if (surf.write_only == true){
    DEBUG_MESSAGE("Cannot get depth texture from write only surface with ID = " + std::to_string(id), MESSAGE_TYPE::M_ERROR);
    return -1;
  }else if (surf.has_depth_buffer == false){
    DEBUG_MESSAGE("Cannot get depth texture from a surface without a depth buffer. Surface ID = " + std::to_string(id), MESSAGE_TYPE::M_ERROR);
    return -1;
  }
  #endif
  return (surf.depth_buffer);
}

}
