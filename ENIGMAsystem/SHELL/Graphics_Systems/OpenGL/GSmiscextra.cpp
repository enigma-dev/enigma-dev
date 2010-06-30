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

#include <GL/gl.h>
#include <string>
#include <stdio.h>

using namespace std;

//Fuck whoever did this to the spec
#ifndef GL_BGR
  #define GL_BGR 0x80E0
#endif

extern int window_get_width();
extern int window_get_height();

int screen_save(string filename) //Assumes native integers are little endian
{
	unsigned int w=window_get_width(),h=window_get_height(),sz=w*h;
	char *scrbuf=new char[sz*3];
	glReadPixels(0,0,w,h,GL_BGR,GL_UNSIGNED_BYTE,scrbuf);
	FILE *bmp=fopen(filename.c_str(),"wb");
	if(!bmp) return -1;
	fwrite("BM",2,1,bmp);
	sz<<=2;
	
	fwrite(&sz,4,1,bmp);
	fwrite("\0\0\0\0\x36\0\0\0\x28\0\0",12,1,bmp);
	fwrite(&w,4,1,bmp);
	fwrite(&h,4,1,bmp);
	fwrite("\1\0\x18\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",28,1,bmp);
	
	if(w&3)
	{
		size_t pad=w&3;
		w*=3;
		sz-=sz>>2;
		for(unsigned int i=0;i<sz;i+=w)
		{
			fwrite(scrbuf+i,w,1,bmp);
			fwrite("\0\0",pad,1,bmp);
		}
	} else fwrite(scrbuf,w*3,h,bmp);
	fclose(bmp);
	delete[] scrbuf;
	return 0;
}

int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h) //Assumes native integers are little endian
{
	unsigned sz=w*h;
	char *scrbuf=new char[sz*3];
	glReadPixels(x,y,w,h,GL_BGR,GL_UNSIGNED_BYTE,scrbuf);
	FILE *bmp=fopen(filename.c_str(),"wb");
	if(!bmp) return -1;
	fwrite("BM",2,1,bmp);
	
	sz <<= 2;
	fwrite(&sz,4,1,bmp);
	fwrite("\0\0\0\0\x36\0\0\0\x28\0\0",12,1,bmp);
	fwrite(&w,4,1,bmp);
	fwrite(&h,4,1,bmp);
	fwrite("\1\0\x18\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",28,1,bmp);
	
	if(w&3)
	{
		size_t pad=w&3;
		w*=3;
		sz-=sz>>2;
		for(unsigned i=0;i<sz;i+=w)
		{
			fwrite(scrbuf+i,w,1,bmp);
			fwrite("\0\0",pad,1,bmp);
		}
	}
	else fwrite(scrbuf,w*3,h,bmp);
	
	fclose(bmp);
	delete[] scrbuf;
	return 0;
}
