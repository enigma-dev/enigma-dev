/** Copyright (C) 2008, 2012 Josh Ventura
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

namespace enigma_user
{

// File attribute constants
enum {
  fa_readonly  = 1,
  fa_hidden    = 2,
  fa_sysfile   = 4,
  fa_volumeid  = 8,
  fa_directory = 16,
  fa_archive   = 32
};

long long file_size(std::string fname);
time_t file_access_time(std::string fname);
time_t file_modified_time(std::string fname);

int file_exists(std::string fname);
int file_delete(std::string fname);
int file_rename(std::string oldname,std::string newname);
int file_copy(std::string fname,std::string newname);
int directory_exists(std::string dname);
int directory_create(std::string dname);
int directory_destroy(std::string dname);
#define directory_delete directory_destroy
int directory_rename(std::string oldname,std::string newname);
int directory_copy(std::string dname,std::string newname);

std::string file_find_next();
void file_find_close();
std::string file_find_first(std::string mask,int attr);

bool file_attributes(std::string fname,int attributes);

/*void export_include_file(std::string fname);
void export_include_file_location(std::string fname,std::string location);
void discard_include_file(std::string fname);*/

extern unsigned game_id;
extern std::string working_directory;
extern std::string program_directory;
extern std::string temp_directory;

}
