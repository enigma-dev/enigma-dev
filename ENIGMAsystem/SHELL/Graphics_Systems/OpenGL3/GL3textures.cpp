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

#include <stdio.h>
#include "../General/OpenGLHeaders.h"
#include <string.h>
#include "../General/GStextures.h"
#include "GL3TextureStruct.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/backgroundstruct.h"
#include "Universal_System/spritestruct.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Bridges/General/GL3Context.h"
#include "GL3aux.h" //glExtension_supported

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

vector<TextureStruct*> textureStructs(0);

#include <vector>
using std::vector;

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_texture(tex,texid,v)\
    if ((texid < -1 || size_t(texid) >= textureStructs.size()) && texid!=-1) {\
      show_error("Attempting to access non-existing texture " + toString(texid), false);\
      return v;\
    }\
    const int tex = (texid==-1?-1:textureStructs[texid]->gltex);
#else
  #define get_texture(tex,texid,v)\
    const int tex = (texid==-1?-1:textureStructs[texid]->gltex);
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

inline unsigned int lgpp2(unsigned int x){//Trailing zero count. lg for perfect powers of two
	x =  (x & -x) - 1;
	x -= ((x >> 1) & 0x55555555);
	x =  ((x >> 2) & 0x33333333) + (x & 0x33333333);
	x =  ((x >> 4) + x) & 0x0f0f0f0f;
	x += x >> 8;
	return (x + (x >> 16)) & 63;
}

TextureStruct::TextureStruct(unsigned gtex)
{
	gltex = gtex;
}

TextureStruct::~TextureStruct()
{
	glDeleteTextures(1, &gltex);
}

namespace enigma
{
  extern int bound_texture_stage;

  //This allows GL3 surfaces to bind and hold many different types of data
  int graphics_create_texture_custom(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap, int internalFormat, unsigned format, unsigned type)
  {
    GLuint texture;
    glGenTextures(1, &texture);
    oglmgr->BindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, fullwidth, fullheight, 0, format, type, pxdata);
    if (mipmap) {
      // This allows us to control the number of mipmaps generated, but Direct3D does not have an option for it, so for now we'll just go with the defaults.
      // Honestly not a big deal, Unity3D doesn't allow you to specify either.
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    oglmgr->BindTexture(GL_TEXTURE_2D, 0);

    TextureStruct* textureStruct = new TextureStruct(texture);
    textureStruct->width = width;
    textureStruct->height = height;
    textureStruct->fullwidth = fullwidth;
    textureStruct->fullheight = fullheight;
    textureStruct->internalFormat = internalFormat;
    textureStruct->format = format;
    textureStruct->type = type;
    textureStructs.push_back(textureStruct);
    return textureStructs.size()-1;
  }

  int graphics_create_texture(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap)
  {
    GLuint texture;
    glGenTextures(1, &texture);
    oglmgr->BindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fullwidth, fullheight, 0, GL_BGRA, GL_UNSIGNED_BYTE, pxdata);
    if (mipmap) {
      // This allows us to control the number of mipmaps generated, but Direct3D does not have an option for it, so for now we'll just go with the defaults.
      // Honestly not a big deal, Unity3D doesn't allow you to specify either.
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    oglmgr->BindTexture(GL_TEXTURE_2D, 0);

    TextureStruct* textureStruct = new TextureStruct(texture);
    textureStruct->width = width;
    textureStruct->height = height;
    textureStruct->fullwidth = fullwidth;
    textureStruct->fullheight = fullheight;
    textureStruct->internalFormat = GL_RGBA;
    textureStruct->format = GL_BGRA;
    textureStruct->type = GL_UNSIGNED_BYTE;
    textureStructs.push_back(textureStruct);
    return textureStructs.size()-1;
  }

  int graphics_duplicate_texture(int tex, bool mipmap)
  {
    GLuint texture = textureStructs[tex]->gltex;
    oglmgr->BindTexture(GL_TEXTURE_2D, texture);
    unsigned w, h, fw, fh;
    w = textureStructs[tex]->width;
    h = textureStructs[tex]->height;
    fw = textureStructs[tex]->fullwidth;
    fh = textureStructs[tex]->fullheight;
    char* bitmap = new char[(fh<<(lgpp2(fw)+2))|2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap);
    unsigned dup_tex = graphics_create_texture(w, h, fw, fh, bitmap, mipmap);
    delete[] bitmap;
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
    oglmgr->BindTexture(GL_TEXTURE_2D, src);
    //We could use glCopyImageSubData here, but it's GL4.3
    char* bitmap = new char[(sfh<<(lgpp2(sfw)+2))|2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap);

    char* cropped_bitmap = new char[sw*sh*4];
    for (unsigned int i=0; i<sh; ++i){
      memcpy(cropped_bitmap+sw*i*4, bitmap+sfw*i*4, sw*4);
    }

    oglmgr->BindTexture(GL_TEXTURE_2D, dst);
    unsigned dw, dh;
    dw = textureStructs[destination]->width;
    dh = textureStructs[destination]->height;
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, (x+sw<=dw?sw:dw-x), (y+sh<=dh?sh:dh-y), GL_BGRA, GL_UNSIGNED_BYTE, cropped_bitmap);

    oglmgr->BindTexture(GL_TEXTURE_2D, 0);

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
    oglmgr->BindTexture(GL_TEXTURE_2D, src);
    //We could use glCopyImageSubData here, but it's GL4.3
    char* bitmap = new char[(sfh<<(lgpp2(sfw)+2))|2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap);

    if (xoff+sw>sfw) sw = sfw-xoff;
    if (yoff+sh>sfh) sh = sfh-yoff;
    char* cropped_bitmap = new char[sw*sh*4];
    for (unsigned int i=0; i<sh; ++i){
      memcpy(cropped_bitmap+sw*i*4, bitmap+xoff*4+sfw*(i+yoff)*4, sw*4);
    }

    oglmgr->BindTexture(GL_TEXTURE_2D, dst);
    unsigned dw, dh;
    dw = textureStructs[destination]->width;
    dh = textureStructs[destination]->height;
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, (x+sw<=dw?sw:dw-x), (y+sh<=dh?sh:dh-y), GL_BGRA, GL_UNSIGNED_BYTE, cropped_bitmap);

    oglmgr->BindTexture(GL_TEXTURE_2D, 0);

    delete[] bitmap;
    delete[] cropped_bitmap;
  }

  void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex)
  {
    GLuint texture = textureStructs[tex]->gltex;
    GLuint copy_texture = textureStructs[copy_tex]->gltex;

    unsigned int fw, fh, size;
    oglmgr->BindTexture(GL_TEXTURE_2D, texture);
    fw = textureStructs[tex]->fullwidth;
    fh = textureStructs[tex]->fullheight;
    size = (fh<<(lgpp2(fw)+2))|2;
    char* bitmap = new char[size];
    char* bitmap2 = new char[size];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap);
    oglmgr->BindTexture(GL_TEXTURE_2D, copy_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap2);

    for (unsigned  int i = 3; i < size; i += 4)
        bitmap[i] = (bitmap2[i-3] + bitmap2[i-2] + bitmap2[i-1])/3;

    oglmgr->BindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fw, fh, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap);

    oglmgr->BindTexture(GL_TEXTURE_2D, 0);

    delete[] bitmap;
    delete[] bitmap2;
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

  struct SamplerState {
    GLuint sampler_index;
    unsigned bound_texture;

    SamplerState(): sampler_index(0) {
    }

    ~SamplerState() {
      glDeleteSamplers(1, &sampler_index);
    }

  };

  SamplerState samplerstates[8];

  void graphics_initialize_samplers() {
    GLuint sampler_ids[8];
    glGenSamplers(8, sampler_ids);
    for (size_t i = 0; i < 8; i++) {
      samplerstates[i].sampler_index = sampler_ids[i];
      glBindSampler(i, samplerstates[i].sampler_index);
      // Default to interpolation disabled, for some reason textures do that by default but not samplers.
      glSamplerParameteri(samplerstates[i].sampler_index, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glSamplerParameteri(samplerstates[i].sampler_index, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
  }
}

namespace enigma_user
{

int texture_add(string filename, bool mipmap) {
  unsigned int w, h, fullwidth, fullheight;
  int img_num;

  unsigned char *pxdata = enigma::image_load(filename,&w,&h,&fullwidth,&fullheight,&img_num,false);
  if (pxdata == NULL) { printf("ERROR - Failed to append sprite to index!\n"); return -1; }
  unsigned texture = enigma::graphics_create_texture(w, h, fullwidth, fullheight, pxdata, mipmap);
  delete[] pxdata;

  return texture;
}

void texture_save(int texid, string fname) {
	unsigned w, h;
	unsigned char* rgbdata = enigma::graphics_get_texture_pixeldata(texid, &w, &h);

  string ext = enigma::image_get_format(fname);

	enigma::image_save(fname, rgbdata, w, h, w, h, false);

	delete[] rgbdata;
}

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
  // Deprecated in ENIGMA and GM: Studio, all textures are automatically preloaded.
  oglmgr->BindTexture(GL_TEXTURE_2D, textureStructs[texid]->gltex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, prio);
}

void texture_set_enabled(bool enable)
{
  (enable?glEnable:glDisable)(GL_TEXTURE_2D);
}

void texture_set_blending(bool enable)
{
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
	return 1.0/textureStructs[texid]->fullwidth;
}

gs_scalar texture_get_texel_height(int texid)
{
	return 1.0/textureStructs[texid]->fullheight;
}

void texture_set_stage(int stage, int texid) {
  //TODO(harijs): Check if stage <0
  get_texture(gt,texid,);
  if (enigma::samplerstates[stage].bound_texture != gt) {
    oglmgr->EndShapesBatching();
    if ((unsigned int)enigma::bound_texture_stage != GL_TEXTURE0 + stage) { glActiveTexture(enigma::bound_texture_stage = (GL_TEXTURE0 + stage)); }
    oglmgr->BindTexture(GL_TEXTURE_2D, enigma::samplerstates[stage].bound_texture = (unsigned)(gt >= 0? gt : 0));
  }
}

void texture_reset() {
  if (enigma::samplerstates[0].bound_texture != 0){
    oglmgr->EndShapesBatching();
  	if (enigma::bound_texture_stage != GL_TEXTURE0) { glActiveTexture(enigma::bound_texture_stage = GL_TEXTURE0); }
  	oglmgr->BindTexture(GL_TEXTURE_2D, enigma::samplerstates[0].bound_texture = 0);
  }
}

void texture_set_interpolation_ext(int sampler, bool enable)
{
  //TODO(harijs): Check if sampler <0
  glSamplerParameteri(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_MIN_FILTER, enable?GL_LINEAR:GL_NEAREST);
  glSamplerParameteri(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_MAG_FILTER, enable?GL_LINEAR:GL_NEAREST);
}

void texture_set_repeat_ext(int sampler, bool repeat)
{
  //TODO(harijs): Check if sampler <0
  glSamplerParameteri(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_WRAP_R, repeat?GL_REPEAT:GL_CLAMP_TO_EDGE);
  glSamplerParameteri(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_WRAP_S, repeat?GL_REPEAT:GL_CLAMP_TO_EDGE);
  glSamplerParameteri(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_WRAP_T, repeat?GL_REPEAT:GL_CLAMP_TO_EDGE);
}

void texture_set_wrap_ext(int sampler, bool wrapu, bool wrapv, bool wrapw)
{
  //TODO(harijs): Check if sampler <0
  glSamplerParameteri(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_WRAP_R, wrapu?GL_REPEAT:GL_CLAMP_TO_EDGE);
  glSamplerParameteri(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_WRAP_S, wrapv?GL_REPEAT:GL_CLAMP_TO_EDGE);
  glSamplerParameteri(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_WRAP_T, wrapw?GL_REPEAT:GL_CLAMP_TO_EDGE);
}

void texture_set_border_ext(int sampler, int r, int g, int b, double a)
{
  //TODO(harijs): Check if sampler <0
  GLint bordercolor[4] = { r, g, b, int(a * 255) };
  glSamplerParameteriv(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_BORDER_COLOR, bordercolor);
}

void texture_set_filter_ext(int sampler, int filter)
{
  GLint min, mag;
  if (filter == tx_trilinear) {
    min = GL_LINEAR_MIPMAP_LINEAR;
    mag = GL_LINEAR;
  } else if (filter == tx_bilinear) {
    min = GL_LINEAR_MIPMAP_NEAREST;
    mag = GL_LINEAR;
  } else if (filter == tx_nearest) {
    min = GL_NEAREST_MIPMAP_NEAREST;
    mag = GL_NEAREST;
  } else {
    min = GL_NEAREST;
    mag = GL_NEAREST;
  }
  glSamplerParameteri(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_MIN_FILTER, min);
  glSamplerParameteri(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_MAG_FILTER, mag);
}

void texture_set_lod_ext(int sampler, double minlod, double maxlod, int maxlevel)
{
  glSamplerParameteri(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_MIN_LOD, minlod);
  glSamplerParameteri(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_MAX_LOD, maxlod);
  glSamplerParameteri(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_MAX_LEVEL, maxlevel);
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

void  texture_anisotropy_filter(int sampler, gs_scalar levels)
{
  glSamplerParameterf(enigma::samplerstates[sampler].sampler_index, GL_TEXTURE_MAX_ANISOTROPY_EXT, levels);
}

}
