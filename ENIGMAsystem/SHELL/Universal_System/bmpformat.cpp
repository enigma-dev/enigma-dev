/*

BSD 2-Clause License

Copyright © 2019, phm97
Copyright © 2021, Samuel Venable
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(_WIN32)
#include "widenarrow.h"
#endif

#define BITMAP_MAGIC_NUMBER 19778


typedef struct bmp_file_header_s bmp_file_header_t;
struct bmp_file_header_s {
/*  int16_t magic_number; */ /* because of padding, we don't put it into the struct */
  int32_t size;
  int32_t app_id;
  int32_t offset;
};

typedef struct bmp_bitmap_info_header_s bmp_bitmap_info_header_t;
struct bmp_bitmap_info_header_s {
  int32_t header_size;
  int32_t width;
  int32_t height;
  int16_t num_planes;
  int16_t bpp;
  int32_t compression;
  int32_t image_size;
  int32_t horizontal_resolution;
  int32_t vertical_resolution;
  int32_t colors_used;
  int32_t colors_important;
};

typedef struct bmp_palette_element_s bmp_palette_element_t;
struct bmp_palette_element_s {
  unsigned char blue;
  unsigned char green;
  unsigned char red;
  unsigned char reserved;  /* alpha ? */
};


static void bmpReadPixelsRLE8(FILE *file, unsigned char *dest, bmp_palette_element_t *palette, int w, int h) {
  unsigned char byte, index, i, *p;
  unsigned char x_ofs, y_ofs;
  char keepreading = 1;
  int current_line = 0;

  
  p = dest;
  while (keepreading) {  /* in each loop, we read a pair of bytes */
    byte = fgetc(file);  /* read the first byte */
    if (byte) {
      index = fgetc(file); /* read the second byte */
      for (i=0; i < byte; i++) {
        *p = palette[index].red; p++;  /* unindex pixels on the fly */
        *p = palette[index].green; p++;
        *p = palette[index].blue; p++;
        *p = 0xFF; p++;          /* add alpha */
      }
    } else {
      byte = fgetc(file);  /* read the second byte */
      switch (byte) 
      {
        case 0 : /* skip the end of the current line and go to the next line */
            current_line++;
            p = dest + current_line*w*4;
        break;
        case 1 : /* stop reading */
            keepreading = 0;
        break;
        case 2 : /* skip y_ofs lines and x_ofs columns. This means that the ignored pixels will be
            filled with black (Or maybe i didn't understand the spec ?). This has already be done :
            before starting to load pixel data, we filled all the dest[] array with 0x00 by a memset() */
            x_ofs = fgetc(file);
            y_ofs = fgetc(file);
            current_line += y_ofs;
            p += y_ofs*w*4 + x_ofs*4;
        break;
        default : /* get the next n pixels, where n = byte */
            for (i=0; i < byte; i++) {
              index = fgetc(file);
              *p = palette[index].red; p++;  /* unindex pixels on the fly */
              *p = palette[index].green; p++;
              *p = palette[index].blue; p++;
              *p = 0xFF; p++;          /* add alpha */
            }
            /* if n is not a multiple of 2, then skip one byte in the file, to respect int16_t alignment */
            if (byte %2) (void)fseek(file, 1, SEEK_CUR);
        break;
      }
    }
    /* the place to which p is pointing is guided by the content of the file. A corrupted file could make p point to a wrong localization.
       Hence, we must check that we don't point outside of the dest[] array. This may prevent an error of segmentation */
    if (p >= dest + w*h*4) keepreading = 0;
  }
}

static void bmpReadPixelsRLE4(FILE *file, unsigned char *dest, bmp_palette_element_t *palette, int w, int h) {
  unsigned char byte1, byte2, index1, index2, *p;
  unsigned char x_ofs, y_ofs;
  unsigned char bitmask = 0x0F; /* bit mask : 00001111 */
  char keepreading = 1;
  int current_line = 0;
  int i;
  
  
  p = dest;
  while (keepreading) {
    byte1 = fgetc(file);
    if (byte1) {  /* encoded mode */
      byte2 = fgetc(file);
      index1 = byte2 >> 4;    /* get the first 4 bits of byte2 */
      index2 = byte2 & bitmask;  /* get the next 4 bits of byte2 */
      for (i=0; i < (byte1 / 2); i++) {
        *p = palette[index1].red; p++;
        *p = palette[index1].green; p++;
        *p = palette[index1].blue; p++;
        *p = 0x0F; p++;
        *p = palette[index2].red; p++;
        *p = palette[index2].green; p++;
        *p = palette[index2].blue; p++;
        *p = 0x0F; p++;
      }
      if (byte1 % 2) {
        *p = palette[index1].red; p++;
        *p = palette[index1].green; p++;
        *p = palette[index1].blue; p++;
        *p = 0x0F; p++;
      }
    } else {  /* absolute mode */
      byte2 = fgetc(file);
      switch (byte2)
      {
        case 0 : /* skip the end of the current line and go to the next line */
            current_line++;
            p = dest + current_line*w*4;
        break;
        case 1 : /* stop reading */
            keepreading = 0;
        break;
        case 2 : /* skip y_ofs lines and x_ofs column */
            x_ofs = fgetc(file);
            y_ofs = fgetc(file);
            current_line += y_ofs;
            p += y_ofs*w*4 + x_ofs*4;
        break;
        default : /* get the next n pixels, where n = byte2 */
          for (i=0; i < (byte2/2); i++) {
            byte1 = fgetc(file);
            index1 = byte1 >> 4;
            *p = palette[index1].red; p++;
            *p = palette[index1].green; p++;
            *p = palette[index1].blue; p++;
            *p = 0x0F; p++;
            index2 = byte1 & bitmask;
            *p = palette[index2].red; p++;
            *p = palette[index2].green; p++;
            *p = palette[index2].blue; p++;
            *p = 0x0F; p++;
          }
          if (byte2 % 2) {
            byte1 = fgetc(file);
            index1 = byte1 >> 4;
            *p = palette[index1].red; p++;
            *p = palette[index1].green; p++;
            *p = palette[index1].blue; p++;
            *p = 0x0F; p++;
          }
          if (((byte2 + 1)/2) % 2) /* int16_t alignment */
            fseek(file, 1, SEEK_CUR);
        break;
      }
    }
    /* the place to which p is pointing is guided by the content of the file. A corrupted file could make p point to a wrong localization.
       Hence, we must check that we don't point outside of the dest[] array. This may prevent an error of segmentation */
    if (p >= dest + w*h*4) keepreading = 0;
  }
}

static void bmpReadPixels32(FILE *file, unsigned char *dest, int w, int h) {
  int i, j;
  unsigned char px[4], *p;
  
  
  p = dest;
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      fread(px, 4, 1, file);
      *p = px[3]; p++; /* convert BGRX to RGBA */
      *p = px[2]; p++;
      *p = px[1]; p++;
      *p = 0xFF; p++;
    }
  }
}

static void bmpReadPixels24(FILE *file, unsigned char *dest, int w, int h) {
  int i, j;
  unsigned char px[3], *p;
  
  p = dest;
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      fread(px, 3, 1, file);
      *p = px[2]; p++;  /* convert BGR to RGBA */
      *p = px[1]; p++;
      *p = px[0]; p++;
      *p = 0xFF;  p++;  /* add alpha component */
    }
    if (w*3 % 4 != 0)
      fseek(file, 4 - (w*3 % 4), SEEK_CUR); /* if the width is not a multiple of 4, skip the end of the line */
  }
}

/* Expected format : XBGR   0 11111 11111 11111 */
static void bmpReadPixels16(FILE *file, unsigned char *dest, int w, int h) {
  uint16_t pxl, r, g, b;
  uint16_t bitmask = 0x1F;
  unsigned char *p;
  int i, j;
  
  
  p = dest;
  for (i=0; i < h; i++) {
    for (j=0; j < w; j++) {
      fread(&pxl, 2, 1, file);
      b = (pxl >> 10) & bitmask;
      g = (pxl >> 5) & bitmask;
      r =  pxl & bitmask;
      *p = r*8; p++; /* fix me */
      *p = g*8; p++;
      *p = b*8; p++;
      *p = 0xFF; p++;
    }
    if ((2*w) % 4 != 0)
      fseek(file, 4 - ((2*w) % 4), SEEK_CUR);
  }
}

static void bmpReadPixels8(FILE *file, unsigned char *dest, bmp_palette_element_t *palette, int w, int h) {
  int i, j;
  unsigned char px, *p;
  
  p = dest;
  for (i=0; i < h; i++) {
    for (j=0; j < w; j++) {
      fread(&px, 1, 1, file);
      *p = palette[px].red; p++;
      *p = palette[px].green; p++;
      *p = palette[px].blue; p++;
      *p = 0xFF; p++;
    }
    if (w % 4 != 0)
      fseek(file, 4 - (w % 4), SEEK_CUR);
  }
}

static void bmpReadPixels4(FILE *file, unsigned char *dest, bmp_palette_element_t *palette, int w, int h) {
  int size = (w + 1)/2;      /* byte alignment */
  unsigned char row_stride[size];  /* not C90 but convenient here */
  unsigned char index, byte, *p;
  unsigned char bitmask = 0x0F;  /* bit mask : 00001111 */
  
  p = dest;
  for (int i=0; i < h; i++) {
    fread(row_stride, size, 1, file);
    for (int j=0; j < h; j++) {
      byte = row_stride[j/2];
      index = (j % 2) ? bitmask&byte : byte>>4;
      *p = palette[index].red; p++;
      *p = palette[index].green; p++;
      *p = palette[index].blue; p++;
      *p = 0xFF; p++;
    }
    if (size % 4 != 0)
      fseek(file, 4 - (size % 4), SEEK_CUR);
  }
}

static void bmpReadPixels1(FILE *file, unsigned char *dest, bmp_palette_element_t *palette, int w, int h) {
  int size = (w + 7) / 8;      /* byte alignment */
  unsigned char row_stride[size];  /* not C90 but convenient here */
  unsigned char index, byte, *p;
  unsigned char bitmask = 0x01;  /* bit mask : 00000001 */
  int bit;
  
  p = dest;
  for (int i=0; i < h; i++) {
    fread(row_stride, size, 1, file);
    for (int j=0; j < w; j++) {
      bit = (j % 8) + 1;
      byte = row_stride[j/8];
      index = byte >> (8-bit);
      index &= bitmask;
      *p = palette[index].red; p++;
      *p = palette[index].green; p++;
      *p = palette[index].blue; p++;
      *p = 0xFF; p++;
    }
    if (size % 4 != 0)
      fseek(file, 4 - (size % 4), SEEK_CUR);
  }
}


int loadBMP(const char *filename, unsigned char **data, int *width, int *height) {
  FILE *file;
  int16_t magic_number;
  bmp_file_header_t file_header;
  bmp_bitmap_info_header_t info_header;
  bmp_palette_element_t *palette = nullptr;
  unsigned char *buf;
  
  #if defined(_WIN32)
  std::wstring wfilename = widen(filename);
  file = _wfopen(wfilename.c_str(), L"rb");
  #else
  file = fopen(filename, "rb");  
  #endif
  if (!file) {
    return 1;
  }
  
  fread(&magic_number, 2, 1, file);
  if (magic_number != BITMAP_MAGIC_NUMBER) {
    fclose(file);
    return 1;
  }
  
  /* read headers */
  fread((void*)&file_header, 12, 1, file);
  fread((void*)&info_header, 40, 1, file);
  
  /* info_header sanity checks */
  /* accepted headers : bitmapinfoheader, bitmapv4header, bitmapv5header */
  if (!(info_header.header_size == 40 || info_header.header_size == 108 || info_header.header_size == 124)) {
    fclose(file);
    return 1;
  }
  if (info_header.num_planes != 1) {
    fclose(file);
    return 1;
  }
  if (info_header.compression == 4 || info_header.compression == 5) {
    fclose(file);
    return 1;
  } 
  if (info_header.height < 0) {
    fclose(file);
    return 1;
  }
  
  /* load palette, if present */
  if (info_header.bpp <= 8) {
    fseek(file, 14 + info_header.header_size, SEEK_SET);
    if ((info_header.bpp == 1) && (info_header.colors_used ==0)) info_header.colors_used = 2;
    if ((info_header.bpp == 4) && (info_header.colors_used ==0)) info_header.colors_used = 16;
    if ((info_header.bpp == 8) && (info_header.colors_used ==0)) info_header.colors_used = 256;
    palette = (bmp_palette_element_t*) malloc(info_header.colors_used * sizeof(bmp_palette_element_t));
    if (!palette) {
      fclose(file);
      return 1;
    } else
      fread((void*)palette, sizeof(bmp_palette_element_t), info_header.colors_used, file);
  }
  
  /* memory allocation */
  buf = (unsigned char *)malloc(info_header.width * info_header.height * 4);
  if (!buf) {
    if (info_header.colors_used) free(palette);
    fclose(file);
    return 1;
  }
  memset(buf, 0x00, info_header.width * info_header.height * 4);
  
  /* load image data */
  fseek(file, file_header.offset, SEEK_SET);
  switch (info_header.bpp)
  {
    case 32 :
        bmpReadPixels32(file, buf, info_header.width, info_header.height);
    break;
    case 24 :
        bmpReadPixels24(file, buf, info_header.width, info_header.height);
    break;
    case 16 : 
        bmpReadPixels16(file, buf, info_header.width, info_header.height);
    break;
    case 8 :
      if (info_header.compression == 1)
        bmpReadPixelsRLE8(file, buf, palette, info_header.width, info_header.height);
      else
        bmpReadPixels8(file, buf, palette, info_header.width, info_header.height);
    break;
    case 4 :
      if (info_header.compression == 2)
        bmpReadPixelsRLE4(file, buf, palette, info_header.width, info_header.height);
      else
        bmpReadPixels4(file, buf, palette, info_header.width, info_header.height);
    break;
    case 1 :
        bmpReadPixels1(file, buf, palette, info_header.width, info_header.height);
    break;
  }
  
  /* picture is ready, export data */
  *width = info_header.width;
  *height = info_header.height;
  *data = buf;
  
  if (info_header.colors_used) free(palette);
  fclose(file);
  
  return 0;
}

int writeBMP(const char* filename, unsigned char *data, int width, int height) {
  FILE *file;
  int16_t magic_number;
  bmp_file_header_t file_header;
  bmp_bitmap_info_header_t info_header;
  unsigned char sample[3], *p;
  int i, j;
  
  #if defined(_WIN32)
  std::wstring wfilename = widen(filename);
  file = _wfopen(wfilename.c_str(), L"wb");
  #else
  file = fopen(filename, "wb");  
  #endif
  if (!file) {
    return 1;
  }
  
  magic_number = BITMAP_MAGIC_NUMBER;
  
  file_header.size = width * height * 4 + 54;
  file_header.app_id = 0;
  file_header.offset = 54;
  
  info_header.header_size = 40;
  info_header.width = width;
  info_header.height = height;
  info_header.num_planes = 1;
  info_header.bpp = 24;
  info_header.compression = 0;
  info_header.image_size = width * height * 4;
  info_header.horizontal_resolution = 0;
  info_header.vertical_resolution = 0;
  info_header.colors_used = 0;
  info_header.colors_important = 0;
  
  fwrite(&magic_number, sizeof(magic_number), 1, file);
  fwrite(&file_header, sizeof(bmp_file_header_t), 1, file);
  fwrite(&info_header, sizeof(bmp_bitmap_info_header_t), 1, file);
  
  p = data;
  for (i=0; i < height; i++) {
    for (j=0; j < width; j++) {
      /* convert RGBA to BGR */
      sample[2] = *p; p++;
      sample[1] = *p; p++;
      sample[0] = *p; p++;
      p++;
      
      fwrite(sample, 3, 1, file);
    }
  }
  
  fclose(file);
  
  return 0;
}
