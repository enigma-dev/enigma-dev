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

/**Sprite adding functions*****************************************************\

int sprite_add(std::string filename,double imgnumb,double precise,double transparent,double smooth,double preload,double x_offset,double y_offset)

\******************************************************************************/

#include <string>
#include <cstring>
#include <GL/gl.h>
#include "../../Universal_System/IMGloading.h"
#include "../../Universal_System/spritestruct.h"

int show_error(std::string errortext,int fatal); //TODO: Include this from somewhere

int sprite_add(std::string filename,double imgnumb,double precise,double transparent,double smooth,double preload,double x_offset,double y_offset)
{
	GLuint texture;
	int width,height,fullwidth,fullheight;
	char *pxdata=enigma::load_bitmap(filename,&width,&height,&fullwidth,&fullheight);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		(int)smooth,
		4,
		fullwidth,
		fullheight,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pxdata);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	//gluBuild2DMipmaps( GL_TEXTURE_2D, 3, bmpwidth, bmpheight, GL_RGB, GL_UNSIGNED_BYTE, readbuffer);
	//Free buffer
	delete[] pxdata;
	//enigma::spritestructarray[enigma::currentspriteind].pixeldata=(void**) malloc(sizeof(void*));
	//enigma::spritestructarray[enigma::currentspriteind].pixeldata[0]=bitmapbuffer;
	enigma::spritestructarray[enigma::currentspriteind]=new enigma::sprite;
	enigma::spritestructarray[enigma::currentspriteind]->id=enigma::currentspriteind;
	enigma::spritestructarray[enigma::currentspriteind]->subcount=1;
	enigma::spritestructarray[enigma::currentspriteind]->width=width;
	enigma::spritestructarray[enigma::currentspriteind]->height=height;
	enigma::spritestructarray[enigma::currentspriteind]->xoffset=(int)x_offset;
	enigma::spritestructarray[enigma::currentspriteind]->yoffset=(int)y_offset;
	enigma::spritestructarray[enigma::currentspriteind]->texbordx=(double) width/fullwidth;
	enigma::spritestructarray[enigma::currentspriteind]->texbordy=(double) height/fullheight;
	enigma::spritestructarray[enigma::currentspriteind]->texturearray[0]=texture;
	return enigma::currentspriteind++;
}



/* These functions are primarily for use of the engine. Experienced users
 * who are familiar with C++ can make use of these, but they were made to
 * use at load time with data read from the executable. These both expect
 * RAW format, RGB only.
 */
inline unsigned int nlpo2dc(unsigned int x) //Next largest power of two minus one
{
	x |= x>>1;
	x |= x>>2;
	x |= x>>4;
	x |= x>>8;
	return x | (x>>16);
}
namespace enigma
{
  //Adds an empty sprite to the list
  int new_sprexe(int sprid, int w, int h, int x, int y, int pl, int sm)
  {
    int fullwidth=nlpo2dc(w)+1,fullheight=nlpo2dc(h)+1;
    enigma::spritestructarray[sprid]=new enigma::sprite;
    
    enigma::spritestructarray[sprid]->id=sprid;
    enigma::spritestructarray[sprid]->subcount=0;
    enigma::spritestructarray[sprid]->width=w;
    enigma::spritestructarray[sprid]->height=h;
    enigma::spritestructarray[sprid]->xoffset=x;
    enigma::spritestructarray[sprid]->yoffset=y;
    enigma::spritestructarray[sprid]->texbordx=(double)w/fullwidth;
    enigma::spritestructarray[sprid]->texbordy=(double)h/fullheight;
    
    return enigma::currentspriteind=sprid+1;
  }

  #if COLLIGMA
  collCustom* generate_bitmask(unsigned char* pixdata,int x,int y,int w,int h)
  {
    collCustom* thenewmask = new collCustom(w,h,x,y,x,y);
    for(int xp=0;xp<w*h;xp++)
      collbitSet(*thenewmask,xp % w,xp/w,pixdata[xp*4+3]>0); //the width times the number of rows, the current column*4, then 3
    return thenewmask;
  }
  #endif


  //Adds a subimage to an existing sprite from the exe
  void sprexe(int sprid, int x,int y, unsigned int w,unsigned int h,unsigned char*chunk)
  {
    GLuint texture;
    unsigned int fullwidth=nlpo2dc(w)+1,fullheight=nlpo2dc(h);
    GLbyte *imgpxdata=new GLbyte[4*fullwidth*fullheight+1],*imgpxptr=imgpxdata;
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
    
    #if COLLIGMA
    collCustom* themask=generate_bitmask(imgpxdata,x,y,w,h);
    #endif
    
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexImage2D(GL_TEXTURE_2D,0,4,fullwidth,fullheight,0,GL_RGBA,GL_UNSIGNED_BYTE,imgpxdata);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    //gluBuild2DMipmaps( GL_TEXTURE_2D, 3, bmpwidth, bmpheight, GL_RGB, GL_UNSIGNED_BYTE, readbuffer);
    glBindTexture(GL_TEXTURE_2D,0);
    delete[] imgpxdata;
    
    enigma::sprite* sprstr = enigma::spritestructarray[sprid];
    if(!sprstr)
      show_error("THE WORLD CAN NO LONGER BE TRUSTED! HEAD FOR THE HILLS!",1);
    
    sprstr->texturearray[sprstr->subcount]=texture;
    #if COLLIGMA
    sprstr->bitmask[enigma::spritestructarray[sprid]->subcount] = themask;
    #endif
    sprstr->subcount++;
  }
}
