/** Copyright (C) 2008-2013 Josh Ventura
*** Copyright (C) 2013,2019 Robert B. Colton
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

#ifdef INCLUDED_FROM_SHELLMAIN
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_GL_TEXTURES_IMPL_H
#define ENIGMA_GL_TEXTURES_IMPL_H

#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GStextures_impl.h"

namespace enigma {

struct GLTexture : Texture {
  GLuint peer;
  GLTexture(GLuint peer): peer(peer) {}
};

GLuint get_texture_peer(int texid);

} // namespace enigma

#endif // ENIGMA_GL_TEXTURES_IMPL_H
