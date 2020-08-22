/** Copyright (C) 2018 Robert B. Colton
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

#include "project.pb.h"
#include "event_reader/event_parser.h"

#include <iostream>
#include <streambuf>
#include <string>

namespace gmk {
extern std::ostream out;
extern std::ostream err;

inline void bind_output_streams(std::ostream &out, std::ostream &err) {
  gmk::out.rdbuf(out.rdbuf());
  gmk::err.rdbuf(err.rdbuf());
}

std::unique_ptr<buffers::Project> LoadGMK(std::string fName, const EventData* event_data);

}  //namespace gmk
