/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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
//using std::string;
#include "../General/GStextures.h"
#include "../General/GLTextureStruct.h"
#include "Universal_System/backgroundstruct.h"
#include "Universal_System/spritestruct.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "../General/GLbinding.h"

vector<GmTexture*> GmTextures(0);

namespace enigma_user {
extern int room_width, room_height;
}

namespace enigma {
  extern size_t background_idmax;
}

GmTexture::GmTexture(unsigned gtex)
{
	gltex = gtex;
}

GmTexture::~GmTexture()
{
	glDeleteTextures(1, &gltex);
}

unsigned get_texture(int texid) {
	return (size_t(texid) >= GmTextures.size())? -1 : GmTextures[texid]->gltex;
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
  int graphics_create_texture(int fullwidth, int fullheight, void* pxdata, bool isfont)
  {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, fullwidth, fullheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pxdata);
	bool interpolate = (interpolate_textures && !isfont);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,interpolate?GL_LINEAR:GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,interpolate?GL_LINEAR:GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

	GmTexture* gmTexture = new GmTexture(texture);
	gmTexture->isFont = isfont;
    GmTextures.push_back(gmTexture);
    return GmTextures.size()-1;
  }

  int graphics_duplicate_texture(int tex)
  {
    GLuint texture = GmTextures[tex]->gltex;
    glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
    glColor4f(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texture);
    int w, h;
	bool interpolate = (interpolate_textures && !GmTextures[tex]->isFont);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,interpolate?GL_LINEAR:GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,interpolate?GL_LINEAR:GL_NEAREST);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT, &h);
    char* bitmap = new char[(h<<(lgpp2(w)+2))|2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    unsigned dup_tex = graphics_create_texture(w, h, bitmap);
    delete[] bitmap;
    glPopAttrib();

	GmTexture* gmTexture = new GmTexture(dup_tex);
	gmTexture->isFont = GmTextures[tex]->isFont;
    GmTextures.push_back(gmTexture);
    return GmTextures.size()-1;
  }

  void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex)
  {
    GLuint texture = GmTextures[tex]->gltex;
    GLuint copy_texture = GmTextures[copy_tex]->gltex;
    glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
    glColor4f(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_BLEND);

    int w, h, size;
    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT, &h);
    size = (h<<(lgpp2(w)+2))|2;
    char* bitmap = new char[size];
    char* bitmap2 = new char[size];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    glBindTexture(GL_TEXTURE_2D, copy_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap2);

    for (int i = 3; i < size; i += 4)
        bitmap[i] = (bitmap2[i-3] + bitmap2[i-2] + bitmap2[i-1])/3;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
	bool interpolate = (interpolate_textures && !GmTextures[tex]->isFont);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,interpolate?GL_LINEAR:GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,interpolate?GL_LINEAR:GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] bitmap;
    delete[] bitmap2;
    glPopAttrib();
  }

// V when this is called its passing the Gluint, but the Gluint is also stored by my GmTexture struct
  void graphics_delete_texture(int tex)
  {
    delete GmTextures[tex];
  }

  unsigned char* graphics_get_texture_rgba(unsigned texture)
  {
    texture_use(texture);

    int w,h;
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&h);

    unsigned char* ret = new unsigned char[(w*h) << 2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, ret);

    return ret;
  }
}

namespace enigma_user
{

void texture_set_enabled(bool enable)
{
  (enable?glEnable:glDisable)(GL_TEXTURE_2D);
}

void texture_set_interpolation(int enable)
{
  enigma::interpolate_textures = enable;
  for (size_t i = 0; i < GmTextures.size(); i++)
  {
	if (GmTextures[i]->isFont) { continue; }
    glBindTexture(GL_TEXTURE_2D, GmTextures[i]->gltex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,enable?GL_LINEAR:GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,enable?GL_LINEAR:GL_NEAREST);
  }
  glBindTexture(GL_TEXTURE_2D, enigma::bound_texture);
}

bool texture_get_interpolation()
{
    return enigma::interpolate_textures;
}

void texture_set_blending(bool enable)
{
    (enable?glEnable:glDisable)(GL_BLEND);
}

int texture_get_texel_width(int texid)
{
  // returns the actual number of pixels in the texture across the xaxis
  GLint width = 0;
  glBindTexture(GL_TEXTURE_2D, texid);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  return width;
}

int texture_get_texel_height(int texid)
{
  // returns the actual number of pixels in the tex across the yaxis
  GLint height = 0;
  glBindTexture(GL_TEXTURE_2D, texid);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &height);
  return height;
}

void texture_set_stage(int stage, int texid) {
	glActiveTexture(GL_TEXTURE0 + stage);
	glBindTexture(GL_TEXTURE_2D, get_texture(texid));
}

void texture_set_repeat(bool repeat)
{
  for (size_t i = 0; i < GmTextures.size(); i++)
  {
    glBindTexture(GL_TEXTURE_2D, GmTextures[i]->gltex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, repeat?GL_REPEAT:GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat?GL_REPEAT:GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat?GL_REPEAT:GL_CLAMP);
  }
}

void texture_set_repeat(int texid, bool repeat)
{
  glBindTexture(GL_TEXTURE_2D, GmTextures[texid]->gltex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, repeat?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat?GL_REPEAT:GL_CLAMP);
}

void texture_set_wrap(int texid, bool wrapr, bool wraps, bool wrapt)
{
  glBindTexture(GL_TEXTURE_2D, GmTextures[texid]->gltex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapr?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt?GL_REPEAT:GL_CLAMP);
}

void texture_preload(int texid)
{

}//functionality has been removed in ENIGMA, all textures are automatically preloaded

void texture_set_priority(int texid, double prio)
{
  glBindTexture(GL_TEXTURE_2D, GmTextures[texid]->gltex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, prio);
}

void texture_set_border(int texid, int r, int g, int b, double a)
{
  GLint color[4] = {(int)r, (int)g, (int)b, int(a * 255)};
  glBindTexture(GL_TEXTURE_2D, GmTextures[texid]->gltex);
  glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
}

void texture_set_swizzle(int texid, int r, int g, int b, double a)
{
  GLint color[4] = {(int)r, (int)g, (int)b, int(a * 255)};
  glBindTexture(GL_TEXTURE_2D, GmTextures[texid]->gltex);
  glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, color);
}

void texture_set_levelofdetail(int texid, double minlod, double maxlod, int maxlevel)
{
  glBindTexture(GL_TEXTURE_2D, GmTextures[texid]->gltex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, minlod);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, maxlod);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxlevel);
}

void texture_mipmapping_filter(int texid, int filter)
{
  glBindTexture(GL_TEXTURE_2D, GmTextures[texid]->gltex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  if (filter == tx_trilinear) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else if (filter == tx_bilinear) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else if (filter == tx_nearest) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }
}

void texture_mipmapping_generate(int texid, int levels)
{
  texture_use(GmTextures[texid]->gltex);
  glGenerateMipmap(GL_TEXTURE_2D);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, levels);
}

bool  texture_anisotropy_supported()
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

void  texture_anisotropy_filter(int texid, gs_scalar levels)
{
  glBindTexture(GL_TEXTURE_2D, GmTextures[texid]->gltex);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, levels);
}

bool  texture_multitexture_supported()
{
  return strstr((char*)glGetString(GL_EXTENSIONS),
           "GL_ARB_multitexture");
}

void texture_multitexture_enable(bool enable)
{

}

}

