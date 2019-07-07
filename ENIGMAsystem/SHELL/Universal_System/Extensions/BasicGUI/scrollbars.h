/** Copyright (C) 2014-2015 Harijs Grinbergs
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

#ifndef BGUI_SCROLLBAR_H
#define BGUI_SCROLLBAR_H

#include "Platforms/General/PFwindow.h"  //mouse_x, mouse_y
#include "common.h"

#include <array>

namespace enigma {
namespace gui {
class Scrollbar {
 public:
  unsigned int id;
  rect box;
  rect indicator_box;
  int state = 0;
  bool visible = true;
  bool active = false;  //Is scroll pressed
  bool drag = false;
  bool direction = false;       //false - horizontal, true - vertical
  std::array<int, 4> callback;  //Script to run on event

  gs_scalar drag_offset = 0.0;

  double value = 0.0;  //Slider return value
  double minValue = 0.0;
  double maxValue = 1.0;
  double incValue = 0.1;
  double size = 0.1;  //Size of the scroll region in terms of abs(max-min)

  gs_scalar scroll_offset = 0.0;

  int parent_id =
      -1;  //ID of some kind of parent (probably a window). It won't render with gui_draw_scrollbars() if it is not -1.

  int style_id = -1;
  int indicator_style_id = -1;

  Scrollbar();
  //Update all possible scroll states (hover, click etc.)
  void update_spos();  //Updates scroll position calculations, needed for things like scrollbar_set_value
  void update(gs_scalar ox = 0, gs_scalar oy = 0, gs_scalar tx = enigma_user::mouse_x,
              gs_scalar ty = enigma_user::mouse_y);
  void draw(gs_scalar ox = 0, gs_scalar oy = 0);
  void callback_execute(int event);
};
}  //namespace gui
}  //namespace enigma

#endif
