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
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_BUFFERS_INTERNAL_H
#define ENIGMA_BUFFERS_INTERNAL_H

#include <vector>

namespace enigma
{
  struct BinaryBuffer
  {
    std::vector<unsigned char> data;
    unsigned position;
    unsigned alignment;
    int type;
    
    BinaryBuffer(unsigned size);
    ~BinaryBuffer() = default;
    unsigned GetSize();
    void Resize(unsigned size);
    void Seek(unsigned offset);  
    unsigned char ReadByte();
    void WriteByte(unsigned char byte);
  };
  
  extern std::vector<BinaryBuffer*> buffers;
}

#ifdef DEBUG_MODE
#include "Widget_Systems/widgets_mandatory.h"
#define get_buffer(binbuff,buff)\
  if (buff < 0 or size_t(buff) >= enigma::buffers.size() or !enigma::buffers[buff]) {\
    DEBUG_MESSAGE("Attempting to access non-existing buffer " + toString(buff), MESSAGE_TYPE::M_USER_ERROR);\
    return;\
  }\
  enigma::BinaryBuffer *binbuff = enigma::buffers[buff];
#define get_bufferr(binbuff,buff,r)\
  if (buff < 0 or size_t(buff) >= enigma::buffers.size() or !enigma::buffers[buff]) {\
    DEBUG_MESSAGE("Attempting to access non-existing buffer " + toString(buff), MESSAGE_TYPE::M_USER_ERROR);\
    return r;\
  }\
  enigma::BinaryBuffer *binbuff = enigma::buffers[buff];
#else
  #define get_buffer(binbuff,buff)\
  enigma::BinaryBuffer *binbuff = enigma::buffers[buff];
  #define get_bufferr(binbuff,buff,r)\
  enigma::BinaryBuffer *binbuff = enigma::buffers[buff];
#endif

#endif // ENIGMA_BUFFERS_INTERNAL_H
