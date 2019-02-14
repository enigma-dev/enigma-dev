/** Copyright (C) 2018-2019 Robert B. Colton
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

unsigned libpng_encode32(unsigned char** out, size_t* outsize, const unsigned char* image, unsigned w, unsigned h);
unsigned libpng_decode32_file(unsigned char** out, unsigned* w, unsigned* h, const char* filename);

std::string libpng_error_text(unsigned error);
