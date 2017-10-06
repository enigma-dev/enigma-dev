/** Copyright (C) 2013 Robert B. Colton
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
  #error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_SHADERSTRUCT
#define ENIGMA_SHADERSTRUCT

#include <string>

namespace enigma
{
	struct ShaderStruct {
		const char* vertex; // the shaders vertex code
		const char* fragment; // the shaders fragment code
		const char* type; // the language of the shader
		bool precompile; // whether or not to compile and link the shader after loading it
	};
	
	extern size_t shader_idmax;
	extern ShaderStruct** shaderdata;
	
	void init_shaders();
}

#endif
