/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura, Dave "biggoron"                            **
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

/**Surface functions***********************************************************\

int surface_create(double width, double height)
int surface_set_target(int id)
int surface_reset_target(void)
int surface_destroy(int id)
int draw_surface(double id, double x, double y)

\******************************************************************************/

//#include <OpenGL/glew.h>
#include "../OpenGLHeaders.h"

extern int room_width, room_height;
/*
PFNGLBINDFRAMEBUFFEREXTPROC      glBindFramebufferEXT;
PFNGLGENFRAMEBUFFERSEXTPROC      glGenFramebuffersEXT;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;

namespace enigma
{
  struct surface
  {
	GLuint tex, fbo;
	int width, height;
  };

  surface **surface_array;
  int surface_max=0;
}

int surface_create(double width, double height)
{
	GLuint tex, fbo;
	int prevFbo;

	int id,
    w=(int)width,
    h=(int)height; //get the integer width and height, and prepare to search for an id

	if (enigma::surface_max==0)
	{
       enigma::surface_array=new enigma::surface*[1];
       enigma::surface_max=1;
    }

	for (id=0; enigma::surface_array[id]!=NULL; id++)
	{
        if (id+1>=enigma::surface_max)
        {
          enigma::surface **oldarray=enigma::surface_array;
          enigma::surface_array=new enigma::surface*[enigma::surface_max+1];
          for (int i=0; i<enigma::surface_max; i++)
          {
            enigma::surface_array[i]=oldarray[i];
          }
          enigma::surface_array[enigma::surface_max]=NULL;
          enigma::surface_max++;
          delete[] oldarray;
        }
    }

	enigma::surface_array[id] = new enigma::surface;
	enigma::surface_array[id]->width = w;
	enigma::surface_array[id]->height = h;

	glGenTextures(1, &tex);
	glGenFramebuffersEXT(1, &fbo);

	glPushAttrib(GL_TEXTURE_BIT);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glClearColor(1,1,1,0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, prevFbo);
	glPopAttrib();

	enigma::surface_array[id]->tex = tex;
	enigma::surface_array[id]->fbo = fbo;

	return id;
}

int surface_set_target(int id)
{
    enigma::surface* surf=enigma::surface_array[id]; //copy the surface data from the array
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surf->fbo); //bind it
    glPushMatrix(); //So you can pop it in the reset
    glPushAttrib(GL_VIEWPORT_BIT); //same
    glViewport(0,0,surf->width,surf->height);
    glLoadIdentity();
    glOrtho(0, surf->width*(1.995/room_width),  0, surf->height*(1.995/room_height), -1, 1);
    return 0;
}

int surface_reset_target(void)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glPopAttrib();
	glPopMatrix();
	return 0;
}

int surface_destroy(int id)
{
	enigma::surface* surf=enigma::surface_array[id];
    surf->width = surf->height = surf->tex = surf->fbo = 0;
	delete surf;
	return 0;
}

int draw_surface(double id, double x, double y)
{
    int s=(int)id;
    if (s>enigma::surface_max)
    {
        #if SHOWERRORS
        show_error("Surface does not exist",0);
        #endif
        return -1;
    }
    if (enigma::surface_array[s]==NULL)
    {
        #if SHOWERRORS
        show_error("Surface does not exist",0);
        #endif
        return -1;
    }

    glPushAttrib(GL_CURRENT_BIT);

    glColor4f(1,1,1,1);

    glBindTexture(GL_TEXTURE_2D,enigma::surface_array[s]->tex);
    int w=enigma::surface_array[s]->width;
    int h=enigma::surface_array[s]->height;

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);		glVertex2f(x,   y);
    glTexCoord2f(1, 0);		glVertex2f(x+w, y);
    glTexCoord2f(1, 1);		glVertex2f(x+w, y+h);
    glTexCoord2f(0, 1);		glVertex2f(x,   y+h);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopAttrib();

    return 0;
}
*/
