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

#include "../General/OpenGLHeaders.h"
#include "../General/GSscreen.h"
#include "../General/GSmatrix.h"
#include "Graphics_Systems/graphics_mandatory.h"

using namespace std;
#include <cstddef>
#include <iostream>
#include <cmath>

#include <stdio.h> //for file writing (surface_save)
#include "Universal_System/nlpo2.h"
#include "Universal_System/spritestruct.h"
#include "Universal_System/backgroundstruct.h"
#include "Collision_Systems/collision_types.h"

#define __GETR(x) (gs_scalar)(((x & 0x0000FF))/255.0)
#define __GETG(x) (gs_scalar)(((x & 0x00FF00) >> 8)/255.0)
#define __GETB(x) (gs_scalar)(((x & 0xFF0000) >> 16)/255.0)

namespace enigma_user {
  extern int room_width, room_height/*, sprite_idmax*/;
}
#include "../General/GSsurface.h"
#include "../General/GLSurfaceStruct.h"
#include "../General/GStextures.h"
#include "GL3TextureStruct.h"

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_surface(surf,id)\
    if (size_t(id) >= enigma::surface_max or !enigma::surface_array[id]) {\
      show_error("Attempting to use non-existing surface " + toString(id), false);\
      return;\
    }\
    enigma::surface* surf = enigma::surface_array[id];
  #define get_surfacev(surf,id,r)\
    if (size_t(id) >= enigma::surface_max or !enigma::surface_array[id]) {\
      show_error("Attempting to use non-existing surface " + toString(id), false);\
      return r;\
    }\
    enigma::surface* surf = enigma::surface_array[id];
#else
  #define get_surface(surf,id)\
    enigma::surface* surf = enigma::surface_array[id];
  #define get_surfacev(surf,id,r)\
    enigma::surface* surf = enigma::surface_array[id];
#endif

namespace enigma
{
  surface **surface_array;
  size_t surface_max=0;
  extern unsigned int bound_framebuffer;
}

namespace enigma_user
{

bool surface_is_supported()
{
    return GLEW_ARB_framebuffer_object;
}

int surface_create(int width, int height, bool depthbuffer)
{
    if (GLEW_ARB_framebuffer_object)
    {
      GLuint fbo;

      unsigned int id,
        w=(int)width,
        h=(int)height; //get the integer width and height, and prepare to search for an id

      if (enigma::surface_max==0) {
        enigma::surface_array=new enigma::surface*[1];
        enigma::surface_max=1;
      }

      for (id=0; enigma::surface_array[id]!=NULL; id++)
      {
        if (id+1>=enigma::surface_max)
        {
          enigma::surface **oldarray=enigma::surface_array;
          enigma::surface_array=new enigma::surface*[enigma::surface_max+1];

          for (unsigned int i=0; i<enigma::surface_max; i++)
            enigma::surface_array[i]=oldarray[i];

          enigma::surface_array[enigma::surface_max]=NULL;
          enigma::surface_max++;
          delete[] oldarray;
        }
      }

      enigma::surface_array[id] = new enigma::surface;
      enigma::surface_array[id]->width = w;
      enigma::surface_array[id]->height = h;

      glGenFramebuffers(1, &fbo);
      int texture = enigma::graphics_create_texture(w,h,w,h,0,false);

      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureStructs[texture]->gltex, 0);

      if (depthbuffer == 1){
        GLuint depthBuffer;
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      }else{
        glClear(GL_COLOR_BUFFER_BIT);
      }
      glClearColor(1,1,1,0);

      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer);

      enigma::surface_array[id]->tex = texture;
      enigma::surface_array[id]->fbo = fbo;

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

  size_t id,
  w = (int)width,
  h = (int)height; //get the integer width and height, and prepare to search for an id

  if (enigma::surface_max==0) {
    enigma::surface_array=new enigma::surface*[1];
    enigma::surface_max=1;
  }

  for (id=0; enigma::surface_array[id]!=NULL; id++)
  {
    if (id+1 >= enigma::surface_max)
    {
      enigma::surface **oldarray=enigma::surface_array;
      enigma::surface_array=new enigma::surface*[enigma::surface_max+1];

      for (size_t i=0; i<enigma::surface_max; i++)
        enigma::surface_array[i]=oldarray[i];

      enigma::surface_array[enigma::surface_max]=NULL;
      enigma::surface_max++;
      delete[] oldarray;
    }
  }

  enigma::surface_array[id] = new enigma::surface;
  enigma::surface_array[id]->width = w;
  enigma::surface_array[id]->height = h;

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

  enigma::surface_array[id]->tex = texture;
  enigma::surface_array[id]->fbo = fbo;

  return id;
}

void surface_set_target(int id)
{
  get_surface(surf,id);
  texture_reset();
  //This fixes several consecutive surface_set_target() calls without surface_reset_target.
  if (enigma::bound_framebuffer != 0) { d3d_transform_stack_pop(); }
  enigma::bound_framebuffer = surf->fbo;
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, surf->fbo); //bind it
  d3d_transform_stack_push();
  glViewport(0, 0, surf->width, surf->height);
  glScissor(0, 0, surf->width, surf->height);
  d3d_set_projection_ortho(0, 0, surf->width, surf->height, 0);
}

void surface_reset_target(void)
{
  texture_reset();
  enigma::bound_framebuffer = 0;
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  d3d_transform_stack_pop();
}

int surface_get_target()
{
  return enigma::bound_framebuffer;
}

void surface_free(int id)
{
  get_surface(surf,id);
  if (enigma::bound_framebuffer == surf->fbo) glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer=0);
  enigma::graphics_delete_texture(surf->tex);
  glDeleteFramebuffers(1, &surf->fbo);
  surf->width = surf->height = surf->tex = surf->fbo = 0;
  delete surf;
  enigma::surface_array[id] = NULL;
}

bool surface_exists(int id)
{
  return size_t(id) < enigma::surface_max && enigma::surface_array[id] != NULL;
}

int surface_get_texture(int id)
{
  get_surfacev(surf,id,-1);
  return (surf->tex);
}

int surface_get_width(int id)
{
  get_surfacev(surf,id,-1);
  return (surf->width);
}

int surface_get_height(int id)
{
  get_surfacev(surf,id,-1);
  return (surf->height);
}

int surface_getpixel(int id, int x, int y)
{
  get_surfacev(surf,id,-1);
  unsigned char *pixelbuf=new unsigned char[3];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf->fbo);
  glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixelbuf);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::bound_framebuffer);
  return pixelbuf[0] + (pixelbuf[1] << 8) + (pixelbuf[2] << 16);
}

int surface_getpixel_ext(int id, int x, int y)
{
  get_surfacev(surf,id,-1);
  unsigned char *pixelbuf=new unsigned char[3];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf->fbo);
  glReadPixels(x,y,1,1,GL_BGRA,GL_UNSIGNED_BYTE,pixelbuf);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::bound_framebuffer);
  return pixelbuf[0] + (pixelbuf[1] << 8) + (pixelbuf[2] << 16) + (pixelbuf[3] << 24);
}


int surface_getpixel_alpha(int id, int x, int y)
{
  get_surfacev(surf,id,-1);
  unsigned char *pixelbuf=new unsigned char[1];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf->fbo);
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
  get_surfacev(surf,id,-1);
  unsigned int w=surf->width,h=surf->height,sz=w*h;

  string ext = enigma::image_get_format(filename);

  unsigned char *rgbdata = new unsigned char[sz*4];

  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf->fbo);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(0,0,w,h,GL_BGRA,GL_UNSIGNED_BYTE,rgbdata);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer);

  int ret = enigma::image_save(filename, rgbdata, w, h, w, h, false);

  delete[] rgbdata;
  return ret;
}

int surface_save_part(int id, string filename, unsigned x, unsigned y, unsigned w, unsigned h)
{
  get_surfacev(surf,id,-1);
  unsigned int sz=w*h;

  string ext = enigma::image_get_format(filename);

  unsigned char *rgbdata = new unsigned char[sz*4];

  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf->fbo);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,rgbdata);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, enigma::bound_framebuffer);

  int ret = enigma::image_save(filename, rgbdata, w, h, w, h, false);

  delete[] rgbdata;
  return ret;
}

int background_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload)
{
  get_surfacev(surf,id,-1);
  int full_width=nlpo2dc(w)+1, full_height=nlpo2dc(h)+1;

  unsigned sz=full_width*full_height;
  unsigned char *surfbuf=new unsigned char[sz*4];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf->fbo);
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
  get_surfacev(surf,id,-1);
  int full_width=nlpo2dc(w)+1, full_height=nlpo2dc(h)+1;
  enigma::spritestructarray_reallocate();
  int sprid=enigma::sprite_idmax;
  enigma::sprite_new_empty(sprid, 1, w, h, xorig, yorig, 0, h, 0, w, preload, smooth);

  unsigned sz=full_width*full_height;
  unsigned char *surfbuf=new unsigned char[sz*4];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf->fbo);
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
  get_surface(surf,id);
  int full_width=nlpo2dc(w)+1, full_height=nlpo2dc(h)+1;

  unsigned sz=full_width*full_height;
  unsigned char *surfbuf=new unsigned char[sz*4];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, surf->fbo);
  glReadPixels(x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::bound_framebuffer);
  enigma::sprite_add_subimage(ind, w, h, surfbuf, surfbuf, enigma::ct_precise); //TODO: Support toggling of precise.
  delete[] surfbuf;
}

void surface_copy_part(int destination, float x, float y, int source, int xs, int ys, int ws, int hs)
{
  get_surface(ssurf,source);
  get_surface(dsurf,destination);
  unsigned char *surfbuf=new unsigned char[ws*hs*4];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, ssurf->fbo);
  glReadPixels(xs,ys,ws,hs,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dsurf->fbo);
  //glPushAttrib(GL_VIEWPORT_BIT);
  screen_set_viewport(0, 0, dsurf->width, dsurf->height);
  d3d_set_projection_ortho(0, 0, dsurf->width, dsurf->height, 0);
  glRasterPos2d(x, y);
  glDrawPixels(ws,hs,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::bound_framebuffer);
  //glPopAttrib();
  glRasterPos2d(0, 0);
  delete[] surfbuf;
}

void surface_copy(int destination, float x, float y, int source)
{
  get_surface(ssurf,source);
  get_surface(dsurf,destination);
  unsigned char *surfbuf=new unsigned char[dsurf->width*dsurf->height*4];
  glBindFramebuffer(GL_READ_FRAMEBUFFER, ssurf->fbo);
  glReadPixels(0,0,dsurf->width,dsurf->height,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dsurf->fbo);
  //glPushAttrib(GL_VIEWPORT_BIT);
  screen_set_viewport(0, 0, dsurf->width, dsurf->height);
  d3d_set_projection_ortho(0, 0, dsurf->width, dsurf->height, 0);
  glRasterPos2d(x, y);
  glDrawPixels(dsurf->width,dsurf->height,GL_BGRA,GL_UNSIGNED_BYTE,surfbuf);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::bound_framebuffer);
  //glPopAttrib();
  glRasterPos2d(0, 0);
  delete[] surfbuf;
}

}
