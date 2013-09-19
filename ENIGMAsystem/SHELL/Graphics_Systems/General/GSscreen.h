/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

#include <string>
using std::string;

namespace enigma_user {
  int screen_save(string filename);
  inline int action_snapshot(string filename)
  {
      return screen_save(filename);
  }
  int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h);
  void screen_redraw();
  void screen_refresh();
  void screen_init();
  
  unsigned display_get_gui_width();
  unsigned display_get_gui_height();
  void display_set_gui_size(unsigned width, unsigned height);
}