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

#include "GLSamplerState.h"
#include "Graphics_Systems/OpenGL/GLtextures_impl.h"
#include "Graphics_Systems/OpenGL/OpenGLHeaders.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GStextures_impl.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/graphics_mandatory.h"

#include "Universal_System/image_formats.h"
#include "Universal_System/background_internal.h"
#include "Universal_System/sprites_internal.h"

#include <stdio.h>
#include <string.h>

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

namespace {

inline unsigned int lgpp2(unsigned int x){//Trailing zero count. lg for perfect powers of two
  x =  (x & -x) - 1;
  x -= ((x >> 1) & 0x55555555);
  x =  ((x >> 2) & 0x33333333) + (x & 0x33333333);
  x =  ((x >> 4) + x) & 0x0f0f0f0f;
  x += x >> 8;
  return (x + (x >> 16)) & 63;
}

enigma::Sampler samplers[8];

// use this helper to change the samplers state so that
// the texture parameter is set to the texture bound
// on the same stage as the sampler
enigma::Sampler* get_sampler(int id) {
  glActiveTexture(GL_TEXTURE0 + id);
  return &samplers[id];
}

} // namespace anonymous

namespace enigma {

GLuint get_texture_peer(int texid) {
  return (size_t(texid) >= textures.size() || texid < 0)
      ? 0 : ((GLTexture*)textures[texid])->peer;
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

  int graphics_duplicate_texture(int tex, bool mipmap)
  {
    GLuint texture = get_texture_peer(tex);
    glBindTexture(GL_TEXTURE_2D, texture);
    unsigned w, h, fw, fh;
    w = textures[tex]->width;
    h = textures[tex]->height;
    fw = textures[tex]->fullwidth;
    fh = textures[tex]->fullheight;
    char* bitmap = new char[(fh<<(lgpp2(fw)+2))|2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap);
    unsigned dup_tex = graphics_create_texture(w, h, fw, fh, bitmap, mipmap);
    delete[] bitmap;
    glBindTexture(GL_TEXTURE_2D, 0);

    return dup_tex;
  }

  void graphics_copy_texture(int source, int destination, int x, int y)
  {
    GLuint src = get_texture_peer(source);
    GLuint dst = get_texture_peer(destination);
    unsigned int sw, sh, sfw, sfh;
    sw = textures[source]->width;
    sh = textures[source]->height;
    sfw = textures[source]->fullwidth;
    sfh = textures[source]->fullheight;
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
    dw = textures[destination]->width;
    dh = textures[destination]->height;
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, (x+sw<=dw?sw:dw-x), (y+sh<=dh?sh:dh-y), GL_BGRA, GL_UNSIGNED_BYTE, cropped_bitmap);
    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] bitmap;
    delete[] cropped_bitmap;
  }

  void graphics_copy_texture_part(int source, int destination, int xoff, int yoff, int w, int h, int x, int y)
  {
    GLuint src = get_texture_peer(source);
    GLuint dst = get_texture_peer(destination);
    unsigned int sw, sh, sfw, sfh;
    sw = w;
    sh = h;
    sfw = textures[source]->fullwidth;
    sfh = textures[source]->fullheight;
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
    dw = textures[destination]->width;
    dh = textures[destination]->height;
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, (x+sw<=dw?sw:dw-x), (y+sh<=dh?sh:dh-y), GL_BGRA, GL_UNSIGNED_BYTE, cropped_bitmap);
    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] bitmap;
    delete[] cropped_bitmap;
  }

  void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex)
  {
    GLuint texture = get_texture_peer(tex);
    GLuint copy_texture = get_texture_peer(copy_tex);

    unsigned fw, fh, size;
    glBindTexture(GL_TEXTURE_2D, texture);
    fw = textures[tex]->fullwidth;
    fh = textures[tex]->fullheight;
    size = (fh<<(lgpp2(fw)+2))|2;
    char* bitmap = new char[size];
    char* bitmap2 = new char[size];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap);
    glBindTexture(GL_TEXTURE_2D, copy_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap2);

    for (unsigned int i = 3; i < size; i += 4)
        bitmap[i] = (bitmap2[i-3] + bitmap2[i-2] + bitmap2[i-1])/3;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, fw, fh, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap);
    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] bitmap;
    delete[] bitmap2;
    glPopAttrib();
  }

  void graphics_delete_texture(int texid)
  {
    const GLuint peer = get_texture_peer(texid);
    glDeleteTextures(1, &peer);
  }

  unsigned char* graphics_get_texture_pixeldata(unsigned texture, unsigned* fullwidth, unsigned* fullheight)
  {
    enigma_user::texture_set(texture);

    *fullwidth = textures[texture]->fullwidth;
    *fullheight = textures[texture]->fullheight;

    unsigned char* ret = new unsigned char[((*fullwidth)*(*fullheight)*4)];
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, ret);

    return ret;
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

void texture_set_stage(int stage, int texid) {
  draw_batch_flush(batch_flush_deferred);
  if (texid == -1) { texture_reset(); return; }
  auto samplerState = &samplers[stage];
  if (samplerState->bound_texture != texid) {
    glActiveTexture(GL_TEXTURE0 + stage);
    samplerState->bound_texture = texid;
    glBindTexture(GL_TEXTURE_2D, enigma::get_texture_peer(texid));
    // synchronize the new texture with the current state of the sampler
    samplerState->ApplyState();
  }
}

void texture_reset() {
  draw_batch_flush(batch_flush_deferred);
  glActiveTexture(GL_TEXTURE0);
  samplers[0].bound_texture = -1;
  glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_set_interpolation_ext(int sampler, bool enable)
{
  draw_batch_flush(batch_flush_deferred);
  auto samplerState = get_sampler(sampler);
  samplerState->min = enable?GL_LINEAR:GL_NEAREST;
  samplerState->mag = enable?GL_LINEAR:GL_NEAREST;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplerState->min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplerState->mag);
}

void texture_set_repeat_ext(int sampler, bool repeat)
{
  draw_batch_flush(batch_flush_deferred);
  auto samplerState = get_sampler(sampler);
  samplerState->wrapu = repeat; samplerState->wrapv = repeat; samplerState->wrapw = repeat;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, repeat?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat?GL_REPEAT:GL_CLAMP);
}

void texture_set_wrap_ext(int sampler, bool wrapu, bool wrapv, bool wrapw)
{
  draw_batch_flush(batch_flush_deferred);
  auto samplerState = get_sampler(sampler);
  samplerState->wrapu = wrapu; samplerState->wrapv = wrapv; samplerState->wrapw = wrapw;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapu?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapv?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapw?GL_REPEAT:GL_CLAMP);
}

void texture_set_border_ext(int sampler, int r, int g, int b, double a)
{
  draw_batch_flush(batch_flush_deferred);
  auto samplerState = get_sampler(sampler);
  samplerState->bordercolor[0] = r;
  samplerState->bordercolor[1] = g;
  samplerState->bordercolor[2] = b;
  samplerState->bordercolor[3] = int(a);
  glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, samplerState->bordercolor);
}

void texture_set_filter_ext(int sampler, int filter)
{
  draw_batch_flush(batch_flush_deferred);
  auto samplerState = get_sampler(sampler);
  if (filter == tx_trilinear) {
    samplerState->min = GL_LINEAR_MIPMAP_LINEAR;
    samplerState->mag = GL_LINEAR;
  } else if (filter == tx_bilinear) {
    samplerState->min = GL_LINEAR_MIPMAP_NEAREST;
    samplerState->mag = GL_LINEAR;
  } else if (filter == tx_nearest) {
    samplerState->min = GL_NEAREST_MIPMAP_NEAREST;
    samplerState->mag = GL_NEAREST;
  } else {
    samplerState->min = GL_NEAREST;
    samplerState->mag = GL_NEAREST;
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplerState->min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplerState->mag);
}

void texture_set_lod_ext(int sampler, double minlod, double maxlod, int maxlevel)
{
  draw_batch_flush(batch_flush_deferred);
  auto samplerState = get_sampler(sampler);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, samplerState->minlod = minlod);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, samplerState->maxlod = maxlod);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, samplerState->maxlevel = maxlevel);
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

void texture_anisotropy_filter(int sampler, gs_scalar levels)
{
  draw_batch_flush(batch_flush_deferred);
  samplers[sampler].anisotropy = levels;
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, levels);
}

}
