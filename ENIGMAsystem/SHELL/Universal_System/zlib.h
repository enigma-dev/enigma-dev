/** Copyright (C) 2008-2011 Josh Ventura
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

#ifndef ENIGMA_ZLIB_H
#define ENIGMA_ZLIB_H

namespace enigma {
unsigned char* zlib_compress(unsigned char* inbuffer, int actualsize);
int zlib_decompress(unsigned char* inbuffer, int insize, int uncompresssize, unsigned char* outbytef);
}  //namespace enigma

#endif  //ENIGMA_ZLIB_H
