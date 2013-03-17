/** Copyright (C) 2008-2011 Josh Ventura
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
#include "OpenGLHeaders.h"
#include <string.h>
//using std::string;
#include "GLtextures.h"
#include "Universal_System/backgroundstruct.h"
#include "Universal_System/spritestruct.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "binding.h"

extern int room_width, room_height;
namespace enigma {
  extern size_t background_idmax;
}

#include <vector>
using std::vector;

struct texture {
};
vector<texture> textures(0);

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
  bool interpolate_textures = false; //NOTE: set value here when game settings are used

  unsigned graphics_create_texture(int fullwidth, int fullheight, void* pxdata)
  {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, fullwidth, fullheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pxdata);
    if (interpolate_textures)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
  }

  unsigned graphics_duplicate_texture(int tex)
  {
    GLuint texture = tex;
    glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
    glColor4f(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texture);
    int w, h;
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT, &h);
    char* bitmap = new char[(h<<(lgpp2(w)+2))|2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    unsigned dup_tex = graphics_create_texture(w, h, bitmap);
    delete[] bitmap;
    glPopAttrib();
    return dup_tex;
  }

  void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex)
  {
    GLuint texture = tex, copy_texture = copy_tex;
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
        bitmap[i] = (bitmap2[i-1] + bitmap2[i-2] + bitmap2[i-3])/3;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    if (interpolate_textures)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] bitmap;
    delete[] bitmap2;
    glPopAttrib();
  }

  void graphics_delete_texture(int tex)
  {
    GLuint texture = tex;
    glDeleteTextures(1, &texture);
  }

  //Retrieve image data from a texture, in unsigned char, RGBA format.
  unsigned char* graphics_get_texture_rgba(unsigned texture)
  {
    bind_texture(texture);

    int w,h;
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&h);

    unsigned char* ret = new unsigned char[(w*h) << 2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, ret);

    return ret;
  }
}

void texture_set_interpolation(int enable)
{
    if (enigma::interpolate_textures == enable)
        return;

    enigma::interpolate_textures = enable;
    enigma::background *back;
    enigma::sprite *spr;
    int i, ii;
    for (i = 0; i < enigma::background_idmax; i++)
	{
        back = enigma::backgroundstructarray[i];
	    if (!back)
            continue;

        glBindTexture(GL_TEXTURE_2D, back->texture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,enable?GL_LINEAR:GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,enable?GL_LINEAR:GL_NEAREST);
	}

	for (i = 0; i < enigma::sprite_idmax; i++)
    {
        spr = enigma::spritestructarray[i];
	    if (!spr)
            continue;

        for (ii = 0; ii < spr->subcount; ii++)
        {
            glBindTexture(GL_TEXTURE_2D, spr->texturearray[ii]);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,enable?GL_LINEAR:GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,enable?GL_LINEAR:GL_NEAREST);
        }
    }
}

bool texture_get_interpolation()
{
    //return enigma::interpolate_textures;
}

void texture_set_blending(bool enable)
{
    (enable?glEnable:glDisable)(GL_BLEND);
}

double texture_get_width(int texid)
{
  // returns floating point scale to the bg or some shit
}

double texture_get_height(int texid)
{
  // so does this one
}

int texture_get_pixwidth(int texid)
{
  // returns the actual number of pixels in the texture across the xaxis
  GLint width = 0;
  glBindTexture(GL_TEXTURE_2D, texid);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  return width;
}

int texture_get_pixheight(int texid)
{
  // returns the actual number of pixels in the tex across the yaxis
  GLint height = 0;
  glBindTexture(GL_TEXTURE_2D, texid);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &height);
  return height;
}

void texture_set_repeat(bool repeat)
{
/*
  enigma::background *back;
  for (int i = 0; i < enigma::background_idmax; i++)
  {
    back = enigma::backgroundstructarray[i];
    if (!back) { continue; }
*/
    //glBindTexture(GL_TEXTURE_2D, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, repeat?GL_REPEAT:GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat?GL_REPEAT:GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat?GL_REPEAT:GL_CLAMP);
  //}
}

void texture_set_repeat(int texid, bool repeat)
{
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, repeat?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat?GL_REPEAT:GL_CLAMP);
}

void texture_set_repeat(int texid, bool repeatu, bool repeatv, bool repeatw)
{
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, repeatu?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatv?GL_REPEAT:GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatw?GL_REPEAT:GL_CLAMP);
}

void texture_preload(int texid)
{
  
}//functionality has been removed in ENIGMA, all textures are automatically preloaded

void texture_set_priority(int texid, double prio)
{
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, prio);
}

void texture_set_border(int texid, int r, int g, int b, double a) 
{
  GLint color[4] = {r, g, b, a * 255};
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
}

void texture_mipmapping_filter(int texid, int filter) 
{
  glBindTexture(GL_TEXTURE_2D, texid);
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
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  for (int i = 0; i < levels; i++) 
  {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
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

void  texture_anisotropy_filter(int texid, float levels)
{
  glBindTexture(GL_TEXTURE_2D, texid);
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

