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

/**Sprite functions************************************************************\

int draw_sprite(ARG sprite,ARG2 subimg,ARG3 x,ARG4 y)
int draw_sprite_stretched(ARG sprite,ARG2 subimg,ARG3 x,ARG4 y,ARG5 w,ARG6 h)
int draw_sprite_part(ARG sprite,ARG2 subimg,ARG3 left,ARG4 top,ARG5 width,ARG6 height,ARG7 x,ARG8 y)
int draw_sprite_part_offset(ARG sprite,ARG2 subimg,ARG3 left,ARG4 top,ARG5 width,ARG6 height,ARG7 x,ARG8 y)
int draw_sprite_ext(ARG sprite,ARG2 subimg,ARG3 x,ARG4 y,ARG5 xscale,ARG6 yscale,ARG7 rot,ARG8 color,ARG9 alpha)
int draw_sprite_part_ext(ARG sprite,ARG2 subimg,ARG3 left,ARG4 top,ARG5 width,ARG6 height,ARG7 x,ARG8 y,ARG9 xscale,ARG10 yscale,ARG11 color,ARG12 alpha)
int draw_sprite_general(ARG sprite,ARG2 subimg,ARG3 left,ARG4 top,ARG5 width,ARG6 height,ARG7 x,ARG8 y,ARG9 xscale,ARG10 yscale,ARG11 rot,ARG12 c1,ARG13 c2,ARG14 c3,ARG15 c4,ARG16 alpha)

\******************************************************************************/

/*
 * Sprite structure. 
 * All sprites are stored in this
 */
#include "../../Universal_System/spritestruct.h"


  
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
    
    if (!sprite_exists(spr))
    return -1;
    
    
    glPushAttrib(GL_CURRENT_COLOR);
    
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != enigma::spritestructarray[sprite]->texturearray[0])
    { glBindTexture(GL_TEXTURE_2D,enigma::spritestructarray[sprite]->texturearray[0]); enigma::current_texture_bound_that_noone_should_ever_change_ever=enigma::spritestructarray[sprite]->texturearray[0]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    glColor4f(1,1,1,1);
    
    
    int xo=enigma::spritestructarray[sprite]->xoffset;
    int yo=enigma::spritestructarray[sprite]->yoffset;
    
    int w=enigma::spritestructarray[sprite]->width;
    int h=enigma::spritestructarray[sprite]->height;
    
    double tbx=enigma::spritestructarray[sprite]->texbordx;
    double tby=enigma::spritestructarray[sprite]->texbordy;
    
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
    
    if (!sprite_exists(spr))
    return -1;
    
    glPushAttrib(GL_CURRENT_COLOR);
    glColor4f(1,1,1,1);
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != enigma::spritestructarray[sprite]->texturearray[0])
    { glBindTexture(GL_TEXTURE_2D,enigma::spritestructarray[sprite]->texturearray[0]); enigma::current_texture_bound_that_noone_should_ever_change_ever=enigma::spritestructarray[sprite]->texturearray[0]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    int xo=enigma::spritestructarray[sprite]->xoffset;
    int yo=enigma::spritestructarray[sprite]->yoffset;
    
    float tbx=enigma::spritestructarray[sprite]->texbordx;
    float tby=enigma::spritestructarray[sprite]->texbordy;
    
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
    glPushAttrib(GL_CURRENT_COLOR);
    glColor4f(1,1,1,1);
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != enigma::spritestructarray[sprite]->texturearray[0])
    { glBindTexture(GL_TEXTURE_2D,enigma::spritestructarray[sprite]->texturearray[0]); enigma::current_texture_bound_that_noone_should_ever_change_ever=enigma::spritestructarray[sprite]->texturearray[0]; }
    
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
}*/

int draw_sprite_part(int spr,int subimg,double left,double top,double width,double height,double x,double y)
{
    int sprite=(int)spr;
    
    if (!sprite_exists(spr))
    return -1;
    
    glPushAttrib(GL_CURRENT_COLOR);
    glColor4f(1,1,1,1);
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != enigma::spritestructarray[sprite]->texturearray[0])
    { glBindTexture(GL_TEXTURE_2D,enigma::spritestructarray[sprite]->texturearray[0]); enigma::current_texture_bound_that_noone_should_ever_change_ever=enigma::spritestructarray[sprite]->texturearray[0]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    int w=enigma::spritestructarray[sprite]->width;
    int h=enigma::spritestructarray[sprite]->height;
    
    float tbw=w/enigma::spritestructarray[sprite]->texbordx;
    float tbh=h/enigma::spritestructarray[sprite]->texbordy;
    
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
    
    glBindTexture(GL_TEXTURE_2D,0);
    glPopAttrib();
    
    return 0;
}

int draw_sprite_part_offset(int spr,int subimg,double left,double top,double width,double height,double x,double y)
{
    int sprite=(int)spr;
    
    if (!sprite_exists(spr))
    return -1;
    
    glPushAttrib(GL_CURRENT_COLOR);
    glColor4f(1,1,1,1);
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != enigma::spritestructarray[sprite]->texturearray[0])
    { glBindTexture(GL_TEXTURE_2D,enigma::spritestructarray[sprite]->texturearray[0]); enigma::current_texture_bound_that_noone_should_ever_change_ever=enigma::spritestructarray[sprite]->texturearray[0]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    int w=enigma::spritestructarray[sprite]->width;
    int h=enigma::spritestructarray[sprite]->height;
    
    int xo=enigma::spritestructarray[sprite]->xoffset;
    int yo=enigma::spritestructarray[sprite]->yoffset;
    
    float tbw=w/enigma::spritestructarray[sprite]->texbordx;
    float tbh=h/enigma::spritestructarray[sprite]->texbordy;
    
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(left/tbw,top/tbh);
    glVertex2f(x-xo,y-yo);
    glTexCoord2f((left+width)/tbw,top/tbh);
    glVertex2f(x+width-xo,y-yo);
    glTexCoord2f(left/tbw,(top+height)/tbh);
    glVertex2f(x-xo,y+height-yo);
    glTexCoord2f((left+width)/tbw,(top+height)/tbh);
    glVertex2f(x+width-xo,y+height-yo);
    
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D,0);
    glPopAttrib();
    
    return 0;
}

int draw_sprite_ext(int spr,int subimg,double x,double y,double xscale,double yscale,double rot,int color,double alpha)
{
    int sprite=(int)spr;
    
    if (!sprite_exists(spr))
    return -1;
    
    glPushAttrib(GL_CURRENT_COLOR);
    glColor4f((color^(((int)(color >> 8)) << 8))/255.0,
              (((int)(color>>8))^(((int)(color>>16))<<8))/255.0,
              ((int)(color>>16))/255.0, alpha);
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != enigma::spritestructarray[sprite]->texturearray[0])
    { glBindTexture(GL_TEXTURE_2D,enigma::spritestructarray[sprite]->texturearray[0]); enigma::current_texture_bound_that_noone_should_ever_change_ever=enigma::spritestructarray[sprite]->texturearray[0]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    int xo=enigma::spritestructarray[sprite]->xoffset;
    int yo=enigma::spritestructarray[sprite]->yoffset;
    
    int w=(int)(enigma::spritestructarray[sprite]->width*xscale);
    int h=(int)(enigma::spritestructarray[sprite]->height*yscale);
    
    float tbx=enigma::spritestructarray[sprite]->texbordx;
    float tby=enigma::spritestructarray[sprite]->texbordy;
    
    double dir,len;
    
    glBegin(GL_TRIANGLE_STRIP);
    
    
    glTexCoord2f(0,0);
    dir=point_direction(xo,yo,0,0)+rot; len=point_distance(xo,yo,0,0); 
    glVertex2f(x+len*cos(degtorad(dir)),y+len*-sin(degtorad(dir)));
    
    glTexCoord2f(tbx,0);
    dir=point_direction(xo,yo,w,0)+rot; len=point_distance(xo,yo,w,0); 
    glVertex2f(x+len*cos(degtorad(dir)),y+len*-sin(degtorad(dir)));
    
    glTexCoord2f(0,tby);
    dir=point_direction(xo,yo,0,h)+rot; len=point_distance(xo,yo,0,h); 
    glVertex2f(x+len*cos(degtorad(dir)),y+len*-sin(degtorad(dir)));
    
    glTexCoord2f(tbx,tby);
    dir=point_direction(xo,yo,w,h)+rot; len=point_distance(xo,yo,w,h); 
    glVertex2f(x+len*cos(degtorad(dir)),y+len*-sin(degtorad(dir)));
    
    
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
    
    if (!sprite_exists(spr))
    return -1;
    
    glPushAttrib(GL_CURRENT_COLOR);
    glColor4f((color^(((int)(color >> 8)) << 8))/255.0,
              (((int)(color>>8))^(((int)(color>>16))<<8))/255.0,
              ((int)(color>>16))/255.0, alpha);
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != enigma::spritestructarray[sprite]->texturearray[0])
    { glBindTexture(GL_TEXTURE_2D,enigma::spritestructarray[sprite]->texturearray[0]); enigma::current_texture_bound_that_noone_should_ever_change_ever=enigma::spritestructarray[sprite]->texturearray[0]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    int w=enigma::spritestructarray[sprite]->width;
    int h=enigma::spritestructarray[sprite]->height;
    
    int xo=enigma::spritestructarray[sprite]->xoffset;
    int yo=enigma::spritestructarray[sprite]->yoffset;
    
    float tbw=w/enigma::spritestructarray[sprite]->texbordx;
    float tbh=h/enigma::spritestructarray[sprite]->texbordy;
    
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(left/tbw,top/tbh);
    glVertex2f(x-xo,y-yo);
    glTexCoord2f((left+width)/tbw,top/tbh);
    glVertex2f(x+width-xo,y-yo);
    glTexCoord2f(left/tbw,(top+height)/tbh);
    glVertex2f(x-xo,y+height-yo);
    glTexCoord2f((left+width)/tbw,(top+height)/tbh);
    glVertex2f(x+width-xo,y+height-yo);
    
    glEnd();
    
    glPopAttrib();
    
    return 0;
}

int draw_sprite_general(int spr,int subimg,double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double alpha)
{
    int sprite=(int)spr;
    
    if (!sprite_exists(spr))
    return -1;
    
    glPushAttrib(GL_CURRENT_COLOR);
    
    if (enigma::current_texture_bound_that_noone_should_ever_change_ever != enigma::spritestructarray[sprite]->texturearray[0])
    { glBindTexture(GL_TEXTURE_2D,enigma::spritestructarray[sprite]->texturearray[0]); enigma::current_texture_bound_that_noone_should_ever_change_ever=enigma::spritestructarray[sprite]->texturearray[0]; }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    int w=(int)(enigma::spritestructarray[sprite]->width*xscale);
    int h=(int)(enigma::spritestructarray[sprite]->height*yscale);
    
    int xo=enigma::spritestructarray[sprite]->xoffset;
    int yo=enigma::spritestructarray[sprite]->yoffset;
    
    int tbx=(int)(enigma::spritestructarray[sprite]->texbordx);
    int tby=(int)(enigma::spritestructarray[sprite]->texbordy);
    
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
