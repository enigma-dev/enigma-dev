/********************************************************************************\
**                                                                              **
**  Copyright (C) 2012 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <string>
#include <sstream>
#include <windows.h>
#include "Universal_System/estring.h"
#include <time.h>
#include <sys/stat.h>

#include <fstream>
#include <algorithm>
#include <direct.h>
#include <tchar.h>

#include "../General/PFini.h"

using namespace std;

static std::string iniFilename = "";

namespace enigma_user
{

void ini_open(std::string fname)
{
	iniFilename = fname;
}

void ini_close()
{
	iniFilename = "";
}

std::string ini_read_string(std::string section, std::string key, std::string defaultValue)
{
	char buffer[1024];
	GetPrivateProfileString(section.c_str(), key.c_str(), defaultValue.c_str(), buffer, 1024, iniFilename.c_str());

	return buffer;
}

float ini_read_real(std::string section, std::string key, float defaultValue)
{
	char res[255];
	char def[255];
	sprintf(def, "%f", defaultValue);
	GetPrivateProfileString(section.c_str(), key.c_str(), def, res, 255, iniFilename.c_str());
	return atof(res);
	//return GetPrivateProfileInt(section.c_str(), key.c_str(), defaultValue, iniFilename.c_str());
}

void ini_write_string(std::string section, std::string key, std::string value)
{
	WritePrivateProfileString(section.c_str(), key.c_str(), value.c_str(), iniFilename.c_str());
}

void ini_write_real(std::string section, std::string key, float value)
{
	std::stringstream ss;
	ss << value;

	WritePrivateProfileString(section.c_str(), key.c_str(), ss.str().c_str(), iniFilename.c_str());
}

bool ini_key_exists(std::string section, std::string key)
{
	char buffer[1024];
	return GetPrivateProfileString(section.c_str(), key.c_str(), "", buffer, 1024, iniFilename.c_str()) != 0;
}

bool ini_section_exists(std::string section)
{
	char buffer[1024];
	return GetPrivateProfileSection(section.c_str(), buffer, 1024, iniFilename.c_str()) != 0;
}

void ini_key_delete(std::string section, std::string key)
{
	WritePrivateProfileString(section.c_str(), key.c_str(), NULL, iniFilename.c_str());
}

void ini_section_delete(std::string section)
{
  WritePrivateProfileString(section.c_str(), NULL, NULL, iniFilename.c_str());
}

/* OS Specific; should be moved */

int file_exists(std::string fname)
{
    std::string str(fname);
    std::replace(str.begin(),str.end(),'/','\\');

    struct stat sb;
    return (stat((char *)str.c_str(),&sb) == 0 &&
        S_ISREG(sb.st_mode));
}

int file_delete(std::string fname)
{
    std::string str(fname);
    std::replace(str.begin(),str.end(),'/','\\');

    struct stat sb;
    if (stat((char *)str.c_str(),&sb) == 0 &&
        S_ISREG(sb.st_mode))
    {
        return (remove((char *)str.c_str()) == 0);
    }

    return 0;
}

int file_rename(std::string oldname,std::string newname)
{
    std::string str1(oldname);
    std::string str2(newname);
    std::replace(str1.begin(),str1.end(),'/','\\');
    std::replace(str2.begin(),str2.end(),'/','\\');

    if (str1 != str2)
    {
        struct stat sb1;
        struct stat sb2;
        if (stat((char *)str1.c_str(),&sb1) == 0 &&
            S_ISREG(sb1.st_mode) &&
            stat((char *)str2.c_str(),&sb2) != 0)
        {
            return (rename((char *)str1.c_str(),(char *)str2.c_str()) == 0);
        }
    }

    return 0;
}

int file_copy(std::string fname,std::string newname)
{
    std::string str1(fname);
    std::string str2(newname);
    std::replace(str1.begin(),str1.end(),'/','\\');
    std::replace(str2.begin(),str2.end(),'/','\\');

    if (str1 != str2)
    {
        struct stat sb;
        if (stat((char *)str1.c_str(),&sb) == 0 &&
            S_ISREG(sb.st_mode))
        {
            std::ifstream srce((char *)str1.c_str(),std::ios::binary);
            if (srce.tellg() < 0) return 0;

            std::ofstream dest((char *)str2.c_str(),std::ios::binary);
            if (dest.tellp() < 0) return 0;

            dest << srce.rdbuf();
            if (srce.tellg()-dest.tellp() != 0) return 0;

            return 1;
        }
    }

    return 0;
}

int directory_exists(std::string dname)
{
    std::string str(dname);
    std::replace(str.begin(),str.end(),'/','\\');

    if (!str.empty())
    {
        while (*str.rbegin() == '\\')
        {
                str.erase(str.size()-1);
        }
    }

    struct stat sb;
    return (stat((char *)str.c_str(),&sb) == 0 &&
        S_ISDIR(sb.st_mode));
}

// NOTICE: May behave differently than GM. May fail if there are
// directories in the path missing, whereas GM would create them all
int directory_create(std::string dname)
{
    std::string str(dname);
    std::replace(str.begin(),str.end(),'/','\\');

    if (!str.empty())
    {
        while (*str.rbegin() == '\\')
        {
            str.erase(str.size()-1);
        }
    }

    struct stat sb;
    if (stat((char *)str.c_str(),&sb) != 0)
    {
        return (_mkdir((char *)str.c_str()) == 0);
    }

    return 0;
}

int directory_destroy(std::string dname)
{
    std::string str(dname);
    std::replace(str.begin(),str.end(),'/','\\');

    if (!str.empty())
    {
        while (*str.rbegin() == '\\')
        {
            str.erase(str.size()-1);
        }
    }

    struct stat sb;
    if (stat((char *)str.c_str(),&sb) == 0 &&
        S_ISDIR(sb.st_mode))
    {
        HANDLE hFind;
        WIN32_FIND_DATA FindFileData;

        TCHAR DirPath[MAX_PATH];
        TCHAR FileName[MAX_PATH];

        _tcscpy(DirPath,(char *)str.c_str());
        _tcscat(DirPath,"\\*");
        _tcscpy(FileName,(char *)str.c_str());
        _tcscat(FileName,"\\");

        hFind = FindFirstFile(DirPath,&FindFileData);
        if (hFind == INVALID_HANDLE_VALUE) return 0;
        _tcscpy(DirPath,FileName);

        bool bSearch = true;
        while (bSearch)
        {
            if (FindNextFile(hFind,&FindFileData))
            {
                if (!(_tcscmp(FindFileData.cFileName,".") &&
                    _tcscmp(FindFileData.cFileName,".."))) continue;
                _tcscat(FileName,FindFileData.cFileName);
                if ((FindFileData.dwFileAttributes &
                    FILE_ATTRIBUTE_DIRECTORY))
                {
                    if (!directory_destroy(FileName))
                    {
                        FindClose(hFind);
                        return 0;
                    }
                    RemoveDirectory(FileName);
                    _tcscpy(FileName,DirPath);
                }
                else
                {
                    if (FindFileData.dwFileAttributes &
                        FILE_ATTRIBUTE_READONLY)
                        _chmod(FileName, _S_IWRITE);

                    if (!DeleteFile(FileName))
                    {
                        FindClose(hFind);
                        return 0;
                    }
                    _tcscpy(FileName,DirPath);
                }
            }
            else
            {
                if (GetLastError() == ERROR_NO_MORE_FILES)
                    bSearch = false;
                else
                {
                    FindClose(hFind);
                    return 0;
                }
            }
        }
        FindClose(hFind);

        return (RemoveDirectory((char *)str.c_str()) == true);
    }

    return 0;
}

int directory_rename(std::string oldname,std::string newname)
{
    std::string str1(oldname);
    std::string str2(newname);
    std::replace(str1.begin(),str1.end(),'/','\\');
    std::replace(str2.begin(),str2.end(),'/','\\');

    if (!str1.empty())
    {
        while (*str1.rbegin() == '\\')
        {
            str1.erase(str1.size()-1);
        }
    }
    if (!str2.empty())
    {
        while (*str2.rbegin() == '\\')
        {
            str2.erase(str2.size()-1);
        }
    }

    if (str1 != str2)
    {
        struct stat sb1;
        struct stat sb2;
        if (stat((char *)str1.c_str(),&sb1) == 0 &&
            S_ISDIR(sb1.st_mode) &&
            stat((char *)str2.c_str(),&sb2) != 0)
        {
            return (rename((char *)str1.c_str(),(char *)str2.c_str()) == 0);
        }
    }

    return 0;
}

int directory_copy(std::string dname,std::string newname)
{
    std::string str1(dname);
    std::string str2(newname);
    std::replace(str1.begin(),str1.end(),'/','\\');
    std::replace(str2.begin(),str2.end(),'/','\\');

    if (!str1.empty())
    {
        while (*str1.rbegin() == '\\')
        {
            str1.erase(str1.size()-1);
        }
    }
    if (!str2.empty())
    {
        while (*str2.rbegin() == '\\')
        {
            str2.erase(str2.size()-1);
        }
    }

    if (str1 != str2)
    {
        struct stat sb1;
        struct stat sb2;
        if (stat((char *)str1.c_str(),&sb1) == 0 &&
            S_ISDIR(sb1.st_mode) &&
            stat((char *)str2.c_str(),&sb2) != 0)
        {
            if (str1 != str2.substr(0,str1.length()) && str2 != str1.substr(0,str2.length()))
            {
                std::string strSource;
                std::string strDestination;
                std::string strPattern;

                HANDLE hFile;
                WIN32_FIND_DATA FileInformation;

                strPattern = str1+"\\*.*";

                if (!CreateDirectory(str2.c_str(),0))
                    return 0;

                hFile = FindFirstFile(strPattern.c_str(),&FileInformation);
                if (hFile != INVALID_HANDLE_VALUE)
                {
                    do
                    {
                        if (FileInformation.cFileName[0] != '.')
                        {
                            strSource.erase();
                            strDestination.erase();

                            strSource = str1+"\\"+FileInformation.cFileName;
                            strDestination = str2+"\\"+FileInformation.cFileName;

                            if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                            {
                                if (!directory_copy((char *)strSource.c_str(),(char *)strDestination.c_str()))
                                    return 0;
                            }
                            else
                            {
                                if (!CopyFile(strSource.c_str(),strDestination.c_str(),true))
                                    return 0;
                            }
                        }
                    }
                    while (FindNextFile(hFile,&FileInformation) == true);

                    FindClose(hFile);

                    DWORD dwError = GetLastError();
                    if (dwError != ERROR_NO_MORE_FILES)
                        return 0;
                }

                return 1;
            }
        }
    }

    return 0;
}


// Maintainer: If this segment errors, it is an error of philosophy.
// The Game Maker constants do not have a standard, but line up with
// their Windows equivalents. Should either change, we have an issue.
enum {
  fa_readonly  = FILE_ATTRIBUTE_READONLY,
  fa_hidden    = FILE_ATTRIBUTE_HIDDEN,
  fa_sysfile   = FILE_ATTRIBUTE_SYSTEM,
  fa_volumeid  = 0x00000008,
  fa_directory = FILE_ATTRIBUTE_DIRECTORY,
  fa_archive   = FILE_ATTRIBUTE_ARCHIVE
};

}

static int ff_attribs = 0;
static HANDLE current_find = INVALID_HANDLE_VALUE;
static WIN32_FIND_DATA found;

namespace enigma_user
{

string file_find_first(string name,int attributes)
{
  if (current_find != INVALID_HANDLE_VALUE)
  { FindClose(current_find); current_find=INVALID_HANDLE_VALUE; }

  ff_attribs=attributes;

  HANDLE d=FindFirstFile(name.c_str(),&found);
  if (d==INVALID_HANDLE_VALUE) return "";
  while (found.dwFileAttributes!=FILE_ATTRIBUTE_NORMAL and !(ff_attribs^found.dwFileAttributes))
  {
    if (FindNextFile(d,&found)==0)
    return "";
  }

  current_find=d;
  return found.cFileName;
}

string file_find_next()
{
  if (current_find==INVALID_HANDLE_VALUE) return "";
  if (FindNextFile(current_find,&found)==0) return "";

  while (found.dwFileAttributes!=FILE_ATTRIBUTE_NORMAL and !(ff_attribs^found.dwFileAttributes)) {
    if (FindNextFile(current_find,&found)==0)
    return "";
  }
  return found.cFileName;
}

int file_find_close() {
  FindClose(current_find);
  return 0;
}

bool file_attributes(std::string fname,int attributes)
{
    DWORD fa = GetFileAttributes(fname.c_str());
    if (fa == 0xFFFFFFFF)
        return false;
    return fa & attributes;
}

/*void export_include_file(std::string fname);
void export_include_file_location(std::string fname,std::string location);
void discard_include_file(std::string fname);*/

extern unsigned game_id;
extern std::string working_directory;
extern std::string program_directory;
extern std::string temp_directory;

long long file_size(std::string fname)
{
    struct stat sb;
    if (stat(fname.c_str(), &sb) == -1) {
        return -1;
    }
    return (long long) sb.st_size;
}

time_t file_access_time(std::string fname)
{
    struct stat sb;
    if (stat(fname.c_str(), &sb) == -1) {
        return -1;
    }
    return sb.st_atime;
}

time_t file_modified_time(std::string fname)
{
    struct stat sb;
    if (stat(fname.c_str(), &sb) == -1) {
        return -1;
    }
    return sb.st_mtime;
}

}
