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

#include <iostream>
using namespace std;

#include "OS_Switchboard.h"
#include "file_find.h"

#if CURRENT_PLATFORM_ID == OS_WINDOWS
   #define byte __windows_byte_workaround
   #include <windows.h>
   #undef byte

  static int ff_attribs=0;
  static HANDLE current_find = INVALID_HANDLE_VALUE;
  static WIN32_FIND_DATA found;

  static bool ff_matches() {
    return found.dwFileAttributes == FILE_ATTRIBUTE_NORMAL
        || (ff_attribs & found.dwFileAttributes);
  }

  string file_find_first(string name,int attributes) 
  {
    if (current_find != INVALID_HANDLE_VALUE) file_find_close();

    ff_attribs = attributes;
    current_find = FindFirstFile(name.c_str(), &found);
    if (current_find == INVALID_HANDLE_VALUE) return "";

    return ff_matches() ? found.cFileName : file_find_next();
  }

  string file_find_next()
  {
    if (current_find == INVALID_HANDLE_VALUE) return "";

    do {
      if (!FindNextFile(current_find, &found)) {
        file_find_close();
        return "";
      }
    } while (!ff_matches());

    return found.cFileName;
  }

  void file_find_close() {
    if (current_find == INVALID_HANDLE_VALUE) return;

    FindClose(current_find);
    current_find = INVALID_HANDLE_VALUE;
  }
#else
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <dirent.h>
  #include <unistd.h>

  static DIR* fff_dir_open = NULL;
  static string fff_mask, fff_path;
  static int fff_attrib;
  
  const unsigned u_root = 0;
  
  static inline bool file_hidden(const string r) {
    return (r[0] == '.' or r[r.length()-1] == '~');
  }
  
  static inline bool readable(string fn) {
    return access(fn.c_str(), W_OK);
  }
  
  string file_find_next()
  {
    if (fff_dir_open == NULL)
      return "";
    
    dirent *rd = readdir(fff_dir_open);
    if (rd==NULL) 
      return "";
    string r = rd->d_name;
    
    // Preliminary filter
    
    const int not_attrib = ~fff_attrib;
    
    if (r == "." or r == ".." // Don't return ./ and 
        || (not_attrib & fa_hidden && file_hidden(r))) { // Filter hidden files
      return file_find_next();
    }
    
    struct stat sb;
    const string fqfn = fff_path + r;
    stat(fqfn.c_str(), &sb);
    
    if  (  (not_attrib & fa_directory and sb.st_mode & S_IFDIR)  // Filter out/for directories
        || (not_attrib & fa_sysfile   and sb.st_uid == u_root)  // Filter system files
        || (not_attrib & fa_readonly  and readable(fqfn))) {   // Filter read-only files
      return file_find_next();
    }
    
    if (~sb.st_mode & S_IFDIR and fff_attrib & fa_nofiles)
      return file_find_next();
    
    return r;
  }
  
  string file_find_first(string name,int attrib)
  {
    if (fff_dir_open != NULL)
      closedir(fff_dir_open);
    
    fff_attrib = attrib;
    size_t lp = name.find_last_of("/");
    if (lp != string::npos)
      fff_mask = name.substr(lp+1),
      fff_path = name.substr(0,lp+1),
      fff_dir_open = opendir(fff_path.c_str());
    else
      fff_mask = name,
      fff_path = "./",
      fff_dir_open = opendir("./");
    fff_attrib = attrib;
    return file_find_next();
  }
  
  void file_find_close() {
    if (fff_dir_open != NULL)
      closedir(fff_dir_open);
    fff_dir_open = NULL;
  }
#endif
