/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010-2011 Alasdair Morrison <tgmg@g-java.com>, Josh Ventura   **
 **  Copyright (C) 2013 Robert B. Colton                                         **
 **                                                                              **
 **  This file is a part of the ENIGMA Development Environment.                  **
 **                                                                              **
 **                                                                              **
 **  ENIGMA is free software: you can redistribute it and/or modify it under the **
 **  terms of the GNU General Public License as published by the Free Software   **
 **  Foundation, version 3 of the license or any later version.                  **
 **                                                                              **
 **  This application and its source code is distributed AS-IS, WITHOUT ANY      **
 **  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
 **  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
 **  details.                                                                    **
 **                                                                              **
 **  You should have recieved a copy of the GNU General Public License along     **
 **  with this code. If not, see <http://www.gnu.org/licenses/>                  **
 **                                                                              **
 **  ENIGMA is an environment designed to create games and other programs with a **
 **  high-level, fully compilable language. Developers of ENIGMA or anything     **
 **  associated with ENIGMA are in no way responsible for its users or           **
 **  applications created by its users, or damages caused by the environment     **
 **  or programs made in the environment.                                        **
 **                                                                              **
 \********************************************************************************/

#include <cstddef>

#include <math.h>
#include "OpenGLHeaders.h"
#include "GSbackground.h"
#include "GStextures.h"
#include "Universal_System/backgroundstruct.h"
#include "Universal_System/spritestruct.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

extern int room_width, room_height;
namespace enigma {
  extern size_t background_idmax;
}


#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
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
#include <string.h> // needed for querying ARB extensions

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

    float tbw = bck2d->width/(float)bck2d->texbordx, tbh = bck2d->height/(float)bck2d->texbordy,
          tbx1 = left/tbw, tbx2 = tbx1 + width/tbw,
          tby1 = top/tbh, tby2 = tby1 + height/tbh;

    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(tbx1,tby1);
    glVertex2f(x,y);
    glTexCoord2f(tbx2,tby1);
    glVertex2f(x+width,y);
    glTexCoord2f(tbx1,tby2);
    glVertex2f(x,y+height);
    glTexCoord2f(tbx2,tby2);
    glVertex2f(x+width,y+height);
    glEnd();

    glPopAttrib();
}

void draw_background_tiled(int back,double x,double y)
{
    get_background(bck2d,back);
    bind_texture(bck2d->texture);
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);

    x = bck2d->width-fmod(x,bck2d->width);
    y = bck2d->height-fmod(y,bck2d->height);

    const float
    tbx = bck2d->texbordx,tby=bck2d->texbordy;

    const int
    hortil = int (ceil(room_width/(bck2d->width*tbx))) + 1,
    vertil = int (ceil(room_height/(bck2d->height*tby))) + 1;

    glBegin(GL_QUADS);
    float xvert1 = -x, xvert2 = xvert1 + bck2d->width, yvert1, yvert2;
    for (int i=0; i<hortil; i++)
    {
        yvert1 = -y; yvert2 = yvert1 + bck2d->height;
        for (int c=0; c<vertil; c++)
        {
            glTexCoord2f(0,0);
            glVertex2f(xvert1,yvert1);
            glTexCoord2f(tbx,0);
            glVertex2f(xvert2,yvert1);
            glTexCoord2f(tbx,tby);
            glVertex2f(xvert2,yvert2);
            glTexCoord2f(0,tby);
            glVertex2f(xvert1,yvert2);
            yvert1 = yvert2;
            yvert2 += bck2d->height;
        }
        xvert1 = xvert2;
        xvert2 += bck2d->width;
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

    rot *= M_PI/180;

    const float
    tbx = bck2d->texbordx, tby = bck2d->texbordy,
    w = bck2d->width*xscale, h = bck2d->height*yscale,
    wsinrot = w*sin(rot), wcosrot = w*cos(rot);

    glBegin(GL_QUADS);

    float
    ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
    ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);
    glTexCoord2f(0,0);
    glVertex2f(ulcx,ulcy);
    glTexCoord2f(tbx,0);
    glVertex2f(ulcx + wcosrot, ulcy - wsinrot);

    const double mpr = 3*M_PI/2 + rot;
    ulcx += h * cos(mpr);
    ulcy -= h * sin(mpr);
    glTexCoord2f(tbx,tby);
    glVertex2f(ulcx + wcosrot, ulcy - wsinrot);
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

    float tbw = bck2d->width/(float)bck2d->texbordx, tbh = bck2d->height/(float)bck2d->texbordy,
          xvert1 = x, xvert2 = xvert1 + width*xscale,
          yvert1 = y, yvert2 = yvert1 + height*yscale,
          tbx1 = left/tbw, tbx2 = tbx1 + width/tbw,
          tby1 = top/tbh, tby2 = tby1 + height/tbh;

    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(tbx1,tby1);
    glVertex2f(xvert1,yvert1);
    glTexCoord2f(tbx2,tby1);
    glVertex2f(xvert2,yvert1);
    glTexCoord2f(tbx1,tby2);
    glVertex2f(xvert1,yvert2);
    glTexCoord2f(tbx2,tby2);
    glVertex2f(xvert2,yvert2);
    glEnd();

    glPopAttrib();
}

void draw_background_tiled_ext(int back,double x,double y,double xscale,double yscale,int color,double alpha)
{
    get_background(bck2d,back);
    bind_texture(bck2d->texture);

    glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    const float
    tbx = bck2d->texbordx, tby = bck2d->texbordy,
    width_scaled = bck2d->width*xscale, height_scaled = bck2d->height*yscale;

    x = width_scaled-fmod(x,width_scaled);
    y = height_scaled-fmod(y,height_scaled);

    const int
    hortil = int(ceil(room_width/(width_scaled*tbx))) + 1,
    vertil = int(ceil(room_height/(height_scaled*tby))) + 1;

    glBegin(GL_QUADS);
    float xvert1 = -x, xvert2 = xvert1 + width_scaled, yvert1, yvert2;
    for (int i=0; i<hortil; i++)
    {
        yvert1 = -y; yvert2 = yvert1 + height_scaled;
        for (int c=0; c<vertil; c++)
        {
            glTexCoord2f(0,0);
            glVertex2f(xvert1,yvert1);
            glTexCoord2f(tbx,0);
            glVertex2f(xvert2,yvert1);
            glTexCoord2f(tbx,tby);
            glVertex2f(xvert2,yvert2);
            glTexCoord2f(0,tby);
            glVertex2f(xvert1,yvert2);
            yvert1 = yvert2;
            yvert2 += height_scaled;
        }
        xvert1 = xvert2;
        xvert2 += width_scaled;
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
    const float wcosrot = w*cos(rot), wsinrot = w*sin(rot);

    float ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
          ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

    glBegin(GL_QUADS);
      glColor4ub(__GETR(c1),__GETG(c1),__GETB(c1),char(a1*255));
      glTexCoord2f(left/tbw,top/tbh);
        glVertex2f(ulcx,ulcy);

      glColor4ub(__GETR(c2),__GETG(c2),__GETB(c2),char(a2*255));
      glTexCoord2f((left+width)/tbw,top/tbh);
        glVertex2f((ulcx + wcosrot), (ulcy - wsinrot));

      ulcx += h * cos(3*M_PI/2 + rot);
      ulcy -= h * sin(3*M_PI/2 + rot);
      glColor4ub(__GETR(c3),__GETG(c3),__GETB(c3),char(a3*255));
      glTexCoord2f((left+width)/tbw,(top+height)/tbh);
        glVertex2f((ulcx + wcosrot), (ulcy - wsinrot));

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

double background_get_texture_width_factor(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->texbordx;
}

double background_get_texture_height_factor(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->texbordy;
}

void texture_set_interpolation(int enable)
{
    if (enigma::interpolate_textures == enable)
        return;

    enigma::interpolate_textures = enable;
    enigma::background *back;
    enigma::sprite *spr;
    int i, ii;
    for (i = 0; i < enigma::background_idmax; i++)
	{
        back = enigma::backgroundstructarray[i];
	    if (!back)
            continue;

        glBindTexture(GL_TEXTURE_2D, back->texture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,enable?GL_LINEAR:GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,enable?GL_LINEAR:GL_NEAREST);
	}

	for (i = 0; i < enigma::sprite_idmax; i++)
    {
        spr = enigma::spritestructarray[i];
	    if (!spr)
            continue;

        for (ii = 0; ii < spr->subcount; ii++)
        {
            glBindTexture(GL_TEXTURE_2D, spr->texturearray[ii]);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,enable?GL_LINEAR:GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,enable?GL_LINEAR:GL_NEAREST);
        }
    }
}

bool texture_get_interpolation()
{
    return enigma::interpolate_textures;
}

void texture_set_blending(bool enable)
{
    (enable?glEnable:glDisable)(GL_BLEND);
}

double texture_get_width(int texid)
{
  // returns floating point scale to the bg or some shit
}

double texture_get_height(int texid)
{
  // so does this one
}

int texture_get_pixwidth(int texid)
{
  // returns the actual number of pixels in the texture across the xaxis
  GLint width = 0;
  glBindTexture(GL_TEXTURE_2D, texid);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  return width;
}

int texture_get_pixheight(int texid)
{
  // returns the actual number of pixels in the tex across the yaxis
  GLint height = 0;
  glBindTexture(GL_TEXTURE_2D, texid);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &height);
  return height;
}

void texture_set_repeat(bool repeat)
{
/*
  enigma::background *back;
  for (int i = 0; i < enigma::background_idmax; i++)
  {
    back = enigma::backgroundstructarray[i];
    if (!back) { continue; }
*/
    //glBindTexture(GL_TEXTURE_2D, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, repeat?GL_REPEAT:GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat?GL_REPEAT:GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat?GL_REPEAT:GL_CLAMP);
  //}
}

void texture_set_repeat(int texid, bool repeat)
{
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, repeat?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat?GL_REPEAT:GL_CLAMP);
}

void texture_set_repeat(int texid, bool repeatu, bool repeatv, bool repeatw)
{
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, repeatu?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatv?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatw?GL_REPEAT:GL_CLAMP);
}

void texture_preload(int texid)
{
  
}//functionality has been removed in ENIGMA, all textures are automatically preloaded

void texture_set_priority(int texid, double prio)
{
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, prio);
}

void texture_set_border(int texid, int r, int g, int b, double a) 
{
  GLint color[4] = {r, g, b, a * 255};
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
}

void texture_mipmapping_filter(int texid, int filter) 
{
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  if (filter == tx_trilinear) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else if (filter == tx_bilinear) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else if (filter == tx_nearest) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  } 
}

void texture_mipmapping_generate(int texid, int levels)
{
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  for (int i = 0; i < levels; i++) 
  {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
}

bool  texture_anisotropy_supported()
{
  return strstr((char*)glGetString(GL_EXTENSIONS), 
           "GL_EXT_texture_filter_anisotropic");
}

float texture_anisotropy_maxlevel()
{
  float maximumAnisotropy;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnisotropy);
  return maximumAnisotropy;
}

void  texture_anisotropy_filter(int texid, float levels)
{
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, levels);
}

bool  texture_multitexture_supported()
{
  return strstr((char*)glGetString(GL_EXTENSIONS), 
           "GL_ARB_multitexture");
}

void texture_multitexture_enable(bool enable)
{

}

