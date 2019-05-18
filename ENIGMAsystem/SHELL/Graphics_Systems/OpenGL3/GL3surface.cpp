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

namespace enigma {

extern unsigned int bound_framebuffer;

//WHERE TO PUT THIS!!!!!????
//This is GL3 only, because I need formats and types that might not be compatible between graphics systems
int graphics_create_texture_custom(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap, GLint internalFormat, GLenum format, GLenum type);

} // namespace enigma

namespace enigma_user {

bool surface_is_supported()
{
  return GLEW_ARB_framebuffer_object;
}

int surface_create(int width, int height, bool depthbuffer, bool stencilbuffer, bool writeonly)
{
  if (!GLEW_ARB_framebuffer_object) return -1;

  GLuint fbo;

  const size_t id = enigma::surfaces.size(),
                w = (int)width, h = (int)height;

  enigma::Surface* surf = new enigma::Surface();
  enigma::surfaces.emplace_back(surf);

  surf->width = w;
  surf->height = h;

    glGenFramebuffers(1, &fbo);
    int texture = enigma::graphics_create_texture(w,h,w,h,0,false);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, enigma::get_texture_peer(texture), 0);

    GLbitfield flags = GL_COLOR_BUFFER_BIT;

    if (writeonly == true){ //These use RenderBuffers which are faster, but cannot be sampled in GLSL
      if (depthbuffer == true && stencilbuffer == true){
          glGenRenderbuffers(1, &surf->depth_buffer);
          glBindRenderbuffer(GL_RENDERBUFFER, surf->depth_buffer);
          glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, w, h);
          glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, surf->depth_buffer);
          flags = flags | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
          surf->has_depth_buffer = true;
      }else{
        if (depthbuffer == true){
          glGenRenderbuffers(1, &surf->depth_buffer);
          glBindRenderbuffer(GL_RENDERBUFFER, surf->depth_buffer);
          glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
          glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, surf->depth_buffer);
          flags = flags | GL_DEPTH_BUFFER_BIT;
          surf->has_depth_buffer = true;
        }else if (stencilbuffer == true){  //NOTE(harijs) : Maybe we need to combine the stencil buffer with the depth here, because stencil alone might not be supported
          glGenRenderbuffers(1, &surf->stencil_buffer);
          glBindRenderbuffer(GL_RENDERBUFFER, surf->stencil_buffer);
          glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, w, h);
          glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, surf->stencil_buffer);
          flags = flags | GL_STENCIL_BUFFER_BIT;
          surf->has_stencil_buffer = true;
        }
      }
      surf->write_only = true;
    }else{ //These use Framebuffers which a slower, but can be sampled
      if (stencilbuffer == true){ //If you use stencilbuffer you MUST also use depth buffer, as a free standing stencil buffer is seldom supported in FBO (GL spec even encourages against it)
        surf->depth_buffer = enigma::graphics_create_texture_custom(w,h,w,h,0,false, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, enigma::get_texture_peer(surf->depth_buffer), 0);
        flags = flags | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
        surf->has_depth_buffer = true;
      }else if (depthbuffer == true){
        surf->depth_buffer = enigma::graphics_create_texture_custom(w,h,w,h,0,false, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, enigma::get_texture_peer(surf->depth_buffer), 0);
        flags = flags | GL_DEPTH_BUFFER_BIT;
        surf->has_depth_buffer = true;
      }
      surf->write_only = false;
    }

    glClear(flags);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer);

    surf->texture = texture;
    surf->fbo = fbo;

    texture_reset();

    return id;
}

int surface_create_msaa(int width, int height, int samples)
{
  if (!GLEW_ARB_framebuffer_object) return -1;

  GLuint fbo;

  const size_t id = enigma::surfaces.size(),
                w = (int)width, h = (int)height;

  enigma::Surface* surf = new enigma::Surface();
  enigma::surfaces.emplace_back(surf);

  surf->width = w;
  surf->height = h;

  int texture = enigma::graphics_create_texture(w,h,w,h,0,false);
  glGenFramebuffers(1, &fbo);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, enigma::get_texture_peer(texture));

  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_BGRA, w, h, false);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, enigma::get_texture_peer(texture), 0);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer);

  surf->texture = texture;
  surf->fbo = fbo;

  return id;
}

void surface_add_colorbuffer(int id, int index, int internalFormat, unsigned format, unsigned type){
  get_surface(surf,id);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, surf.fbo);
  int texture = enigma::graphics_create_texture_custom(surf.width, surf.height, surf.width, surf.height, 0, false, internalFormat, format, type);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, enigma::get_texture_peer(texture), 0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer);
}

void surface_add_depthbuffer(int id, int internalFormat, unsigned format, unsigned type){
  get_surface(surf,id);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, surf.fbo);
  int texture = enigma::graphics_create_texture_custom(surf.width, surf.height, surf.width, surf.height, 0, false, internalFormat, format, type);
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
    show_error("Cannot get depth texture from write only surface with ID = " + std::to_string(id), false);
    return -1;
  }else if (surf.has_depth_buffer == false){
    show_error("Cannot get depth texture from a surface without a depth buffer. Surface ID = " + std::to_string(id), false);
    return -1;
  }
  #endif
  return (surf.depth_buffer);
}

}
