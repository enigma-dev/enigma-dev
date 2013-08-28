/**
 *  @file include.hpp
 *  @section License
 *
 *      Copyright (C) 2013 Daniel Hrabovcak
 *
 *      This file is a part of the ENIGMA Development Environment.
 *
 *      This program is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/
#pragma once
#ifndef _ENIGMA_INIFILESYSTEM__HPP
#define _ENIGMA_INIFILESYSTEM__HPP
#include "Universal_System/estring.h"

#ifndef ENIGMA_INI_BUFFER_SIZE
#define ENIGMA_INI_BUFFER_SIZE 512
#endif

namespace enigma_user
{
	variant json_decode(string data);

	string json_encode(variant ds_map);
}


#endif // _ENIGMA_INIFILESYSTEM__HPP
