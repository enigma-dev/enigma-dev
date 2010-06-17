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

#include "object.h"

namespace enigma
{
  struct inst_iter
  {
    object_basic* inst;     // Inst is first member for non-arithmetic dereference
    inst_iter *next, *prev; // Double linked for active removal
    bool dead;              // Whether or not this instance has been destroyed. Should be accessed rarely.
    inst_iter(object_basic* i,inst_iter *n,inst_iter *p);
  };
  
  // This structure is for composing lists of events to execute.
  struct event_iter
  {
    inst_iter *insts; // Instances for which to perform this event
    inst_iter *last;  // The last instance for which to perform it. (Both of these can be NULL)
    std::string name; // Event name
    void add_inst(object_basic* inst);  // Append an instance to the list
    event_iter();
  };
  
  // This structure will store info about and lists of each object by index.
  struct objectid_base
  {
    inst_iter *insts; // First instance on the list
    inst_iter *last;  // Last instance on the list  (Both of these can be NULL)
    size_t count;     // Number of instances on this list
    void add_inst(object_basic* inst);  // Append an instance to the list
    objectid_base();
  };
  
  // This is an iterator typedef for use with the red-black backbone of the instance list.
  typedef std::map<int,inst_iter*>::iterator instance_list_iterator;
  
  // The rest is documented by comment in the corresponding source file.
  extern event_iter *events;
  extern objectid_base *objects;
  extern std::map<int,inst_iter*> instance_list;
  extern object_basic *ENIGMA_global_instance;
  extern inst_iter *instance_event_iterator;
  extern std::vector<inst_iter*> cleanups;
  
  inst_iter*    instance_list_first();
  inst_iter*    fetch_inst_iter_by_id(int id);
  inst_iter*    fetch_inst_iter_by_int(int x);
  object_basic* fetch_instance_by_int(int x);
  
  void link_instance(object_basic* who);
  void link_obj_instance(object_basic* who, int oid);
}
