/**
  @file  compile_common.h
  @brief Another file on its way out.
  
  @section License
    Copyright (C) 2013 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

#ifndef _COMPILE_COMMON__H
#define _COMPILE_COMMON__H

#include <map>
#include <set>
#include "compile_organization.h"
#include "parser/object_storage.h"

extern const char* license;

inline void* lgmRoomBGColor(int c) {
  return reinterpret_cast<void*>((c & 0xFF)?(((c & 0x00FF0000) >> 8) | ((c & 0x0000FF00) << 8) | ((c & 0xFF000000) >> 24)):0xFFFFFFFF);
}

inline string system_get_uppermost_tier() {
  return "object_collisions";
}

#endif
