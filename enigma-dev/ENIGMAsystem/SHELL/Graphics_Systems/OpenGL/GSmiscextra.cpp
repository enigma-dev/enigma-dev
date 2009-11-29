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

#include <GL/gl.h>
#include <string>
#include <stdio.h>

extern int window_get_width();
extern int window_get_height();

int screen_save(std::string filename)
{
  int w=window_get_width(),h=window_get_height();
  int sz=w*h*4; int intout; unsigned char bout;
  char *scrbuf=new char[sz];
  glReadPixels(0,0,w,h,GL_RGBA,GL_UNSIGNED_BYTE,scrbuf);
  char pad=(4-((w*3)&3))&3;
  
  FILE* bmp=fopen(filename.c_str(),"wb");
  if (bmp==NULL) { return -1; }
  
  fwrite("BM",2,1,bmp);
  fwrite(&sz,4,1,bmp); 
  intout=0; fwrite(&intout,2+2,1,bmp);
  
  intout=54; fwrite(&intout,4,1,bmp);
  intout=40; fwrite(&intout,4,1,bmp);
  
  fwrite(&w,4,1,bmp);
  fwrite(&h,4,1,bmp);
  
  bout=1; fwrite(&bout,1,1,bmp);
  bout=0; fwrite(&bout,1,1,bmp);
  bout=24; fwrite(&bout,1,1,bmp);
  bout=0; fwrite(&bout,1,1,bmp);
  
  intout=0; 
  fwrite(&intout,4,1,bmp);
  fwrite(&intout,4,1,bmp);
  fwrite(&intout,4,1,bmp);
  fwrite(&intout,4,1,bmp);
  fwrite(&intout,4,1,bmp);
  fwrite(&intout,4,1,bmp);
  
  w=w*4;
  intout=0;
  for (int i=0;i<sz;i+=w)
  {
   for (int ii=0;ii<w;ii+=4)
    for (int iii=2;iii>=0;iii--)
     fwrite(&(scrbuf[i+ii+iii]),1,1,bmp);
   fwrite(&intout,pad,1,bmp);
  }
  
  fclose(bmp);
  delete[] scrbuf;
  return 0;
}
