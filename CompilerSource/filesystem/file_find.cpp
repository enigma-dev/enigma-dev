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

#if CURRENT_PLATFORM_ID == OS_WINDOWS
  #include <windows.h>
  const int fa_archive   = FILE_ATTRIBUTE_ARCHIVE;   //0x0020
  const int fa_directory = FILE_ATTRIBUTE_DIRECTORY; //0x0010
  const int fa_hidden    = FILE_ATTRIBUTE_HIDDEN;    //0x0002
  const int fa_readonly  = FILE_ATTRIBUTE_READONLY;  //0x0001
  const int fa_sysfile   = FILE_ATTRIBUTE_SYSTEM;    //0x0004
  const int fa_volumeid  = 0x0008;

  static int ff_attribs=0;
  static HANDLE current_find = INVALID_HANDLE_VALUE;
  static WIN32_FIND_DATA found;

  string file_find_first(string name,int attributes) 
  {
    if (current_find!=INVALID_HANDLE_VALUE)
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
    if (current_find==INVALID_HANDLE_VALUE)
    return "";
    
    if (FindNextFile(current_find,&found)==0)
    return "";
    
    while (found.dwFileAttributes!=FILE_ATTRIBUTE_NORMAL and !(ff_attribs^found.dwFileAttributes))
    {
      if (FindNextFile(current_find,&found)==0)
      return "";
    }
    
    return found.cFileName;
  }

  int file_find_close()
  {
    FindClose(current_find);
    return 0;
  }
#else
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <dirent.h>
  #include <unistd.h>

  const int fa_archive   = 0x000020; // Does nothing
  const int fa_directory = 0x000010;
  const int fa_hidden    = 0x000002;
  const int fa_readonly  = 0x000001;
  const int fa_sysfile   = 0x000004;
  const int fa_volumeid  = 0x000008;
  const int fa_nofiles   = 0x800000;

  static DIR* fff_dir_open = NULL;
  static string fff_mask, fff_path;
  static int fff_attrib;
  
  const unsigned u_root = 0;
  
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
    or ((r[0] == '.' or r[r.length()-1] == '~') and not_attrib & fa_hidden) // Filter hidden files
    ) return file_find_next(); 
    
    struct stat sb;
    const string fqfn = fff_path + r;
    stat(fqfn.c_str(), &sb);
    
    if ((sb.st_mode & S_IFDIR     and not_attrib & fa_directory) // Filter out/for directories
    or  (sb.st_uid == u_root      and not_attrib & fa_sysfile)    // Filter system files
    or  (not_attrib & fa_readonly and access(fqfn.c_str(),W_OK)) // Filter read-only files
    ) return file_find_next();
    
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
  void file_find_close()
  {
    if (fff_dir_open != NULL)
      closedir(fff_dir_open);
    fff_dir_open = NULL;
  }
#endif
