/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Josh Ventura                                             **
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

/// While the code that manages tiles and drawing is to be declared and managed
/// by the files under Graphics_Systems, this file exists to provide a way to
/// structure layers of depth, for both tiles and instances.
#ifdef INCLUDED_FROM_SHELLMAIN
  #error This file is high-impact and should not be included from SHELLmain.cpp.
#endif

#ifndef ENIGMA_DEPTH_DRAW_H
#define ENIGMA_DEPTH_DRAW_H

#include "Instances/instance_system.h"
#include "roomsystem.h"

#include <map>
#include <set>
#include <vector>

namespace enigma
{
struct depth_layer
{
  std::vector<tile> tiles;
  event_iter* draw_events;

  depth_layer();
};

extern std::map<double,depth_layer> drawing_depths;
extern std::map<int,std::pair<double,double> > id_to_currentnextdepth;
typedef std::map<double,depth_layer>::reverse_iterator diter;

} //namespace enigma

#endif
