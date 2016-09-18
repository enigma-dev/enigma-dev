/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
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

#define with(x) \
  for (enigma::iterator::with with(enigma::fetch_inst_iter_by_int(x)); \
      enigma::instance_event_iterator; \
      enigma::instance_event_iterator = enigma::instance_event_iterator->next)

//NOTE: This macro is ONLY to be used (in place of "with") for "room instance creation" code; that is, code which initializes a single instance
//      and is defined in the room editor. It does the same thing as "with", but checks instance_deactivated_list first.
#define with_room_inst(x) \
  for (enigma::iterator::with $E_with(enigma::fetch_roominst_iter_by_id(x)); \
      enigma::instance_event_iterator; \
      enigma::instance_event_iterator = enigma::instance_event_iterator->next)
