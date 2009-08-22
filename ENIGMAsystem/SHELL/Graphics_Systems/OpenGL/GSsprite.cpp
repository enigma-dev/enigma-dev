/*********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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
\*********************************************************************************/

#include <map>
#include <math.h>
#include <GL/gl.h>
#include "../../Universal_System/spritestruct.h"

#define pi 3.1415926535897932384626433832795
#define ldx(x,y) (cos(y)*x)
#define ldy(x,y) (-sin(y)*x)

namespace enigma
{  extern unsigned int current_texture_bound_that_noone_should_ever_change_ever; }

int sprite_exists(double sprite)
{
    int spr=(int)sprite;
    
    if (enigma::spritestructarray.empty())
    return 0;
    
    for (enigma::spriteiter=enigma::spritestructarray.begin();enigma::spriteiter != enigma::spritestructarray.end();enigma::spriteiter++)
    if (enigma::spriteiter->second->id==spr)
    return 1;
    
    return 0;
}

int draw_sprite(double spr,double subimg,double x,double y)
{
    int sprite=(int)spr;
    
    enigma::sprite *spr2d;
    if ((spr2d=enigma::spritestructarray[sprite])==NULL)
    return -1;
    
    
    glPushAttrib(GL_CURRENT_BIT);
    
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != spr2d->texturearray[(int)subimg])
    { glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[(int)subimg]); enigma::current_texture_bound_that_noone_should_ever_change_ever=spr2d->texturearray[(int)subimg]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    glColor4f(1,1,1,1);
    
    
    int xo=spr2d->xoffset;
    int yo=spr2d->yoffset;
    
    int w=spr2d->width;
    int h=spr2d->height;
    
    double tbx=spr2d->texbordx;
    double tby=spr2d->texbordy;
    
    glBegin(GL_QUADS);
    
    glTexCoord2f(0,0);
    glVertex2f(x-xo,y-yo);
    glTexCoord2f(tbx,0);
    glVertex2f(x-xo+w,y-yo);
    glTexCoord2f(tbx,tby);
    glVertex2f(x-xo+w,y-yo+h);
    glTexCoord2f(0,tby);
    glVertex2f(x-xo,y-yo+h);
    
    glEnd();
    
    glPopAttrib();
    
    return 0;
}

int draw_sprite_stretched(int spr,int subimg,double x,double y,double w,double h)
{
    int sprite=(int)spr;
    
    enigma::sprite *spr2d;
    if ((spr2d=enigma::spritestructarray[sprite])==NULL)
    return -1;
    
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != spr2d->texturearray[(int)subimg])
    { glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[(int)subimg]); enigma::current_texture_bound_that_noone_should_ever_change_ever=spr2d->texturearray[(int)subimg]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    int xo=spr2d->xoffset;
    int yo=spr2d->yoffset;
    
    float tbx=spr2d->texbordx;
    float tby=spr2d->texbordy;
    
    glBegin(GL_QUADS);
    glTexCoord2f(tbx,0);
    glVertex2f(x-xo,y-yo);
    glTexCoord2f(0,0);
    glVertex2f(x+w-xo,y-yo);
    glTexCoord2f(0,tby);
    glVertex2f(x+w-xo,y+h-yo);
    glTexCoord2f(tbx,tby);
    glVertex2f(x-xo,y+h-yo);
    
    glEnd();
    
    glPopAttrib();
    
    return 0;
}

/*enigma::4args int draw_sprite_tiled(ARG sprite,ARG2 subimg,ARG3 x,ARG4 y)
{
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != spr2d->texturearray[(int)subimg])
    { glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[(int)subimg]); enigma::current_texture_bound_that_noone_should_ever_change_ever=spr2d->texturearray[(int)subimg]; }
    
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
}*/

int draw_sprite_part(int spr,int subimg,double left,double top,double width,double height,double x,double y)
{
    int sprite=(int)spr;
    
    enigma::sprite *spr2d;
    if ((spr2d=enigma::spritestructarray[sprite])==NULL)
    return -1;
    
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != spr2d->texturearray[(int)subimg])
    { glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[(int)subimg]); enigma::current_texture_bound_that_noone_should_ever_change_ever=spr2d->texturearray[(int)subimg]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    
    int w=spr2d->width;
    int h=spr2d->height;
    
    double tbw=w/(double)spr2d->texbordx;
    double tbh=h/(double)spr2d->texbordy;
    
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(left/tbw,top/tbh);
    glVertex2f(x,y);
    glTexCoord2f((left+width)/tbw,top/tbh);
    glVertex2f(x+width,y);
    glTexCoord2f(left/tbw,(top+height)/tbh);
    glVertex2f(x,y+height);
    glTexCoord2f((left+width)/tbw,(top+height)/tbh);
    glVertex2f(x+width,y+height);
    
    glEnd();
    
    glPopAttrib();
    
    return 0;
}

int draw_sprite_part_offset(int spr,int subimg,double left,double top,double width,double height,double x,double y)
{
    int sprite=(int)spr;
    
    enigma::sprite *spr2d;
    if ((spr2d=enigma::spritestructarray[sprite])==NULL)
    return -1;
    
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != spr2d->texturearray[(int)subimg])
    { glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[(int)subimg]); enigma::current_texture_bound_that_noone_should_ever_change_ever=spr2d->texturearray[(int)subimg]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    int w=spr2d->width;
    int h=spr2d->height;
    
    int xo=spr2d->xoffset;
    int yo=spr2d->yoffset;
    
    float tbw=w/spr2d->texbordx;
    float tbh=h/spr2d->texbordy;
    
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(left/tbw,top/tbh);
    glVertex2f(x-xo,y-yo);
    glTexCoord2f((left+width-1)/tbw,top/tbh);
    glVertex2f(x+width-1-xo,y-yo);
    glTexCoord2f(left/tbw,(top+height-1)/tbh);
    glVertex2f(x-xo,y+height-1-yo);
    glTexCoord2f((left+width-1)/tbw,(top+height-1)/tbh);
    glVertex2f(x+width-1-xo,y+height-1-yo);
    
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D,0);
    glPopAttrib();
    
    return 0;
}


int draw_sprite_ext(int spr,int subimg,double x,double y,double xscale,double yscale,double rot,double blend,double alpha)
{
    int sprite=(int)spr;
    int color=(int)blend;
    
    enigma::sprite *spr2d;
    
    if ((spr2d=enigma::spritestructarray[sprite])==NULL)
    return -1;
    
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != spr2d->texturearray[(int)subimg])
    { glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[(int)subimg]); enigma::current_texture_bound_that_noone_should_ever_change_ever=spr2d->texturearray[(int)subimg]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    
    int xo=spr2d->xoffset;
    int yo=spr2d->yoffset;
    
    int w=(int)(spr2d->width*xscale);
    int h=(int)(spr2d->height*yscale);
    
    float tbx=spr2d->texbordx;
    float tby=spr2d->texbordy;
    
    rot=rot*pi/180.0;
    
    glBegin(GL_TRIANGLE_STRIP);
    
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f((color&0x0000FF)/255.0,((color&0x00FF00)>>8)/255.0,((color&0xFF0000)>>16)/255.0,alpha);
    
    glTexCoord2f(0,0);
    double ulcx=x+ldx(xo,pi+rot)+ldx(yo,pi/2+rot),ulcy=y+ldy(xo,pi+rot)+ldy(yo,pi/2+rot);
    glVertex2f(ulcx,ulcy);
    
    glTexCoord2f(tbx,0);
    glVertex2f(ulcx+ldx(w,rot),ulcy+ldy(w,rot));
    
    glTexCoord2f(0,tby);
    ulcx+=ldx(h,1.5*pi+rot); ulcy+=ldy(h,1.5*pi+rot);
    glVertex2f(ulcx,ulcy);
    
    glTexCoord2f(tbx,tby);
    glVertex2f(ulcx+ldx(w,rot),ulcy+ldy(w,rot));
    
    glEnd();
    
    glPopAttrib();
    
    return 0;
}
/*
draw_sprite_stretched_ext(sprite,subimg,x,y,w,h,color,alpha)

//draw_sprite_tiled_ext(sprite,subimg,x,y,xscale,yscale,color,alpha)
*/

int draw_sprite_part_ext(int spr,int subimg,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha)
{
    int sprite=(int)spr;
    
    enigma::sprite *spr2d;
    if ((spr2d=enigma::spritestructarray[sprite])==NULL)
    return -1;
    
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f((color^(((int)(color >> 8)) << 8))/255.0,
              (((int)(color>>8))^(((int)(color>>16))<<8))/255.0,
              ((int)(color>>16))/255.0, alpha);
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != spr2d->texturearray[(int)subimg])
    { glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[(int)subimg]); enigma::current_texture_bound_that_noone_should_ever_change_ever=spr2d->texturearray[(int)subimg]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    int w=spr2d->width;
    int h=spr2d->height;
    
    int xo=spr2d->xoffset;
    int yo=spr2d->yoffset;
    
    float tbw=w/spr2d->texbordx;
    float tbh=h/spr2d->texbordy;
    
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(left/tbw,top/tbh);
    glVertex2f(x-xo,y-yo);
    glTexCoord2f((left+width-1)/tbw,top/tbh);
    glVertex2f(x+width-1-xo,y-yo);
    glTexCoord2f(left/tbw,(top+height-1)/tbh);
    glVertex2f(x-xo,y+height-1-yo);
    glTexCoord2f((left+width-1)/tbw,(top+height-1)/tbh);
    glVertex2f(x+width-1-xo,y+height-1-yo);
    
    glEnd();
    
    glPopAttrib();
    
    return 0;
}

int draw_sprite_general(int spr,int subimg,double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double alpha)
{
    int sprite=(int)spr;
    
    enigma::sprite *spr2d;
    if ((spr2d=enigma::spritestructarray[sprite])==NULL)
    return -1;
    
    glPushAttrib(GL_CURRENT_BIT);
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != spr2d->texturearray[(int)subimg])
    { glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[(int)subimg]); enigma::current_texture_bound_that_noone_should_ever_change_ever=spr2d->texturearray[(int)subimg]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    int w=(int)(spr2d->width*xscale);
    int h=(int)(spr2d->height*yscale);
    
    int xo=spr2d->xoffset;
    int yo=spr2d->yoffset;
    
    int tbx=(int)(spr2d->texbordx);
    int tby=(int)(spr2d->texbordy);
    
    glBegin(GL_TRIANGLE_STRIP);
    
    glColor4f((c1^(((int)(c1 >> 8)) << 8))/255.0,
              (((int)(c1>>8))^(((int)(c1>>16))<<8))/255.0,
              ((int)(c1>>16))/255.0, alpha);
    glTexCoord2f(width/tbx,top/tby);
    glVertex2f(x-xo,y-yo);
    glColor4f((c2^(((int)(c2 >> 8)) << 8))/255.0,
              (((int)(c2>>8))^(((int)(c2>>16))<<8))/255.0,
              ((int)(c2>>16))/255.0, alpha);
    glTexCoord2f(left/tbx,top/tby);
    glVertex2f(x+w*xscale-xo,y-yo);
    glColor4f((c3^(((int)(c3 >> 8)) << 8))/255.0,
              (((int)(c3>>8))^(((int)(c3>>16))<<8))/255.0,
              ((int)(c3>>16))/255.0, alpha);
    glTexCoord2f(width/tbx,height/tby);
    glVertex2f(x-xo,y+h*yscale-yo);
    glColor4f((c4^(((int)(c4 >> 8)) << 8))/255.0,
              (((int)(c4>>8))^(((int)(c4>>16))<<8))/255.0,
              ((int)(c4>>16))/255.0, alpha);
    glTexCoord2f(left/tbx,height/tby);
    glVertex2f(x+w*xscale-xo,y+h*yscale-yo);
    
    glEnd();
    
    glPopAttrib();
    
    return 0;
}

#undef pi
#undef ldx
#undef ldy
