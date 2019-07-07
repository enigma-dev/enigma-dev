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

#include "Graphics_Systems/General/GSfont.h"  //string_width, font_height, draw_get_font
#include "Platforms/General/PFwindow.h"       //mouse_x, mouse_y
#include "common.h"
#include "parents.h"

#include <array>
#include <string>
#include <vector>

namespace enigma {
namespace gui {
class Textbox {
 public:
  rect box;
  std::vector<string> text{""};
  std::vector<double> text_width{0.0};

  std::array<int, 6> callback;  //Script to run on event

  unsigned int id;
  int state = 0;
  int parent_id =
      -1;  //ID of the parent of some kind (probably a window). It won't render with gui_draw_textboxes() if it is.

  int style_id = -1;  //The style we use
  int marker_style_id = -1;

  //Cursor position in chars and line
  size_t cursor_position = 0;
  size_t cursor_line = 0;
  size_t lines = 1;

  size_t line_limit = 2;    //How many lines
  size_t char_limit = 100;  //How many chars per line

  int blink_timer = 0;
  int repeat_timer = 0;
  int mouse_timer = 0;  //Double/triple clicks

  int mark_start_pos = 0;
  int mark_start_line = 0;
  int mark_end_pos = 0;
  int mark_end_line = 0;
  int mark_pos = 0;
  int mark_line = 0;

  bool double_click = false;
  bool mark = false;
  bool numbers_only = false;  //Meaning 0-9 and .
  bool visible = true;
  bool active = false;  //Is textbox is used

  //This is text offset for alignmets
  double textx = 0.0;
  double texty = 0.0;

  //Cursor Position in pixels relative to the widget
  double cursor_x = 0;
  double cursor_y = 0;

  Parent parenter;

  Textbox();
  //Update all possible textbox states (hover, click, type, etc.)
  void update(gs_scalar ox = 0, gs_scalar oy = 0, gs_scalar tx = enigma_user::mouse_x,
              gs_scalar ty = enigma_user::mouse_y);
  void draw(gs_scalar ox = 0, gs_scalar oy = 0);
  void update_text_pos();
  void callback_execute(int event);

  std::string get_text();

  void set_cursor(double x, double y);  //x/y in global pixels

  inline void update_cursorx() { cursor_x = enigma_user::string_width(text[cursor_line].substr(0, cursor_position)); }
  inline void update_cursory() {
    cursor_y = cursor_line * (double)enigma_user::font_height(enigma_user::draw_get_font());
  }
  inline void update_cursor() {
    update_cursorx();
    update_cursory();
  }

  inline void clear() {
    mark = false;
    text.clear();
    text.push_back("");
    cursor_line = 0;
    cursor_position = 0;
    update_cursor();
  }

  void set_marker(int line, int pos);

  inline void set_marker_start(int line, int pos) {
    mark_line = line;
    mark_pos = pos;
  }

  void marker_delete();
  std::string marker_string();
  void marker_insert(std::string str);
};
}  // namespace gui
}  //namespace enigma

#endif
