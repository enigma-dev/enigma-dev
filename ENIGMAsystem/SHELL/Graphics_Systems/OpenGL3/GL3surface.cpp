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
#include <cmath>
#include <algorithm>
#include <fstream>      // std::ofstream

#include "../General/GLbinding.h"
#include <stdio.h> //for file writing (surface_save)
#include "Universal_System/nlpo2.h"
#include "Universal_System/spritestruct.h"
#include "Collision_Systems/collision_types.h"

#include "Universal_System/lodepng.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma_user {
  extern int room_width, room_height/*, sprite_idmax*/;
}
#include "../General/GSsurface.h"

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
    return GLEW_ARB_framebuffer_object;
}

int surface_create(int width, int height)
{
    if (GLEW_ARB_framebuffer_object)
    {
      GLuint tex, fbo;
      int prevFbo;

      int id,
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

          for (int i=0; i<enigma::surface_max; i++)
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

      glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
      glDrawBuffer(GL_COLOR_ATTACHMENT0);
      glReadBuffer(GL_COLOR_ATTACHMENT0);
        glClearColor(1,1,1,0);
      glClear(GL_COLOR_BUFFER_BIT);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prevFbo);
      glPopAttrib();

      enigma::surface_array[id]->tex = tex;
      enigma::surface_array[id]->fbo = fbo;

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

  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  glReadBuffer(GL_COLOR_ATTACHMENT0);
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
  get_surface(surf,id);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, surf->fbo); //bind it
  glPushMatrix(); //So you can pop it in the reset
  glPushAttrib(GL_VIEWPORT_BIT); //same
  glViewport(0,0,surf->width,surf->height);
  glLoadIdentity();
  glOrtho(0, surf->width, 0, surf->height, -1, 1);
}

void surface_reset_target(void)
{
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glPopAttrib();
  glPopMatrix();
}

void surface_free(int id)
{
  get_surface(surf,id);
  glDeleteTextures(1, &surf->tex);
  glDeleteFramebuffers(1, &surf->fbo);
  surf->width = surf->height = surf->tex = surf->fbo = 0;
  delete surf;
  enigma::surface_array[id] = NULL;
}

bool surface_exists(int id)
{
    return size_t(id) < enigma::surface_max && enigma::surface_array[id] != NULL;
}

void draw_surface(int id, float x, float y)
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

void draw_surface_stretched(int id, float x, float y, float w, float h)
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

void draw_surface_part(int id, float left, float top, float width, float height, float x, float y)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
  glColor4f(1,1,1,1);

  const float tbw=surf->width,tbh=surf->height;
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

void draw_surface_tiled(int id, float x, float y)
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

void draw_surface_tiled_area(int id, float x, float y, float x1, float y1, float x2, float y2)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);

    float sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = surf->width;
    sh = surf->height;

    i = x1-(fmod(x1,sw) - fmod(x,sw)) - sw*(fmod(x1,sw)<fmod(x,sw));
    j = y1-(fmod(y1,sh) - fmod(y,sh)) - sh*(fmod(y1,sh)<fmod(y,sh));
    jj = j;

    glBegin(GL_QUADS);
    for(i=i; i<=x2; i+=sw)
    {
      for(j=j; j<=y2; j+=sh)
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

void draw_surface_ext(int id, float x, float y, float xscale, float yscale, double rot, int color, double alpha)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    const float w=surf->width*xscale, h=surf->height*yscale;
    rot *= M_PI/180;

    float ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
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

void draw_surface_stretched_ext(int id, float x, float y, float w, float h, int color, double alpha)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    glBegin(GL_QUADS);
      glTexCoord2f(0,0);
        glVertex2f(x,y);
      glTexCoord2f(1,0);
        glVertex2f(x+w,y);
      glTexCoord2f(1,1);
        glVertex2f(x+w,y+h);
      glTexCoord2f(0,1);
        glVertex2f(x,y+h);
    glEnd();
  glPopAttrib();
}

void draw_surface_part_ext(int id, float left, float top, float width, float height, float x, float y, float xscale, float yscale, int color, double alpha)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
  glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

  const float tbw = surf->width, tbh = surf->height;

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

void draw_surface_tiled_ext(int id, float x, float y, float xscale, float yscale, int color, double alpha)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));
    const float w=surf->width*xscale, h=surf->height*yscale;
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

void draw_surface_tiled_area_ext(int id, float x, float y, float x1, float y1, float x2, float y2, float xscale, float yscale, int color, double alpha)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    float sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = surf->width*xscale;
    sh = surf->height*yscale;

    i = x1-(fmod(x1,sw) - fmod(x,sw)) - sw*(fmod(x1,sw)<fmod(x,sw));
    j = y1-(fmod(y1,sh) - fmod(y,sh)) - sh*(fmod(y1,sh)<fmod(y,sh));
    jj = j;

    glBegin(GL_QUADS);
    for(i=i; i<=x2; i+=sw)
    {
      for(j=j; j<=y2; j+=sh)
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

void draw_surface_general(int id, float left, float top, float width, float height, float x, float y, float xscale, float yscale, double rot, int c1, int c2, int c3, int c4, double a1, double a2, double a3, double a4)
{
  get_surface(surf,id);
  texture_use(surf->tex);

  glPushAttrib(GL_CURRENT_BIT);
    const float tbw = surf->width, tbh = surf->height,
      w = width*xscale, h = height*yscale;

    rot *= M_PI/180;

    float ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
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
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, surf->fbo);
	glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixelbuf);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, prevFbo);
    return pixelbuf[0] + (pixelbuf[1] << 8) + (pixelbuf[2] << 16);
}

int surface_getpixel_alpha(int id, int x, int y)
{
    get_surfacev(surf,id,-1);
    unsigned char *pixelbuf=new unsigned char[1];
    int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, surf->fbo);
	glReadPixels(x,y,1,1,GL_ALPHA,GL_UNSIGNED_BYTE,pixelbuf);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, prevFbo);
    return pixelbuf[0];
}

int surface_get_bound()
{
    int prevFbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
    return prevFbo;
}

}

//////////////////////////////////////SAVE TO FILE AND CREATE SPRITE FUNCTIONS/////////
//Fuck whoever did this to the spec
#ifndef GL_BGR
  #define GL_BGR 0x80E0
#endif

#include "Universal_System/estring.h"

int image_save_bmp(string filename, unsigned char *surfbuf, int w, int h)
{
    //3 bytes per pixel used for both input and output.
    int inputChannels = 3;
    int outputChannels = 3;

    std::vector<unsigned char> bmp;
    //bytes 0-13
    bmp.push_back('B'); bmp.push_back('M'); //0: bfType
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); //2: bfSize; size not yet known for now, filled in later.
    bmp.push_back(0); bmp.push_back(0); //6: bfReserved1
    bmp.push_back(0); bmp.push_back(0); //8: bfReserved2
    bmp.push_back(54 % 256); bmp.push_back(54 / 256); bmp.push_back(0); bmp.push_back(0); //10: bfOffBits (54 header bytes)

    //bytes 14-53
    bmp.push_back(40); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //14: biSize
    bmp.push_back(w % 256); bmp.push_back(w / 256); bmp.push_back(0); bmp.push_back(0); //18: biWidth
    bmp.push_back(h % 256); bmp.push_back(h / 256); bmp.push_back(0); bmp.push_back(0); //22: biHeight
    bmp.push_back(1); bmp.push_back(0); //26: biPlanes
    bmp.push_back(outputChannels * 8); bmp.push_back(0); //28: biBitCount
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //30: biCompression
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //34: biSizeImage
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //38: biXPelsPerMeter
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //42: biYPelsPerMeter
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //46: biClrUsed
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //50: biClrImportant

    /*
    Convert the input RGBRGBRGB pixel buffer to the BMP pixel buffer format. There are 3 differences with the input buffer:
    -BMP stores the rows inversed, from bottom to top
    -BMP stores the color channels in BGR instead of RGB order
    -BMP requires each row to have a multiple of 4 bytes, so sometimes padding bytes are added between rows
    */

    int imagerowbytes = outputChannels * w;
    imagerowbytes = imagerowbytes % 4 == 0 ? imagerowbytes : imagerowbytes + (4 - imagerowbytes % 4); //must be multiple of 4

    for(int y = h - 1; y >= 0; y--) //the rows are stored inversed in bmp
    {
        int c = 0;
        for(int x = 0; x < imagerowbytes; x++)
        {
          if(x < w * outputChannels)
          {
              bmp.push_back(surfbuf[inputChannels * (w * y + x / outputChannels) + c]);
          }
          else bmp.push_back(0);
          c++;
          if(c >= outputChannels) c = 0;
        }
    }

    // Fill in the size
    bmp[2] = bmp.size() % 256;
    bmp[3] = (bmp.size() / 256) % 256;
    bmp[4] = (bmp.size() / 65536) % 256;
    bmp[5] = bmp.size() / 16777216;

    std::ofstream file(filename.c_str(), std::ios::out|std::ios::binary);
    file.write(bmp.empty() ? 0 : (char*)&bmp[0], std::streamsize(bmp.size()));
    file.close();
    return 1;
}

int image_save_png(string filename, const unsigned char *surfbuf, int w, int h)
{
    unsigned char* buffer;
    size_t buffersize;
    unsigned error = lodepng_encode_memory(&buffer, &buffersize, surfbuf, w, h, LCT_RGBA, 8);
    if(!error){
        std::ofstream file(filename.c_str(), std::ios::out|std::ios::binary);
        file.write(reinterpret_cast<const char*>(buffer), std::streamsize(buffersize));
        file.close();
    }
    free(buffer);

    if (error) return -1; else return 1;
}

namespace enigma_user
{

int surface_save(int id, string filename)
{
    get_surfacev(surf,id,-1);
	unsigned int w=surf->width,h=surf->height,sz=w*h;
    int prevFbo;

    size_t fp = filename.find_last_of(".");
    if (fp == string::npos){
        return -1;
    }
    string ext = filename.substr(fp);
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    GLenum format;
    int bytes;
    if (ext == ".bmp") format = GL_BGR, bytes = 3;
    else if (ext == ".png") format = GL_RGBA, bytes = 4;
    else return -1;

    unsigned char *surfbuf=new unsigned char[sz*bytes];
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, surf->fbo);
	glReadPixels(0,0,w,h,format,GL_UNSIGNED_BYTE,surfbuf);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, prevFbo);

    int status;
    if (ext == ".bmp") status=image_save_bmp(filename, surfbuf, w, h);
    else if (ext == ".png") status=image_save_png(filename, surfbuf, w, h);
    delete[] surfbuf;
    return status;
}

int surface_save_part(int id, string filename, unsigned x, unsigned y, unsigned w, unsigned h)
{
    get_surfacev(surf,id,-1);
	unsigned int sz=w*h;
    int prevFbo;

    size_t fp = filename.find_last_of(".");
    if (fp == string::npos){
        return -1;
    }
    string ext = filename.substr(fp);
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    GLenum format;
    int bytes;
    if (ext == ".bmp") format = GL_BGR, bytes = 3;
    else if (ext == ".png") format = GL_RGBA, bytes = 4;
    else return -1;

    unsigned char *surfbuf=new unsigned char[sz*bytes];
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, surf->fbo);
	glReadPixels(x,y,w,h,format,GL_UNSIGNED_BYTE,surfbuf);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, prevFbo);

    int status;
    if (ext == ".bmp") status=image_save_bmp(filename, surfbuf, w, h);
    else if (ext == ".png") status=image_save_png(filename, surfbuf, w, h);
    delete[] surfbuf;
    return status;
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
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
 	glBindFramebuffer(GL_READ_FRAMEBUFFER, surf->fbo);
	glReadPixels(x,y,w,h,GL_RGBA,GL_UNSIGNED_BYTE,surfbuf);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, prevFbo);
    enigma::sprite_set_subimage(sprid, 0, xorig, yorig, w, h, surfbuf, surfbuf, enigma::ct_precise); //TODO: Support toggling of precise.
    delete[] surfbuf;
    return sprid;
}

void surface_copy_part(int destination, float x, float y, int source, int xs, int ys, int ws, int hs)
{
    get_surface(ssurf,source);
    get_surface(dsurf,destination);
    unsigned char *surfbuf=new unsigned char[ws*hs*4];
    int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, ssurf->fbo);
	glReadPixels(xs,ys,ws,hs,GL_RGBA,GL_UNSIGNED_BYTE,surfbuf);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dsurf->fbo);
    glPushMatrix();
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0,0,dsurf->width,dsurf->height);
    glLoadIdentity();
    glOrtho(-1, dsurf->width, -1, dsurf->height, -1, 1);
	glRasterPos2d(x, y);
	glDrawPixels(ws,hs,GL_RGBA,GL_UNSIGNED_BYTE,surfbuf);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, prevFbo);
    glPopAttrib();
    glPopMatrix();
	glRasterPos2d(0, 0);
	delete[] surfbuf;
}

void surface_copy(int destination, float x, float y, int source)
{
    get_surface(ssurf,source);
    get_surface(dsurf,destination);
    unsigned char *surfbuf=new unsigned char[dsurf->width*dsurf->height*4];
    int prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, ssurf->fbo);
	glReadPixels(0,0,dsurf->width,dsurf->height,GL_RGBA,GL_UNSIGNED_BYTE,surfbuf);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dsurf->fbo);
    glPushMatrix();
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0,0,dsurf->width,dsurf->height);
    glLoadIdentity();
    glOrtho(-1, dsurf->width, -1, dsurf->height, -1, 1);
	glRasterPos2d(x, y);
	glDrawPixels(dsurf->width,dsurf->height,GL_RGBA,GL_UNSIGNED_BYTE,surfbuf);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, prevFbo);
    glPopAttrib();
    glPopMatrix();
	glRasterPos2d(0, 0);
	delete[] surfbuf;
}

}

