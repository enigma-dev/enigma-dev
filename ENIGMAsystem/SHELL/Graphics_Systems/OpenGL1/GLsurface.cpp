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
#include "Graphics_Systems/OpenGL/OpenGLHeaders.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GSsurface.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GStextures_impl.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/sprites_internal.h"
#include "Universal_System/background_internal.h"
#include "Collision_Systems/collision_types.h"

#include <cstddef>
#include <iostream>

#include <math.h>
#include <string.h>
#include <stdio.h> //for file writing (surface_save)

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

int surface_getpixel(int id, int x, int y)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  unsigned char *pixelbuf=new unsigned char[3];
  int prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf.fbo);
  glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixelbuf);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
  return pixelbuf[0] + (pixelbuf[1] << 8) + (pixelbuf[2] << 16);
}

int surface_getpixel_ext(int id, int x, int y)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  unsigned char *pixelbuf=new unsigned char[3];
  int prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf.fbo);
  glReadPixels(x,y,1,1,GL_BGRA,GL_UNSIGNED_BYTE,pixelbuf);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
  return pixelbuf[0] + (pixelbuf[1] << 8) + (pixelbuf[2] << 16) + (pixelbuf[3] << 24);
}

int surface_getpixel_alpha(int id, int x, int y)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  unsigned char *pixelbuf=new unsigned char[1];
  int prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf.fbo);
  glReadPixels(x,y,1,1,GL_ALPHA,GL_UNSIGNED_BYTE,pixelbuf);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
  return pixelbuf[0];
}

}

//////////////////////////////////////SAVE TO FILE AND CTEATE SPRITE FUNCTIONS/////////
//Fuck whoever did this to the spec
#ifndef GL_BGR
  #define GL_BGR 0x80E0
#endif

#include "Universal_System/estring.h"

namespace enigma_user
{

int surface_save(int id, string filename)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  unsigned int w=surf.width,h=surf.height,sz=w*h;

    string ext = enigma::image_get_format(filename);

    unsigned char *rgbdata = new unsigned char[sz*4];

  int prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, surf.fbo);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(0,0,w,h,GL_BGRA,GL_UNSIGNED_BYTE,rgbdata);
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, prevFbo);

	int ret = enigma::image_save(filename, rgbdata, w, h, w, h, false);

	delete[] rgbdata;
	return ret;
}

int surface_save_part(int id, string filename, unsigned x, unsigned y, unsigned w, unsigned h)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  unsigned int sz=w*h;

    string ext = enigma::image_get_format(filename);

    unsigned char *rgbdata = new unsigned char[sz*4];

  int prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, surf.fbo);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,rgbdata);
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, prevFbo);

	int ret = enigma::image_save(filename, rgbdata, w, h, w, h, false);

	delete[] rgbdata;
	return ret;
}

int background_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  int full_width=enigma::nlpo2dc(w)+1, full_height=enigma::nlpo2dc(h)+1;

  unsigned sz=full_width*full_height;
  unsigned char *surfbuf=new unsigned char[sz*4];
  int prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf.fbo);
  glReadPixels(x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
  enigma::backgroundstructarray_reallocate();
  int bckid=enigma::background_idmax;
  enigma::background_new(bckid, w, h, surfbuf, removeback, smooth, preload, false, 0, 0, 0, 0, 0, 0);
  delete[] surfbuf;
  enigma::background_idmax++;
  return bckid;
}

int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  int full_width=enigma::nlpo2dc(w)+1, full_height=enigma::nlpo2dc(h)+1;
  enigma::spritestructarray_reallocate();
  int sprid=enigma::sprite_idmax;
  enigma::sprite_new_empty(sprid, 1, w, h, xorig, yorig, 0, h, 0, w, preload, smooth);

  unsigned sz=full_width*full_height;
  unsigned char *surfbuf=new unsigned char[sz*4];
  int prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf.fbo);
  glReadPixels(x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
  enigma::sprite_set_subimage(sprid, 0, w, h, surfbuf, surfbuf, enigma::ct_precise); //TODO: Support toggling of precise.
  delete[] surfbuf;
  return sprid;
}

int sprite_create_from_surface(int id, int x, int y, int w, int h,
    bool removeback, bool smooth, int xorig, int yorig) {
  return sprite_create_from_surface(
      id, x, y, w, h, removeback, smooth, true, xorig, yorig);
}

void sprite_add_from_surface(int ind, int id, int x, int y, int w, int h, bool removeback, bool smooth)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(surf,id);
  int full_width=enigma::nlpo2dc(w)+1, full_height=enigma::nlpo2dc(h)+1;

  unsigned sz=full_width*full_height;
  unsigned char *surfbuf=new unsigned char[sz*4];
  int prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf.fbo);
  glReadPixels(x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
  enigma::sprite_add_subimage(ind, w, h, surfbuf, surfbuf, enigma::ct_precise); //TODO: Support toggling of precise.
  delete[] surfbuf;
}

void surface_copy_part(int destination, gs_scalar x, gs_scalar y, int source, int xs, int ys, int ws, int hs)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(ssurf,source);
  get_surface(dsurf,destination);
  unsigned char *surfbuf=new unsigned char[ws*hs*4];
  int prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ssurf.fbo);
  glReadPixels(xs,ys,ws,hs,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, dsurf.fbo);
  glPushMatrix();
  glPushAttrib(GL_VIEWPORT_BIT);
  screen_set_viewport(0, 0, dsurf.width, dsurf.height);
  d3d_set_projection_ortho(0, 0, dsurf.width, dsurf.height, 0);
  glRasterPos2d(x, y);
  glDrawPixels(ws,hs,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
  glPopAttrib();
  glPopMatrix();
  glRasterPos2d(0, 0);
  delete[] surfbuf;
}

void surface_copy(int destination, gs_scalar x, gs_scalar y, int source)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(ssurf,source);
  get_surface(dsurf,destination);
  unsigned char *surfbuf=new unsigned char[dsurf.width*dsurf.height*4];
  int prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ssurf.fbo);
  glReadPixels(0,0,dsurf.width,dsurf.height,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, dsurf.fbo);
  glPushMatrix();
  glPushAttrib(GL_VIEWPORT_BIT);
  screen_set_viewport(0, 0, dsurf.width, dsurf.height);
  d3d_set_projection_ortho(0, 0, dsurf.width, dsurf.height, 0);
  glRasterPos2d(x, y);
  glDrawPixels(dsurf.width,dsurf.height,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
  glPopAttrib();
  glPopMatrix();
  glRasterPos2d(0, 0);
  delete[] surfbuf;
}

}
