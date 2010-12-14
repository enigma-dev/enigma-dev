/********************************************************************************\
**                                                                              **
**  Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>                          **
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

bool is_solid(const enigma::object_collisions *obj) {
 return true;
}

bool place_free(double x,double y) {
 enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
 const int ox1 = (int)x,           oy1 = (int)y,
           bl1 = inst1->bbox_left, br1 = inst1->bbox_right,
           bt1 = inst1->bbox_top,  bb1 = inst1->bbox_bottom;
 for (enigma::inst_iter *it = enigma::instance_list_first(); it != NULL; it = it->next) {
  const enigma::object_collisions* inst2 = (enigma::object_collisions*)it->inst;
  if (!is_solid(inst2)) continue;
  const int ox2 = (int)inst2->x,    oy2 = (int)inst2->y,
            bl2 = inst2->bbox_left, br2 = inst2->bbox_right,
            bt2 = inst2->bbox_top,  bb2 = inst2->bbox_bottom;
  if (ox1+bl1<ox2+br2 && ox1+br1>ox2+bl2
  &&  oy1+bt1<oy2+bb2 && oy1+bb1>oy2+bt2)
   return false;
 }
 return true;
}

bool place_empty(double x,double y) {
 enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
 const int ox1 = (int)x,           oy1 = (int)y,
           bl1 = inst1->bbox_left, br1 = inst1->bbox_right,
           bt1 = inst1->bbox_top,  bb1 = inst1->bbox_bottom;
 for (enigma::inst_iter *it = enigma::instance_list_first(); it != NULL; it = it->next) {
  const enigma::object_collisions* inst2 = (enigma::object_collisions*)it->inst;
  const int ox2 = (int)inst2->x,    oy2 = (int)inst2->y,
            bl2 = inst2->bbox_left, br2 = inst2->bbox_right,
            bt2 = inst2->bbox_top,  bb2 = inst2->bbox_bottom;
  if (ox1+bl1<ox2+br2 && ox1+br1>ox2+bl2
  &&  oy1+bt1<oy2+bb2 && oy1+bb1>oy2+bt2)
   return false;
 }
 return true;
}

bool place_meeting(double x, double y, int object) {
 enigma::object_collisions* const inst = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
 const int ox = (int)x,          oy = (int)y,
           bl = inst->bbox_left, br = inst->bbox_right,
           bt = inst->bbox_top,  bb = inst->bbox_bottom;
 return collision_bbox_rect(object,ox+bl,oy+bt,ox+br,oy+bb);
}

