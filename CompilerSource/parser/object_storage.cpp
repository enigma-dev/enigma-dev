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

#include <map>
#include <string>

using namespace std;

#include "../general/darray.h"

#include "object_storage.h"


dectrip::dectrip(): type(), prefix(), suffix() {}
dectrip::dectrip(string t): type(t), prefix(), suffix() {}
dectrip::dectrip(string t,string p, string s): type(t), prefix(p), suffix(s) {}

parsed_event::parsed_event(): id(0), mainId(0), otherObjId(-4) {}
parsed_event::parsed_event(int m, int s): id(s), mainId(m), otherObjId(-4) {}
parsed_object::parsed_object() {}
parsed_object::parsed_object(string n, int i, int s): name(n), id(i), sprite_index(s) {}
map<int,parsed_object*> parsed_objects;
