/*

BSD 2-Clause License

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

#include <vector>

#include <cstring>

#include "rgbtorgba.h"

void rgb_to_rgba(unsigned char *src, unsigned char **dst, int w, int h, bool flip) {
  int offset = 0;
  std::vector<unsigned char> pixels(w * h * 4);
  std::vector<unsigned char> data  (w * h * 4);
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      pixels[offset + 0] = src[offset + 0];
      pixels[offset + 1] = src[offset + 1];
      pixels[offset + 2] = src[offset + 2];
      pixels[offset + 3] = 255;
      offset += 4;
    }
  }
  for (int i = 0; i < h; i++) {
    memcpy(&data[i * w * 4 * sizeof(unsigned char)],
    &pixels[(flip ? (h - i - 1) : i) * w * 4 * sizeof(unsigned char)],
    w * 4 * sizeof(unsigned char));  
  }
  offset = 0;
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      (*dst)[offset + 0] = data[offset + 0];
      (*dst)[offset + 1] = data[offset + 1];
      (*dst)[offset + 2] = data[offset + 2];
      (*dst)[offset + 3] = data[offset + 3];
      offset += 4;
    }
  }
}

void copy_rgba(unsigned char *src, unsigned char **dst, int w, int h) {
  int offset = 0;
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      (*dst)[offset + 0] = src[offset + 0];
      (*dst)[offset + 1] = src[offset + 1];
      (*dst)[offset + 2] = src[offset + 2];
      (*dst)[offset + 3] = src[offset + 3];
      offset += 4;
    }
  }
}
