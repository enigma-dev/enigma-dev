// 
// Copyright (C) 2014 Seth N. Hetu
// Copyright (C) 2013 Daniel Hrabovcak
// 
// This file is a part of the ENIGMA Development Environment.
// 
// ENIGMA is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, version 3 of the license or any later version.
// 
// This application and its source code is distributed AS-IS, WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with this code. If not, see <http://www.gnu.org/licenses/>
//

#include <fstream>
#include <sstream>

#include "Platforms/General/PFini.h"
#include "Universal_System/estring.h"
#include "Widget_Systems/widgets_mandatory.h"

#include "IniFileIndex.h"


namespace {

//The "name" we give to files loaded from strings. Must be an invalid name for a normal file.
const std::string IniFromStringFilename = "<IniFromString>";

//List of invalid characters on Windows, which is a superset of OSX and Linux's (much smaller) sets.
const std::string InvalidFilenameChars = "<>:\"/\\|?*\0";

//The name of the currently-open file. Empty string means "no open file".
std::string currIniFile;

//Datastructure associated with the currently-open ini file, which is flushed on exit.
enigma::IniFileIndex currIni;

} //End un-named namespace


/*static int section;
static FILE *enigma_ini_file;
static char buff[ENIGMA_INI_BUFFER_SIZE];*/

namespace enigma_user
{
	void ini_open(std::string filename)
	{
		//Opening an ini file without closing the previous one is an error.
		//TODO: Check how GM reports the error; we might have to use #DEBUG_MODE and/or show_error();
		if (!currIniFile.empty()) { return; }

		//Opening an invalidly-named file is an error.
		//TODO: Again, check what GM does.
		if (filename.find_first_of(InvalidFilenameChars)<filename.size()) { return; }

		//Save it.
		currIniFile = filename;

		//If the file already exists, parse it.
		//NOTE: Loading the file in its entirety is consistent with how GM handles inis.
		std::ifstream infile(filename.c_str());
		if (infile.good()) {
			currIni.load(infile);
		}
	}

	void ini_open_from_string(std::string inistr) 
	{
		//Opening an ini file without closing the previous one is an error.
		//TODO: Check how GM reports the error; we might have to use #DEBUG_MODE and/or show_error();
		if (!currIniFile.empty()) { return; }

		//Save it.
		currIniFile = IniFromStringFilename;

		//Parse it.
		std::istringstream str(inistr);
		currIni.load(str);
	}

	std::string ini_full_file_text()
	{
		if (!currIniFile.empty()) {
			return currIni.toString();
		}

		//TODO: Consider whether this should be an error.
		return "";
	}

	//NOTE: In GM, ini_close() returns the entire ini file, but this is almost always a waste. 
	//Use "ini_full_file_text()" if you really need to retrieve this.
	void ini_close()
	{
		//TODO: Check if GM considers closing a non-opened ini an error.
		if (!currIniFile.empty()) {
			//Non-string-loaded inis are saved back to their original files.
			currIni.saveToFile(currIniFile);

			//Now reset.
			currIniFile = "";
			currIni.clear();
		}
	}

	//TODO: GM writes floats (or maybe doubles); Enigma uses ints.
	void ini_write_real(string section, string key, float value)
	{
		//TODO: Check GM's behavior with the read/write functions when no ini has been "opened".
		if (!currIniFile.empty()) {
			currIni.write(section, key, value);
		}
	}	

	void ini_write_string(string section, string key, string value)
	{
		if (!currIniFile.empty()) {
			currIni.write(section, key, value);
		}
	}

	string ini_read_string(string section, string key, string def)
	{
		if (!currIniFile.empty()) {
			return currIni.read(section, key, def);
		}
		return def;
	}
	
	//TODO: GM reads floats (or maybe doubles); Enigma uses ints.
	float ini_read_real(string section, string key, float def)
	{
		if (!currIniFile.empty()) {
			return currIni.read(section, key, def);
		}
		return def;
	}
}

