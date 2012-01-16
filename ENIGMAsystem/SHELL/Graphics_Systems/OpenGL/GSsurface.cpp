/** Copyright (C) 2011 Josh Ventura, Dave "biggoron", Harijs Grînbers
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
#include <cstddef>
#include <iostream>
#include <math.h>

#include "binding.h"

using namespace std;

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

extern int room_width, room_height;

#ifdef DEBUG_MODE
#  include "Widget_Systems/widgets_mandatory.h"
#endif

namespace enigma
{
  struct surface
  {
  GLuint tex, fbo;
  int width, height;
  };

  surface **surface_array;
  int surface_max=0;
}

int surface_create(int width, int height)
{
  if (GLEW_EXT_framebuffer_object)
      std::cout<<"Extension supported!!"<<std::endl;
  else
      std::cout<<"Extension NOT supported!!"<<std::endl;
  
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

int surface_set_target(int id)
{
  enigma::surface* surf=enigma::surface_array[id]; //copy the surface data from the array
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf->fbo); //bind it
  glPushMatrix(); //So you can pop it in the reset
  glPushAttrib(GL_VIEWPORT_BIT); //same
  glViewport(0,0,surf->width,surf->height);
  glLoadIdentity();
  glOrtho(-1, surf->width, -1, surf->height, -1, 1);
  return 0;
}

int surface_reset_target(void)
{
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glPopAttrib();
  glPopMatrix();
  return 0;
}

int surface_destroy(int id)
{
  enigma::surface* surf=enigma::surface_array[id];
    surf->width = surf->height = surf->tex = surf->fbo = 0;
  delete surf;
  return 0;
}

void draw_surface(int id, double x, double y)
{
  if (id>enigma::surface_max)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }
  if (enigma::surface_array[id]==NULL)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }

  glPushAttrib(GL_CURRENT_BIT);

  glColor4f(1,1,1,1);

  glBindTexture(GL_TEXTURE_2D,enigma::surface_array[id]->tex);
  int w=enigma::surface_array[id]->width;
  int h=enigma::surface_array[id]->height;

  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);    glVertex2f(x,   y);
  glTexCoord2f(1, 0);    glVertex2f(x+w, y);
  glTexCoord2f(1, 1);    glVertex2f(x+w, y+h);
  glTexCoord2f(0, 1);    glVertex2f(x,   y+h);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);

  glPopAttrib();
}

void draw_surface_stretched(int id, double x, double y, double w, double h)
{
  if (id>enigma::surface_max)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }
  if (enigma::surface_array[id]==NULL)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }

  bind_texture(enigma::surface_array[id]->tex);
  
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

void draw_surface_part(int id,double left,double top,double width,double height,double x,double y)
{
  if (id>enigma::surface_max)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }
  if (enigma::surface_array[id]==NULL)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }

  bind_texture(enigma::surface_array[id]->tex);
  
  glPushAttrib(GL_CURRENT_BIT);
  glColor4f(1,1,1,1);
  
  const float tbw=enigma::surface_array[id]->width,tbh=enigma::surface_array[id]->height;
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

void draw_surface_tiled(int id,double x,double y)
{
  if (id>enigma::surface_max)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }
  if (enigma::surface_array[id]==NULL)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }

  bind_texture(enigma::surface_array[id]->tex);
  
  glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    x=enigma::surface_array[id]->width-fmod(x,enigma::surface_array[id]->width);
    y=enigma::surface_array[id]->height-fmod(y,enigma::surface_array[id]->height);
    const int hortil= int (ceil(room_width/(enigma::surface_array[id]->width))),
              vertil= int (ceil(room_height/(enigma::surface_array[id]->height)));

    glBegin(GL_QUADS);
      for (int i=0; i<hortil; i++)
      {
        for (int c=0; c<vertil; c++)
        {
          glTexCoord2f(0,0);
            glVertex2f(i*enigma::surface_array[id]->width-x,c*enigma::surface_array[id]->height-y);
          glTexCoord2f(1,0);
            glVertex2f((i+1)*enigma::surface_array[id]->width-x,c*enigma::surface_array[id]->height-y);
          glTexCoord2f(1,1);
            glVertex2f((i+1)*enigma::surface_array[id]->width-x,(c+1)*enigma::surface_array[id]->height-y);
          glTexCoord2f(0,1);
            glVertex2f(i*enigma::surface_array[id]->width-x,(c+1)*enigma::surface_array[id]->height-y);
        }
      }
    glEnd();
  glPopAttrib();
}

void draw_surface_tiled_area(int id,double x,double y,double x1,double y1,double x2,double y2)
{
  if (id>enigma::surface_max)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }
  if (enigma::surface_array[id]==NULL)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }

  bind_texture(enigma::surface_array[id]->tex);
  
  glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);

    float sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = enigma::surface_array[id]->width;
    sh = enigma::surface_array[id]->height;

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

void draw_surface_ext(int id,double x,double y,double xscale,double yscale,double rot,int color,double alpha)
{
  if (id>enigma::surface_max)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }
  if (enigma::surface_array[id]==NULL)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }

  bind_texture(enigma::surface_array[id]->tex);
  
  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    const float w=enigma::surface_array[id]->width*xscale, h=enigma::surface_array[id]->height*yscale;
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

void draw_surface_stretched_ext(int id,double x,double y,double w,double h,int color,double alpha)
{
  if (id>enigma::surface_max)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }
  if (enigma::surface_array[id]==NULL)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }

  bind_texture(enigma::surface_array[id]->tex);
  
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

void draw_surface_part_ext(int id,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha)
{
  if (id>enigma::surface_max)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }
  if (enigma::surface_array[id]==NULL)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }

  bind_texture(enigma::surface_array[id]->tex);
  
  glPushAttrib(GL_CURRENT_BIT);
  glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

  const float tbw = enigma::surface_array[id]->width, tbh = enigma::surface_array[id]->height;

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

void draw_surface_tiled_ext(int id,double x,double y,double xscale,double yscale,int color,double alpha)
{
  if (id>enigma::surface_max)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }
  if (enigma::surface_array[id]==NULL)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }

  bind_texture(enigma::surface_array[id]->tex);
  
  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));
    const float w=enigma::surface_array[id]->width*xscale, h=enigma::surface_array[id]->height*yscale;
    const int hortil= int (ceil(room_width/(enigma::surface_array[id]->width))),
        vertil= int (ceil(room_height/(enigma::surface_array[id]->height)));
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

void draw_surface_tiled_area_ext(int id,double x,double y,double x1,double y1,double x2,double y2, double xscale, double yscale, int color, double alpha)
{
  if (id>enigma::surface_max)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }
  if (enigma::surface_array[id]==NULL)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }

  bind_texture(enigma::surface_array[id]->tex);
  
  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    float sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = enigma::surface_array[id]->width*xscale;
    sh = enigma::surface_array[id]->height*yscale;

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

void draw_surface_general(int id, double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a1,double a2,double a3,double a4)
{
  if (id>enigma::surface_max)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }
  if (enigma::surface_array[id]==NULL)
  {
    #ifdef DEBUG_MODE
    show_error("Surface does not exist",0);
    #endif
    return;
  }

  bind_texture(enigma::surface_array[id]->tex);
  
  glPushAttrib(GL_CURRENT_BIT);
    const float tbw = enigma::surface_array[id]->width, tbh = enigma::surface_array[id]->height,
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
    return (enigma::surface_array[id]->tex);
}

int surface_get_width(int id)
{
    return (enigma::surface_array[id]->width);
}

int surface_get_height(int id)
{
    return (enigma::surface_array[id]->height);
}
