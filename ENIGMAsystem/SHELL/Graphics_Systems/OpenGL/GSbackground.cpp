/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Harijs Grînbergs, Josh Ventura, Alasdair Morrison        **
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

#include <math.h>
#include "OpenGLHeaders.h"
#include "../../Universal_System/backgroundstruct.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

extern int room_width, room_height;
namespace enigma{extern unsigned bound_texture;}

int draw_background(int back, double x, double y)
{
  enigma::background *bck2d = enigma::backgroundstructarray[back];
  if (!bck2d)
    return -1;
  
  if (enigma::bound_texture != bck2d->texture)
  {
      glBindTexture(GL_TEXTURE_2D,bck2d->texture);
      enigma::bound_texture = bck2d->texture;
  }
  
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  
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
  return 0;
}

int draw_background_stretched(int back, double x, double y, double w, double h)
{
  enigma::background *bck2d = enigma::backgroundstructarray[back];
  if (!bck2d)
    return -1;
  
  if (enigma::bound_texture != bck2d->texture)
  {
      glBindTexture(GL_TEXTURE_2D,bck2d->texture);
      enigma::bound_texture = bck2d->texture;
  }
  
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  
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
  return 0;
}

int draw_background_part(int back,double left,double top,double width,double height,double x,double y)
{
    enigma::background *bck2d = enigma::backgroundstructarray[back];
    if (!bck2d)
        return -1;

    if (enigma::bound_texture != bck2d->texture)
    {
        glBindTexture(GL_TEXTURE_2D,bck2d->texture);
        enigma::bound_texture = bck2d->texture;
    }

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

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
    return 0;
}

int draw_background_tiled(int back,double x,double y)
{
  enigma::background *bck2d = enigma::backgroundstructarray[back];
  if (!bck2d)
      return -1;
  
  if (enigma::bound_texture != bck2d->texture)
  {
      glBindTexture(GL_TEXTURE_2D,bck2d->texture);
      enigma::bound_texture = bck2d->texture;
  }
  
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  
  glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    x=bck2d->width-fmod(x,bck2d->width);
    y=bck2d->height-fmod(y,bck2d->height);
    const float tbx=bck2d->texbordx,tby=bck2d->texbordy;
    const int hortil= int (ceil(room_width/(bck2d->width*tbx))),
              vertil= int (ceil(room_height/(bck2d->height*tby)));
    
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
  return 0;
}

int draw_background_tiled_area(int back,double x,double y,double x1,double y1,double x2,double y2)
{
    enigma::background *bck2d = enigma::backgroundstructarray[back];
    if (!bck2d)
      return -1;

    if (enigma::bound_texture != bck2d->texture)
    {
      glBindTexture(GL_TEXTURE_2D,bck2d->texture);
      enigma::bound_texture = bck2d->texture;
    }


  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

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
  return 0;
}

int draw_background_ext(int back,double x,double y,double xscale,double yscale,double rot,int color,double alpha)
{
  enigma::background *bck2d = enigma::backgroundstructarray[back];
  if (!bck2d)
      return -1;
  
  if (enigma::bound_texture != bck2d->texture)
  {
      glBindTexture(GL_TEXTURE_2D,bck2d->texture);
      enigma::bound_texture = bck2d->texture;
  }
  
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  
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
  return 0;
}

int draw_background_stretched_ext(int back,double x,double y,double w,double h,int color,double alpha)
{
  enigma::background *bck2d = enigma::backgroundstructarray[back];
  if (!bck2d)
      return -1;
  
  if (enigma::bound_texture != bck2d->texture)
  {
      glBindTexture(GL_TEXTURE_2D,bck2d->texture);
      enigma::bound_texture = bck2d->texture;
  }
  
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  
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
  return 0;
}

int draw_background_part_ext(int back,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha)
{
  enigma::background *bck2d = enigma::backgroundstructarray[back];
  if (!bck2d)
      return -1;
  
  if (enigma::bound_texture != bck2d->texture)
  {
      glBindTexture(GL_TEXTURE_2D,bck2d->texture);
      enigma::bound_texture = bck2d->texture;
  }
  
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  
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
  return 0;
}

int draw_background_tiled_ext(int back,double x,double y,double xscale,double yscale,int color,double alpha)
{
  enigma::background *bck2d = enigma::backgroundstructarray[back];
  if (!bck2d)
    return -1;
  
  if (enigma::bound_texture != bck2d->texture)
  {
      glBindTexture(GL_TEXTURE_2D,bck2d->texture);
      enigma::bound_texture = bck2d->texture;
  }
  
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  
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
  return 0;
}

int draw_background_tiled_area_ext(int back,double x,double y,double x1,double y1,double x2,double y2, double xscale, double yscale, int color, double alpha)
{
  enigma::background *bck2d = enigma::backgroundstructarray[back];
  if (!bck2d)
    return -1;
  
  if (enigma::bound_texture != bck2d->texture)
  {
    glBindTexture(GL_TEXTURE_2D,bck2d->texture);
    enigma::bound_texture = bck2d->texture;
  }
  
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  
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
  return 0;
}

int draw_background_general(int back,double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a1,double a2,double a3,double a4)
{
  enigma::background *bck2d = enigma::backgroundstructarray[back];
  if (!bck2d)
      return -1;
  
  if (enigma::bound_texture != bck2d->texture)
  {
      glBindTexture(GL_TEXTURE_2D,bck2d->texture);
      enigma::bound_texture = bck2d->texture;
  }
  
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  
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
  return 0;
}
