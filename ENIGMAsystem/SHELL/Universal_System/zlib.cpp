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

#include "libEGMstd.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "zlib.h"

#include <string>
#include <zlib.h>

namespace enigma {

unsigned char* zlib_compress(unsigned char* inbuffer,int actualsize)
{
    uLongf outsize=(int)(actualsize*1.1)+12;
    Bytef* outbytef=new Bytef[outsize];

    int res=compress(outbytef,&outsize,(Bytef*)inbuffer,actualsize);

    if (res != Z_OK)
    {
     #if DEBUG_MODE
     if (res==Z_MEM_ERROR)
     DEBUG_MESSAGE("Zlib failed to compress the buffer. Out of memory.", MESSAGE_TYPE::M_ERROR);
     if (res==Z_BUF_ERROR)
     DEBUG_MESSAGE("Zlib failed to compress the buffer. Output size greater than allotted.", MESSAGE_TYPE::M_ERROR);
     #endif
    }

    return (unsigned char*)outbytef;
}

int zlib_decompress(unsigned char* inbuffer, int insize, int uncompresssize,unsigned char* outbytef)
{
	uLongf outused=uncompresssize;
	switch(uncompress(outbytef,&outused,(Bytef*)inbuffer,insize)){
	case Z_OK:return outused;
	case Z_MEM_ERROR:
		#if DEBUG_MODE
			DEBUG_MESSAGE("Zerror: Memory out", MESSAGE_TYPE::M_ERROR);
		#endif
		return -1;
	case Z_BUF_ERROR:
		#if DEBUG_MODE
			DEBUG_MESSAGE("Zerror: Output of " + toString(outused) + " above allotted " + toString(uncompresssize), MESSAGE_TYPE::M_ERROR);
		#endif
		return -2;
	case Z_DATA_ERROR:
		#if DEBUG_MODE
			DEBUG_MESSAGE("Zerror: Invalid data", MESSAGE_TYPE::M_ERROR);
		#endif
		return -3;
	default:return -4;
	}
}

}  //namespace enigma
