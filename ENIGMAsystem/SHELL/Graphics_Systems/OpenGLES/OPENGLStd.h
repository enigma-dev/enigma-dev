/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/
/*
#ifdef CODEBLOX
#  include <GL/glee.h>
#else
#  include <../additional/glee/GLee.h>
#endif*/

//#include "OpenGLHeaders.h"

namespace enigma
{
  extern unsigned bound_texture;
  extern unsigned char currentcolor[4];
  extern bool glew_isgo;
  extern bool pbo_isgo;
}

#if COLORSLIB
    #include "GScolors.h"
#endif
#if PRIMTVLIB
    #include "GSprmtvs.h"
#endif
#if STDRAWLIB
    #include "GSstdraw.h"
#endif
#if BLENDMODE
    #include "GSblend.h"
#endif
#if GMSURFACE
    #include "GSsurface.h"
#endif
#include "GSmiscextra.h"

