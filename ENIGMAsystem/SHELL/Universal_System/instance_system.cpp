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
#include <math.h>
#include <vector>
#include <string>
#include "var_cr3.h"
#include "reflexive_types.h"

#include "instance_system.h"

namespace enigma
{
  inst_iter::inst_iter(object_basic* i,inst_iter *n,inst_iter *p): inst(i), next(n), prev(p) {}
  
  /* **  Variables ** */
  // This will be instantiated for each event with a unique ID or Sub ID.
  event_iter *events; // It will be allocated towards the beginning.
  
  // Through these, we will list objects by object_index, and implement heredity.
  objectid_base *objects;
  
  // This is the all-inclusive, centralized list of instances.
  std::map<int,inst_iter*> instance_list;
  typedef std::map<int,inst_iter*>::iterator iliter;
  
  // When you say "global.vname", this is the structure that answers
  object_basic *ENIGMA_global_instance; // We also need an iterator for only global.
  inst_iter ENIGMA_global_instance_iterator(ENIGMA_global_instance,0,0);
  
  // This is basically a garbage collection list for when instances are destroyed
  std::vector<inst_iter*> cleanups; // We'll use vector
  
  // It's a good idea to centralize an event iterator so error reporting can tell where it is.
  inst_iter *instance_event_iterator; // Not bad for efficiency, either.
  
  /* **  Methods ** */
  // Retrieve the first instance on the complete list.
  inst_iter* instance_list_first()
  {
    iliter a = instance_list.begin();
    return a != instance_list.end() ? a->second : NULL;
  }
  
  object_basic* fetch_instance_by_int(int x)
  {
    if (x < 0) switch (x)
    {
      case self:
      case local:  return NULL;
      case other:  return NULL;
      case all: {  inst_iter *i = instance_list_first();
                   return  i ? i->inst : NULL; }
      case global: return ENIGMA_global_instance;
      case noone:
         default:  return NULL;
    }
    
    if (x < 100000)
      return objects[x].insts->inst;
    iliter a = instance_list.find(x);
    return a != instance_list.end() ? a->second->inst : NULL;
  }
  inst_iter* fetch_inst_iter_by_int(int x)
  {
    if (x < 0) switch (x) // Keyword-based lookup
    {
      case self:
      case local:  return NULL;
      case other:  return NULL;
      case all:    return instance_list_first();
      case global: return &ENIGMA_global_instance_iterator;
      case noone:
         default:  return NULL;
    }
    
    if (x < 100000) // Object-index-based lookup
      return objects[x].insts;
    
    // ID-based lookup
    iliter a = instance_list.find(x);
    return a != instance_list.end() ? a->second : NULL;
  }
  inst_iter* fetch_inst_iter_by_id(int x)
  {
    if (x < 100000)
      return NULL;
    
    iliter a = instance_list.find(x);
    return a != instance_list.end() ? a->second : NULL;
  }
  
  
  //Link in an instance
  void link_instance(object_basic* who)
  {
    
  }
  
  
  //This is the universal create event code
  void constructor(object_basic* instance);
  void step_basic(object_basic* instance);
}
