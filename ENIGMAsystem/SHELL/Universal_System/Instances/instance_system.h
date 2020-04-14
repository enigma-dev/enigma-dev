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

#ifdef INCLUDED_FROM_SHELLMAIN
#error This file is high-impact and should not be included from SHELLmain.cpp.
#endif

#ifndef ENIGMA_INSTANCE_SYSTEM_H
#define ENIGMA_INSTANCE_SYSTEM_H

#include "instance_iterator.h"
#include "Universal_System/Object_Tiers/object.h"
#include "Universal_System/reflexive_types.h"
#include "Universal_System/var4.h"

#include <map>
#include <set>

namespace enigma {

typedef std::map<int, inst_iter*>::iterator instance_list_iterator;
extern std::map<int, inst_iter*> instance_list;
extern std::map<int, object_basic*> instance_deactivated_list;
extern std::set<object_basic*> cleanups;
void unlink_main(instance_list_iterator who);

}  //namespace enigma

#endif  //ENIGMA_INSTANCE_SYSTEM_H
