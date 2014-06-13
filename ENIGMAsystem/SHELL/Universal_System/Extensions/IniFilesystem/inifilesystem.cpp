/**
 *  @file ini-filesystem.cpp
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
#include <cstdio>
#include "Platforms/General/PFini.h"
#include "Universal_System/estring.h"

#ifndef ENIGMA_INI_BUFFER_SIZE
#define ENIGMA_INI_BUFFER_SIZE 512
#endif

static int section;
static FILE *enigma_ini_file;
static char buff[ENIGMA_INI_BUFFER_SIZE];

namespace enigma_user
{
	void ini_open(string filename)
	{
#ifdef DEBUG_MODE
		if (enigma_ini_file != NULL)
		{
			show_error("An ini file is already opened.");
			return;
		}
#endif
		enigma_ini_file = fopen(filename.c_str(), "r+");
		if (fgetc(enigma_ini_file) != '[')
		{
#ifdef DEBUG_MODE
			show_error("File is not an ini file.");
#endif
			fclose(enigma_ini_file);
			return;
		}
#ifdef DEBUG_MODE
		if (ferror(enigma_ini_file))
		{
			show_error("Unable to open the indicated ini file.");
		}
#endif
	}

	void ini_close()
	{
#ifdef DEBUG_MODE
		if (enigma_ini_file == NULL)
		{
			show_error("Cannot close an ini file before it is not open.");
			return;
		}
#endif
		fclose(enigma_ini_file);
		enigma_ini_file = NULL;
	}

	/*void finsert(FILE *file, char *buffer, const size_t buffsize, const char *insert, const size_t size)
	{
#ifdef DEBUG_MODE
		if (size > buffsize)
		{
			show_error("Ini buffer too small.");
			return;
		}
#endif
		size_t pos = ftell(file);
		fseek(file, 0, SEEK_END);
		std::cout << "Size: " << ftell(file) << std::endl;
		std::cout << "Ammount: " << ftell(file)-pos << std::endl;
		/while(pos+buffsize > ftell(file));
		{
			fseek(file, -buffsize, SEEK_CUR);
			fgets(buff, buffsize, file);
			fwrite(buff, sizeof(char), buffsize, file);
			fseek(file, -buffsize, SEEK_CUR);
		}/
		long amm = ftell(file)-pos;
		std::cout << "Ammount: " << -amm << std::endl;
		fseek(file, -amm, SEEK_CUR);
		std::cout << "Current: " << ftell(file) << std::endl;

		// fgets implementation without newline checking.
		char *c = buffer;
		for(; amm > 0; --amm)
		{
			(*c) = getc(file);
			++c;
		}

		std::cout << "Whole: " << buff << std::endl;
		std::cout << "Current: " << ftell(file) << std::endl;
		fseek(file, -amm, SEEK_CUR);
		//fseek(file, size, SEEK_CUR);
		std::cout << "Current: " << ftell(file) << std::endl;
		fwrite(buff, sizeof(char), amm, file);
		fseek(file, -amm, SEEK_CUR);

		//fwrite(insert, sizeof(char), size, file);
	}*/

	void ini_write_string(string section, string key, string value)
	{
		//string str = "hello";
		//finsert(enigma_ini_file, buff, ENIGMA_INI_BUFFER_SIZE, str.c_str(), str.size());
	}

	string ini_read_string(string section, string key, string def)
	{
		int cur, pos = ftell(enigma_ini_file);

		string str;
		str.reserve(section.size() > key.size() ? section.size() : key.size());

		char c;

		while(true)
		{
			cur = ftell(enigma_ini_file);
			while ((c = fgetc(enigma_ini_file)) && c != ']')
			{
				str.push_back(c);
			}

			// Whitespace and Windows carriage return fix.
			while (fgetc(enigma_ini_file) != '\n');

			// Check if the section is correct.
			if (str == section)
			{
				str.clear();
				while(true)
				{
					while ((c = fgetc(enigma_ini_file)) && c != '=')
					{
						str.push_back(c);
					}

					if (str == key)
					{
						str.clear();
						while ((c = fgetc(enigma_ini_file)) && c != '\n')
						{
							str.push_back(c);
						}
						fseek(enigma_ini_file, cur, SEEK_SET);
						return str;
					}

					while (fgetc(enigma_ini_file) != '\n');
					if (fgetc(enigma_ini_file) == '[')
					{
						// Section does not exist.
						fseek(enigma_ini_file, cur, SEEK_SET);
						return def;
					}
					str.clear();
				}
			}
			str.clear();
			while(true)
			{
				if (fgetc(enigma_ini_file) == '\n')
				{
					if (fgetc(enigma_ini_file) == '[')
					{
						// If we've looped through the whole file, it's not there.
						if (ftell(enigma_ini_file) == pos)
						{
							return def;
						}
						break;
					}

				}
				if (feof(enigma_ini_file))
				{
					// Start over if we have not found the section yet.
					rewind(enigma_ini_file);
					fgetc(enigma_ini_file);
					if (pos == 1)
					{
						return def;
					}
					break;
				}
			}
		}
	}
	
	variant ini_read_real(string section, string key, variant def)
	{
		int cur, pos = ftell(enigma_ini_file);

		string str;
		str.reserve(section.size() > key.size() ? section.size() : key.size());

		char c;

		while(true)
		{
			cur = ftell(enigma_ini_file);
			while ((c = fgetc(enigma_ini_file)) && c != ']')
			{
				str.push_back(c);
			}

			// Whitespace and Windows carriage return fix.
			while (fgetc(enigma_ini_file) != '\n');

			// Check if the section is correct.
			if (str == section)
			{
				str.clear();
				while(true)
				{
					while ((c = fgetc(enigma_ini_file)) && c != '=')
					{
						str.push_back(c);
					}

					if (str == key)
					{
						str.clear();
						while ((c = fgetc(enigma_ini_file)) && c != '\n')
						{
							str.push_back(c);
						}
						fseek(enigma_ini_file, cur, SEEK_SET);
						return variant(str);
					}

					while (fgetc(enigma_ini_file) != '\n');
					if (fgetc(enigma_ini_file) == '[')
					{
						// Section does not exist.
						fseek(enigma_ini_file, cur, SEEK_SET);
						return def;
					}
					str.clear();
				}
			}
			str.clear();
			while(true)
			{
				if (fgetc(enigma_ini_file) == '\n')
				{
					if (fgetc(enigma_ini_file) == '[')
					{
						// If we've looped through the whole file, it's not there.
						if (ftell(enigma_ini_file) == pos)
						{
							return def;
						}
						break;
					}

				}
				if (feof(enigma_ini_file))
				{
					// Start over if we have not found the section yet.
					rewind(enigma_ini_file);
					fgetc(enigma_ini_file);
					if (pos == 1)
					{
						return def;
					}
					break;
				}
			}
		}
	}
}
