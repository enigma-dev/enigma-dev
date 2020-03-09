/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2013-2014 Robert B. Colton
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

#include <string>
#include <sstream>
using std::string;

#define byte __windows_byte_workaround
#include <windows.h>
#undef byte


#include "../General/PFregistry.h"

#include "Platforms/platforms_mandatory.h"

namespace enigma_user {

extern unsigned int game_id;

}

static HKEY registryCurrentRoot = HKEY_CURRENT_USER;

namespace enigma_user
{

void registry_write_string(std::string name, std::string str)
{
	std::stringstream ss;
	ss << "Software\\EnigmaGM\\" << game_id;

	// Write to registry
	registry_write_string_ext(ss.str(), name, str);
}

void registry_write_real(std::string name, int x)
{
	std::stringstream ss;
	ss << "Software\\EnigmaGM\\" << game_id;

	// Write to registry
	registry_write_real_ext(ss.str(), name, x);
}

std::string registry_read_string(std::string name)
{
	std::stringstream ss;
	ss << "Software\\EnigmaGM\\" << game_id;

	// Read from registry
	return registry_read_string_ext(ss.str(), name);
}

int registry_read_real(std::string name)
{
	std::stringstream ss;
	ss << "Software\\EnigmaGM\\" << game_id;

	// Read from registry
	return registry_read_real_ext(ss.str(), name);
}

bool registry_exists(std::string name)
{
	std::stringstream ss;
	ss << "Software\\EnigmaGM\\" << game_id;

	return registry_exists_ext(ss.str(), name);
}

void registry_write_string_ext(std::string key, std::string name, std::string str)
{
	HKEY hKey;

	// Open registry key
	if (RegCreateKeyEx(registryCurrentRoot, key.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
		return;

	// Write file and close key
	RegSetValueEx(hKey, name.c_str(), 0, REG_SZ, (LPBYTE)str.c_str(), str.length() + 1);
	RegCloseKey(hKey);
}

void registry_write_real_ext(std::string key, std::string name, int x)
{
	HKEY hKey;

	// Open registry key
	if (RegCreateKeyEx(registryCurrentRoot, key.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
		return;

	// Write value and close key
	RegSetValueEx(hKey, name.c_str(), 0, REG_DWORD, (LPBYTE)&x, sizeof(int));
	RegCloseKey(hKey);
}

std::string registry_read_string_ext(std::string key, std::string name)
{
	char buffer[1024];
	DWORD type = REG_SZ, len = 1024;
	HKEY hKey;

	// Open registry key
	if (RegOpenKeyEx(registryCurrentRoot, key.c_str(), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return "";

	// Read value and close key
	RegQueryValueEx(hKey, (LPCTSTR)name.c_str(), 0, &type, (LPBYTE)buffer, &len);
	RegCloseKey(hKey);

	return buffer;
}

int registry_read_real_ext(std::string key, std::string name)
{
	DWORD type = REG_DWORD, len = sizeof(int);
	HKEY hKey;
	int value;

	// Open registry key
	if (RegOpenKeyEx(registryCurrentRoot, key.c_str(), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return 0;

	// Read value and close key
	RegQueryValueEx(hKey, (LPCTSTR)name.c_str(), 0, &type, (LPBYTE)&value, &len);
	RegCloseKey(hKey);

	return value;
}

bool registry_exists_ext(std::string key, std::string name)
{
	HKEY hKey;
	bool value;

	// Open registry key
	if (RegOpenKeyEx(registryCurrentRoot, key.c_str(), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return false;

	// Read value and close key
	value = RegQueryValueEx(hKey, (LPCTSTR)name.c_str(), 0, NULL, NULL, NULL) != ERROR_FILE_NOT_FOUND;
	RegCloseKey(hKey);

	return value;
}

void registry_set_root(int root)
{
	const HKEY keyLookup[4] = { HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, HKEY_USERS };
	if (root >= 0 && root < 4)
		registryCurrentRoot = keyLookup[root];
}

}

