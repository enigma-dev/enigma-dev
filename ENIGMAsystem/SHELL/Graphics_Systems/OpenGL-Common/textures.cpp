/** Copyright (C) 2008-2013, Josh Ventura
*** Copyright (C) 2013-2014,2019 Robert B. Colton
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

#include "textures_impl.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GStextures_impl.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/graphics_mandatory.h"

#include <cstring> // for std::memcpy

#include "Platforms/General/PFwindow.h"

namespace enigma {

// Because GLES won't do the conversion for us
unsigned char* bgra2rgba(unsigned char* in, int w, int h) {
  unsigned char* out = new unsigned char[w * h * 4];
  int offset = 0;
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      out[offset] = in[offset + 2];
      out[offset + 1] = in[offset + 1];
      out[offset + 2] = in[offset];
      out[offset + 3] = in[offset + 3];
      offset += 4;
    }
  }
  return out;
}

GLuint get_texture_peer(int texid) {
  return (size_t(texid) >= textures.size() || texid < 0)
      ? 0 : ((GLTexture*)textures[texid])->peer;
}

//This allows GL3 surfaces to bind and hold many different types of data
int graphics_create_texture_custom(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap, GLint internalFormat, GLenum format, GLenum type)
{
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, fullwidth, fullheight, 0, format, type, pxdata);
  if (mipmap) {
    // This allows us to control the number of mipmaps generated, but Direct3D does not have an option for it, so for now we'll just go with the defaults.
    // Honestly not a big deal, Unity3D doesn't allow you to specify either.
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  glBindTexture(GL_TEXTURE_2D, 0);

  GLTexture* textureStruct = new GLTexture(texture);
  textureStruct->width = width;
  textureStruct->height = height;
  textureStruct->fullwidth = fullwidth;
  textureStruct->fullheight = fullheight;
  const int id = textures.size();
  textures.push_back(textureStruct);
  return id;
}

int graphics_create_texture(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap) {
  unsigned char* converted_pxdata = bgra2rgba((unsigned char*)pxdata, width, height);
  int ret = graphics_create_texture_custom(width, height, fullwidth, fullheight, converted_pxdata, mipmap, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
  delete[] converted_pxdata;
  return ret;
}

void graphics_delete_texture(int texid) {
  const GLuint peer = get_texture_peer(texid);
  glDeleteTextures(1, &peer);
}

unsigned char* graphics_copy_texture_pixels(int texture, int x, int y, int width, int height) {
  unsigned fw, fh;
  //We could use glCopyImageSubData here instead of cropping, but it's GL4.3
  unsigned char* pxdata = graphics_copy_texture_pixels(texture, &fw, &fh);
  const int bpp = 4; // bytes per pixel
  const int dp=width*bpp, // destination pitch
            sp=fw*bpp; // source pitch

  unsigned char* cropped = new unsigned char[height*dp];
  for (int i = 0; i < height; ++i) {
    std::memcpy(cropped + i*dp, pxdata + (i+y)*sp + x*bpp, dp);
  }
  delete[] pxdata;

  return cropped;
}

void graphics_push_texture_pixels(int texture, int x, int y, int width, int height, unsigned char* pxdata) {
  glBindTexture(GL_TEXTURE_2D, get_texture_peer(texture));

  glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pxdata);
}

void graphics_push_texture_pixels(int texture, int width, int height, unsigned char* pxdata) {
  glBindTexture(GL_TEXTURE_2D, get_texture_peer(texture));
  unsigned char* converted_pxdata = bgra2rgba(pxdata, width, height);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted_pxdata);
  delete[] converted_pxdata;
}

} // namespace enigma
