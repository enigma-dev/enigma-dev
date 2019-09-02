/*
 * Copyright (C) 2012, Paul Evans <leonerd@leonerd.org.uk>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "XLIBicon.h"

#include <cstdio>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/Xatom.h>
#include <gd.h>

void load_icon(char *filename, uint *ndata, unsigned long int **data) {
  FILE *iconfile = fopen(filename, "r");
  gdImagePtr icon = gdImageCreateFromPng(iconfile);
  fclose(iconfile);
  
  int width, height;
  width = gdImageSX(icon);
  height = gdImageSY(icon);
  (*ndata) = (width * height) + 2;
  (*data) = (unsigned long int *)malloc((*ndata) * sizeof(unsigned long int));

  int i = 0;
  (*data)[i++] = width;
  (*data)[i++] = height;
  
  int x, y;
  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      // data is RGBA
      // We'll do some horrible data-munging here
      unsigned char * cols = (unsigned char *)&((*data)[i++]);
      
      int pixcolour = gdImageGetPixel(icon, x, y);

      cols[0] = gdImageBlue(icon, pixcolour);
      cols[1] = gdImageGreen(icon, pixcolour);
      cols[2] = gdImageRed(icon, pixcolour);

      /* Alpha is more difficult */
      int alpha = 127 - gdImageAlpha(icon, pixcolour); // 0 to 127
      
      // Scale it up to 0 to 255; remembering that 2 * 127 should be max
      if (alpha == 127)
        alpha = 255;
      else
        alpha *= 2;
      
      cols[3] = alpha;
    }
  }

  gdImageDestroy(icon);
}

namespace enigma {

void XSetIcon(Display *display, Window window, const char *icon) {
  XSynchronize(display, True);
  Atom property = XInternAtom(display, "_NET_WM_ICON", 0);

  unsigned int nelements;
  unsigned long int *data;

  load_icon((char *)icon, &nelements, &data);
  XChangeProperty(display, window, property, XA_CARDINAL, 32, PropModeReplace, 
    (unsigned char *)data, nelements);

  XFlush(display);
}

}
