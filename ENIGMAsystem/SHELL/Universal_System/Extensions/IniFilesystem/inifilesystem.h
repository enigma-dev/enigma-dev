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
	/**************************************************//*!
	*	@brief	Opens an ini with the indicated filename.
	*			NOTE: The file must end with a newline
	*			character to be read correctly.
	******************************************************/
	void ini_open(string filename);

	/**************************************************//*!
	*	@brief	Closes the currently opened ini.
	*	@return	The contents of the ini file that is closed.
	******************************************************/
	string ini_close();

	//void ini_write_real(string section, string key, variant value);

	/**************************************************//*!
	*	@brief	Writes the value to the indicated key
	*			inside the indicated section.
	******************************************************/
	//void ini_write_string(string section, string key, string value);

	variant ini_read_real(string section, string key, variant def);

	/**************************************************//*!
	*	@return	The value of the indicated key inside the
	*			indicated section.
	******************************************************/
	string ini_read_string(string section, string key, string def);

	/*bool ini_key_exists(string section, string key);

	bool ini_section_exists(string section)

	void ini_key_delete(string section, string key);

	void ini_section_delete(string section);

	void ini_open_from_string(string str);*/
}


#endif // _ENIGMA_INIFILESYSTEM__HPP
