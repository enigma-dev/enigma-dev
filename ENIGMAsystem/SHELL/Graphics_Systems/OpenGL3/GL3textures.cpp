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

#include "GL3aux.h" //glExtension_supported
#include "Graphics_Systems/OpenGL/GLtextures_impl.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GStextures_impl.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/graphics_mandatory.h"

#include <stdio.h>

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_texture(tex,texid,v)\
    if ((texid < -1 || size_t(texid) >= enigma::textures.size()) && texid!=-1) {\
      show_error("Attempting to access non-existing texture " + toString(texid), false);\
      return v;\
    }\
    const GLuint tex = (texid==-1?0:((enigma::GLTexture*)enigma::textures[texid])->peer);
#else
  #define get_texture(tex,texid,v)\
    const GLuint tex = (texid==-1?0:((enigma::GLTexture*)enigma::textures[texid])->peer);
#endif

/*enum {
  //Formats and internal formats
  tx_rgba = GL_RGBA,
  tx_rgb = GL_RGB,
  tx_rg = GL_RG,
  tx_red = GL_RED,
  tx_bgra = GL_BGRA,
  tx_bgr =  GL_BGR,
  tx_depth_component = GL_DEPTH_COMPONENT
};

enum {
  //Internal formats only
  tx_rgb32f = GL_RGB32F,
  tx_depth_component32f = GL_DEPTH_COMPONENT32F,
  tx_depth_component24 = GL_DEPTH_COMPONENT24,
  tx_depth_component16 = GL_DEPTH_COMPONENT16,
};

enum {
  //Types
  tx_unsigned_byte = GL_UNSIGNED_BYTE,
  tx_byte = GL_BYTE,
  tx_unsigned_short = GL_UNSIGNED_SHORT,
  tx_short = GL_SHORT,
  tx_unsigned_int = GL_UNSIGNED_INT,
  tx_int = GL_INT,
  tx_float = GL_FLOAT;
};*/

namespace enigma {

GLuint get_texture_peer(int texid) {
  return (size_t(texid) >= textures.size() || texid < 0)
      ? 0 : ((GLTexture*)textures[texid])->peer;
}

//This allows GL3 surfaces to bind and hold many different types of data
int graphics_create_texture_custom(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap, int internalFormat, unsigned format, unsigned type)
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

int graphics_create_texture(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap)
{
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fullwidth, fullheight, 0, GL_BGRA, GL_UNSIGNED_BYTE, pxdata);
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

void graphics_delete_texture(int texid) {
  const GLuint peer = get_texture_peer(texid);
  glDeleteTextures(1, &peer);
}

unsigned char* graphics_copy_texture_pixels(int texture, unsigned* fullwidth, unsigned* fullheight) {
  glBindTexture(GL_TEXTURE_2D, get_texture_peer(texture));

  *fullwidth = textures[texture]->fullwidth;
  *fullheight = textures[texture]->fullheight;

  unsigned char* ret = new unsigned char[((*fullwidth)*(*fullheight)*4)];
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, ret);

  return ret;
}

unsigned char* graphics_copy_texture_pixels(int texture, int x, int y, int width, int height) {
  unsigned fw, fh;
  //We could use glCopyImageSubData here, but it's GL4.3
  unsigned char* pxdata = graphics_copy_texture_pixels(texture, &fw, &fh);
  return pxdata + (x*4) + (y*fw*4);
}

void graphics_push_texture_pixels(int texture, int x, int y, int width, int height, unsigned char* pxdata) {
  glBindTexture(GL_TEXTURE_2D, get_texture_peer(texture));

  glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pxdata);
}

void graphics_push_texture_pixels(int texture, int width, int height, unsigned char* pxdata) {
  glBindTexture(GL_TEXTURE_2D, get_texture_peer(texture));

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pxdata);
}

} // namespace enigma

namespace enigma_user {

void texture_set_priority(int texid, double prio)
{
  draw_batch_flush(batch_flush_deferred);
  // Deprecated in ENIGMA and GM: Studio, all textures are automatically preloaded.
  glBindTexture(GL_TEXTURE_2D, enigma::get_texture_peer(texid));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, prio);
}

bool texture_mipmapping_supported()
{
  return enigma::gl_extension_supported("glGenerateMipmap");
}

bool texture_anisotropy_supported()
{
  return enigma::gl_extension_supported("GL_EXT_texture_filter_anisotropic");
}

float texture_anisotropy_maxlevel()
{
  float maximumAnisotropy;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnisotropy);
  return maximumAnisotropy;
}

} // namespace enigma_user
