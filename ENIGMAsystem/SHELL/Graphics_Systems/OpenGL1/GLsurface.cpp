/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton, Dave "biggoron", Harijs Grinbergs
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
using namespace std;
#include <cstddef>
#include <iostream>
#include <math.h>
#include <string.h>

#include "../General/GLbinding.h"
#include <stdio.h> //for file writing (surface_save)
#include "Universal_System/nlpo2.h"
#include "Universal_System/spritestruct.h"
#include "Collision_Systems/collision_types.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma_user {
  extern int room_width, room_height/*, sprite_idmax*/;
}
#include "../General/GSsurface.h"
#include "../General/GLSurfaceStruct.h"

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
}

namespace enigma_user
{

bool surface_is_supported()
{
    return GLEW_EXT_framebuffer_object;
}

int surface_create(int width, int height)
{
  if (!GLEW_EXT_framebuffer_object)
  {
    return -1;
  }

  GLuint tex, fbo;
  int prevFbo;

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

  glGenTextures(1, &tex);
  glGenFramebuffers(1, &fbo);

  glPushAttrib(GL_TEXTURE_BIT);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);
  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
  glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prevFbo);
  glPopAttrib();

  enigma::surface_array[id]->tex = tex;
  enigma::surface_array[id]->fbo = fbo;

  return id;
}

int surface_create_msaa(int width, int height, int samples)
{
  if (!GLEW_EXT_framebuffer_object)
  {
    return -1;
  }

  GLuint tex, fbo;
  int prevFbo;

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

  glGenTextures(1, &tex);
  glGenFramebuffers(1, &fbo);
  glPushAttrib(GL_TEXTURE_BIT);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);

  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, w, h, false);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);
  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
  glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prevFbo);
  glPopAttrib();

  enigma::surface_array[id]->tex = tex;
  enigma::surface_array[id]->fbo = fbo;

  return id;
}

void surface_set_target(int id)
{
  //This fixes several consecutive surface_set_target() calls without surface_reset_target.
  int prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
  if (prevFbo != 0) glPopAttrib(); glPopMatrix();
  //
  get_surface(surf,id);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf->fbo); //bind it
  glPushMatrix(); //So you can pop it in the reset
  glPushAttrib(GL_VIEWPORT_BIT); //same
  glViewport(0,0,surf->width,surf->height);
  glLoadIdentity();
  glOrtho(-1, surf->width, -1, surf->height, -1, 1);
}

void surface_reset_target(void)
{
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glPopAttrib();
  glPopMatrix();
}

void surface_free(int id)
{
  get_surface(surf,id);
  surf->width = surf->height = surf->tex = surf->fbo = 0;
  delete surf;
  enigma::surface_array[id] = NULL;
}

bool surface_exists(int id)
{
    return size_t(id) < enigma::surface_max && enigma::surface_array[id] != NULL;
}

void draw_surface(int id, gs_scalar x, gs_scalar y)
{
  get_surface(surf,id);
  texture_use(surf->tex);
  glPushAttrib(GL_CURRENT_BIT);
  glColor4f(1,1,1,1);
  int w=surf->width;
  int h=surf->height;

  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);    glVertex2f(x,   y);
  glTexCoord2f(1, 0);    glVertex2f(x+w, y);
  glTexCoord2f(1, 1);    glVertex2f(x+w, y+h);
  glTexCoord2f(0, 1);    glVertex2f(x,   y+h);
  glEnd();

  glPopAttrib();
}

void draw_surface_stretched(int id, gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h)
{
  get_surface(surf,id);
  texture_use(surf->tex);
  glPushAttrib(GL_CURRENT_BIT);
  glColor4f(1,1,1,1);

  glBegin(GL_QUADS);
    glTexCoord2f(0,0);      glVertex2f(x,y);
    glTexCoord2f(1,0);      glVertex2f(x+w,y);
    glTexCoord2f(1,1);      glVertex2f(x+w,y+h);
    glTexCoord2f(0,1);      glVertex2f(x,y+h);
  glEnd();

  glPopAttrib();
}

void draw_surface_part(int id, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
  glColor4f(1,1,1,1);

  const gs_scalar tbw=surf->width,tbh=surf->height;
  glBegin(GL_QUADS);
    glTexCoord2f(left/tbw,top/tbh);
      glVertex2f(x,y);
    glTexCoord2f((left+width)/tbw,top/tbh);
      glVertex2f(x+width,y);
    glTexCoord2f((left+width)/tbw,(top+height)/tbh);
      glVertex2f(x+width,y+height);
    glTexCoord2f(left/tbw,(top+height)/tbh);
      glVertex2f(x,y+height);
  glEnd();

  glPopAttrib();
}

void draw_surface_tiled(int id, gs_scalar x, gs_scalar y)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    x=surf->width-fmod(x,surf->width);
    y=surf->height-fmod(y,surf->height);
    const int hortil= int (ceil(room_width/(surf->width))),
              vertil= int (ceil(room_height/(surf->height)));

    glBegin(GL_QUADS);
      for (int i=0; i<hortil; i++)
      {
        for (int c=0; c<vertil; c++)
        {
          glTexCoord2f(0,0);
            glVertex2f(i*surf->width-x,c*surf->height-y);
          glTexCoord2f(1,0);
            glVertex2f((i+1)*surf->width-x,c*surf->height-y);
          glTexCoord2f(1,1);
            glVertex2f((i+1)*surf->width-x,(c+1)*surf->height-y);
          glTexCoord2f(0,1);
            glVertex2f(i*surf->width-x,(c+1)*surf->height-y);
        }
      }
    glEnd();
  glPopAttrib();
}

void draw_surface_tiled_area(int id, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);

    gs_scalar sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = surf->width;
    sh = surf->height;

    i = x1-(fmod(x1,sw) - fmod(x,sw)) - sw*(fmod(x1,sw)<fmod(x,sw));
    j = y1-(fmod(y1,sh) - fmod(y,sh)) - sh*(fmod(y1,sh)<fmod(y,sh));
    jj = j;

    glBegin(GL_QUADS);
    for(; i<=x2; i+=sw)
    {
      for(; j<=y2; j+=sh)
      {
        if(i <= x1) left = x1-i;
        else left = 0;
        X = i+left;

        if(j <= y1) top = y1-j;
        else top = 0;
        Y = j+top;

        if(x2 <= i+sw) width = ((sw)-(i+sw-x2)+1)-left;
        else width = sw-left;

        if(y2 <= j+sh) height = ((sh)-(j+sh-y2)+1)-top;
        else height = sh-top;

        glTexCoord2f(left/sw,top/sh);
          glVertex2f(X,Y);
        glTexCoord2f((left+width)/sw,top/sh);
          glVertex2f(X+width,Y);
        glTexCoord2f((left+width)/sw,(top+height)/sh);
          glVertex2f(X+width,Y+height);
        glTexCoord2f(left/sw,(top+height)/sh);
          glVertex2f(X,Y+height);
      }
      j = jj;
    }
    glEnd();
  glPopAttrib();
}

void draw_surface_ext(int id, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int color, gs_scalar alpha)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    const gs_scalar w=surf->width*xscale, h=surf->height*yscale;
    rot *= M_PI/180;

    gs_scalar ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
          ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

    glBegin(GL_QUADS);
      glTexCoord2f(0,0);
        glVertex2f(ulcx,ulcy);
      glTexCoord2f(1,0);
        glVertex2f(ulcx + w*cos(rot), ulcy - w*sin(rot));
      glTexCoord2f(1,1);
        ulcx += h * cos(3*M_PI/2 + rot);
        ulcy -= h * sin(3*M_PI/2 + rot);
        glVertex2f(ulcx + w*cos(rot), ulcy - w*sin(rot));
      glTexCoord2f(0,1);
        glVertex2f(ulcx,ulcy);
    glEnd();
  glPopAttrib();
}

void draw_surface_stretched_ext(int id, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, gs_scalar alpha)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    glBegin(GL_QUADS);
      glTexCoord2f(0,0);
        glVertex2f(x,y);
      glTexCoord2f(1,0);
        glVertex2f(x+width,y);
      glTexCoord2f(1,1);
        glVertex2f(x+width,y+height);
      glTexCoord2f(0,1);
        glVertex2f(x,y+height);
    glEnd();
  glPopAttrib();
}

void draw_surface_part_ext(int id, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
  glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

  const gs_scalar tbw = surf->width, tbh = surf->height;

  glBegin(GL_QUADS);
    glTexCoord2f(left/tbw,top/tbh);
      glVertex2f(x,y);
    glTexCoord2f((left+width)/tbw,top/tbh);
      glVertex2f(x+width*xscale,y);
    glTexCoord2f((left+width)/tbw,(top+height)/tbh);
      glVertex2f(x+width*xscale,y+height*yscale);
    glTexCoord2f(left/tbw,(top+height)/tbh);
      glVertex2f(x,y+height*yscale);
  glEnd();

  glPopAttrib();
}

void draw_surface_tiled_ext(int id, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));
    const gs_scalar w=surf->width*xscale, h=surf->height*yscale;
    const int hortil= int (ceil(room_width/(surf->width))),
        vertil= int (ceil(room_height/(surf->height)));
    x=w-fmod(x,w);
    y=h-fmod(y,h);
    glBegin(GL_QUADS);
    for (int i=0; i<hortil; i++)
    {
      for (int c=0; c<vertil; c++)
      {
        glTexCoord2f(0,0);
          glVertex2f(i*w-x,c*h-y);
        glTexCoord2f(1,0);
          glVertex2f((i+1)*w-x,c*h-y);
        glTexCoord2f(1,1);
          glVertex2f((i+1)*w-x,(c+1)*h-y);
        glTexCoord2f(0,1);
          glVertex2f(i*w-x,(c+1)*h-y);
      }
    }
    glEnd();
  glPopAttrib();
}

void draw_surface_tiled_area_ext(int id, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    gs_scalar sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = surf->width*xscale;
    sh = surf->height*yscale;

    i = x1-(fmod(x1,sw) - fmod(x,sw)) - sw*(fmod(x1,sw)<fmod(x,sw));
    j = y1-(fmod(y1,sh) - fmod(y,sh)) - sh*(fmod(y1,sh)<fmod(y,sh));
    jj = j;

    glBegin(GL_QUADS);
    for(; i<=x2; i+=sw)
    {
      for(; j<=y2; j+=sh)
      {
        if(i <= x1) left = x1-i;
        else left = 0;
        X = i+left;

        if(j <= y1) top = y1-j;
        else top = 0;
        Y = j+top;

        if(x2 <= i+sw) width = ((sw)-(i+sw-x2)+1)-left;
        else width = sw-left;

        if(y2 <= j+sh) height = ((sh)-(j+sh-y2)+1)-top;
        else height = sh-top;

        glTexCoord2f(left/sw,top/sh);
          glVertex2f(X,Y);
        glTexCoord2f((left+width)/sw,top/sh);
          glVertex2f(X+width,Y);
        glTexCoord2f((left+width)/sw,(top+height)/sh);
          glVertex2f(X+width,Y+height);
        glTexCoord2f(left/sw,(top+height)/sh);
          glVertex2f(X,Y+height);
      }
      j = jj;
    }
    glEnd();
  glPopAttrib();
}

void draw_surface_general(int id, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar a1, gs_scalar a2, gs_scalar a3, gs_scalar a4)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
    const gs_scalar tbw = surf->width, tbh = surf->height,
      w = width*xscale, h = height*yscale;

    rot *= M_PI/180;

    gs_scalar ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
          ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

    glBegin(GL_QUADS);
      glColor4ub(__GETR(c1),__GETG(c1),__GETB(c1),char(a1*255));
      glTexCoord2f(left/tbw,top/tbh);
        glVertex2f(ulcx,ulcy);

      glColor4ub(__GETR(c2),__GETG(c2),__GETB(c2),char(a2*255));
      glTexCoord2f((left+width)/tbw,top/tbh);
        glVertex2f((ulcx + w*cos(rot)), (ulcy - w*sin(rot)));

      ulcx += h * cos(3*M_PI/2 + rot);
      ulcy -= h * sin(3*M_PI/2 + rot);
      glColor4ub(__GETR(c3),__GETG(c3),__GETB(c3),char(a3*255));
      glTexCoord2f((left+width)/tbw,(top+height)/tbh);
        glVertex2f((ulcx + w*cos(rot)), (ulcy - w*sin(rot)));

      glColor4ub(__GETR(c4),__GETG(c4),__GETB(c4),char(a4*255));
      glTexCoord2f(left/tbw,(top+height)/tbh);
        glVertex2f(ulcx,ulcy);
    glEnd();
  glPopAttrib();
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
    int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf->fbo);
	glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixelbuf);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
    return pixelbuf[0] + (pixelbuf[1] << 8) + (pixelbuf[2] << 16);
}

int surface_getpixel_alpha(int id, int x, int y)
{
    get_surfacev(surf,id,-1);
    unsigned char *pixelbuf=new unsigned char[1];
    int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf->fbo);
	glReadPixels(x,y,1,1,GL_ALPHA,GL_UNSIGNED_BYTE,pixelbuf);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
    return pixelbuf[0];
}

int surface_get_bound()
{
    int prevFbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
    return prevFbo;
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
    get_surfacev(surf,id,-1);
	FILE *bmp=fopen(filename.c_str(),"wb");
	if(!bmp) return -1;
	unsigned int w=surf->width,h=surf->height,sz=w*h;
	char *surfbuf=new char[sz*3];
	char *revbuf=new char[sz*3];
    int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf->fbo);
	glReadPixels(0,0,w,h,GL_BGR,GL_UNSIGNED_BYTE,surfbuf);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);

    //This code flips the buffer vertically. It needs to be done or else the picture will be upside down.
    for (unsigned int i=0; i<h; i++)
    {
        for (unsigned int c=0; c<w; c++)
        {
            revbuf[(c+(i)*w)*3]=surfbuf[(c+(h-i)*w)*3];
            revbuf[(c+(i)*w)*3+1]=surfbuf[(c+(h-i)*w)*3+1];
            revbuf[(c+(i)*w)*3+2]=surfbuf[(c+(h-i)*w)*3+2];
        }
    }

	fwrite("BM",2,1,bmp);
	sz<<=2;

	fwrite(&sz,4,1,bmp);
	fwrite("\0\0\0\0\x36\0\0\0\x28\0\0",12,1,bmp);
	fwrite(&w,4,1,bmp);
	fwrite(&h,4,1,bmp);
	fwrite("\1\0\x18\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",28,1,bmp);

	if(w&3)
	{
		size_t pad=w&3;
		w*=3;
		sz-=sz>>2;
		for(unsigned int i=0;i<sz;i+=w)
		{
			fwrite(revbuf+i,w,1,bmp);
			fwrite("\0\0",pad,1,bmp);
		}
	} else fwrite(revbuf,w*3,h,bmp);
	fclose(bmp);
	delete[] surfbuf;
	delete[] revbuf;
	return 1;
}

int surface_save_part(int id, string filename, unsigned x, unsigned y, unsigned w, unsigned h)
{
    get_surfacev(surf,id,-1);
	FILE *bmp=fopen(filename.c_str(),"wb");
	if(!bmp) return -1;
    int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf->fbo);
	unsigned sz=w*h;
	char *surfbuf=new char[sz*3];
	glReadPixels(x,y,w,h,GL_BGR,GL_UNSIGNED_BYTE,surfbuf);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);

    char *revbuf=new char[sz*3];
    //This code flips the buffer vertically. It needs to be done or else the picture will be upside down.
    for (unsigned int i=0; i<h; i++)
    {
        for (unsigned int c=0; c<w; c++)
        {
            revbuf[(c+(i)*w)*3]=surfbuf[(c+(h-i)*w)*3];
            revbuf[(c+(i)*w)*3+1]=surfbuf[(c+(h-i)*w)*3+1];
            revbuf[(c+(i)*w)*3+2]=surfbuf[(c+(h-i)*w)*3+2];
        }
    }

	fwrite("BM",2,1,bmp);

	sz <<= 2;
	fwrite(&sz,4,1,bmp);
	fwrite("\0\0\0\0\x36\0\0\0\x28\0\0",12,1,bmp);
	fwrite(&w,4,1,bmp);
	fwrite(&h,4,1,bmp);
	fwrite("\1\0\x18\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",28,1,bmp);

	if(w&3)
	{
		size_t pad=w&3;
		w*=3;
		sz-=sz>>2;
		for(unsigned i=0;i<sz;i+=w)
		{
			fwrite(revbuf+i,w,1,bmp);
			fwrite("\0\0",pad,1,bmp);
		}
	}
	else fwrite(revbuf,w*3,h,bmp);

	fclose(bmp);
	delete[] surfbuf;
    delete[] revbuf;
	return 1;
}

int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig)
{
    get_surfacev(surf,id,-1);
    int full_width=nlpo2dc(w)+1, full_height=nlpo2dc(h)+1;
    enigma::spritestructarray_reallocate();
    int sprid=enigma::sprite_idmax;
    enigma::sprite_new_empty(sprid, 1, w, h, xorig, yorig, 0, h, 0, w, 1,0);

    unsigned sz=full_width*full_height;
    unsigned char *surfbuf=new unsigned char[sz*4];
	int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
 	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf->fbo);
	glReadPixels(x,y,w,h,GL_RGBA,GL_UNSIGNED_BYTE,surfbuf);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
    enigma::sprite_set_subimage(sprid, 0, xorig, yorig, w, h, surfbuf, surfbuf, enigma::ct_precise); //TODO: Support toggling of precise.
    delete[] surfbuf;
    return sprid;
}

void surface_copy_part(int destination, gs_scalar x, gs_scalar y, int source, int xs, int ys, int ws, int hs)
{
    get_surface(ssurf,source);
    get_surface(dsurf,destination);
    unsigned char *surfbuf=new unsigned char[ws*hs*4];
    int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ssurf->fbo);
	glReadPixels(xs,ys,ws,hs,GL_RGBA,GL_UNSIGNED_BYTE,surfbuf);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, dsurf->fbo);
    glPushMatrix();
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0,0,dsurf->width,dsurf->height);
    glLoadIdentity();
    glOrtho(-1, dsurf->width, -1, dsurf->height, -1, 1);
	glRasterPos2d(x, y);
	glDrawPixels(ws,hs,GL_RGBA,GL_UNSIGNED_BYTE,surfbuf);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
    glPopAttrib();
    glPopMatrix();
	glRasterPos2d(0, 0);
	delete[] surfbuf;
}

void surface_copy(int destination, gs_scalar x, gs_scalar y, int source)
{
    get_surface(ssurf,source);
    get_surface(dsurf,destination);
    unsigned char *surfbuf=new unsigned char[dsurf->width*dsurf->height*4];
    int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ssurf->fbo);
	glReadPixels(0,0,dsurf->width,dsurf->height,GL_RGBA,GL_UNSIGNED_BYTE,surfbuf);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, dsurf->fbo);
    glPushMatrix();
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0,0,dsurf->width,dsurf->height);
    glLoadIdentity();
    glOrtho(-1, dsurf->width, -1, dsurf->height, -1, 1);
	glRasterPos2d(x, y);
	glDrawPixels(dsurf->width,dsurf->height,GL_RGBA,GL_UNSIGNED_BYTE,surfbuf);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
    glPopAttrib();
    glPopMatrix();
	glRasterPos2d(0, 0);
	delete[] surfbuf;
}

}

