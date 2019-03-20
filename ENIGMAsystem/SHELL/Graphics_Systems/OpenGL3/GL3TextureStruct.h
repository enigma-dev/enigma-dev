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

#include "Graphics_Systems/General/OpenGLHeaders.h"
#include "Universal_System/scalar.h"

#ifndef ENIGMA_GL3_TEXTURESTRUCT_H
#define ENIGMA_GL3_TEXTURESTRUCT_H

#include <vector>
using std::vector;

struct TextureStruct {
	GLuint gltex;
	unsigned width,height;
	unsigned fullwidth,fullheight;
	int internalFormat; //GLint
	unsigned format, type; //GLenum
	TextureStruct(GLuint gtex);
	~TextureStruct();
};
extern vector<TextureStruct*> textureStructs;

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_texture(tex,texid,v)\
    if ((texid < -1 || size_t(texid) >= textureStructs.size()) && texid!=-1) {\
      show_error("Attempting to access non-existing texture " + toString(texid), false);\
      return v;\
    }\
    const GLuint tex = (texid==-1?0:textureStructs[texid]->gltex);
#else
  #define get_texture(tex,texid,v)\
    const GLuint tex = (texid==-1?0:textureStructs[texid]->gltex);
#endif

#endif // ENIGMA_GL3_TEXTURESTRUCT_H
