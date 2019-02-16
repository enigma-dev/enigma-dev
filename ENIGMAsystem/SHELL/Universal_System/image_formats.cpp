/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2013 Ssss
*** Copyright (C) 2013-2014 Robert B. Colton
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

#include "image_formats.h"
#include "Universal_System/estring.h"

#include "gif_format.h"

#include <map>
#include <fstream>      // std::ofstream
#include <algorithm>
#include <string>
#include <cstring>

#include <stdlib.h>
using namespace std;

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

std::map<std::string, ImageLoadFunction> image_load_handlers;
std::map<std::string, ImageSaveFunction> image_save_handlers;

void image_add_loader(std::string format, ImageLoadFunction fnc) {
  image_load_handlers[format] = fnc;
}

void image_add_saver(std::string format, ImageSaveFunction fnc) {
  image_save_handlers[format] = fnc;
}

unsigned char* image_flip(const unsigned char* data, unsigned width, unsigned height, unsigned bytes) {
  //flipped upside down
  unsigned sz = width * height;
  unsigned char* rgbdata = new unsigned char[sz * bytes];
  for (unsigned int i = 0; i < height; i++) { // Doesn't matter the order now
    memcpy(&rgbdata[i*width*bytes*sizeof(unsigned char)],           // address of destination
           &data[(height-i-1)*width*bytes*sizeof(unsigned char)],   // address of source
           width*bytes*sizeof(unsigned char) );                     // number of bytes to copy
  }

  return rgbdata;
}

string image_get_format(string filename) {
  size_t fp = filename.find_last_of(".");
  if (fp == string::npos){
    return "";
  }
  string ext = filename.substr(fp);
  transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
  return ext;
}

/// Generic all-purpose image loading call.
unsigned char* image_load(string filename, string format, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight, int* imgnumb, bool flipped) {
  if (format.compare(".bmp") == 0) {
    return image_load_bmp(filename, width, height, fullwidth, fullheight, flipped);
  } else if (format.compare(".gif") == 0) {
    return image_load_gif(filename, width, height, fullwidth, fullheight, imgnumb, flipped);
  }
  auto handler = image_load_handlers.find(format);
  if (handler != image_load_handlers.end()) {
    return (*handler).second(filename, width, height, fullwidth, fullheight, flipped);
  }
  return image_load_bmp(filename, width, height, fullwidth, fullheight, flipped);
}


/// Generic all-purpose image loading call that will regexp the filename for the format and call the appropriate function.
unsigned char* image_load(string filename, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight, int* imgnumb, bool flipped) {
  string format = image_get_format(filename);
  if (format.empty()) {
    format = ".bmp";
  }
  return image_load(filename, format, width, height, fullwidth, fullheight, imgnumb, flipped);
}

/// Generic all-purpose image saving call.
int image_save(string filename, const unsigned char* data, string format, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped) {
  if (format.compare(".bmp") == 0) {
    return image_save_bmp(filename, data, width, height, fullwidth, fullheight, flipped);
  }
  auto handler = image_save_handlers.find(format);
  if (handler != image_save_handlers.end()) {
    return (*handler).second(filename, data, width, height, fullwidth, fullheight, flipped);
  }
  return image_save_bmp(filename, data, width, height, fullwidth, fullheight, flipped);
}

/// Generic all-purpose image saving call that will regexp the filename for the format and call the appropriate function.
int image_save(string filename, const unsigned char* data, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped) {
  string format = image_get_format(filename);
  if (format.empty()) {
    format = ".bmp";
  }
  return image_save(filename, data, format, width, height, fullwidth, fullheight, flipped);
}

unsigned char* image_load_bmp(string filename, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight, bool flipped) {
  FILE *imgfile;
  unsigned bmpstart,bmpwidth,bmpheight;
  if(!(imgfile=fopen(filename.c_str(),"rb"))) return 0;
  fseek(imgfile,0,SEEK_END);
  fseek(imgfile,0,SEEK_SET);
  if (fgetc(imgfile)!=0x42 && fgetc(imgfile)!=0x4D) // Not a BMP
  {
    fclose(imgfile);
    return NULL;
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

  unsigned
    widfull = nlpo2dc(bmpwidth) + 1,
    hgtfull = nlpo2dc(bmpheight) + 1,
    ih,iw;
  const unsigned bitmap_size = widfull*hgtfull*4;
  unsigned char* bitmap = new unsigned char[bitmap_size](); // Initialize to zero.
  long int pad=bmpwidth & 3; //This is that set of nulls that follows each line
  fseek(imgfile,bmpstart,SEEK_SET);

  for (ih = 0; ih < bmpheight; ih++)
  {
    unsigned tmp = 0;
    if (flipped) {
      tmp = ih*widfull*4;
    } else {
      tmp = (bmpheight - 1 - ih)*widfull*4;
    }
    for (iw = 0; iw < bmpwidth; iw++) {
      if (bitdepth == 24)
      {
        bitmap[tmp+0] = fgetc(imgfile);
        bitmap[tmp+1] = fgetc(imgfile);
        bitmap[tmp+2] = fgetc(imgfile);
        bitmap[tmp+3] = (char)0xFF;
      }
      if (bitdepth == 32)
      {
        if (bgramask) //BGRA
        {
          bitmap[tmp+0] = fgetc(imgfile);
          bitmap[tmp+1] = fgetc(imgfile);
         bitmap[tmp+2] = fgetc(imgfile);
          bitmap[tmp+3] = fgetc(imgfile);
        }
        else //ABGR
        {
          bitmap[tmp+3] = fgetc(imgfile);
          bitmap[tmp+0] = fgetc(imgfile);
          bitmap[tmp+1] = fgetc(imgfile);
          bitmap[tmp+2]   = fgetc(imgfile);
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

unsigned char* image_load_gif(string filename, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight, int* imgnumb, bool flipped) {
  unsigned int error = 0;
  unsigned char* image = 0;

  error = load_gif_file(filename.c_str(), image, *width, *height, *fullwidth, *fullheight, *imgnumb);
  if (error) {
    printf("error %u: %s\n", error, load_gif_error_text(error));
    return NULL;
  }

  return image;
}

int image_save_bmp(string filename, const unsigned char* data, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped) {
  unsigned sz = width * height;
  FILE *bmp = fopen(filename.c_str(), "wb");
  if (!bmp) return -1;
  fwrite("BM", 2, 1, bmp);

  sz <<= 2;
  fwrite(&sz,4,1,bmp);
  fwrite("\0\0\0\0\x36\0\0\0\x28\0\0",12,1,bmp);
  fwrite(&width,4,1,bmp);
  fwrite(&height,4,1,bmp);
  //NOTE: x20 = 32bit full color, x18 = 24bit no alpha
  fwrite("\1\0\x20\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",28,1,bmp);

  unsigned bytes = 4;

  width *= bytes;
  fullwidth *= bytes;
  unsigned lastbyte = fullwidth * height;

  for (unsigned i = 0; i < lastbyte; i += fullwidth) {
    unsigned tmp = i;
    if (!flipped) {
      tmp = lastbyte - i;
    }
    for (unsigned ii = 0; ii < width; ii += bytes) {
      fwrite(&data[tmp + ii + 0],sizeof(char),1,bmp);
      fwrite(&data[tmp + ii + 1],sizeof(char),1,bmp);
      fwrite(&data[tmp + ii + 2],sizeof(char),1,bmp);
      fwrite(&data[tmp + ii + 3],sizeof(char),1,bmp);
    }
  }

  fclose(bmp);
  return 0;
}

}
