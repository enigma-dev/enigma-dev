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

/**Sprite adding functions*****************************************************\

int sprite_add(string filename,double imgnumb,double precise,double transparent,double smooth,double preload,double x_offset,double y_offset)

\******************************************************************************/

#include <string>
#include <cstring>
#include "OpenGLHeaders.h"
using namespace std;

#include "Universal_System/IMGloading.h"
#include "Universal_System/spritestruct.h"
#include "libEGMstd.h"

int sprite_add(string filename,double imgnumb,double precise,double transparent,double smooth,double preload,double x_offset,double y_offset)
{
	GLuint texture;
	int width,height,fullwidth,fullheight;
	char *pxdata=enigma::load_bitmap(filename,&width,&height,&fullwidth,&fullheight);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		(int)smooth,
		GL_RGBA,//4, OPENGLES
		fullwidth,
		fullheight,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pxdata);

	//test code
	//end test code

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE); //OPENGLES
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE); //OPENGLES
	//gluBuild2DMipmaps( GL_TEXTURE_2D, 3, bmpwidth, bmpheight, GL_RGB, GL_UNSIGNED_BYTE, readbuffer);
	//Free buffer
	delete[] pxdata;
	//ns.pixeldata=(void**) malloc(sizeof(void*));
	//ns.pixeldata[0]=bitmapbuffer;
	enigma::sprite *ns = enigma::spritestructarray[enigma::sprite_idmax] = new enigma::sprite;
	ns->id = enigma::sprite_idmax;
	ns->subcount  = 1;
	ns->width     = width;
	ns->height    = height;
	ns->xoffset   = (int)x_offset;
	ns->yoffset   = (int)y_offset;
	ns->texturew  = (double) width/fullwidth;
	ns->textureh  = (double) height/fullheight;
	ns->texturearray[0] = texture;
	return enigma::sprite_idmax++;
}



/* These functions are primarily for use of the engine. Experienced users
 * who are familiar with C++ can make use of these, but they were made to
 * use at load time with data read from the executable. These both expect
 * RAW format, RGB only.
 */
#include "nlpo2.h"
namespace enigma
{
  //Allocates and zero-fills the array at game start
  void sprites_allocate_initial(int spr_highid)
  {
    spritestructarray = new sprite*[spr_highid+1];
    for (int i = 0; i < spr_highid; i++)
      spritestructarray[i] = NULL;
  }

  //Adds an empty sprite to the list
 /* int sprite_new_empty(unsigned sprid, unsigned subc, int w, int h, int x, int y, int pl, int sm)
  {
    int fullwidth=nlpo2dc(w)+1,fullheight=nlpo2dc(h)+1;
    sprite *as = new sprite(subc);
    spritestructarray[sprid] = as;

    as->id=sprid;
    as->subcount=0;
    as->width=w;
    as->height=h;
    as->xoffset=x;
    as->yoffset=y;
    as->texturew=(double)w/fullwidth;
    as->textureh=(double)h/fullheight;

    if (enigma::sprite_idmax < sprid+1)
      enigma::sprite_idmax = sprid+1;

    return sprid;
  }*/

  //Adds a subimage to an existing sprite from the exe
  void sprite_add_subimage(int sprid, int x,int y, unsigned int w,unsigned int h,unsigned char*chunk)
  {
    GLuint texture;
    unsigned int fullwidth=nlpo2dc(w)+1,fullheight=nlpo2dc(h)+1;
    GLbyte *imgpxdata = new GLbyte[4*fullwidth*fullheight+1], *imgpxptr = imgpxdata;
    unsigned int rowindex,colindex;
    for (rowindex = 0; rowindex < h; rowindex++)
    {
      for(colindex = 0; colindex < w; colindex++)
      {
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
      }
      memset(imgpxptr, 0, (fullwidth-colindex) << 2);
      imgpxptr += (fullwidth-colindex) << 2;
    }
    memset(imgpxptr,0,(fullheight-h) * fullwidth);

    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
	  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // OPENGLES added
	  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // OPENGLES added

	  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,fullwidth,fullheight,0,GL_RGBA,GL_UNSIGNED_BYTE,imgpxdata); //OPENGLES changed 4 to GL_RGBA
    //removed fullwidth, fullheight
	  //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); //OPENGLES
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); //OPENGLES
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE); //OPENGLES
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE); //OPENGLES

    //gluBuild2DMipmaps( GL_TEXTURE_2D, 3, bmpwidth, bmpheight, GL_RGB, GL_UNSIGNED_BYTE, readbuffer);
    glBindTexture(GL_TEXTURE_2D,0);
    delete[] imgpxdata;

    enigma::sprite* sprstr = enigma::spritestructarray[sprid];

    sprstr->texturearray[sprstr->subcount] = texture;
    sprstr->subcount++;
    //std::c out << "Added subimage " << sprstr->subcount << " to sprite " << sprid << std::endl;
  }
}
