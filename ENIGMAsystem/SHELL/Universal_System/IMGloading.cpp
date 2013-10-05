/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2013 Ssss
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
#include <cstring>
#include "lodepng.h"
#include <stdlib.h>
using namespace std;
#include "IMGloading.h"

#include "nlpo2.h"
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
  char* load_png(string filename,int* width,int* height, int* fullwidth, int* fullheight)
  {
    unsigned error;
    unsigned char* image;
    unsigned bmpwidth, bmpheight;

    error = lodepng_decode32_file(&image, &bmpwidth, &bmpheight, filename.c_str());
    if(error)
    {
      printf("error %u: %s\n", error, lodepng_error_text(error));
      return NULL;
    }

    unsigned
      widfull = nlpo2dc(bmpwidth) + 1,
      hgtfull = nlpo2dc(bmpheight) + 1,
      ih,iw;
    const int bitmap_size = widfull*hgtfull*4;
    char* bitmap=new char[bitmap_size](); // Initialize to zero.

    for(ih = 0; ih <= bmpheight - 1; ih++)
    {
      int tmp = ih*widfull*4;
      for (iw=0; iw < bmpwidth; iw++){
                bitmap[tmp+3] = image[4*bmpwidth*ih+iw*4+3];
                bitmap[tmp+2] = image[4*bmpwidth*ih+iw*4+2];
                bitmap[tmp+1] = image[4*bmpwidth*ih+iw*4+1];
                bitmap[tmp]   = image[4*bmpwidth*ih+iw*4];
        tmp+=4;
      }
    }

    free(image);
    *width  = bmpwidth;
    *height = bmpheight;
    *fullwidth  = widfull;
    *fullheight = hgtfull;
    return bitmap;
  }

  char* load_bitmap(string filename, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight)
  {
    FILE *imgfile;
    int bmpstart,bmpwidth,bmpheight;
    if(!(imgfile=fopen(filename.c_str(),"rb"))) return 0;
    fseek(imgfile,0,SEEK_END);
    fseek(imgfile,0,SEEK_SET);
    if (fgetc(imgfile)!=0x42 && fgetc(imgfile)!=0x4D) // Not a BMP
    {
      fclose(imgfile);
      return load_png(filename,(int*)width,(int*)height,(int*)fullwidth,(int*)fullheight);
    }
    fseek(imgfile,10,SEEK_SET);
    if (fread(&bmpstart,1,4,imgfile) != 4)
      return NULL;
    fseek(imgfile,18,SEEK_SET);
    if (fread(&bmpwidth,1,4,imgfile) != 4)
      return NULL;
    if (fread(&bmpheight,1,4,imgfile) != 4)
      return NULL;

    fseek(imgfile,28,SEEK_SET); // Color depth

    // Only take 24 or 32-bit bitmaps for now
    int bitdepth=fgetc(imgfile);
    if(bitdepth != 24 && bitdepth != 32)
      return 0;

    fseek(imgfile,69,SEEK_SET); // Alpha in last byte
    int bgramask=fgetc(imgfile);

    int
      widfull = nlpo2dc(bmpwidth) + 1,
      hgtfull = nlpo2dc(bmpheight) + 1,
      ih,iw;
    const int bitmap_size = widfull*hgtfull*4;
    char* bitmap=new char[bitmap_size](); // Initialize to zero.
    long int pad=bmpwidth & 3; //This is that set of nulls that follows each line
      fseek(imgfile,bmpstart,SEEK_SET);

    for(ih = bmpheight - 1; ih >= 0; ih--)
    {
      int tmp = ih*widfull*4;
      for (iw=0; iw < bmpwidth; iw++){
        if(bitdepth == 24)
        {
                bitmap[tmp+3] = (char)0xFF;
                bitmap[tmp+2] = fgetc(imgfile);
                bitmap[tmp+1] = fgetc(imgfile);
                bitmap[tmp]   = fgetc(imgfile);
        }
        if(bitdepth == 32)
        {
                if (bgramask) //BGRA
                {
                        bitmap[tmp+2] = fgetc(imgfile);
                        bitmap[tmp+1] = fgetc(imgfile);
                        bitmap[tmp] = fgetc(imgfile);
                        bitmap[tmp+3]   = fgetc(imgfile);
                }
                else //ABGR
                {
                        bitmap[tmp+3] = fgetc(imgfile);
                        bitmap[tmp+2] = fgetc(imgfile);
                        bitmap[tmp+1] = fgetc(imgfile);
                        bitmap[tmp]   = fgetc(imgfile);
                }
        }
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
