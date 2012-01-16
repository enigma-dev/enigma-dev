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

#include <string>
using namespace std;
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/instance_system.h"
#include <cstdlib>
#include <cstdio>

#include "GameSettings.h"

namespace enigma
{
  bool widget_system_initialize()
  {
    return 0;
  }
}

void show_error(string err, const bool fatal)
{
  printf("ERROR in some action of some event for object %d, instance id %d: %s\n",
         (enigma::instance_event_iterator == NULL? global :
            enigma::instance_event_iterator->inst == NULL? noone :
              enigma::instance_event_iterator->inst->object_index),
         (enigma::instance_event_iterator == NULL? global :
            enigma::instance_event_iterator->inst == NULL? noone :
              enigma::instance_event_iterator->inst->id),
         err.c_str()
    );
  if (fatal) exit(0);
  ABORT_ON_ALL_ERRORS();
}

