/** Copyright (C) 2008-2011 Josh Ventura
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
#include <string>
using std::string;

#include "OpenGLHeaders.h"
#include "GSsprite.h"
#include "binding.h"

#include "../../Universal_System/spritestruct.h"
#include "../../Universal_System/instance_system.h"
#include "../../Universal_System/graphics_object.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)


#ifdef DEBUG_MODE
  #include "../../libEGMstd.h"
  #include "../../Widget_Systems/widgets_mandatory.h"
  #define get_sprite(spr,id,r) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return r; \
    } const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_spritev(spr,id) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return; \
    } const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_sprite_null(spr,id,r) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return r; \
    } const enigma::sprite *const spr = enigma::spritestructarray[id];
#else
  #define get_sprite(spr,id,r) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_spritev(spr,id) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_sprite_null(spr,id,r) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
#endif


bool sprite_exists(int spr) {
    return (unsigned(spr) < enigma::sprite_idmax) and bool(enigma::spritestructarray[spr]);
}

void draw_sprite(int spr,int subimg,double x,double y)
{
    get_spritev(spr2d,spr);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    bind_texture(spr2d->texturearray[usi]);

    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);

    const float tbx = spr2d->texbordx, tby = spr2d->texbordy,
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
}

void draw_sprite_stretched(int spr,int subimg,double x,double y,double w,double h)
{
    get_spritev(spr2d,spr);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    bind_texture(spr2d->texturearray[usi]);

	glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);

    const float tbx = spr2d->texbordx, tby = spr2d->texbordy,
                xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + w,
                yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + h;

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
}

void draw_sprite_part(int spr,int subimg,double left,double top,double width,double height,double x,double y)
{
    get_spritev(spr2d,spr);
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    bind_texture(spr2d->texturearray[usi]);

    float tbw = spr2d->width/(float)spr2d->texbordx, tbh = spr2d->height/(float)spr2d->texbordy,
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

void draw_sprite_part_offset(int spr,int subimg,double left,double top,double width,double height,double x,double y)
{
    get_spritev(spr2d,spr);
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    bind_texture(spr2d->texturearray[usi]);

    float tbw = spr2d->width/spr2d->texbordx, tbh = spr2d->height/spr2d->texbordy,
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
}

void draw_sprite_ext(int spr,int subimg,double x,double y,double xscale,double yscale,double rot,int blend,double alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    bind_texture(spr2d->texturearray[usi]);

    glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(blend),__GETG(blend),__GETB(blend),char(alpha*255)); //Implement "blend" parameter

    rot *= M_PI/180;

    const float
    w = spr2d->width*xscale, h = spr2d->height*yscale,
    tbx = spr2d->texbordx, tby = spr2d->texbordy,
    wsinrot = w*sin(rot), wcosrot = w*cos(rot);

    glBegin(GL_TRIANGLE_STRIP);

    float
    ulcx = x - xscale * spr2d->xoffset * cos(rot) + yscale * spr2d->yoffset * cos(M_PI/2+rot),
    ulcy = y + xscale * spr2d->xoffset * sin(rot) - yscale * spr2d->yoffset * sin(M_PI/2+rot);
    glTexCoord2f(0,0);
    glVertex2f(ulcx,ulcy);
    glTexCoord2f(tbx,0);
    glVertex2f(ulcx + wcosrot, ulcy - wsinrot);

    const double mpr = 3*M_PI/2 + rot;
    ulcx += h * cos(mpr);
    ulcy -= h * sin(mpr);
    glTexCoord2f(0,tby);
    glVertex2f(ulcx,ulcy);
    glTexCoord2f(tbx,tby);
    glVertex2f(ulcx + wcosrot, ulcy - wsinrot);

    glEnd();

    glPopAttrib();
}

void draw_sprite_part_ext(int spr,int subimg,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha)
{
    get_spritev(spr2d,spr);
	glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    bind_texture(spr2d->texturearray[usi]);

    float tbw = spr2d->width/(float)spr2d->texbordx, tbh = spr2d->height/(float)spr2d->texbordy,
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

/* Copyright (C) 2010 Harijs Grînbergs, Josh Ventura
 * The applicable license does not change for this portion of the file.
 */

void draw_sprite_general(int spr,int subimg,double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a1, double a2, double a3, double a4)
{
    get_spritev(spr2d,spr);
    glPushAttrib(GL_CURRENT_BIT);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    bind_texture(spr2d->texturearray[usi]);

    const float
    tbx = spr2d->texbordx,  tby = spr2d->texbordy,
    tbw = spr2d->width/tbx, tbh = spr2d->height/tby,
    w = width*xscale, h = height*yscale;

    rot *= M_PI/180;
    const float wcosrot = w*cos(rot), wsinrot = w*sin(rot);

    glBegin(GL_QUADS);

    float
    ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
    ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);
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

void draw_sprite_stretched_ext(int spr,int subimg,double x,double y,double w,double h, int blend, double alpha)
{
    get_spritev(spr2d,spr);
    glPushAttrib(GL_CURRENT_BIT);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    bind_texture(spr2d->texturearray[usi]);

    const float tbx = spr2d->texbordx, tby = spr2d->texbordy,
                xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + w,
                yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + h;

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
}

// These two leave a bad taste in my mouth because they depend on views, which should be removable.
// However, for now, they stay.

#include <string>
using std::string;
#include "../../Universal_System/var4.h"
#include "../../Universal_System/roomsystem.h"

void draw_sprite_tiled(int spr,int subimg,double x,double y)
{
    get_spritev(spr2d,spr);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    bind_texture(spr2d->texturearray[usi]);

    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);

    const float
    tbx  = spr2d->texbordx, tby  = spr2d->texbordy,
    xoff = spr2d->xoffset+x, yoff = spr2d->yoffset+y;

    const int
    hortil = int(ceil((view_enabled ? int(view_xview[view_current] + view_wview[view_current]) : room_width) / (spr2d->width*tbx))) + 1,
    vertil = int(ceil((view_enabled ? int(view_yview[view_current] + view_hview[view_current]) : room_height) / (spr2d->height*tby))) + 1;

    glBegin(GL_QUADS);
    float xvert1 = -xoff, xvert2 = xvert1 + spr2d->width, yvert1, yvert2;
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
}

void draw_sprite_tiled_ext(int spr,int subimg,double x,double y, double xscale,double yscale,int color,double alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg != -1 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    bind_texture(spr2d->texturearray[usi]);

    glPushAttrib(GL_CURRENT_BIT);
    glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

    const float
    tbx  = spr2d->texbordx, tby  = spr2d->texbordy,
    xoff = spr2d->xoffset*xscale+x, yoff = spr2d->yoffset*yscale+y,
    width_scaled = spr2d->width*xscale, height_scaled = spr2d->height*yscale;

    const int
    hortil = int(ceil((view_enabled ? int(view_xview[view_current] + view_wview[view_current]) : room_width) / (width_scaled*tbx))) + 1,
    vertil = int(ceil((view_enabled ? int(view_yview[view_current] + view_hview[view_current]) : room_height) / (height_scaled*tby))) + 1;

    glBegin(GL_QUADS);
    float xvert1 = -xoff, xvert2 = xvert1 + width_scaled, yvert1, yvert2;
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
}
