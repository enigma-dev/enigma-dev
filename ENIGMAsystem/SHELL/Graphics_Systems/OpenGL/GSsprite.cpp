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

#include <math.h>
#include <GL/gl.h>
#include "../../Universal_System/spritestruct.h"

#define __GETRf(x) fmod(x,256)
#define __GETGf(x) fmod(x/256,256)
#define __GETBf(x) fmod(x/65536,256)

namespace enigma{extern unsigned cur_bou_tha_noo_sho_eve_cha_eve;}

int sprite_exists(int spr)
{
  return unsigned(spr) < enigma::sprite_idmax and bool(enigma::spritestructarray[spr]);
}

int draw_sprite(int spr,int subimg,double x,double y)
{
  enigma::sprite *spr2d = enigma::spritestructarray[spr];
  if (!spr2d)
    return -1;
  
  if (enigma::cur_bou_tha_noo_sho_eve_cha_eve != spr2d->texturearray[subimg % spr2d->subcount])
  {
    glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[subimg % spr2d->subcount]);
    enigma::cur_bou_tha_noo_sho_eve_cha_eve = spr2d->texturearray[subimg % spr2d->subcount];
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
    if (enigma::cur_bou_tha_noo_sho_eve_cha_eve!=spr2d->texturearray[subimg % spr2d->subcount])
    {
      glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[subimg % spr2d->subcount]);
      enigma::cur_bou_tha_noo_sho_eve_cha_eve=spr2d->texturearray[subimg % spr2d->subcount];
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
    if (enigma::cur_bou_tha_noo_sho_eve_cha_eve != spr2d->texturearray[subimg % spr2d->subcount])
    {
      glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[subimg % spr2d->subcount]);
      enigma::cur_bou_tha_noo_sho_eve_cha_eve=spr2d->texturearray[subimg % spr2d->subcount];
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
    if (enigma::cur_bou_tha_noo_sho_eve_cha_eve != spr2d->texturearray[subimg % spr2d->subcount])
    {
      glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[subimg % spr2d->subcount]);
      enigma::cur_bou_tha_noo_sho_eve_cha_eve=spr2d->texturearray[subimg % spr2d->subcount];
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

int draw_sprite_ext(int spr,int subimg,double x,double y,double xscale,double yscale,double rot,double blend,double alpha)
{
	enigma::sprite *spr2d=enigma::spritestructarray[spr];
	if (!spr2d)
    return -1;
  if (enigma::cur_bou_tha_noo_sho_eve_cha_eve != spr2d->texturearray[subimg % spr2d->subcount])
  {
    glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[subimg % spr2d->subcount]);
    enigma::cur_bou_tha_noo_sho_eve_cha_eve=spr2d->texturearray[subimg % spr2d->subcount];
  }
  
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  
  int w = int(spr2d->width*xscale),   h = int(spr2d->height*yscale);
  float tbx = spr2d->texbordx,        tby = spr2d->texbordy;
  rot *= M_PI/180;
  
  glPushAttrib(GL_CURRENT_BIT);
    glBegin(GL_TRIANGLE_STRIP);
      float
        ulcx = x + spr2d->xoffset*cos(M_PI+rot) + spr2d->yoffset*cos(M_PI/2+rot),
        ulcy = y + spr2d->xoffset*-sin(M_PI+rot) + spr2d->yoffset*-sin(M_PI/2+rot);
      
      glColor4f(__GETRf(blend)/255,__GETGf(blend)/255,__GETBf(blend)/255,alpha); //Implement "blend" parameter
        glTexCoord2f(0,0);
          glVertex2f(ulcx,ulcy);
        glTexCoord2f(tbx,0);
          glVertex2f(ulcx+w*cos(rot),ulcy-w*sin(rot));
        glTexCoord2f(0,tby);
          ulcx+=h*cos(3*M_PI/2+rot);
        ulcy-=h*sin(3*M_PI/2+rot);
          glVertex2f(ulcx,ulcy);
        glTexCoord2f(tbx,tby);
          glVertex2f(ulcx+w*cos(rot),ulcy-w*sin(rot));
      //End of blend color
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
    glColor4f(__GETRf(color)/255,__GETGf(color)/255,__GETBf(color)/255,alpha);
    if (enigma::cur_bou_tha_noo_sho_eve_cha_eve != spr2d->texturearray[subimg % spr2d->subcount])
    {
      glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[subimg % spr2d->subcount]);
      enigma::cur_bou_tha_noo_sho_eve_cha_eve=spr2d->texturearray[subimg % spr2d->subcount];
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

int draw_sprite_general(int spr,int subimg,double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double alpha)
{
	enigma::sprite *spr2d=enigma::spritestructarray[spr];
	if (!spr2d)
    return -1;
	
  glPushAttrib(GL_CURRENT_BIT);
    if (enigma::cur_bou_tha_noo_sho_eve_cha_eve != spr2d->texturearray[subimg % spr2d->subcount])
    {
      glBindTexture(GL_TEXTURE_2D,spr2d->texturearray[subimg % spr2d->subcount]);
      enigma::cur_bou_tha_noo_sho_eve_cha_eve=spr2d->texturearray[subimg % spr2d->subcount];
    }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    int w = int(spr2d->width * xscale),h = int(spr2d->height*yscale);
    float tbx=spr2d->texbordx,tby=spr2d->texbordy;
    
    glBegin(GL_TRIANGLE_STRIP);
      glColor4f(__GETRf(c1)/255,__GETGf(c1)/255,__GETBf(c1)/255,alpha);
        glTexCoord2f(width/tbx,top/tby);
          glVertex2f(x-spr2d->xoffset,y-spr2d->yoffset);
      glColor4f(__GETRf(c2)/255,__GETGf(c2)/255,__GETBf(c2)/255,alpha);
        glTexCoord2f(left/tbx,top/tby);
          glVertex2f(x+w-spr2d->xoffset,y-spr2d->yoffset);
      glColor4f(__GETRf(c3)/255,__GETGf(c3)/255,__GETBf(c3)/255,alpha);
        glTexCoord2f(width/tbx,height/tby);
          glVertex2f(x-spr2d->xoffset,y+h-spr2d->yoffset);
      glColor4f(__GETRf(c4)/255,__GETGf(c4)/255,__GETBf(c4)/255,alpha);
        glTexCoord2f(left/tbx,height/tby);
          glVertex2f(x+w-spr2d->xoffset,y+h-spr2d->yoffset);
    glEnd();
	glPopAttrib();
	return 0;
}
