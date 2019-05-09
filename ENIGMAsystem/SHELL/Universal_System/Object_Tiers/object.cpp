/** Copyright (C) 2008-2011 Josh Ventura
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


#include "var4.h"
#include "reflexive_types.h"
#include "object.h"
#include "libEGMstd.h"

#ifdef DEBUG_MODE
  #include "instance_system.h"
  #include "resource_data.h" // TODO: We don't guarantee these functions exist. But they're useful for debugging. We need a debug namespace that offers this, too.
  #include <Widget_Systems/widgets_mandatory.h> // show_error
#endif

#include <map>
#include <math.h>
#include <string>
#include <vector>

namespace enigma
{
    extern int maxid;
    objectstruct** objectdata;
    int id_current =0;

    #ifdef DEBUG_MODE
      using enigma_user::show_error;
      static inline int DEBUG_ID_CHECK(int id, int objind) {
        std::map<int, inst_iter*>::iterator it = instance_list.find(id);
        if (it != instance_list.end()) {
          show_error("Two instances were given the same ID! Object `" + enigma_user::object_get_name(it->second->inst->object_index)
                     + "' and new object `" + enigma_user::object_get_name(objind)
                     + "' both have ID " + toString(id)
                     + "': A new ID has been assigned so the game can continue, but references by this ID may fail."
                     " This bug is likely caused by the IDE, and can be worked around by using the 'Defrag IDs'"
                     " feature to assign new, ordered IDs to all instances.", false);
          return maxid++;
        }
        return id;
      }
    #else
    # define DEBUG_ID_CHECK(x, y) (x)
    #endif

    double newinst_x, newinst_y;
    int newinst_obj, newinst_id;

    void object_basic::unlink()     {}
    void object_basic::deactivate() {}
    void object_basic::activate()   {}
    variant object_basic::myevent_create()    { return 0; }
    variant object_basic::myevent_gamestart() { return 0; }
    variant object_basic::myevent_gameend() { return 0; }
    variant object_basic::myevent_closebutton() { return 0; }
    variant object_basic::myevent_roomstart()   { return 0; }
    variant object_basic::myevent_roomend()   { return 0; }
    variant object_basic::myevent_destroy()   { return 0; }

    object_basic::object_basic(): id(-4), object_index(-4) {}
    object_basic::object_basic(int uid, int uoid): id(DEBUG_ID_CHECK(uid, uoid)), object_index(uoid) {}
    object_basic::~object_basic() {}
    bool object_basic::can_cast(int obj) const { return false; }

    extern std::vector<objectstruct> objs;
    extern size_t object_idmax;

    void objectdata_load()
    {
        objectdata = new objectstruct*[object_idmax];
        for (int i = 0; i < objectcount; i++)
            objectdata[objs[i].id] = &objs[i];
    }
}

#if defined(SHOW_ERRORS) && SHOW_ERRORS
  #define errcheck(objid,err) \
  if (objid < 0 or size_t(objid) >= enigma::object_idmax or !enigma::objectdata[objid]) \
    return (show_error(err,0), 0)
  #define errcheck(objid,err) \
  if (objid < 0 or size_t(objid) >= enigma::object_idmax or !enigma::objectdata[objid]) \
    show_error(err,0)
#else
  #define errcheck(objid,err)
  #define errcheck_v(objid,err)
#endif

namespace enigma_user
{

bool object_exists(int objid)
{
  return ((objid >= 0) && (size_t(objid) < enigma::object_idmax) && bool(enigma::objectdata[objid]));
}

void object_set_depth(int objid, int val)
{
  errcheck_v(objid,"Object doesn't exist");
  enigma::objectdata[objid]->depth = val;
}

void object_set_mask(int objid, int val)
{
  errcheck_v(objid,"Object doesn't exist");
  enigma::objectdata[objid]->mask = val;
}

void object_set_persistent(int objid, bool val)
{
  errcheck_v(objid,"Object doesn't exist");
  enigma::objectdata[objid]->persistent = val;
}

void object_set_solid(int objid, bool val)
{
  errcheck_v(objid,"Object doesn't exist");
  enigma::objectdata[objid]->solid = val;
}

void object_set_sprite(int objid, int val)
{
  errcheck_v(objid,"Object doesn't exist");
  enigma::objectdata[objid]->sprite = val;
}

void object_set_visible(int objid, bool val)
{
  errcheck_v(objid,"Object doesn't exist");
  enigma::objectdata[objid]->visible = val;
}

int object_get_depth(int objid)
{
  errcheck(objid,"Object doesn't exist");
  return enigma::objectdata[objid]->depth;
}

int object_get_mask(int objid)
{
  errcheck(objid,"Object doesn't exist");
  return enigma::objectdata[objid]->mask;
}

int object_get_parent(int objid)
{
  errcheck(objid,"Object doesn't exist");
  return enigma::objectdata[objid]->parent;
}

bool object_get_persistent(int objid)
{
  errcheck(objid,"Object doesn't exist");
  return enigma::objectdata[objid]->persistent;
}

bool object_get_solid(int objid)
{
  errcheck(objid,"Object doesn't exist");
  return enigma::objectdata[objid]->solid;
}

int object_get_sprite(int objid)
{
  errcheck(objid,"Object doesn't exist");
  return enigma::objectdata[objid]->sprite;
}

bool object_get_visible(int objid)
{
  errcheck(objid,"Object doesn't exist");
  return enigma::objectdata[objid]->visible;
}

bool object_is_ancestor(int objid, int acid)
{
  errcheck(objid,"Object doesn't exist");
  errcheck(acid,"Anchestor id doesn't exist");
  while (objid >= 0)
    if (objid == acid)
      return true;
    else objid = enigma::objectdata[objid]->parent;
  return false;
}

}
