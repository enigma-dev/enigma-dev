/** Copyright (C) 2008-2013, Josh Ventura
*** Copyright (C) 2013-2014, Robert B. Colton
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

#include "GLTextureStruct.h"
#include "Graphics_Systems/General/OpenGLHeaders.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GSprimitives.h"

#include "Universal_System/image_formats.h"

#include <stdio.h>
#include <string.h>

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

vector<TextureStruct*> textureStructs(0);

TextureStruct::TextureStruct(GLuint gtex)
{
  gltex = gtex;
}

TextureStruct::~TextureStruct()
{
  glDeleteTextures(1, &gltex);
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
  GLuint get_texture(int texid) {
    return (size_t(texid) >= textureStructs.size() || texid < 0)
        ? 0 : textureStructs[texid]->gltex;
  }

  int graphics_create_texture(unsigned width, unsigned height,
      unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, fullwidth, fullheight, 0, GL_BGRA, GL_UNSIGNED_BYTE, pxdata);
    if (mipmap) {
      // This allows us to control the number of mipmaps generated, but Direct3D does not have an option for it, so for now we'll just go with the defaults.
      // Honestly not a big deal, Unity3D doesn't allow you to specify either.
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    TextureStruct* textureStruct = new TextureStruct(texture);
    textureStruct->width = width;
    textureStruct->height = height;
    textureStruct->fullwidth = fullwidth;
    textureStruct->fullheight = fullheight;
    textureStructs.push_back(textureStruct);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureStructs.size()-1;
  }

  int graphics_duplicate_texture(int tex, bool mipmap)
  {
    GLuint texture = textureStructs[tex]->gltex;
    glBindTexture(GL_TEXTURE_2D, texture);
    unsigned w, h, fw, fh;
    w = textureStructs[tex]->width;
    h = textureStructs[tex]->height;
    fw = textureStructs[tex]->fullwidth;
    fh = textureStructs[tex]->fullheight;
    char* bitmap = new char[(fh<<(lgpp2(fw)+2))|2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap);
    unsigned dup_tex = graphics_create_texture(w, h, fw, fh, bitmap, mipmap);
    delete[] bitmap;

    glBindTexture(GL_TEXTURE_2D, 0);

    return dup_tex;
  }

  void graphics_copy_texture(int source, int destination, int x, int y)
  {
    GLuint src = textureStructs[source]->gltex;
    GLuint dst = textureStructs[destination]->gltex;
    unsigned int sw, sh, sfw, sfh;
    sw = textureStructs[source]->width;
    sh = textureStructs[source]->height;
    sfw = textureStructs[source]->fullwidth;
    sfh = textureStructs[source]->fullheight;
    glBindTexture(GL_TEXTURE_2D, src);
    //We could use glCopyImageSubData here, but it's GL4.3
    char* bitmap = new char[(sfh<<(lgpp2(sfw)+2))|2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap);

    char* cropped_bitmap = new char[sw*sh*4];
    for (unsigned int i=0; i<sh; ++i){
      memcpy(cropped_bitmap+sw*i*4, bitmap+sfw*i*4, sw*4);
    }

    glBindTexture(GL_TEXTURE_2D, dst);
    unsigned dw, dh;
    dw = textureStructs[destination]->width;
    dh = textureStructs[destination]->height;
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, (x+sw<=dw?sw:dw-x), (y+sh<=dh?sh:dh-y), GL_BGRA, GL_UNSIGNED_BYTE, cropped_bitmap);

    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] bitmap;
    delete[] cropped_bitmap;
  }

  void graphics_copy_texture_part(int source, int destination, int xoff, int yoff, int w, int h, int x, int y)
  {
    GLuint src = textureStructs[source]->gltex;
    GLuint dst = textureStructs[destination]->gltex;
    unsigned int sw, sh, sfw, sfh;
    sw = w;
    sh = h;
    sfw = textureStructs[source]->fullwidth;
    sfh = textureStructs[source]->fullheight;
    glBindTexture(GL_TEXTURE_2D, src);
    //We could use glCopyImageSubData here, but it's GL4.3
    char* bitmap = new char[(sfh<<(lgpp2(sfw)+2))|2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap);

    if (xoff+sw>sfw) sw = sfw-xoff;
    if (yoff+sh>sfh) sh = sfh-yoff;
    char* cropped_bitmap = new char[sw*sh*4];
    for (unsigned int i=0; i<sh; ++i){
      memcpy(cropped_bitmap+sw*i*4, bitmap+xoff*4+sfw*(i+yoff)*4, sw*4);
    }

    glBindTexture(GL_TEXTURE_2D, dst);
    unsigned dw, dh;
    dw = textureStructs[destination]->width;
    dh = textureStructs[destination]->height;
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, (x+sw<=dw?sw:dw-x), (y+sh<=dh?sh:dh-y), GL_BGRA, GL_UNSIGNED_BYTE, cropped_bitmap);

    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] bitmap;
    delete[] cropped_bitmap;
  }

  void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex)
  {
    GLuint texture = textureStructs[tex]->gltex;
    GLuint copy_texture = textureStructs[copy_tex]->gltex;

    unsigned fw, fh, size;
    glBindTexture(GL_TEXTURE_2D, texture);
    fw = textureStructs[tex]->fullwidth;
    fh = textureStructs[tex]->fullheight;
    size = (fh<<(lgpp2(fw)+2))|2;
    char* bitmap = new char[size];
    char* bitmap2 = new char[size];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap);
    glBindTexture(GL_TEXTURE_2D, copy_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap2);

    for (unsigned i = 3; i < size; i += 4) {
      bitmap[i] = (bitmap2[i-3] + bitmap2[i-2] + bitmap2[i-1])/3;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, fw, fh, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap);
    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] bitmap;
    delete[] bitmap2;
    glPopAttrib();
  }

  void graphics_delete_texture(int texid)
  {
    delete textureStructs[texid];
  }

  unsigned char* graphics_get_texture_pixeldata(unsigned texture, unsigned* fullwidth, unsigned* fullheight)
  {
    enigma_user::texture_set(texture);

    *fullwidth = textureStructs[texture]->fullwidth;
    *fullheight = textureStructs[texture]->fullheight;

    unsigned char* ret = new unsigned char[((*fullwidth)*(*fullheight)*4)];
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, ret);

    return ret;
  }
}

namespace enigma_user {

void texture_delete(int texid) {
  delete textureStructs[texid];
}

bool texture_exists(int texid) {
  return textureStructs[texid] != NULL;
}

void texture_preload(int texid)
{
  // Deprecated in ENIGMA and GM: Studio, all textures are automatically preloaded.
}

void texture_set_priority(int texid, double prio)
{
  draw_batch_flush(batch_flush_deferred);
  // Deprecated in ENIGMA and GM: Studio, all textures are automatically preloaded.
  glBindTexture(GL_TEXTURE_2D, textureStructs[texid]->gltex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, prio);
}

void texture_set_enabled(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
  (enable?glEnable:glDisable)(GL_TEXTURE_2D);
}

void texture_set_blending(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
  (enable?glEnable:glDisable)(GL_BLEND);
}

gs_scalar texture_get_width(int texid) {
  return textureStructs[texid]->width / textureStructs[texid]->fullwidth;
}

gs_scalar texture_get_height(int texid)
{
  return textureStructs[texid]->height / textureStructs[texid]->fullheight;
}

gs_scalar texture_get_texel_width(int texid)
{
  return 1.0/textureStructs[texid]->width;
}

gs_scalar texture_get_texel_height(int texid)
{
  return 1.0/textureStructs[texid]->height;
}

bool texture_mipmapping_supported()
{
  return strstr((char*)glGetString(GL_EXTENSIONS),
           "glGenerateMipmap");
}

bool texture_anisotropy_supported()
{
  return strstr((char*)glGetString(GL_EXTENSIONS),
           "GL_EXT_texture_filter_anisotropic");
}

float texture_anisotropy_maxlevel()
{
  float maximumAnisotropy;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnisotropy);
  return maximumAnisotropy;
}

}
