/*

 MIT License
 
 Copyright © 2020 Samuel Venable
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
*/

#include "Platforms/General/PFwindow.h"

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xinerama.h>

static int displayX            = -1;
static int displayY            = -1;
static int displayWidth        = -1;
static int displayHeight       = -1;

static int displayXGetter      = -1;
static int displayYGetter      = -1;
static int displayWidthGetter  = -1;
static int displayHeightGetter = -1;

static void display_get_position(bool i, int *result) {
  Display *display = XOpenDisplay(NULL);
  *result = 0; Rotation original_rotation; 
  Window root = XDefaultRootWindow(display);
  XRRScreenConfiguration *conf = XRRGetScreenInfo(display, root);
  SizeID original_size_id = XRRConfigCurrentConfiguration(conf, &original_rotation);
  if (XineramaIsActive(display)) {
    int m = 0; XineramaScreenInfo *xrrp = XineramaQueryScreens(display, &m);
    if (!i) *result = xrrp[original_size_id].x_org;
    else if (i) *result = xrrp[original_size_id].y_org;
    XFree(xrrp);
  }
  XCloseDisplay(display);
}

static void display_get_size(bool i, int *result) {
  Display *display = XOpenDisplay(NULL);
  *result = 0; int num_sizes; Rotation original_rotation; 
  Window root = XDefaultRootWindow(display);
  int screen = XDefaultScreen(display);
  XRRScreenConfiguration *conf = XRRGetScreenInfo(display, root);
  SizeID original_size_id = XRRConfigCurrentConfiguration(conf, &original_rotation);
  if (XineramaIsActive(display)) {
    XRRScreenSize *xrrs = XRRSizes(display, screen, &num_sizes);
    if (!i) *result = xrrs[original_size_id].width;
    else if (i) *result = xrrs[original_size_id].height;
  } else if (!i) *result = XDisplayWidth(display, screen);
  else if (i) *result = XDisplayHeight(display, screen);
  XCloseDisplay(display);
}

namespace enigma_user {

int display_get_x() {
  if (displayXGetter == displayX && displayX != -1)
    return displayXGetter;
  display_get_position(false, &displayXGetter);
  int result = displayXGetter;
  displayX = result;
  return result;
}

int display_get_y() { 
  if (displayYGetter == displayY && displayY != -1)
    return displayYGetter;
  display_get_position(true, &displayYGetter);
  int result = displayYGetter;
  displayY = result;
  return result;
}

int display_get_width() {
  if (displayWidthGetter == displayWidth && displayWidth != -1) 
    return displayWidthGetter;
  display_get_size(false, &displayWidthGetter);
  int result = displayWidthGetter;
  displayWidth = result;
  return result;
}

int display_get_height() {
  if (displayHeightGetter == displayHeight && displayHeight != -1)
    return displayHeightGetter;
  display_get_size(true, &displayHeightGetter);
  int result = displayHeightGetter;
  displayHeight = result;
  return result;
}

} // namespace enigma_user
