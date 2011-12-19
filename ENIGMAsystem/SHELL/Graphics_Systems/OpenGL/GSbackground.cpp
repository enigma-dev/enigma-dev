/** Copyright (C) 2010-2011 Harijs Grînbergs, Josh Ventura, Alasdair Morrison
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

#include <cstddef>

#include <math.h>
#include "OpenGLHeaders.h"
#include "GSbackground.h"
#include "GStextures.h"
#include "../../Universal_System/backgroundstruct.h"
#include "../../Universal_System/spritestruct.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

extern int room_width, room_height;
namespace enigma {
  extern size_t background_idmax;
}


#ifdef DEBUG_MODE
  #include <string>
  #include "../../libEGMstd.h"
  #include "../../Widget_Systems/widgets_mandatory.h"
  #define get_background(bck2d,back)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return;\
    }\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return r;\
    }\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#else
  #define get_background(bck2d,back)\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#endif

#include "binding.h"

void draw_background(int back, double x, double y)
{
  get_background(bck2d,back);
  bind_texture(bck2d->texture);

  glPushAttrib(GL_CURRENT_BIT);
  glColor4f(1,1,1,1);

  const float tbx=bck2d->texbordx,tby=bck2d->texbordy;
  glBegin(GL_QUADS);
    glTexCoord2f(0,0);
      glVertex2f(x,y);
    glTexCoord2f(tbx,0);
      glVertex2f(x+bck2d->width,y);
    glTexCoord2f(tbx,tby);
      glVertex2f(x+bck2d->width,y+bck2d->height);
    glTexCoord2f(0,tby);
      glVertex2f(x,y+bck2d->height);
  glEnd();

  glPopAttrib();
}

void draw_background_stretched(int back, double x, double y, double w, double h)
{
  get_background(bck2d,back);
  bind_texture(bck2d->texture);

  glPushAttrib(GL_CURRENT_BIT);
  glColor4f(1,1,1,1);

  const float tbx=bck2d->texbordx,tby=bck2d->texbordy;
  glBegin(GL_QUADS);
    glTexCoord2f(0,0);
      glVertex2f(x,y);
    glTexCoord2f(tbx,0);
      glVertex2f(x+w,y);
    glTexCoord2f(tbx,tby);
      glVertex2f(x+w,y+h);
    glTexCoord2f(0,tby);
      glVertex2f(x,y+h);
  glEnd();

  glPopAttrib();
}

void draw_background_part(int back,double left,double top,double width,double height,double x,double y)
{
    get_background(bck2d,back);
    bind_texture(bck2d->texture);

    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);

    const float tbx=bck2d->texbordx,tby=bck2d->texbordy,
        tbw=bck2d->width/tbx,tbh=bck2d->height/tby;
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

void draw_background_tiled(int back,double x,double y)
{
  get_background(bck2d,back);
  bind_texture(bck2d->texture);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    x=bck2d->width-fmod(x,bck2d->width);
    y=bck2d->height-fmod(y,bck2d->height);
    const float tbx=bck2d->texbordx,tby=bck2d->texbordy;
    const int hortil= int (ceil(room_width/(bck2d->width*tbx))) + 1,
              vertil= int (ceil(room_height/(bck2d->height*tby))) + 1;

    glBegin(GL_QUADS);
      for (int i=0; i<hortil; i++)
      {
        for (int c=0; c<vertil; c++)
        {
          glTexCoord2f(0,0);
            glVertex2f(i*bck2d->width-x,c*bck2d->height-y);
          glTexCoord2f(tbx,0);
            glVertex2f((i+1)*bck2d->width-x,c*bck2d->height-y);
          glTexCoord2f(tbx,tby);
            glVertex2f((i+1)*bck2d->width-x,(c+1)*bck2d->height-y);
          glTexCoord2f(0,tby);
            glVertex2f(i*bck2d->width-x,(c+1)*bck2d->height-y);
        }
      }
    glEnd();
  glPopAttrib();
}

void draw_background_tiled_area(int back,double x,double y,double x1,double y1,double x2,double y2)
{
  get_background(bck2d,back);
  bind_texture(bck2d->texture);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);

    const float tbx=bck2d->texbordx,tby=bck2d->texbordy;
    float sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = bck2d->width;
    sh = bck2d->height;

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

        glTexCoord2f(left/sw*tbx,top/sh*tby);
          glVertex2f(X,Y);
        glTexCoord2f((left+width)/sw*tbx,top/sh*tby);
          glVertex2f(X+width,Y);
        glTexCoord2f((left+width)/sw*tbx,(top+height)/sh*tby);
          glVertex2f(X+width,Y+height);
        glTexCoord2f(left/sw*tbx,(top+height)/sh*tby);
          glVertex2f(X,Y+height);
      }
      j = jj;
    }
    glEnd();
  glPopAttrib();
}

void draw_background_ext(int back,double x,double y,double xscale,double yscale,double rot,int color,double alpha)
{
  get_background(bck2d,back);
  bind_texture(bck2d->texture);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    const float tbx=bck2d->texbordx,tby=bck2d->texbordy, w=bck2d->width*xscale, h=bck2d->height*yscale;
    rot *= M_PI/180;

    float ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
          ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

    glBegin(GL_QUADS);
      glTexCoord2f(0,0);
        glVertex2f(ulcx,ulcy);
      glTexCoord2f(tbx,0);
        glVertex2f(ulcx + w*cos(rot), ulcy - w*sin(rot));
      glTexCoord2f(tbx,tby);
        ulcx += h * cos(3*M_PI/2 + rot);
        ulcy -= h * sin(3*M_PI/2 + rot);
        glVertex2f(ulcx + w*cos(rot), ulcy - w*sin(rot));
      glTexCoord2f(0,tby);
        glVertex2f(ulcx,ulcy);
    glEnd();
  glPopAttrib();
}

void draw_background_stretched_ext(int back,double x,double y,double w,double h,int color,double alpha)
{
  get_background(bck2d,back);
  bind_texture(bck2d->texture);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    const float tbx=bck2d->texbordx, tby=bck2d->texbordy;

    glBegin(GL_QUADS);
      glTexCoord2f(0,0);
        glVertex2f(x,y);
      glTexCoord2f(tbx,0);
        glVertex2f(x+w,y);
      glTexCoord2f(tbx,tby);
        glVertex2f(x+w,y+h);
      glTexCoord2f(0,tby);
        glVertex2f(x,y+h);
    glEnd();
  glPopAttrib();
}

void draw_background_part_ext(int back,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha)
{
  get_background(bck2d,back);
  bind_texture(bck2d->texture);

  glPushAttrib(GL_CURRENT_BIT);
  glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

  const float
    tbx = bck2d->texbordx,  tby = bck2d->texbordy,
    tbw = bck2d->width/tbx, tbh = bck2d->height/tby;

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

void draw_background_tiled_ext(int back,double x,double y,double xscale,double yscale,int color,double alpha)
{
  get_background(bck2d,back);
  bind_texture(bck2d->texture);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));
    const float tbx=bck2d->texbordx,tby=bck2d->texbordy, w=bck2d->width*xscale, h=bck2d->height*yscale;
    const int hortil= int (ceil(room_width/(bck2d->width*tbx))),
        vertil= int (ceil(room_height/(bck2d->height*tby)));
    x=w-fmod(x,w);
    y=h-fmod(y,h);
    glBegin(GL_QUADS);
    for (int i=0; i<hortil; i++)
    {
      for (int c=0; c<vertil; c++)
      {
        glTexCoord2f(0,0);
          glVertex2f(i*w-x,c*h-y);
        glTexCoord2f(tbx,0);
          glVertex2f((i+1)*w-x,c*h-y);
        glTexCoord2f(tbx,tby);
          glVertex2f((i+1)*w-x,(c+1)*h-y);
        glTexCoord2f(0,tby);
          glVertex2f(i*w-x,(c+1)*h-y);
      }
    }
    glEnd();
  glPopAttrib();
}

void draw_background_tiled_area_ext(int back,double x,double y,double x1,double y1,double x2,double y2, double xscale, double yscale, int color, double alpha)
{
  get_background(bck2d,back);
  bind_texture(bck2d->texture);

  glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    const float tbx=bck2d->texbordx,tby=bck2d->texbordy;
    float sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = bck2d->width*xscale;
    sh = bck2d->height*yscale;

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

        glTexCoord2f(left/sw*tbx,top/sh*tby);
          glVertex2f(X,Y);
        glTexCoord2f((left+width)/sw*tbx,top/sh*tby);
          glVertex2f(X+width,Y);
        glTexCoord2f((left+width)/sw*tbx,(top+height)/sh*tby);
          glVertex2f(X+width,Y+height);
        glTexCoord2f(left/sw*tbx,(top+height)/sh*tby);
          glVertex2f(X,Y+height);
      }
      j = jj;
    }
    glEnd();
  glPopAttrib();
}

void draw_background_general(int back,double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a1,double a2,double a3,double a4)
{
  get_background(bck2d,back);
  bind_texture(bck2d->texture);

  glPushAttrib(GL_CURRENT_BIT);
    const float
      tbx = bck2d->texbordx,  tby = bck2d->texbordy,
      tbw = bck2d->width/tbx, tbh = bck2d->height/tby,
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

int background_get_texture(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->texture;
}

int background_get_width(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->width;
}

int background_get_height(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->height;
}

void texture_set_interpolation(int enable)
{
    if (enigma::interpolate_textures == enable)
        return;

    enigma::interpolate_textures = enable;
    enigma::background *back;
    enigma::sprite *spr;
    for (unsigned int i = 0; i < enigma::background_idmax; i++)
	{
        back = enigma::backgroundstructarray[i];
	    if (!back)
            continue;

        glBindTexture(GL_TEXTURE_2D, back->texture);
        if (enable)
        {
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        }
	}

    int ii;
	for (unsigned int i = 0; i < enigma::sprite_idmax; i++)
    {
        spr = enigma::spritestructarray[i];
	    if (!spr)
            continue;

        for (ii = 0; ii < spr->subcount; ii++)
        {
            glBindTexture(GL_TEXTURE_2D, spr->texturearray[ii]);
            if (enable)
            {
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
            }
        }
    }
}

int texture_get_width(int texId)
{
    int w;
    glBindTexture(GL_TEXTURE_2D, texId);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
    return w;
}

int texture_get_height(int texId)
{
    int h;
    glBindTexture(GL_TEXTURE_2D, texId);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT, &h);
    return h;
}

bool texture_get_interpolation()
{
    return enigma::interpolate_textures;
}

void texture_set_blending(bool enable)
{
    (enable?glEnable:glDisable)(GL_BLEND);
}

void texture_set_repeat(bool repeat)
{
}//functionality has been removed in ENIGMA, repeat is always used as clamping is useless

void texture_preload(int texid)
{
}//functionality has been removed in ENIGMA, all textures are automatically preloaded

void texture_set_priority(int texid, double prio)
{
}//functionality has been removed in ENIGMA, all textures are automatically preloaded
