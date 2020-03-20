/** Copyright (C) 2014 Josh Ventura
*** Copyright (C) 2014 Seth N. Hetu
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

/**
  @file  timelines_object.h
  @brief Adds a timelines tier following the main tier.
*/

//FIXME: this should be ifdef shellmain but enigmas in a sorry state
#ifdef JUST_DEFINE_IT_RUN
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_TIMELINES_OBJECT_H
#define ENIGMA_TIMELINES_OBJECT_H

#include "planar_object.h"
#include "Universal_System/var4.h"
#include "Universal_System/scalar.h"

#include <vector>
#include <map>

namespace enigma
{
  struct object_timelines : object_planar
  {
    //Used as a global lookup for timeline moments. Filled at runtime.
    //vector is indexed by timeline_id. map::key is moment_time; map::value is moment_id
    static std::vector< std::map<int, int> > timeline_moments_maps;

    //Timeline properties.
    int timeline_index;    //-1 means "no timeline running"
    bool timeline_running; //True if running, False if not. Setting to True again will continue execution; it's more like a "pause" button.
    gs_scalar timeline_speed; //Can be set to fractions, negative, zero, etc. Defaults to 1.
    gs_scalar timeline_position; //How far along "time" is in this timeline. Bounded by [0,lastMoment)
    bool timeline_loop; //Allows looping from lastMoment->0 and vice versa.

    //Constructors
    object_timelines();
    object_timelines(unsigned x, int y);
    virtual ~object_timelines();

    //Object-local timelines functionality.
    void advance_curr_timeline();
    void loop_curr_timeline();
    virtual void timeline_call_moment_script(int timeline_index, int moment_index) {} //This will be provided by the object_locals subclass in compiled code.
  };
} //namespace enigma

#endif //ENIGMA_TIMELINES_OBJECT_H
