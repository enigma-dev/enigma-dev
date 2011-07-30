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



std::string file_find_first(std::string mask,int attr);

enum {
  fa_readonly  = 1,
  fa_hidden    = 2,
  fa_sysfile   = 4,
  fa_volumeid  = 8,
  fa_directory = 16,
  fa_archive   = 32
};

std::string file_find_next();
void file_find_close();
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
std::string parameter_string(int n);

std::string environment_get_variable(std::string name);
