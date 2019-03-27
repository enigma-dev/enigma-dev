/** Copyright (C) 2014 Harijs Grinbergs
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
#include <cstring>
#include "Graphics_Systems/OpenGL/OpenGLHeaders.h"
#include "GL3aux.h"

namespace enigma {

	bool gl_extension_supported(std::string extension){
		GLint n, i;
		glGetIntegerv(GL_NUM_EXTENSIONS, &n);
		for (i = 0; i < n; ++i) {
			if (std::strstr((char*)glGetStringi(GL_EXTENSIONS, i),extension.c_str())!=NULL) return true;
		}
		return false;
	}
}
