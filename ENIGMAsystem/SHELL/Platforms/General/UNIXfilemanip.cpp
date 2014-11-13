/** Copyright (C) 2009-2013 Josh Ventura
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

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>

#include <string>
#include "PFfilemanip.h"
using namespace std;

/* UNIX-ready port of file manipulation */

namespace enigma_user
{

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

int directory_delete(string dname) {
  return rmdir(dname.c_str());
}

}