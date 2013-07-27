/** Copyright (C) 2008-2013 Josh Ventura
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

#include <cmath>
#include <cstdlib>

#include "OpenGLHeaders.h"

#include "Universal_System/spritestruct.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma{extern unsigned bound_texture;}

bool sprite_exists(int spr) {
    return (unsigned(spr) < enigma::sprite_idmax) and bool(enigma::spritestructarray[spr]);
}

int draw_sprite(int spr, int subimg, gs_scalar x, gs_scalar y)
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

    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);

    const gs_scalar tbx = spr2d->texbordx, tby = spr2d->texbordy,
                xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + spr2d->width,
                yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + spr2d->height;

    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(xvert1,yvert1);
    glTexCoord2f(tbx,0);
    glVertex2f(xvert2,yvert1);
    glTexCoord2f(tbx,tby);
    glVertex2f(xvert2,yvert2);
    glTexCoord2f(0,tby);
    glVertex2f(xvert1,yvert2);
    glEnd();

	glPopAttrib();
	return 0;
}

int draw_sprite_stretched(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height)
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

    const gs_scalar tbx = spr2d->texbordx, tby = spr2d->texbordy,
                xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + width,
                yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + height;

    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(xvert1,yvert1);
    glTexCoord2f(tbx,0);
    glVertex2f(xvert2,yvert1);
    glTexCoord2f(tbx,tby);
    glVertex2f(xvert2,yvert2);
    glTexCoord2f(0,tby);
    glVertex2f(xvert1,yvert2);
    glEnd();

	glPopAttrib();
	return 0;
}

int draw_sprite_part(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
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

    gs_scalar tbw = spr2d->width/(gs_scalar)spr2d->texbordx, tbh = spr2d->height/(gs_scalar)spr2d->texbordy,
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
	return 0;
}

int draw_sprite_part_offset(int spr,int subimg,gs_scalar left, gs_scalar top,gs_scalar width, gs_scalar height,gs_scalar x, gs_scalar y)
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

    gs_scalar tbw = spr2d->width/spr2d->texbordx, tbh = spr2d->height/spr2d->texbordy,
          xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + spr2d->width,
          yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + spr2d->height,
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
	return 0;
}

int draw_sprite_ext(int spr,int subimg,gs_scalar x, gs_scalar y,gs_scalar xscale, gs_scalar yscale,double rot,int blend,double alpha)
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

    glPushAttrib(GL_CURRENT_BIT);
    glBegin(GL_TRIANGLE_STRIP);
    const gs_scalar w = spr2d->width*xscale,   h = spr2d->height*yscale;
    const gs_scalar tbx = spr2d->texbordx,   tby = spr2d->texbordy;
    rot *= M_PI/180;

    gs_scalar
    ulcx = x + xscale * spr2d->xoffset * cos(M_PI+rot) + yscale * spr2d->yoffset * cos(M_PI/2+rot),
    ulcy = y - yscale * spr2d->xoffset * sin(M_PI+rot) - yscale * spr2d->yoffset * sin(M_PI/2+rot);
    glColor4ub(__GETR(blend),__GETG(blend),__GETB(blend),char(alpha*255)); //Implement "blend" parameter

    glTexCoord2f(0,0);
    glVertex2f(ulcx,ulcy);
    glTexCoord2f(tbx,0);
    glVertex2f(ulcx + w*cos(rot), ulcy - w*sin(rot));

    const double mpr = 3*M_PI/2 + rot;
    ulcx += h * cos(mpr);
    ulcy -= h * sin(mpr);

    glTexCoord2f(0,tby);
    glVertex2f(ulcx,ulcy);
    glTexCoord2f(tbx,tby);
    glVertex2f(ulcx + w*cos(rot), ulcy - w*sin(rot));

    glEnd();
    glPopAttrib();
    return 0;
}

int draw_sprite_part_ext(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
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

    gs_scalar tbw = spr2d->width/(gs_scalar)spr2d->texbordx, tbh = spr2d->height/(gs_scalar)spr2d->texbordy,
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
	return 0;
}

/* Copyright (C) 2010 Harijs Grînbergs, Josh Ventura
 * The applicable license does not change for this portion of the file.
 */

int draw_sprite_general(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, double a1, double a2, double a3, double a4)
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

    const gs_scalar
    tbx = spr2d->texbordx,  tby = spr2d->texbordy,
    tbw = spr2d->width/tbx, tbh = spr2d->height/tby,
    w = width*xscale, h = height*yscale;

    rot *= M_PI/180;
    const gs_scalar wcosrot = w*cos(rot), wsinrot = w*sin(rot);

    gs_scalar ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
    ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

    glBegin(GL_QUADS);

    glColor4ub(__GETR(c1),__GETG(c1),__GETB(c1),char(a1*255));
    glTexCoord2f(left/tbw,top/tbh);
    glVertex2f(ulcx,ulcy);

    glColor4ub(__GETR(c2),__GETG(c2),__GETB(c2),char(a2*255));
    glTexCoord2f((left+width)/tbw,top/tbh);
    glVertex2f((ulcx + wcosrot), (ulcy - wsinrot));

    const double mpr = 3*M_PI/2 + rot;
    ulcx += h * cos(mpr);
    ulcy -= h * sin(mpr);

    glColor4ub(__GETR(c3),__GETG(c3),__GETB(c3),char(a3*255));
    glTexCoord2f((left+width)/tbw,(top+height)/tbh);
    glVertex2f((ulcx + wcosrot), (ulcy - wsinrot));

    glColor4ub(__GETR(c4),__GETG(c4),__GETB(c4),char(a4*255));
    glTexCoord2f(left/tbw,(top+height)/tbh);
    glVertex2f(ulcx,ulcy);

    glEnd();
    glPopAttrib();
    return 0;
}

int draw_sprite_stretched_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int blend, double alpha)
{
    enigma::sprite *spr2d=enigma::spritestructarray[spr];
    if (!spr2d)
        return -1;

    glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(blend),__GETG(blend),__GETB(blend),char(alpha*255)); //Implement "blend" parameter
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    if (enigma::bound_texture!=spr2d->texturearray[usi])
    {
        glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[usi]);
        enigma::bound_texture=spr2d->texturearray[usi];
    }

    const gs_scalar tbx = spr2d->texbordx, tby = spr2d->texbordy,
                xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + width,
                yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + height;

    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(xvert1,yvert1);
    glTexCoord2f(tbx,0);
    glVertex2f(xvert2,yvert1);
    glTexCoord2f(tbx,tby);
    glVertex2f(xvert2,yvert2);
    glTexCoord2f(0,tby);
    glVertex2f(xvert1,yvert2);
    glEnd();

	glPopAttrib();
    return 0;
}

// These two leave a bad taste in my mouth because they depend on views, which should be removable.
// However, for now, they stay.

#include <string>
using std::string;
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"

int draw_sprite_tiled(int spr, int subimg, gs_scalar x, gs_scalar y)
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

    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);

    const gs_scalar
    tbx  = spr2d->texbordx,  tby  = spr2d->texbordy,
    xoff = spr2d->xoffset+x, yoff = spr2d->yoffset+y;
    const int
    hortil = int (ceil((view_enabled ? int(view_xview[view_current] + view_wview[view_current]) : room_width) / (spr2d->width*tbx))),
    vertil = int (ceil((view_enabled ? int(view_yview[view_current] + view_hview[view_current]) : room_height) / (spr2d->height*tby)));

    glBegin(GL_QUADS);
    gs_scalar xvert1 = -xoff, xvert2 = xvert1 + spr2d->width, yvert1, yvert2;
    for (int i=0; i<hortil; i++)
    {
        yvert1 = -yoff; yvert2 = yvert1 + spr2d->height;
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
            yvert2 += spr2d->height;
        }
        xvert1 = xvert2;
        xvert2 += spr2d->width;
    }
    glEnd();

    glPopAttrib();
    return 0;
}

int draw_sprite_tiled_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
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

    glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    const gs_scalar
    tbx  = spr2d->texbordx,         tby  = spr2d->texbordy,
    xoff = spr2d->xoffset*xscale+x, yoff = spr2d->yoffset*yscale+y,
    width_scaled = spr2d->width*xscale,
    height_scaled = spr2d->height*yscale;

    const int
    hortil = int(ceil((view_enabled ? int(view_xview[view_current] + view_wview[view_current]) : room_width) / (width_scaled*tbx))),
    vertil = int(ceil((view_enabled ? int(view_yview[view_current] + view_hview[view_current]) : room_height) / (height_scaled*tby)));

    glBegin(GL_QUADS);
    gs_scalar xvert1 = -xoff, xvert2 = xvert1 + width_scaled, yvert1, yvert2;
    for (int i=0; i<hortil; i++)
    {
        yvert1 = -yoff; yvert2 = yvert1 + height_scaled;
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
    return 0;
}
