/** Copyright (C) 2008-2014 Josh Ventura, DatZach, Polygone, Harijs Grinbergs
*** Copyright (C) 2013-2014, 2019 Robert B. Colton
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

#ifndef ENIGMA_OPENGL_ENUMS
#define ENIGMA_OPENGL_ENUMS

#include "OpenGLHeaders.h"
#include "Graphics_Systems/OpenGL-Common/enums.h"

namespace {

const GLenum fogmodes[3] = {
  GL_EXP, GL_EXP2, GL_LINEAR
};

const GLenum fillmodes[3] = {
  GL_POINT, GL_LINE, GL_FILL
};

} // namespace anonymous

#endif
