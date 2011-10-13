/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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
#include <windows.h>

using namespace std;

/* OS Specific; should be moved */

int file_exists(std::string fname) {
    DWORD attributes = GetFileAttributes(fname.c_str());
    if(attributes == 0xFFFFFFFF) {
        return 0;
    } else {
        return 1;
    }
}

int file_delete(std::string fname) {
    DWORD result = DeleteFileA(fname.c_str());

    switch(result) {
        case 0:
            return 0;
            break;
        case ERROR_FILE_NOT_FOUND:
            return 0;
            break;
        case ERROR_ACCESS_DENIED:
            return 0;
            break;
        default:
            return 1;
            break;
    }
}

int file_rename(std::string oldname, std::string newname) {
    DWORD result = MoveFileA(oldname.c_str(), newname.c_str());

    switch(result) {
        case 0:
            return 0;
            break;
        default:
            return 1;
            break;
    }
}

int file_copy(std::string fname, std::string newname) {
    DWORD result = CopyFileA(fname.c_str(), newname.c_str(), false);

    switch(result) {
        case 0:
            return 0;
            break;
        default:
            return 1;
            break;
    }
}

int directory_exists(std::string dname);

// NOTICE: May behave differently than GM. May fail if there are
// directories in the path missing, whereas GM would create them all
int directory_create(std::string dname) {
    DWORD result = CreateDirectoryA(dname.c_str(), NULL);

    switch(result) {
        case 0:
            return 0;
            break;
        case ERROR_ALREADY_EXISTS:
            return 0;
            break;
        case ERROR_PATH_NOT_FOUND:
            return 0;
            break;
        default:
            return 1;
            break;
    }
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


static int ff_attribs = 0;
static HANDLE current_find = INVALID_HANDLE_VALUE;
static WIN32_FIND_DATA found;

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

bool file_attributes(std::string fname,int attr);

std::string filename_name(std::string fname);
std::string filename_path(std::string fname);
std::string filename_dir(std::string fname);
std::string filename_drive(std::string fname);
std::string filename_ext(std::string fname);
std::string filename_change_ext(std::string fname,std::string newext);

void export_include_file(std::string fname);
void export_include_file_location(std::string fname,std::string location);
void discard_include_file(std::string fname);

extern unsigned game_id;
extern std::string working_directory;
extern std::string program_directory;
extern std::string temp_directory;


int parameter_count();
string parameter_string(int n);

string environment_get_variable(std::string name);
