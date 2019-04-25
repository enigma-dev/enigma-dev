/** Copyright (C) 2008-2013 Josh Ventura
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
#  include <additional/glee/GLee.h> //no additional
#endif*/

//#include "OpenGLHeaders.h"

namespace enigma
{
  extern unsigned bound_texture;
}

#if COLORSLIB
    #include "../General/GScolors.h"
#endif
#if PRIMTVLIB
    #include "GLESprmtvs.h"
#endif
#if STDRAWLIB
    #include "../General/GSstdraw.h"
#endif
#if BLENDMODE
    #include "../General/GSblend.h"
#endif
#if GMSURFACE
    #include "../General/GSsurface.h"
#endif
#include "../General/GSscreen.h"
