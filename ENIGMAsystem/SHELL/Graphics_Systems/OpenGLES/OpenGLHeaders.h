/** Copyright (C) 2008-2013 Josh Ventura, Alasdair Morrison
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

#include "API_Switchboard.h"
//#include <graphics_info.h>

#define ENIGMA_WS_IPHONE 1

#if ENIGMA_WS_COCOA !=0
  #include <OpenGL/gl.h>	

#elif ENIGMA_WS_IPHONE != 0 //|| TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
  #include <OpenGLES/ES1/gl.h>
#elif ENIGMA_WS_ANDROID
#include <GLES/gl.h>
#else
  #include <GL/gl.h>
#endif
