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

#include "PFini.h"
#include "Universal_System/estring.h"
#include "Widget_Systems/widgets_mandatory.h" //show_error()

#include "UNIXiniindex.h"


namespace {

//The "name" we give to files loaded from strings. Must be an invalid name for a normal file.
const std::string IniFromStringFilename = "<IniFromString>";

//List of invalid characters on Windows, which is a superset of OSX and Linux's (much smaller) sets.
//NOTE: At the moment, we only ban NULL; there might be cause to ban other characters later.
//      Don't forget that an absolute path can be sent, so banning "/", "\", etc., won't work.
const std::string InvalidFilenameChars = "\0";

//The name of the currently-open file. Empty string means "no open file".
std::string currIniFile;

//Datastructure associated with the currently-open ini file, which is flushed on exit.
enigma::IniFileIndex currIni;

//Default comment character
char commentChar = ';';

} //End un-named namespace


namespace enigma_user
{
	void ini_set_comment_char(char ch=';')
	{
		commentChar = ch;
	}

	void ini_open(std::string filename)
	{
		//GM will silently fail to save anything if an invalidly-named file is selected. Since we flush the ini file on close, 
		// we should try to filter out bad inis as early as possible (the final test will be in ini_close()).
		if (filename.find_first_of(InvalidFilenameChars)!=std::string::npos) {
			DEBUG_MESSAGE("IniFileSystem - cannot open new ini file; filename contains invalid characters.", MESSAGE_TYPE::M_ERROR);
			return;
		}

		//Opening an ini file without closing the previous one will simply call ini_close() first.
		if (!currIniFile.empty()) {
			ini_close();
		}

		//Save it.
		currIniFile = filename;

		//If the file already exists, parse it.
		//NOTE: Loading the file in its entirety is consistent with how GM handles inis.
		std::ifstream infile(filename.c_str());
		if (infile.good()) { //If the file isn't good, it might be because we are creating a new ini file, so it's not an error.
			currIni.load(infile, commentChar);
		}
	}

	void ini_open_from_string(std::string inistr) 
	{
		//Opening an ini file without closing the previous one will simply call ini_close() first.
		if (!currIniFile.empty()) {
			ini_close();
		}

		//Save it.
		currIniFile = IniFromStringFilename;

		//Parse it.
		std::istringstream str(inistr);
		currIni.load(str, commentChar);
	}

	std::string ini_full_file_text()
	{
		if (currIniFile.empty()) {
			DEBUG_MESSAGE("IniFileSystem - cannot get full ini text, as there is no ini file currently open.", MESSAGE_TYPE::M_ERROR);
			return "";
		}

		return currIni.toString();
	}

	//NOTE: In GM, ini_close() returns the entire ini file, but this is almost always a waste. 
	//Use "ini_full_file_text()" if you really need to retrieve this.
	void ini_close()
	{
		//Only act if we actually have an ini file opened.
		if (!currIniFile.empty()) {
			//Non-string-loaded inis are saved back to their original files.
			if (!currIni.saveToFile(currIniFile)) {
				DEBUG_MESSAGE("IniFileSystem - could not save ini file to output; perhaps it's in a protected location?", MESSAGE_TYPE::M_ERROR);
			}

			//Now reset.
			currIniFile = "";
			currIni.clear();
		}
	}

	void ini_key_delete(std::string section, std::string key)
	{
		//This won't work if the file isn't open.
		if (currIniFile.empty()) {
			DEBUG_MESSAGE("IniFileSystem - cannot delete key, as there is no ini file currently open.", MESSAGE_TYPE::M_ERROR);
		} else {
			currIni.delKey(section, key);
		}
	}


	void ini_section_delete(std::string section)
	{
		//This won't work if the file isn't open.
		if (currIniFile.empty()) {
			DEBUG_MESSAGE("IniFileSystem - cannot delete section, as there is no ini file currently open.", MESSAGE_TYPE::M_ERROR);
		} else {
			currIni.delSection(section);
		}
	}


	bool ini_key_exists(std::string section, std::string key)
	{
		//This won't work if the file isn't open.
		if (currIniFile.empty()) {
			DEBUG_MESSAGE("IniFileSystem - cannot check key, as there is no ini file currently open.", MESSAGE_TYPE::M_ERROR);
			return false;
		} else {
			return currIni.keyExists(section, key);
		}
	}


	bool ini_section_exists(std::string section)
	{
		//This won't work if the file isn't open.
		if (currIniFile.empty()) {
			DEBUG_MESSAGE("IniFileSystem - cannot check section, as there is no ini file currently open.", MESSAGE_TYPE::M_ERROR);
			return false;
		} else {
			return currIni.sectionExists(section);
		}
	}


	//TODO: GM writes floats (or maybe doubles); Enigma uses ints.
	void ini_write_real(string section, string key, float value)
	{
		//GM will err out when trying to read/write an unopened ini file.
		if (currIniFile.empty()) {
			DEBUG_MESSAGE("IniFileSystem - cannot write real, as there is no ini file currently open.", MESSAGE_TYPE::M_ERROR);
		} else {
			currIni.write(section, key, value);
		}
	}	

	void ini_write_string(string section, string key, string value)
	{
		//GM will err out when trying to read/write an unopened ini file.
		if (currIniFile.empty()) {
			DEBUG_MESSAGE("IniFileSystem - cannot write string, as there is no ini file currently open.", MESSAGE_TYPE::M_ERROR);
		} else {
			currIni.write(section, key, value);
		}
	}

	string ini_read_string(string section, string key, string def)
	{
		//GM will err out when trying to read/write an unopened ini file.
		if (currIniFile.empty()) {
			DEBUG_MESSAGE("IniFileSystem - cannot read string, as there is no ini file currently open.", MESSAGE_TYPE::M_ERROR);
			return def;
		} else {
			return currIni.read(section, key, def);
		}
	}
	
	//TODO: GM reads floats (or maybe doubles); Enigma uses ints.
	float ini_read_real(string section, string key, float def)
	{
		//GM will err out when trying to read/write an unopened ini file.
		if (currIniFile.empty()) {
			DEBUG_MESSAGE("IniFileSystem - cannot read real, as there is no ini file currently open.", MESSAGE_TYPE::M_ERROR);
			return def;
		} else {
			return currIni.read(section, key, def);
		}
	}
}

