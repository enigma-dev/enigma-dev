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

#include "Graphics_Systems/OpenGL/GLsurface_impl.h"
#include "Graphics_Systems/OpenGL/GLtextures_impl.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GSsurface.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GStextures_impl.h"

#include <cstddef>
#include <iostream>

using namespace std;

namespace enigma {

extern int viewport_x, viewport_y, viewport_w, viewport_h;

} // namespace enigma

namespace enigma_user {

bool surface_is_supported()
{
  return GLEW_EXT_framebuffer_object;
}

int surface_create(int width, int height, bool depthbuffer, bool, bool)
{
    if (!GLEW_EXT_framebuffer_object) return -1;

    GLuint fbo;
    int prevFbo;

    const size_t id = enigma::surfaces.size(),
                 w = (int)width, h = (int)height;

    enigma::Surface* surf = new enigma::Surface();
    enigma::surfaces.emplace_back(surf);

    surf->width = w;
    surf->height = h;

    glGenFramebuffers(1, &fbo);
    int texture = enigma::graphics_create_texture(w,h,w,h,0,false);

    glPushAttrib(GL_TEXTURE_BIT);

    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, enigma::get_texture_peer(texture), 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glClearColor(1,1,1,0);

    if (depthbuffer == true){
        GLuint depthBuffer;
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }else{
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prevFbo);
    glPopAttrib();

    surf->texture = texture;
    surf->fbo = fbo;

    return id;
}

int surface_create_msaa(int width, int height, int samples)
{
  if (!GLEW_EXT_framebuffer_object) return -1;

  GLuint fbo;
  int prevFbo;

  const size_t id = enigma::surfaces.size(),
                w = (int)width, h = (int)height;

  enigma::Surface* surf = new enigma::Surface();
  enigma::surfaces.emplace_back(surf);

  surf->width = w;
  surf->height = h;

  int texture = enigma::graphics_create_texture(w,h,w,h,0,false);
  glGenFramebuffers(1, &fbo);
  glPushAttrib(GL_TEXTURE_BIT);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, enigma::get_texture_peer(texture));

  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_BGRA, w, h, false);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D_MULTISAMPLE, enigma::get_texture_peer(texture), 0);
  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
  glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prevFbo);
  glPopAttrib();

  surf->texture = texture;
  surf->fbo = fbo;

  return id;
}

void surface_set_target(int id)
{
  draw_batch_flush(batch_flush_deferred);

  //This fixes several consecutive surface_set_target() calls without surface_reset_target.
  int prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  if (prevFbo != 0) { glPopAttrib(); glPopMatrix(); d3d_projection_stack_pop(); }
  get_surface(surf,id);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf.fbo); //bind it
  glPushMatrix(); //So you can pop it in the reset
  d3d_projection_stack_push();
  glPushAttrib(GL_VIEWPORT_BIT); //same
  glViewport(0, 0, surf.width, surf.height);
  glScissor(0, 0, surf.width, surf.height);
  d3d_set_projection_ortho(0, surf.height, surf.width, -surf.height, 0);
}

void surface_reset_target(void)
{
  draw_batch_flush(batch_flush_deferred);

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glPopAttrib();
  glPopMatrix();
  d3d_projection_stack_pop();
  glViewport(enigma::viewport_x, enigma::viewport_y, enigma::viewport_w, enigma::viewport_h);
  glScissor(enigma::viewport_x, enigma::viewport_y, enigma::viewport_w, enigma::viewport_h);
}

int surface_get_target()
{
  GLint prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  return prevFbo;
}

void surface_free(int id)
{
  get_surface(surf,id);
  GLint prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  if ((GLuint) prevFbo == surf.fbo) {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  }
  enigma::graphics_delete_texture(surf.texture);
  glDeleteFramebuffers(1, &surf.fbo);
  delete enigma::surfaces[id];
}

}
