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

#ifndef BGUI_TEXTBOXES_H
#define BGUI_TEXTBOXES_H
#include <array>
#include <string>
#include <vector>
using std::array;
using std::string;
using std::vector;

#include "common.h"
#include "parents.h"

namespace enigma_user
{
  extern double mouse_x, mouse_y;
}

namespace gui
{
  class Textbox{
    public:
      unsigned int id;
      rect box;
      vector<string> text{""};

      int state = 0;
      bool visible = true;
      bool active = false; //Is textbox is used
      array<int,4> callback; //Script to run on event

      //Cursor position in chars and line
      int cursor_position = 0;
      int cursor_line = 0; 
      int lines = 1;
      int blink_timer = 0;

      int repeat_timer = 0;

      //Cursor Position in pixels relative to the widget
      double cursor_x = 0;
      double cursor_y = 0;

      int parent_id = -1; //ID of the parent of some kind (probably a window). It won't render with gui_draw_textboxes() if it is.

      int style_id = -1; //The style we use

      Textbox();
      //Update all possible textbox states (hover, click, type, etc.)
      void update(gs_scalar ox = 0, gs_scalar oy = 0, gs_scalar tx = enigma_user::mouse_x, gs_scalar ty = enigma_user::mouse_y);
      void draw(gs_scalar ox = 0, gs_scalar oy = 0);
      void update_text_pos(int state = -1);
      void callback_execute(int event);

      Parent parenter;
  };
}

#endif
