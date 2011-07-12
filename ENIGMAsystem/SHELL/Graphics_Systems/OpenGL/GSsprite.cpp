/********************************************************************************\
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
 \********************************************************************************/

#include <cmath>
#include <cstdlib>

#include "OpenGLHeaders.h"

#include "../../Universal_System/spritestruct.h"
#include "../../Universal_System/instance_system.h"
#include "../../Universal_System/graphics_object.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma{extern unsigned bound_texture;}

bool sprite_exists(int spr) {
    return (unsigned(spr) < enigma::sprite_idmax) and bool(enigma::spritestructarray[spr]);
}

int draw_sprite(int spr,int subimg,double x,double y)
{
    enigma::sprite *spr2d = enigma::spritestructarray[spr];
    if (!spr2d)
        return -1;
    
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    if (enigma::bound_texture != spr2d->texturearray[usi])
    {
        glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[usi]);
        enigma::bound_texture = spr2d->texturearray[usi];
    }
    
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    
    const float tbx=spr2d->texbordx,tby=spr2d->texbordy;
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(x-spr2d->xoffset,y-spr2d->yoffset);
    glTexCoord2f(tbx,0);
    glVertex2f(x+spr2d->width-spr2d->xoffset,y-spr2d->yoffset);
    glTexCoord2f(tbx,tby);
    glVertex2f(x+spr2d->width-spr2d->xoffset,y+spr2d->height-spr2d->yoffset);
    glTexCoord2f(0,tby);
    glVertex2f(x-spr2d->xoffset,y+spr2d->height-spr2d->yoffset);
    glEnd();
	
	glPopAttrib();
	return 0;
}

int draw_sprite_stretched(int spr,int subimg,double x,double y,double w,double h)
{
	enigma::sprite *spr2d=enigma::spritestructarray[spr];
	if (!spr2d)
        return -1;
	
	glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    if (enigma::bound_texture!=spr2d->texturearray[usi])
    {
        glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[usi]);
        enigma::bound_texture=spr2d->texturearray[usi];
    }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    glBegin(GL_QUADS);
    glTexCoord2f(spr2d->texbordx,0);
    glVertex2f(x-spr2d->xoffset,y-spr2d->yoffset);
    glTexCoord2f(0,0);
    glVertex2f(x+w-spr2d->xoffset,y-spr2d->yoffset);
    glTexCoord2f(0,spr2d->texbordy);
    glVertex2f(x+w-spr2d->xoffset,y+h-spr2d->yoffset);
    glTexCoord2f(spr2d->texbordx,spr2d->texbordy);
    glVertex2f(x-spr2d->xoffset,y+h-spr2d->yoffset);
    glEnd();
	glPopAttrib();
	return 0;
}

int draw_sprite_part(int spr,int subimg,double left,double top,double width,double height,double x,double y)
{
    enigma::sprite *spr2d=enigma::spritestructarray[spr];
    if (!spr2d)
        return -1;
    
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    if (enigma::bound_texture != spr2d->texturearray[usi])
    {
        glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[usi]);
        enigma::bound_texture=spr2d->texturearray[usi];
    }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    float tbw=spr2d->width/(float)spr2d->texbordx,tbh=spr2d->height/(float)spr2d->texbordy;
    
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
    enigma::sprite *spr2d=enigma::spritestructarray[spr];
    if (!spr2d)
        return -1;
    
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    if (enigma::bound_texture != spr2d->texturearray[usi])
    {
        glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[usi]);
        enigma::bound_texture=spr2d->texturearray[usi];
    }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    float tbw=spr2d->width/spr2d->texbordx,tbh=spr2d->height/spr2d->texbordy;
    
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(left/tbw,top/tbh);
    glVertex2f(x-spr2d->xoffset,y-spr2d->yoffset);
    glTexCoord2f((left+width-1)/tbw,top/tbh);
    glVertex2f(x+width-1-spr2d->xoffset,y-spr2d->yoffset);
    glTexCoord2f(left/tbw,(top+height-1)/tbh);
    glVertex2f(x-spr2d->xoffset,y+height-1-spr2d->yoffset);
    glTexCoord2f((left+width-1)/tbw,(top+height-1)/tbh);
    glVertex2f(x+width-1-spr2d->xoffset,y+height-1-spr2d->yoffset);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);
	glPopAttrib();
	return 0;
}

int draw_sprite_ext(int spr,int subimg,double x,double y,double xscale,double yscale,double rot,int blend,double alpha)
{
    enigma::sprite *spr2d = enigma::spritestructarray[spr];
    if (!spr2d)
        return -1;
    
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    if (enigma::bound_texture != spr2d->texturearray[usi])
    {
        glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[usi]);
        enigma::bound_texture = spr2d->texturearray[usi];
    }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    glPushAttrib(GL_CURRENT_BIT);
    glBegin(GL_TRIANGLE_STRIP);
    const float w = spr2d->width*xscale,   h = spr2d->height*yscale;
    const float tbx = spr2d->texbordx,   tby = spr2d->texbordy;
    rot *= M_PI/180;
    
    float
    ulcx = x + xscale * spr2d->xoffset * cos(M_PI+rot) + yscale * spr2d->yoffset * cos(M_PI/2+rot),
    ulcy = y - yscale * spr2d->xoffset * sin(M_PI+rot) - yscale * spr2d->yoffset * sin(M_PI/2+rot);
    
    glColor4ub(__GETR(blend),__GETG(blend),__GETB(blend),char(alpha*255)); //Implement "blend" parameter
    glTexCoord2f(0,0);
    glVertex2f(ulcx,ulcy);
    glTexCoord2f(tbx,0);
    glVertex2f(ulcx + w*cos(rot), ulcy - w*sin(rot));
    glTexCoord2f(0,tby);
    ulcx += h * cos(3*M_PI/2 + rot);
    ulcy -= h * sin(3*M_PI/2 + rot);
    glVertex2f(ulcx,ulcy);
    glTexCoord2f(tbx,tby);
    glVertex2f(ulcx + w*cos(rot), ulcy - w*sin(rot));
    glEnd();
    glPopAttrib();
    return 0;
}

int draw_sprite_part_ext(int spr,int subimg,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha)
{
	enigma::sprite *spr2d=enigma::spritestructarray[spr];
	if (!spr2d)
        return -1;
	
	glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    if (enigma::bound_texture != spr2d->texturearray[usi])
    {
        glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[usi]);
        enigma::bound_texture=spr2d->texturearray[usi];
    }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    float tbw=spr2d->width/spr2d->texbordx,tbh=spr2d->height/spr2d->texbordy;
    
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(left/tbw,top/tbh);
    glVertex2f(x-spr2d->xoffset,y-spr2d->yoffset);
    glTexCoord2f((left+width-1)/tbw,top/tbh);
    glVertex2f(x+width-1-spr2d->xoffset,y-spr2d->yoffset);
    glTexCoord2f(left/tbw,(top+height-1)/tbh);
    glVertex2f(x-spr2d->xoffset,y+height-1-spr2d->yoffset);
    glTexCoord2f((left+width-1)/tbw,(top+height-1)/tbh);
    glVertex2f(x+width-1-spr2d->xoffset,y+height-1-spr2d->yoffset);
    glEnd();
	glPopAttrib();
	return 0;
}

/* Copyright (C) 2010 Harijs Grînbergs, Josh Ventura
 * The applicable license does not change for this portion of the file.
 */

int draw_sprite_general(int spr,int subimg,double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a1, double a2, double a3, double a4)
{
    enigma::sprite *spr2d=enigma::spritestructarray[spr];
    if (!spr2d)
        return -1;
    
    glPushAttrib(GL_CURRENT_BIT);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    if (enigma::bound_texture != spr2d->texturearray[usi])
    {
        glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[usi]);
        enigma::bound_texture=spr2d->texturearray[usi];
    }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    const float
    tbx = spr2d->texbordx,  tby = spr2d->texbordy,
    tbw = spr2d->width/tbx, tbh = spr2d->height/tby,
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

int draw_sprite_stretched_ext(int spr,int subimg,double x,double y,double w,double h, int blend, double alpha)
{
    enigma::sprite *spr2d=enigma::spritestructarray[spr];
    if (!spr2d)
        return -1;
    
    glPushAttrib(GL_CURRENT_BIT);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    if (enigma::bound_texture!=spr2d->texturearray[usi])
    {
        glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[usi]);
        enigma::bound_texture=spr2d->texturearray[usi];
    }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    glBegin(GL_QUADS);
    glColor4ub(__GETR(blend),__GETG(blend),__GETB(blend),char(alpha*255)); //Implement "blend" parameter
    glTexCoord2f(spr2d->texbordx,0);
    glVertex2f(x-spr2d->xoffset,y-spr2d->yoffset);
    glTexCoord2f(0,0);
    glVertex2f(x+w-spr2d->xoffset,y-spr2d->yoffset);
    glTexCoord2f(0,spr2d->texbordy);
    glVertex2f(x+w-spr2d->xoffset,y+h-spr2d->yoffset);
    glTexCoord2f(spr2d->texbordx,spr2d->texbordy);
    glVertex2f(x-spr2d->xoffset,y+h-spr2d->yoffset);
    glEnd();
    glPopAttrib();
    return 0;
}

// These two leave a bad taste in my mouth because they depend on views, which should be removable.
// However, for now, they stay.

#include <string>
using std::string;
#include "../../Universal_System/var4.h"
#include "../../Universal_System/roomsystem.h"

int draw_sprite_tiled(int spr,int subimg,double x,double y)
{
    enigma::sprite *spr2d = enigma::spritestructarray[spr];
    if (!spr2d)
        return -1;
    
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    if (enigma::bound_texture != spr2d->texturearray[usi])
    {
        glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[usi]);
        enigma::bound_texture = spr2d->texturearray[usi];
    }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    
    const float 
    tbx  = spr2d->texbordx,  tby  = spr2d->texbordy,
    xoff = spr2d->xoffset+x, yoff = spr2d->yoffset+y;
    const int
    hortil = int (ceil(   (view_enabled ? int(view_xview[view_current] + view_wview[view_current]) : room_width)    / (spr2d->width*tbx))),
    vertil = int (ceil(   (view_enabled ? int(view_yview[view_current] + view_hview[view_current]) : room_height)   / (spr2d->height*tby)));
    
    glBegin(GL_QUADS);
    for (int i=0; i<hortil; i++)
    {
        for (int c=0; c<vertil; c++)
        {
            glTexCoord2f(0,0);
            glVertex2f(i*spr2d->width-xoff,c*spr2d->height-yoff);
            glTexCoord2f(tbx,0);
            glVertex2f((i+1)*spr2d->width-xoff,c*spr2d->height-yoff);
            glTexCoord2f(tbx,tby);
            glVertex2f((i+1)*spr2d->width-xoff,(c+1)*spr2d->height-yoff);
            glTexCoord2f(0,tby);
            glVertex2f(i*spr2d->width-xoff,(c+1)*spr2d->height-yoff);
        }
    }
    glEnd();
    glPopAttrib();
    return 0;
}

int draw_sprite_tiled_ext(int spr,int subimg,double x,double y, double xscale,double yscale,int color,double alpha)
{
    enigma::sprite *spr2d = enigma::spritestructarray[spr];
    if (!spr2d)
        return -1;
    
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    if (enigma::bound_texture != spr2d->texturearray[usi])
    {
        glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[usi]);
        enigma::bound_texture = spr2d->texturearray[usi];
    }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
    
    glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));
    
    const float 
    tbx  = spr2d->texbordx,         tby  = spr2d->texbordy,
    xoff = spr2d->xoffset*xscale+x, yoff = spr2d->yoffset*yscale+y;
    const int
    hortil= int (ceil(   (view_enabled ? int(view_xview[view_current] + view_wview[view_current]) : room_width)    / (spr2d->width*tbx*xscale))),
    vertil= int (ceil(   (view_enabled ? int(view_yview[view_current] + view_hview[view_current]) : room_height)   / (spr2d->height*tby*yscale)));
    
    glBegin(GL_QUADS);
    for (int i=0; i<hortil; i++)
    {
        for (int c=0; c<vertil; c++)
        {
            glTexCoord2f(0,0);
            glVertex2f(i*spr2d->width*xscale-xoff,c*spr2d->height*yscale-yoff);
            glTexCoord2f(tbx,0);
            glVertex2f((i+1)*spr2d->width*xscale-xoff,c*spr2d->height*yscale-yoff);
            glTexCoord2f(tbx,tby);
            glVertex2f((i+1)*spr2d->width*xscale-xoff,(c+1)*spr2d->height*yscale-yoff);
            glTexCoord2f(0,tby);
            glVertex2f(i*spr2d->width*xscale-xoff,(c+1)*spr2d->height*yscale-yoff);
        }
    }
    glEnd();
    
    glPopAttrib();
    return 0;
}