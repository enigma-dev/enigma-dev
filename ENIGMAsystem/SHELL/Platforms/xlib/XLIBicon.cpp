/** Copyright (C) 2019 Samuel Venable
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
#include <X11/Xatom.h>

#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/nlpo2.h"

#include "libpng-util.h"
#include <png.h>

#include <string>
using std::string;

namespace enigma {

void XSetIcon(Display *display, Window window, const char *icon) {
  XSynchronize(display, True);
  Atom property = XInternAtom(display, "_NET_WM_ICON", False);

  unsigned error;
  unsigned char *data = nullptr;
  unsigned pngwidth, pngheight;

  error = libpng_decode32_file(&data, &pngwidth, &pngheight, icon);
  if (error) {
    DEBUG_MESSAGE("libpng-util error " + std::to_string(error), MESSAGE_TYPE::M_ERROR);
    return;
  }
  
  unsigned
    widfull = nlpo2dc(pngwidth) + 1,
    hgtfull = nlpo2dc(pngheight) + 1,
    ih,iw;
  const int bitmap_size = widfull * hgtfull * 4;
  unsigned char *bitmap = new unsigned char[bitmap_size]();

  string output;
  unsigned i = 0;
  unsigned elem_numb = 2 + pngwidth * pngheight;
  unsigned long int *result = new unsigned long int[elem_numb]();

  result[i++] = pngwidth;
  result[i++] = pngheight;
  for (ih = 0; ih < pngheight; ih++) {
    unsigned tmp = ih * widfull * 4;
    for (iw = 0; iw < pngwidth; iw++) {
      bitmap[tmp + 0] = data[4 * pngwidth * ih + iw * 4 + 2];
      bitmap[tmp + 1] = data[4 * pngwidth * ih + iw * 4 + 1];
      bitmap[tmp + 2] = data[4 * pngwidth * ih + iw * 4 + 0];
      bitmap[tmp + 3] = data[4 * pngwidth * ih + iw * 4 + 3];
      result[i++] = bitmap[tmp + 0] | (bitmap[tmp + 1] << 8) | (bitmap[tmp + 2] << 16) | (bitmap[tmp + 3] << 24);
      tmp += 4;
    }
  }
  
  XChangeProperty(display, window, property, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)result, elem_numb);
  
  XFlush(display);
  delete[] result;
  delete[] bitmap;
  delete[] data;
}

}
