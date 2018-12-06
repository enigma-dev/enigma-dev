/** Copyright (C) 2011 Josh Ventura
*** Copyright (C) 2014 Seth N. Hetu
*** Copyright (C) 2018 Robert Colton
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

#include "codegen/project.pb.h"

#include <string>

int game_write_assets(const buffers::Game& game, bool append, const std::string& gameFname);
void game_launch(const char* exe_filename, const char* proj_filename, int mode);
