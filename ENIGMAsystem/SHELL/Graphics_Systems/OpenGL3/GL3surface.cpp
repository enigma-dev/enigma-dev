/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton, Dave "biggoron", Harijs Grinbergs
*** Copyright (C) 2014 Robert B. Colton, Harijs Grinbergs
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

#include "GL3TextureStruct.h"
#include "Graphics_Systems/General/OpenGLHeaders.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GSsurface.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Graphics_Systems/General/GLSurfaceStruct.h"
#include "Graphics_Systems/General/GStextures.h"

#include "Universal_System/nlpo2.h"
#include "Universal_System/sprites_internal.h"
#include "Universal_System/background_internal.h"
#include "Collision_Systems/collision_types.h"

#include <cstddef>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <stdio.h> //for file writing (surface_save)

using namespace std;

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_surface(surf,id)\
    if (surface_exists(id) == false) {\
      show_error("Attempting to use non-existing surface " + toString(id), false);\
      return;\
    }\
    enigma::surface &surf = enigma::surface_array[id];
  #define get_surfacev(surf,id,r)\
    if (surface_exists(id) == false) {\
      show_error("Attempting to use non-existing surface " + toString(id), false);\
      return r;\
    }\
    enigma::surface &surf = enigma::surface_array[id];
  #define showerror(error, ...) show_error(error, false);
#else
  #define get_surface(surf,id)\
    enigma::surface &surf = enigma::surface_array[id];
  #define get_surfacev(surf,id,r)\
    enigma::surface &surf = enigma::surface_array[id];
  #define showerror(fmt, ...) printf(fmt, __VA_ARGS__);
#endif

namespace enigma
{
  unordered_map<unsigned int, surface> surface_array;
  size_t surface_max=0;
  extern unsigned int bound_framebuffer;
  extern int viewport_x, viewport_y, viewport_w, viewport_h;

  //WHERE TO PUT THIS!!!!!????
  //This is GL3 only, because I need formats and types that might not be compatible between graphics systems
  int graphics_create_texture_custom(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap, int internalFormat, unsigned format, unsigned type);
}

namespace enigma_user
{

bool surface_is_supported()
{
    return GLEW_ARB_framebuffer_object;
}

int surface_create(int width, int height, bool depthbuffer, bool stencilbuffer, bool writeonly)
{
  if (GLEW_ARB_framebuffer_object)
  {
    GLuint fbo;

    size_t id = enigma::surface_max,
      w=(int)width,
      h=(int)height; //get the integer width and height, and prepare to search for an id

    bool found_empty = false;
    for (unsigned int i=0; i<enigma::surface_max; ++i){ //Find first empty slot
      if (enigma::surface_array.find(i) == enigma::surface_array.end()){
        id = i;
        found_empty = true;
        break;
      }
    }

    enigma::surface_array.emplace(id,enigma::surface());
    if (found_empty == false){ enigma::surface_max++; }

    enigma::surface_array[id].width = w;
    enigma::surface_array[id].height = h;

    glGenFramebuffers(1, &fbo);
    int texture = enigma::graphics_create_texture(w,h,w,h,0,false);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureStructs[texture]->gltex, 0);

    GLbitfield flags = GL_COLOR_BUFFER_BIT;

    if (writeonly == true){ //These use RenderBuffers which are faster, but cannot be sampled in GLSL
      if (depthbuffer == true && stencilbuffer == true){
          glGenRenderbuffers(1, &enigma::surface_array[id].depth_buffer);
          glBindRenderbuffer(GL_RENDERBUFFER, enigma::surface_array[id].depth_buffer);
          glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, w, h);
          glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, enigma::surface_array[id].depth_buffer);
          flags = flags | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
          enigma::surface_array[id].has_depth_buffer = true;
      }else{
        if (depthbuffer == true){
          glGenRenderbuffers(1, &enigma::surface_array[id].depth_buffer);
          glBindRenderbuffer(GL_RENDERBUFFER, enigma::surface_array[id].depth_buffer);
          glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
          glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, enigma::surface_array[id].depth_buffer);
          flags = flags | GL_DEPTH_BUFFER_BIT;
          enigma::surface_array[id].has_depth_buffer = true;
        }else if (stencilbuffer == true){  //NOTE(harijs) : Maybe we need to combine the stencil buffer with the depth here, because stencil alone might not be supported
          glGenRenderbuffers(1, &enigma::surface_array[id].stencil_buffer);
          glBindRenderbuffer(GL_RENDERBUFFER, enigma::surface_array[id].stencil_buffer);
          glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, w, h);
          glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, enigma::surface_array[id].stencil_buffer);
          flags = flags | GL_STENCIL_BUFFER_BIT;
          enigma::surface_array[id].has_stencil_buffer = true;
        }
      }
      enigma::surface_array[id].write_only = true;
    }else{ //These use Framebuffers which a slower, but can be sampled
      if (stencilbuffer == true){ //If you use stencilbuffer you MUST also use depth buffer, as a free standing stencil buffer is seldom supported in FBO (GL spec even encourages against it)
        enigma::surface_array[id].depth_buffer = enigma::graphics_create_texture_custom(w,h,w,h,0,false, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureStructs[enigma::surface_array[id].depth_buffer]->gltex, 0);
        flags = flags | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
        enigma::surface_array[id].has_depth_buffer = true;
      }else if (depthbuffer == true){
        enigma::surface_array[id].depth_buffer = enigma::graphics_create_texture_custom(w,h,w,h,0,false, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureStructs[enigma::surface_array[id].depth_buffer]->gltex, 0);
        flags = flags | GL_DEPTH_BUFFER_BIT;
        enigma::surface_array[id].has_depth_buffer = true;
      }
      enigma::surface_array[id].write_only = false;
    }

    glClear(flags);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer);

    enigma::surface_array[id].tex = texture;
    enigma::surface_array[id].fbo = fbo;

    texture_reset();

    return id;
  }
  return -1;
}

int surface_create_msaa(int width, int height, int samples)
{
  if (!GLEW_ARB_framebuffer_object)
  {
    return -1;
  }

  GLuint fbo;

  size_t id = enigma::surface_max,
  w = (int)width,
  h = (int)height; //get the integer width and height, and prepare to search for an id

  for (unsigned int i=0; i<enigma::surface_max; ++i){ //Find first empty slot
    if (enigma::surface_array.find(i) == enigma::surface_array.end()){
      id = i;
      break;
    }
  }

  enigma::surface_array.emplace(id,enigma::surface());
  enigma::surface_max++;

  enigma::surface_array[id].width = w;
  enigma::surface_array[id].height = h;

  int texture = enigma::graphics_create_texture(w,h,w,h,0,false);
  glGenFramebuffers(1, &fbo);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureStructs[texture]->gltex);

  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_BGRA, w, h, false);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureStructs[texture]->gltex, 0);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer);

  enigma::surface_array[id].tex = texture;
  enigma::surface_array[id].fbo = fbo;

  return id;
}

void surface_add_colorbuffer(int id, int index, int internalFormat, unsigned format, unsigned type){
  get_surface(surf,id);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, surf.fbo);
  int texture = enigma::graphics_create_texture_custom(surf.width, surf.height, surf.width, surf.height, 0, false, internalFormat, format, type);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, textureStructs[texture]->gltex, 0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer);
}

void surface_add_depthbuffer(int id, int internalFormat, unsigned format, unsigned type){
  get_surface(surf,id);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, surf.fbo);
  int texture = enigma::graphics_create_texture_custom(surf.width, surf.height, surf.width, surf.height, 0, false, internalFormat, format, type);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureStructs[texture]->gltex, 0);
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
  glViewport(enigma::viewport_x, enigma::viewport_y, enigma::viewport_w, enigma::viewport_h);
  glScissor(enigma::viewport_x, enigma::viewport_y, enigma::viewport_w, enigma::viewport_h);
}

int surface_get_target()
{
  return enigma::bound_framebuffer;
}

void surface_free(int id)
{
  get_surface(surf,id);
  if (enigma::bound_framebuffer == surf.fbo) glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer=0);
  enigma::graphics_delete_texture(surf.tex);
  if (surf.write_only == true){
    if (surf.has_depth_buffer == true) { glDeleteRenderbuffers(1, &surf.depth_buffer); }
    if (surf.has_stencil_buffer == true) { glDeleteRenderbuffers(1, &surf.stencil_buffer); }
  }else if (surf.has_depth_buffer == true){
    enigma::graphics_delete_texture(surf.depth_buffer);
  }
  glDeleteFramebuffers(1, &surf.fbo);
  enigma::surface_array.erase(id);
}

bool surface_exists(int id)
{
  return size_t(id) < enigma::surface_max && (enigma::surface_array.find(id) != enigma::surface_array.end());
}

int surface_get_texture(int id)
{
  get_surfacev(surf,id,-1);
  return (surf.tex);
}

int surface_get_depth_texture(int id)
{
  get_surfacev(surf,id,-1);
  if (surf.write_only == true){
    showerror("Cannot get depth texture from write only surface with ID = %i\n",id);
    return -1;
  }else if (surf.has_depth_buffer == false){
    showerror("Cannot get depth texture from a surface without a depth buffer. Surface ID = %i\n",id);
    return -1;
  }
  return (surf.depth_buffer);
}

int surface_get_width(int id)
{
  get_surfacev(surf,id,-1);
  return (surf.width);
}

int surface_get_height(int id)
{
  get_surfacev(surf,id,-1);
  return (surf.height);
}

int surface_getpixel(int id, int x, int y)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  unsigned char *pixelbuf=new unsigned char[3];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf.fbo);
  glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixelbuf);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::bound_framebuffer);
  return pixelbuf[0] + (pixelbuf[1] << 8) + (pixelbuf[2] << 16);
}

int surface_getpixel_ext(int id, int x, int y)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  unsigned char *pixelbuf=new unsigned char[4];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf.fbo);
  glReadPixels(x,y,1,1,GL_BGRA,GL_UNSIGNED_BYTE,pixelbuf);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::bound_framebuffer);
  return pixelbuf[0] + (pixelbuf[1] << 8) + (pixelbuf[2] << 16) + (pixelbuf[3] << 24);
}


int surface_getpixel_alpha(int id, int x, int y)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  unsigned char *pixelbuf=new unsigned char[1];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf.fbo);
  glReadPixels(x,y,1,1,GL_ALPHA,GL_UNSIGNED_BYTE,pixelbuf);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::bound_framebuffer);
  return pixelbuf[0];
}

}

//////////////////////////////////////SAVE TO FILE AND CREATE SPRITE FUNCTIONS/////////
//Fuck whoever did this to the spec
#ifndef GL_BGR
  #define GL_BGR 0x80E0
#endif

#include "Universal_System/estring.h"
#include "Universal_System/image_formats.h"

namespace enigma_user
{

int surface_save(int id, string filename)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  unsigned int w=surf.width,h=surf.height,sz=w*h;

  string ext = enigma::image_get_format(filename);

  unsigned char *rgbdata = new unsigned char[sz*4];

  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf.fbo);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(0,0,w,h,GL_BGRA,GL_UNSIGNED_BYTE,rgbdata);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer);

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

  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf.fbo);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,rgbdata);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer);

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
  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf.fbo);
  glReadPixels(x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::bound_framebuffer);
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
  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf.fbo);
  glReadPixels(x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::bound_framebuffer);
  enigma::sprite_set_subimage(sprid, 0, w, h, surfbuf, surfbuf, enigma::ct_precise); //TODO: Support toggling of precise.
  delete[] surfbuf;
  return sprid;
}

int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig)
{
	return sprite_create_from_surface(id, x, y, w, h, removeback, smooth, true, xorig, yorig);
}

void sprite_add_from_surface(int ind, int id, int x, int y, int w, int h, bool removeback, bool smooth)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(surf,id);
  int full_width=enigma::nlpo2dc(w)+1, full_height=enigma::nlpo2dc(h)+1;

  unsigned sz=full_width*full_height;
  unsigned char *surfbuf=new unsigned char[sz*4];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf.fbo);
  glReadPixels(x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::bound_framebuffer);
  enigma::sprite_add_subimage(ind, w, h, surfbuf, surfbuf, enigma::ct_precise); //TODO: Support toggling of precise.
  delete[] surfbuf;
}

void surface_copy_part(int destination, float x, float y, int source, int xs, int ys, int ws, int hs)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(ssurf,source);
  get_surface(dsurf,destination);
  unsigned char *surfbuf=new unsigned char[ws*hs*4];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, ssurf.fbo);
  glReadPixels(xs,ys,ws,hs,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dsurf.fbo);
  //glPushAttrib(GL_VIEWPORT_BIT);
  screen_set_viewport(0, 0, dsurf.width, dsurf.height);
  d3d_set_projection_ortho(0, 0, dsurf.width, dsurf.height, 0);
  glRasterPos2d(x, y);
  glDrawPixels(ws,hs,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::bound_framebuffer);
  //glPopAttrib();
  glRasterPos2d(0, 0);
  delete[] surfbuf;
}

void surface_copy(int destination, float x, float y, int source)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(ssurf,source);
  get_surface(dsurf,destination);
  unsigned char *surfbuf=new unsigned char[dsurf.width*dsurf.height*4];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, ssurf.fbo);
  glReadPixels(0,0,dsurf.width,dsurf.height,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dsurf.fbo);
  //glPushAttrib(GL_VIEWPORT_BIT);
  screen_set_viewport(0, 0, dsurf.width, dsurf.height);
  d3d_set_projection_ortho(0, 0, dsurf.width, dsurf.height, 0);
  glRasterPos2d(x, y);
  glDrawPixels(dsurf.width,dsurf.height,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::bound_framebuffer);
  //glPopAttrib();
  glRasterPos2d(0, 0);
  delete[] surfbuf;
}

}
