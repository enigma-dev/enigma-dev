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

void surface_add_colorbuffer(int id, int index, int internalFormat, unsigned format, unsigned type){
  get_surface(surf,id);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, surf.fbo);
  int texture = enigma::graphics_create_texture_custom(enigma::RawImage(nullptr, surf.width, surf.height), false, nullptr, nullptr, internalFormat, format, type);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, enigma::get_texture_peer(texture), 0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer);
}

void surface_add_depthbuffer(int id, int internalFormat, unsigned format, unsigned type){
  get_surface(surf,id);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, surf.fbo);
  int texture = enigma::graphics_create_texture_custom(enigma::RawImage(nullptr, surf.width, surf.height), false, nullptr, nullptr, internalFormat, format, type);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, enigma::get_texture_peer(texture), 0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer);
}

void surface_set_target(int id)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(surf,id);
  //This fixes several consecutive surface_set_target() calls without surface_reset_target.
  if (enigma::bound_framebuffer != 0) { d3d_transform_stack_pop(); d3d_projection_stack_pop();}
  enigma::bound_framebuffer = surf.fbo;
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, surf.fbo); //bind it
  d3d_transform_stack_push();
  d3d_projection_stack_push();
  glViewport(0, 0, surf.width, surf.height);
  glScissor(0, 0, surf.width, surf.height);
  d3d_set_projection_ortho(0, surf.height, surf.width, -surf.height, 0);
}

void surface_reset_target(void)
{
  draw_batch_flush(batch_flush_deferred);

  enigma::bound_framebuffer = 0;
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  d3d_transform_stack_pop();
  d3d_projection_stack_pop();
  screen_reset_viewport();
}

int surface_get_target()
{
  return enigma::bound_framebuffer;
}

void surface_free(int id)
{
  get_surface(surf,id);
  if (enigma::bound_framebuffer == surf.fbo) glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer=0);
  enigma::graphics_delete_texture(surf.texture);
  if (surf.write_only == true){
    if (surf.has_depth_buffer == true) { glDeleteRenderbuffers(1, &surf.depth_buffer); }
    if (surf.has_stencil_buffer == true) { glDeleteRenderbuffers(1, &surf.stencil_buffer); }
  }else if (surf.has_depth_buffer == true){
    enigma::graphics_delete_texture(surf.depth_buffer);
  }
  glDeleteFramebuffers(1, &surf.fbo);
  delete enigma::surfaces[id];
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
