/** Copyright (C) 2008 Josh Ventura
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

#ifndef ENIGMA_INSTANCE_CREATE_H
#define ENIGMA_INSTANCE_CREATE_H

namespace enigma
{
  void instance_change_inst(int obj, bool perf, object_graphics* inst)
  {
      if (inst->object_index == obj) return; //prevents infinite loop

      double  x=inst->x, y=inst->y;
      double  xprevious=inst->xprevious, yprevious=inst->yprevious;
      double  xstart=inst->xstart, ystart=inst->ystart;
      double image_index=inst->image_index;
      double image_speed=inst->image_speed;
      double image_xscale=inst->image_xscale;
      double image_yscale=inst->image_yscale;
      double image_angle=inst->image_angle;
      vspeedv vspeed=inst->vspeed;
      hspeedv hspeed=inst->hspeed;

      //For reference, these properties should remain as default:
      //  visible,

      //the instance id is the same
      int idn = inst->id;

      // Destroy the instance
      instance_destroy(idn, perf);

      // Re-create the instance
      object_basic* ob = NULL;
      switch((int)obj)
      {
      #define NEW_OBJ_PREFIX ob =
          #include "Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h"
          default:
          #ifdef DEBUG_MODE
              DEBUG_MESSAGE("Object doesn't exist", MESSAGE_TYPE::M_USER_ERROR);
          #endif
              return;
      }
      (void)ob;

      object_graphics* newinst = (object_graphics*) (*fetch_inst_iter_by_int(idn));
      newinst->x=x; newinst->y=y; newinst->yprevious=yprevious; newinst->xprevious=xprevious;
      newinst->xstart=xstart; newinst->ystart=ystart;
      newinst->image_index=image_index; newinst->image_speed=image_speed;
      newinst->image_xscale=image_xscale; newinst->image_yscale=image_yscale; newinst->image_angle=image_angle;
      newinst->hspeed=hspeed; newinst->vspeed=vspeed;
      if (perf) newinst->myevent_create();
  }

  object_basic* instance_create_id(int x,int y,int object,int idn)
  { //This is for use by the system only. Please leave be.
    if (maxid < idn)
      maxid = idn;
    object_basic* ob;
    switch (object)
    {
        #define NEW_OBJ_PREFIX ob =
      #include "Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h"
      default:
          #ifdef DEBUG_MODE
          DEBUG_MESSAGE("Object doesn't exist", MESSAGE_TYPE::M_USER_ERROR);
          #endif
          return NULL;
    }
    return ob;
  }
} //namespace enigma

namespace enigma_user
{
  enigma::instance_t instance_create(int x,int y,int object)
  {
      int idn = enigma::maxid++;
    enigma::object_basic* ob;
      switch((int)object)
      {
        #define NEW_OBJ_PREFIX ob =
      #include "Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h"
      default:
          #ifdef DEBUG_MODE
          DEBUG_MESSAGE("Object doesn't exist", MESSAGE_TYPE::M_USER_ERROR);
          #endif
        return -1;
    }
    ob->myevent_create();
    return idn;
  }

  inline void action_change_object(int obj, bool perf) {instance_change(obj,perf);}

  void instance_change(int obj, bool perf) {
      enigma::object_graphics* inst = (enigma::object_graphics*) enigma::instance_event_iterator->inst;
      enigma::instance_change_inst(obj, perf, inst);
  }

  void instance_copy(bool perf)
  {
    enigma::object_graphics* inst = (enigma::object_graphics*) enigma::instance_event_iterator->inst;
    int idn = enigma::maxid++;

    enigma::object_basic* ob;
    ob = NULL;
    double x = inst->x, y = inst->y;
    switch((int)inst->object_index)
    {
    #define NEW_OBJ_PREFIX ob =
        #include "Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h"
        default:
          #ifdef DEBUG_MODE
          DEBUG_MESSAGE("Object doesn't exist", MESSAGE_TYPE::M_USER_ERROR);
          #endif
            (void)x; (void)y;
            return;
    }
    (void)ob;

    enigma::object_graphics* newinst = (enigma::object_graphics*) (*enigma::fetch_inst_iter_by_int(idn));
    if (perf) newinst->myevent_create();
    newinst->yprevious=inst->yprevious; newinst->xprevious=inst->xprevious;
    newinst->xstart=inst->xstart; newinst->ystart=inst->ystart;
    newinst->image_index=inst->image_index; newinst->image_speed=inst->image_speed;
    newinst->visible=inst->visible; newinst->image_xscale=inst->image_xscale; newinst->image_yscale=inst->image_yscale; newinst->image_angle=inst->image_angle;
    newinst->hspeed=inst->hspeed; newinst->vspeed=inst->vspeed;
  }
} //namespace enigma_user

#endif //ENIGMA_INSTANCE_CREATE_H
