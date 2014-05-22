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

/**
  @file    object.h
  @summary This file outlines the most basic local variables needed by different systems
*/

#ifndef _object_h
#define _object_h

namespace enigma_user {
  enum {
    self =   -1,
    other =  -2,
    all =    -3,
    noone =  -4,
    global = -5,
    local =  -7
  };
}

#include "var4.h"

namespace enigma
{
    extern int maxid;
    extern int instancecount;
    extern int id_current;
    extern int objectcount;

    extern double newinst_x, newinst_y;
    extern int newinst_obj, newinst_id;

    struct object_basic
    {
      const unsigned id;
      const int object_index;

      virtual void unlink();
      virtual void deactivate();
      virtual void activate();
      virtual variant myevent_create();
      virtual variant myevent_gamestart();
      virtual variant myevent_gameend();
	  virtual variant myevent_closebutton();
	  virtual variant myevent_asyncdialog();
	  virtual variant myevent_asynchttp();
	  virtual variant myevent_asyncimageloaded();
	  virtual variant myevent_asyncsoundloaded();
	  virtual variant myevent_asyncnetworking();
	  virtual variant myevent_asynciap();
	  virtual variant myevent_asynccloud();
	  virtual variant myevent_asyncsteam();
	  virtual variant myevent_asyncsocial();
	  virtual variant myevent_asyncpushnotification();
      virtual variant myevent_draw();
      virtual bool myevent_draw_subcheck();
	  virtual variant myevent_drawgui();
    virtual bool myevent_drawgui_subcheck();
	  virtual variant myevent_drawresize();
	  virtual variant myevent_roomstart();
      virtual variant myevent_roomend();
      virtual variant myevent_destroy();

      object_basic();
      object_basic(int uid, int uoid);
      virtual ~object_basic();
    };

    struct objectstruct
    {
        int sprite;
        bool solid, visible;
        double depth;
        bool persistent;
        double mask;
        double parent;
        int id;
    };
    void objectdata_load();
    void constructor(object_basic* instance);
}

namespace enigma_user {
bool object_exists(int objid);
void object_set_depth(int objid, int val);
void object_set_mask(int objid, int val);
void object_set_persistent(int objid, bool val);
void object_set_solid(int objid, bool val);
void object_set_sprite(int objid, int val);
void object_set_visible(int objid, bool val);
int object_get_depth(int objid);
int object_get_mask(int objid);
int object_get_parent(int objid);
bool object_get_persistent(int objid);
bool object_get_solid(int objid);
int object_get_sprite(int objid);
bool object_get_visible(int objid);
bool object_is_ancestor(int objid, int acid);
}

#endif //_object_h
