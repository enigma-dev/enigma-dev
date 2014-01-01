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
#include "Bridges/General/GL3Context.h"

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

vector<TextureStruct*> textureStructs(0);

namespace enigma_user {
  extern int room_width, room_height;
}

#include <vector>
using std::vector;

namespace enigma {
  extern size_t background_idmax;
}

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

unsigned get_texture(int texid)
{
	if (texid < 0 || texid >= textureStructs.size()) {
		return -1;
	} else {
		return textureStructs[texid]->gltex;
	}
}

namespace enigma
{
  int graphics_create_texture(int fullwidth, int fullheight, void* pxdata, bool isfont)
  {
    GLuint texture;
    glGenTextures(1, &texture);
    oglmgr->BindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, fullwidth, fullheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pxdata);
	bool interpolate = (interpolate_textures && !isfont);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,interpolate?GL_LINEAR:GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,interpolate?GL_LINEAR:GL_NEAREST);
    oglmgr->ResetTextureStates();

	TextureStruct* textureStruct = new TextureStruct(texture);
	textureStruct->isFont = isfont;
    textureStructs.push_back(textureStruct);
    return textureStructs.size()-1;
  }

  int graphics_duplicate_texture(int tex)
  {
    GLuint texture = textureStructs[tex]->gltex;
    oglmgr->BindTexture(GL_TEXTURE_2D, texture);
    int w, h;
	bool interpolate = (interpolate_textures && !textureStructs[tex]->isFont);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,interpolate?GL_LINEAR:GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,interpolate?GL_LINEAR:GL_NEAREST);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT, &h);
    char* bitmap = new char[(h<<(lgpp2(w)+2))|2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    unsigned dup_tex = graphics_create_texture(w, h, bitmap, textureStructs[tex]->isFont);
    delete[] bitmap;
    oglmgr->ResetTextureStates();
    glPopAttrib();
    return dup_tex;
  }

  void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex)
  {
    GLuint texture = textureStructs[tex]->gltex;
    GLuint copy_texture = textureStructs[copy_tex]->gltex;

    int w, h, size;
    oglmgr->BindTexture(GL_TEXTURE_2D, texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT, &h);
    size = (h<<(lgpp2(w)+2))|2;
    char* bitmap = new char[size];
    char* bitmap2 = new char[size];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    oglmgr->BindTexture(GL_TEXTURE_2D, copy_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap2);

    for (int i = 3; i < size; i += 4)
        bitmap[i] = (bitmap2[i-1] + bitmap2[i-2] + bitmap2[i-3])/3;

    oglmgr->BindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
	bool interpolate = (interpolate_textures && !textureStructs[tex]->isFont);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,interpolate?GL_LINEAR:GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,interpolate?GL_LINEAR:GL_NEAREST);
    oglmgr->ResetTextureStates();

    delete[] bitmap;
    delete[] bitmap2;
    glPopAttrib();
  }

  void graphics_delete_texture(int tex)
  {
    glDeleteTextures(1, &textureStructs[tex]->gltex);
    textureStructs.erase(textureStructs.begin() + tex);
  }

  unsigned char* graphics_get_texture_rgba(unsigned texture)
  {
    enigma_user::texture_set(texture);

    int w,h;
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&h);

    unsigned char* ret = new unsigned char[(w*h*4)];
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	// The following line does not work when the texture is non power of two.
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
  for (unsigned i = 0; i < textureStructs.size(); i++)
  {
	if (textureStructs[i]->isFont) { continue; }
    oglmgr->BindTexture(GL_TEXTURE_2D, textureStructs[i]->gltex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,enable?GL_LINEAR:GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,enable?GL_LINEAR:GL_NEAREST);
  }
  oglmgr->ResetTextureStates();
}

bool texture_get_interpolation()
{
    return enigma::interpolate_textures;
}

void texture_set_blending(bool enable)
{
    (enable?glEnable:glDisable)(GL_BLEND);
}

gs_scalar texture_get_width(int texid)
{
  // returns floating point scale to the bg or some shit
  return 0;
}

gs_scalar texture_get_height(int texid)
{
  // so does this one
  return 0;
}

int texture_get_texel_width(int texid)
{
  // returns the actual number of pixels in the texture across the xaxis
  GLint width = 0;
  texture_set(textureStructs[texid]->gltex);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  return width;
}

int texture_get_texel_height(int texid)
{
  // returns the actual number of pixels in the tex across the yaxis
  GLint height = 0;
  texture_set(textureStructs[texid]->gltex);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &height);
  return height;
}

void texture_set(int texid) {
	glActiveTexture(GL_TEXTURE0);
	oglmgr->BindTexture(GL_TEXTURE_2D, texid);
}

void texture_set_stage(int stage, int texid) {
	glActiveTexture(GL_TEXTURE0 + stage);
	oglmgr->BindTexture(GL_TEXTURE_2D, get_texture(texid));
}

void texture_reset() {
	glActiveTexture(GL_TEXTURE0);
	oglmgr->BindTexture(GL_TEXTURE_2D, 0);
	oglmgr->EndShapesBatching();
}

void texture_set_repeat(bool repeat)
{
  for (unsigned i = 0; i < textureStructs.size(); i++) {
    glBindTexture(GL_TEXTURE_2D, textureStructs[i]->gltex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, repeat?GL_REPEAT:GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat?GL_REPEAT:GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat?GL_REPEAT:GL_CLAMP);
  }
  oglmgr->ResetTextureStates();
}

void texture_set_repeat(int texid, bool repeat)
{
  texture_set(textureStructs[texid]->gltex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, repeat?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat?GL_REPEAT:GL_CLAMP);
}

void texture_set_wrap(int texid, bool wrapr, bool wraps, bool wrapt)
{
  texture_set(textureStructs[texid]->gltex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapr?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt?GL_REPEAT:GL_CLAMP);
}

void texture_preload(int texid)
{

}//functionality has been removed in ENIGMA, all textures are automatically preloaded

void texture_set_priority(int texid, double prio)
{
  texture_set(textureStructs[texid]->gltex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, prio);
}

void texture_set_border(int texid, int r, int g, int b, double a)
{
  GLint color[4] = {r, g, b, (int)a * 255};
  texture_set(textureStructs[texid]->gltex);
  glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
}

void texture_set_swizzle(int texid, int r, int g, int b, double a)
{
  GLint color[4] = {r, g, b, (int)a * 255};
  texture_set(textureStructs[texid]->gltex);
  glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, color);
}

void texture_set_levelofdetail(int texid, double minlod, double maxlod, int maxlevel)
{
  texture_set(textureStructs[texid]->gltex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, minlod);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, maxlod);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxlevel);
}

void texture_mipmapping_filter(int texid, int filter)
{
  texture_set(textureStructs[texid]->gltex);
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
  texture_set(textureStructs[texid]->gltex);
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
  texture_set(textureStructs[texid]->gltex);
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

