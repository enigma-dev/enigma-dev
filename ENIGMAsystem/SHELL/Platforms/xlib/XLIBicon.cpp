/** Copyright (C) 2018-2019 Robert B. Colton
*** Copyright (C) 2019 Samuel Venable
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

#include "XLIBicon.h"

#include "Graphics_Systems/graphics_mandatory.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/Resources/sprites_internal.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <X11/Xatom.h>

namespace enigma {

void XSetIconFromSprite(Display *display, Window window, int ind, unsigned subimg) {
  sprite *spr; if (!get_sprite_mtx(spr, ind)) return;  
  XSynchronize(display, True);
  Atom property = XInternAtom(display, "_NET_WM_ICON", False);
  unsigned char *data = nullptr; unsigned pngwidth, pngheight;
  data = graphics_copy_texture_pixels(spr->texturearray[subimg], &pngwidth, &pngheight);
  unsigned elem_numb = 2 + pngwidth * pngheight;
  unsigned long *result = bgra_to_argb(data, pngwidth, pngheight);
  XChangeProperty(display, window, property, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)result, elem_numb);
  XFlush(display);
  delete[] result;
  delete[] data;
}

} // namespace enigma
