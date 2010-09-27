/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
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

#include "GSbackground.h"
#include "OpenGLHeaders.h"
#include "../../Universal_System/backgroundstruct.h"


//TGMG move these to GSbackground.cpp
int draw_background(int back, double x, double y)
{
	enigma::background *spr2d = enigma::backgroundstructarray[back];
	if (!spr2d)
		return -1;
	
	spr2d->texture;
	glBindTexture(GL_TEXTURE_2D,spr2d->texture);
	
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	
	
	glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    
    const float tbx=1,tby=1;//for now
    glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex2f(x,y);
	glTexCoord2f(tbx,0);
	glVertex2f(x+spr2d->width,y);
	glTexCoord2f(tbx,tby);
	glVertex2f(x+spr2d->width,y+spr2d->height);
	glTexCoord2f(0,tby);
	glVertex2f(x,y+spr2d->height);
    glEnd();
	
	glPopAttrib();
	return 0;
}
