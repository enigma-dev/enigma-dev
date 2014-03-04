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
#include "file_manip.h"
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

bool set_working_directory(string dir) {
  return !chdir(dir.c_str());
}

}

// File iteration functions and environment functions

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

static DIR* fff_dir_open = NULL;
static string fff_mask, fff_path;
static int fff_attrib;

#define u_root 0

namespace enigma_user
{
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
    
    if ((sb.st_mode & S_IFDIR and not_attrib & fa_directory) // Filter out/for directories
    or (sb.st_uid == u_root and not_attrib & fa_sysfile) // Filter system files
    or (not_attrib & fa_readonly and access(fqfn.c_str(),W_OK)) // Filter read-only files
    ) return file_find_next();
    
    return r;
  }
  string file_find_first(string name, int attrib)
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
  
  string environment_get_variable(string name) {
    char *ev = getenv(name.c_str());
    return ev? ev : "";
  }
}


// TODO: Implement these...
/*
bool file_attributes(string fname,int attr);

void export_include_file(string fname);
void export_include_file_location(string fname,string location);
void discard_include_file(string fname);

extern unsigned game_id;
extern string working_directory;
extern string program_directory;
extern string temp_directory;



*/
