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

#include "../OS_Switchboard.h"

#if TARGET_PLATFORM_ID == OS_WINDOWS
  #include <windows.h>
  const int fa_archive=FILE_ATTRIBUTE_ARCHIVE;
  const int fa_directory=FILE_ATTRIBUTE_DIRECTORY;
  const int fa_hidden=FILE_ATTRIBUTE_HIDDEN;
  const int fa_readonly=FILE_ATTRIBUTE_READONLY;
  const int fa_sysfile=FILE_ATTRIBUTE_SYSTEM;
  const int fa_volumeid=0x00000008;

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
  #include <dirent.h>

  DIR* fff_dir_open = NULL;
  char *fff_mask; int fff_attrib;
  string file_find_next()
  {
    if (fff_dir_open == NULL)
      return "";
    
    dirent *rd = readdir(fff_dir_open);
    if (rd==NULL) 
      return "";
    return rd->d_name;
  }
  string file_find_first(string name,int attrib)
  {
    if (fff_dir_open != NULL)
      closedir(fff_dir_open);
    
    fff_dir_open = opendir(name.c_str());
    return file_find_next();
  }
  void file_find_close()
  {
    if (fff_dir_open != NULL)
      closedir(fff_dir_open);
    fff_dir_open = NULL;
  }
#endif
