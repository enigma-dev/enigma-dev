/** Copyright (C) 2008-2011 Josh Ventura
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

#include <map>
#include <math.h>
#include <string>
#include "var4.h"
#include "reflexive_types.h"

#include "object.h"
#include "../libEGMstd.h"

namespace enigma
{
    extern int maxid;
    int instancecount = 0;
    int id_current =0;

    double newinst_x, newinst_y;
    int newinst_obj, newinst_id;

    void object_basic::unlink() {}
    variant object_basic::myevent_draw()    { return 0; }
    variant object_basic::myevent_roomend() { return 0; }
    variant object_basic::myevent_destroy() { return 0; }

    object_basic::object_basic(): id(0), object_index(-4) {}
    object_basic::object_basic(int uid, int uoid): id(uid), object_index(uoid) {}
    object_basic::~object_basic() {}
}

/*
extern int object_idmax;

#ifdef DEBUG_MODE
  #include "../Widget_Systems/widgets_mandatory.h"
  #define get_object(obj,id) \
    if (id < -1 or size_t(id) > enigma::object_idmax or !enigma::objectdata[id]) { \
      show_error("Cannot access object with id " + toString(id), false); \
      return 0; \
    } const enigma::object *const obj = enigma::objectdata[id];
#else
  #define get_object(obj,id) \
    const enigma::object *const obj = enigma::objectdata[id];
#endif

bool object_exists(unsigned objectid)
{
    return objectid < enigma::object_idmax && enigma::objectdata[objectid];
}

int object_get_depth(unsigned id)
{
    get_object(obj,id);
    return obj.depth;
}

int object_get_mask(unsigned id)
{
    get_object(obj,id);
    return obj.maskId;
}

int object_get_parent(unsigned id)
{
    get_object(obj,id);
    return obj.parentId;
}

bool object_get_persistent(unsigned id)
{
    get_object(obj,id);
    return obj.persistent;
}

bool object_get_solid(unsigned id)
{
    get_object(obj,id);
    return obj.solid;
}

bool object_get_visible(unsigned id)
{
    get_object(obj,id);
    return obj.visible;
}*/
