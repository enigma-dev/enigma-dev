/** Copyright (C) 2008 Josh Ventura
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

//TODO: Move most of this duplicate code into a Platforms/General/ source to reduce duplicate code for Mac/Linux/UNIX

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <string>
using namespace std;

/* UNIX-ready port of file manipulation */

enum {
  fa_readonly  = 1,
  fa_hidden    = 2,
  fa_sysfile   = 4,
  fa_volumeid  = 8,
  fa_directory = 16,
  fa_archive   = 32
};

string file_find_next();
void file_find_close();
bool file_attributes(string fname,int attr);

/*void export_include_file(string fname);
void export_include_file_location(string fname,string location);
void discard_include_file(string fname);*/

extern unsigned game_id;
extern string working_directory;
extern string program_directory;
extern string temp_directory;
