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

#include <string>
#include <stdio.h>
using namespace std;
#include "IMGloading.h"

inline unsigned int nlpo2dc(unsigned int x){//Next largest power of two minus one
	printf("INPUT: %d\n", x);
  --x;
	x |= x>>1;
	x |= x>>2;
	x |= x>>4;
	x |= x>>8;
  printf("OUTPUT: %d\n", x | (x>>16));
	return x | (x>>16);
}
inline unsigned int lgpp2(unsigned int x){//Trailing zero count. lg for perfect powers of two
	x =  (x & -x) - 1;
	x -= ((x >> 1) & 0x55555555);
	x =  ((x >> 2) & 0x33333333) + (x & 0x33333333);
	x =  ((x >> 4) + x) & 0x0f0f0f0f;
	x += x >> 8;
	return (x + (x >> 16)) & 63;
}
namespace enigma
{
  char* load_bitmap(string filename,int* width,int* height, int* fullwidth, int* fullheight)
  {
    FILE *imgfile;
    int bmpstart,bmpwidth,bmpheight;
    if(!(imgfile=fopen(filename.c_str(),"r"))) return 0;
    fseek(imgfile,0,SEEK_END);
    fseek(imgfile,10,SEEK_SET);
    if (fread(&bmpstart,1,4,imgfile) != 4)
      return NULL;
    fseek(imgfile,18,SEEK_SET);
    if (fread(&bmpwidth,1,4,imgfile) != 4)
      return NULL;
    if (fread(&bmpheight,1,4,imgfile) != 4)
      return NULL;
    
    fseek(imgfile,28,SEEK_SET); // Color depth
    
    if(fgetc(imgfile) != 24) // Only take 24-bit bitmaps for now
      return 0;
    
    int
      wfdc = nlpo2dc(bmpwidth), widfull = wfdc+1, wflgp2 = lgpp2(widfull)+2,
      hgtfull = nlpo2dc(bmpheight),
      ih,iw,tmp=hgtfull++<<wflgp2;
    char* bitmap=new char[(hgtfull<<wflgp2)|2];
    long int pad=bmpwidth & 3; //This is that set of nulls that follows each line
      fseek(imgfile,bmpstart,SEEK_SET);
    
    for (ih = 0; ih < hgtfull-bmpheight; ih++) {
      for (iw = 0; iw < widfull; iw++) {
        bitmap[tmp]=0;
        bitmap[tmp+1]=0;
        bitmap[tmp+2]=0;
        bitmap[tmp+3]=0;
        tmp+=4;
      }
    }
    for(ih=0; ih < bmpheight; ih++)
    {
      for (iw=0; iw < bmpwidth; iw++){
        bitmap[tmp+3] = 255;
        bitmap[tmp+2] = fgetc(imgfile);
        bitmap[tmp+1] = fgetc(imgfile);
        bitmap[tmp]   = fgetc(imgfile);
        tmp+=4;
      }
      for (iw=0; iw < widfull-bmpwidth; iw++){
        bitmap[tmp]   = 0;
        bitmap[tmp+1] = 0;
        bitmap[tmp+2] = 0;
        bitmap[tmp+3] = 0;
        tmp+=4;
      }
      fseek(imgfile,pad,SEEK_CUR);
    }
    fclose(imgfile);
    *width  = bmpwidth;
    *height = bmpheight;
    *fullwidth  = widfull;
    *fullheight = hgtfull;
    return bitmap;
  }
}
