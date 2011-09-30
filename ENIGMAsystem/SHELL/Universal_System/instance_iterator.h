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

#ifndef _INSTANCE_ITERATOR_H
#define _INSTANCE_ITERATOR_H

namespace enigma {
  struct iterator {
    struct inst_iter* it;
    bool temp;
    
    operator bool();
    object_basic* operator*();
    object_basic* operator->();
    
    iterator &operator++();
    iterator operator++(int);
    iterator &operator--();
    iterator operator--(int);
    
    iterator &operator=(iterator&);
    iterator &operator=(const iterator&);
    iterator &operator=(inst_iter*);
    iterator &operator=(object_basic*);
    
    iterator(iterator&);
    iterator(const iterator&);
    iterator(inst_iter*,bool=false);
    iterator(object_basic*);
    iterator();
    
    ~iterator();
    
    private: void addme();
  };
  
  void update_iterators_for_destroy(const iterator&);
  iterator instance_list_first();
  iterator fetch_inst_iter_by_id(int id);
  iterator fetch_inst_iter_by_int(int x);
}

#endif
