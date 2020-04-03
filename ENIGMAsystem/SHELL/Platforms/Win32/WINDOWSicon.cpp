/** Copyright (C) 2020 Samuel Venable
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

// Josh will probably gaze at this code lazily and think he has a better way to do it
// but not realize we are dependent on converting the char array to an HICON datatype

#include "WINDOWSicon.h"

#include "Graphics_Systems/graphics_mandatory.h"
#include "Universal_System/Resources/sprites_internal.h"
#include "Widget_Systems/widgets_mandatory.h"

namespace enigma {

void SetIconFromSprite(HWND window, int ind, unsigned subimg) {
  sprite *spr; if (!get_sprite_mtx(spr, ind)) return;

  unsigned char *data = nullptr; unsigned pngwidth, pngheight;
  data = graphics_copy_texture_pixels(spr->texturearray[subimg], &pngwidth, &pngheight);
  HBITMAP hBitmap = CreateBitmap(pngwidth, pngheight, 1, 32, data);

  ICONINFO iconinfo;
  iconinfo.fIcon = TRUE;
  iconinfo.xHotspot = 0;
  iconinfo.yHotspot = 0;
  iconinfo.hbmMask = hBitmap;
  iconinfo.hbmColor = hBitmap;
  HICON hIcon = CreateIconIndirect(&iconinfo);
  PostMessage(window, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

  DeleteObject(hIcon);
  DeleteObject(hBitmap);
}

} // namespace enigma
