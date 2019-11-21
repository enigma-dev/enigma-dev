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

#ifndef ENIGMA_GSSCREEN_H
#define ENIGMA_GSSCREEN_H

#include "Universal_System/scalar.h"

#include <string>
using std::string;

namespace enigma {
  extern unsigned gui_width;
  extern unsigned gui_height;

  void scene_begin();
  void scene_end();

  void ScreenRefresh(); // implemented by bridges
}

namespace enigma_user {
  int screen_save(string filename);
  inline int action_snapshot(string filename)
  {
      return screen_save(filename);
  }
  int screen_save_part(string filename,unsigned int x,unsigned int y,unsigned int w,unsigned int h);
  void screen_redraw();
  void screen_refresh();
  void screen_init();
  void screen_set_viewport(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height);
  void screen_reset_viewport();

  unsigned int display_get_gui_width();
  unsigned int display_get_gui_height();
  void display_set_gui_size(unsigned int width, unsigned int height);
}

#endif //ENIGMA_GSSCREEN_H
