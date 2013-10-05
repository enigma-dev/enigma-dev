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

#include "Universal_System/scalar.h"

#ifndef _GLTEXTURESTRUCT__H
#define _GLTEXTURESTRUCT__H

#include <vector>
using std::vector;

struct GmTexture {
	unsigned gltex;
	bool isFont;
	GmTexture(unsigned gtex);
	~GmTexture();
};
extern vector<GmTexture*> GmTextures;

#endif
