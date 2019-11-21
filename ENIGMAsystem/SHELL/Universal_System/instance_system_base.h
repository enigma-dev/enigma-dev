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

#ifndef _INSTANCE_SYSTEM_BASE__H
#define _INSTANCE_SYSTEM_BASE__H

#include "instance_iterator.h"

namespace enigma
{
  typedef int instance_t;

  struct inst_iter
  {
    object_basic* inst;     // Inst is first member for non-arithmetic dereference
    inst_iter *next, *prev; // Double linked for active removal
    bool dead;              // Whether or not this instance has been destroyed. Should be accessed rarely.
    //std::deque<inst_iter*>::iterator instance_id_index;
    inst_iter(object_basic* i,inst_iter *n,inst_iter *p);
  };

  class temp_event_scope
  {
    object_basic *oinst;
    inst_iter *oiter;
    object_basic* prev_other; //Should always be NULL, but just in case...
    
    public:
    temp_event_scope(object_basic*);
    ~temp_event_scope();
  };

  // This structure is for composing lists of events to execute.
  struct event_iter: inst_iter // It is a special instance iterator with an add_inst member and a name.
  {
    // Inherits object_basic *inst: should be NULL
    // Inherits inst_iter *next:    First of instances for which to perform this event (Can be NULL)
    // Inherits inst_iter *prev:    The last instance for which to perform it. (Can be NULL)
    string name; // Event name
    inst_iter *add_inst(object_basic* inst);  // Append an instance to the list
    void unlink(inst_iter*);
    event_iter(string name);
    event_iter();
  };

  // This structure will store info about and lists of each object by index.
  struct objectid_base: inst_iter
  {
    // Inherits object_basic *inst: should be NULL
    // Inherits inst_iter *next:    First of instances for which to perform this event (Can be NULL)
    // Inherits inst_iter *prev:    The last instance for which to perform it. (Can be NULL)
    size_t count;     // Number of instances on this list
    inst_iter *add_inst(object_basic* inst);  // Append an instance to the list
    objectid_base();
  };

  // Centralized stack of iterators in use by with() statements and the like.
  // Never empty; always contains a pointer to the instance_event_iterator.
  struct iterator_level {
    inst_iter* it;
    object_basic* other;
    iterator_level *last;
    iterator_level(inst_iter*,object_basic* oth,iterator_level*);
    void push(inst_iter*,object_basic* oth);
    void pop();
  };
  extern iterator_level *il_base, *il_top;

  // The rest is decently commented on in the corresponding source file.
  extern event_iter *events;
  extern objectid_base *objects;
  extern object_basic *ENIGMA_global_instance;
  extern inst_iter *instance_event_iterator;
  extern object_basic *instance_other;
/* INSTANTLY ANTIQUATED
  inst_iter*    instance_list_first();
  inst_iter*    fetch_inst_iter_by_id(int id);
  inst_iter*    fetch_inst_iter_by_int(int x);
*/
  object_basic* fetch_instance_by_int(int x);
  object_basic* fetch_instance_by_id(int x);
}

// Other
namespace enigma_user {
  extern int instance_count;
}
#endif
