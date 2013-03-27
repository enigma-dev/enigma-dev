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

/*
#ifdef CODEBLOX
#  include <GL/glee.h>
#else
#  include <additional/glee/GLee.h>
#endif*/

//#include "OpenGLHeaders.h"

namespace enigma
{
  extern unsigned bound_texture;
  extern unsigned char currentcolor[4];
  extern bool glew_isgo;
  extern bool pbo_isgo;
}

#include "GL3colors.h"
#include "GL3primitives.h"
#include "GL3d3d.h"
#include "GL3draw.h"
#include "GL3blend.h"
#include "GL3surface.h"
#include "GL3miscextra.h"

