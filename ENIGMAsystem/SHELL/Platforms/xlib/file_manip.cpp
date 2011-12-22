/** Copyright (C) 2009-2011 Josh Ventura
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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <string>
#include "file_manip.h"
using namespace std;

/* UNIX-ready port of file manipulation */

int file_exists(string fname)
{
  struct stat st;
  return (stat(fname.c_str(),&st) == 0) and !(S_ISDIR(st.st_mode));
}
int file_delete(string fname)
{
  return remove(fname.c_str());
}
int file_rename(string oldname,string newname)
{
  return rename(oldname.c_str(),newname.c_str());
}
int file_copy(string fname,string newname)
{
  return system(("cp "+fname+" "+newname).c_str()); // Hackish, but there's no good implementation on Linux
}
int directory_exists(string dname)
{
  struct stat st;
  return (stat(dname.c_str(),&st) == 0) and (S_ISDIR(st.st_mode));
}
int directory_create(string dname) {
  return mkdir(dname.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
}


// TODO: Implement these...
/*
string file_find_first(string mask,int attr);
string file_find_next();
void file_find_close();
bool file_attributes(string fname,int attr);

void export_include_file(string fname);
void export_include_file_location(string fname,string location);
void discard_include_file(string fname);

extern unsigned game_id;
extern string working_directory;
extern string program_directory;
extern string temp_directory;

string environment_get_variable(string name);
*/
