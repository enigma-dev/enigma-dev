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

#include <memory>

#include "XLIBicon.h"

#include "Universal_System/Resources/sprites.h"
#include "Universal_System/Resources/sprites_internal.h"
#include "Universal_System/nlpo2.h"

#include <X11/Xatom.h>

namespace enigma {

void XSetIconFromSprite(Display *display, Window window, int ind, int subimg) {
  if (!enigma_user::sprite_exists(ind)) return;
  RawImage img = sprite_get_raw(ind, subimg);
  if (img.pxdata == nullptr) return;

  XSynchronize(display, true);
  Atom property = XInternAtom(display, "_NET_WM_ICON", true);

  unsigned
    widfull = nlpo2(img.w) + 1,
    hgtfull = nlpo2(img.h) + 1,
    ih, iw;

  const int bitmap_size = widfull * hgtfull * 4;
  unsigned char *bitmap = new unsigned char[bitmap_size]();

  unsigned i = 0;
  unsigned elem_numb = elem_numb = 2 + img.w * img.h;
  unsigned long *result = new unsigned long[elem_numb]();

  result[i++] = img.w;
  result[i++] = img.h;
  for (ih = 0; ih < img.h; ih++) {
    unsigned tmp = ih * widfull * 4;
    for (iw = 0; iw < img.w; iw++) {
      bitmap[tmp + 0] = img.pxdata[4 * img.w * ih + iw * 4 + 0];
      bitmap[tmp + 1] = img.pxdata[4 * img.w * ih + iw * 4 + 1];
      bitmap[tmp + 2] = img.pxdata[4 * img.w * ih + iw * 4 + 2];
      bitmap[tmp + 3] = img.pxdata[4 * img.w * ih + iw * 4 + 3];
      result[i++] = bitmap[tmp + 0] | (bitmap[tmp + 1] << 8) | (bitmap[tmp + 2] << 16) | (bitmap[tmp + 3] << 24);
      tmp += 4;
    }
  }

  XChangeProperty(display, window, property, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)result, elem_numb);
  XFlush(display);
  delete[] result;
  delete[] bitmap;
}

} // namespace enigma
