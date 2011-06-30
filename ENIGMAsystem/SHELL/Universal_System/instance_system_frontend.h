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

#include "instance_system_base.h"

namespace enigma
{
  // This is an iterator typedef for use with the red-black backbone of the instance list.
  typedef struct winstance_list_iterator *pinstance_list_iterator;
  void winstance_list_iterator_delete(pinstance_list_iterator);

  // Linking
  pinstance_list_iterator link_instance(object_basic* who);
  inst_iter *link_obj_instance(object_basic* who, int oid);

  // Unlinking/Destroying
  void instance_iter_queue_for_destroy(pinstance_list_iterator whop);
  void dispose_destroyed_instances();
  void unlink_main(pinstance_list_iterator);
  void unlink_object_id_iter(inst_iter*,int);
}
