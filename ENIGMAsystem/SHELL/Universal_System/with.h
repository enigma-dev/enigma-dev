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

#define with(x) for (enigma::with_iter ENIGMA_WITHITER(enigma::fetch_inst_iter_by_int(x),enigma::instance_event_iterator->inst); \
enigma::instance_event_iterator; enigma::instance_event_iterator = enigma::instance_event_iterator->next)

namespace enigma
{
  struct with_iter
  {
    iterator_level *my_il;
    iterator my_iterator;
    with_iter(enigma::iterator nt, object_basic* other): my_iterator(nt)
    {
      il_top = my_il = new iterator_level(instance_event_iterator,instance_other,il_top);
      instance_event_iterator = nt.it;
      instance_other = other;
    }
    ~with_iter()
    {
      il_top = my_il->last;
      instance_event_iterator = my_il->it;
      instance_other = my_il->other;
      delete my_il;
    }
  };
}
