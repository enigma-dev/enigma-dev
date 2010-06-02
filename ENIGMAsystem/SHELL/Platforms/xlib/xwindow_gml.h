/*
 * Copyright (C) 2008 IsmAvatar <cmagicj@nni.com>
 *
 * This file is part of ENIGMA.
 *
 * ENIGMA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ENIGMA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License (COPYING) for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <X11/Xlib.h>
#define uint unsigned int

//////////
// INIT //
//////////
Cursor NoCursor,DefCursor;
void gmw_init() {
 //Default cursor
 DefCursor = XCreateFontCursor(disp,68);

 //Blank cursor
 XColor dummy;
 char data[1] = { 0 };
 Pixmap blank = XCreateBitmapFromData(disp,win,data,1,1);
 if (blank == None) //out of memory
  NoCursor = DefCursor;
 else {
  NoCursor = XCreatePixmapCursor(disp,blank,blank,&dummy,&dummy,0,0);
  XFreePixmap(disp,blank);
 }
}

void Sleep(int ms) {
 if (ms > 1000) sleep(ms / 1000);
 usleep((ms % 1000) * 1000);
}

/////////////
// VISIBLE //
/////////////
int window_set_position(double x,double y);
int visx = -1, visy = -1;
int window_set_visible(double visible) {
 if (!visible) {
  XUnmapWindow(disp, win);
  return 0;
 }
 XMapRaised(disp,win);
 GLXContext glxc = glXGetCurrentContext();
 glXMakeCurrent(disp,win,glxc);
 if (visx != -1 && visy != -1) {
  printf("moving\n");
  window_set_position(visx,visy);
 }
 return 0;
}
int window_get_visible() {
 XWindowAttributes wa;
 XGetWindowAttributes(disp,win,&wa);
 return wa.map_state != IsUnmapped;
}

/////////////
// CAPTION //
/////////////
void window_set_caption(char* caption)
{
  XStoreName(disp,win,caption);
}
char *window_get_caption()
{
 char *caption;
 XFetchName(disp,win,&caption);
 return caption;
}

///////////
// MOUSE //
///////////
inline int getMouse(int i)
{
 Window r1, r2;
 int rx,ry,wx,wy;
 unsigned int mask;
 XQueryPointer(disp,win,&r1,&r2,&rx,&ry,&wx,&wy,&mask);
 switch (i) {
  case 0: return rx;
  case 1: return ry;
  case 2: return wx;
  case 3: return wy;
  default:return -1;
 }
}

int display_mouse_get_x() { return getMouse(0); }
int display_mouse_get_y() { return getMouse(1); }
int window_mouse_get_x()  { return getMouse(2); }
int window_mouse_get_y()  { return getMouse(3); }

int window_mouse_set(double x,double y)
{
 XWarpPointer(disp,None,win,0,0,0,0,(int) x,(int) y);
 return 0;
}
int display_mouse_set(double x,double y)
{
 XWarpPointer(disp,None,DefaultRootWindow(disp),0,0,0,0,(int) x,(int) y);
 return 0;
}

////////////
// WINDOW //
////////////
int getWindowDimension(int i)
{
  XFlush(disp);
  XWindowAttributes wa;
  XGetWindowAttributes(disp,win,&wa);
  if (i == 2) return wa.width;
  if (i == 3) return wa.height;
  
  Window root,parent,*child;
  uint children;
  XQueryTree(disp,win,&root,&parent,&child,&children);
  XWindowAttributes pwa;
  XGetWindowAttributes(disp,parent,&pwa);
  if (i == 0) return pwa.x + wa.x;
  if (i == 1) return pwa.y + wa.y;
  return -1;
}

//Getters
int window_get_x()      { return getWindowDimension(0); }
int window_get_y()      { return getWindowDimension(1); }
int window_get_width()  { return getWindowDimension(2); }
int window_get_height() { return getWindowDimension(3); }

//Setters
void window_set_position(double x,double y)
{
  XWindowAttributes wa;
  XGetWindowAttributes(disp, win, &wa);
  XMoveWindow(disp, win, x-wa.x, y-wa.y);
}
void window_set_size(unsigned int w,unsigned int h)
{
  XResizeWindow(disp,win,(uint) w,(uint) h);
}
void window_set_rectangle(double x,double y,double w,double h)
{
  XMoveResizeWindow(disp,win,(int)x,(int)y,(uint)w,(uint)h);
}

//Center
void window_center()
{
  Window r;
  int x,y;
  uint w,h,b,d;
  XGetGeometry(disp,win,&r,&x,&y,&w,&h,&b,&d);
  Screen *s = DefaultScreenOfDisplay(disp);
  XMoveWindow(disp,win,s->width/2-w/2,s->height/2-h/2);
}

void geom()
{
  Window r;
  int x,y;
  uint w,h,b,d;
  XGetGeometry(disp,win,&r,&x,&y,&w,&h,&b,&d);
}

////////////////
// FULLSCREEN //
////////////////
enum {
  _NET_WM_STATE_REMOVE = 0,
  _NET_WM_STATE_ADD    = 1,
  _NET_WM_STATE_TOGGLE = 2
};

int window_set_fullscreen(double full)
{
  Atom wmState = XInternAtom(disp, "_NET_WM_STATE", False);
  Atom aFullScreen = XInternAtom(disp,"_NET_WM_STATE_FULLSCREEN", False);
  
  XEvent xev;
  xev.xclient.type=ClientMessage;
  xev.xclient.serial = 0;
  xev.xclient.send_event=True;
  xev.xclient.window=win;
  xev.xclient.message_type=wmState;
  xev.xclient.format=32;
  xev.xclient.data.l[0] = (full ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE);
  xev.xclient.data.l[1] = aFullScreen;
  xev.xclient.data.l[2] = 0;
  
  XSendEvent(disp, DefaultRootWindow(disp), False,
             SubstructureRedirectMask | SubstructureNotifyMask, &xev);
}
int window_get_fullscreen() 
{
  Atom aFullScreen = XInternAtom(disp,"_NET_WM_STATE_FULLSCREEN", False);
  Atom ra;
  int ri;
  unsigned long nr, bar;
  unsigned char *data;
  
  int stat = XGetWindowProperty(disp,win,aFullScreen,0L,
             0L,False,AnyPropertyType,&ra,&ri,&nr,&bar,&data);
  if (stat != Success) {
  printf("Status: %d\n",stat);
  return 0;
  }
  //printf("%d %d %d %d\n",ra,ri,nr,bar);
  int i;
  for (i = 0; i < nr; i++) printf("%02X ",data[i]);
  printf("\n");
}

////////////
// CURSOR //
////////////
enum {
  cr_default    = 0,
  cr_none       = -1,
  cr_arrow      = -2,
  cr_cross      = -3,
  cr_beam       = -4,
  cr_size_nesw  = -6,
  cr_size_ns    = -7,
  cr_size_nwse  = -8,
  cr_size_we    = -9,
  cr_uparrow    = -10,
  cr_hourglass  = -11,
  cr_drag       = -12,
  cr_nodrop     = -13,
  cr_hsplit     = -14,
  cr_vsplit     = -15,
  cr_multidrag  = -16,
  cr_sqlwait    = -17,
  cr_no         = -18,
  cr_appstart   = -19,
  cr_help       = -20,
  cr_handpoint  = -21,
  cr_size_all   = -22
};


/*
display_get_width() // Returns the width of the display in pixels.
display_get_height() // Returns the height of the display in pixels.
display_set_size(w,h) Sets the width and height of the display in pixels. Returns whether this was
successful. (Realize that only certain combinations are allowed.)
display_get_colordepth() Returns the color depth in bits.
display_get_frequency() Returns the refresh frequency of the display.
display_set_colordepth(coldepth) Sets the color depth. In general only 16 and 32 are allowed values. Returns whether successful.
display_set_frequency(frequency) Sets the refresh frequency for the display. Only few frequencies are allowed. Typically you could set this to 60 with a same room speed to get smooth 60 frames per second motion. Returns whether successful.

display_set_all(w,h,frequency,coldepth) Sets all at once. Use -1 for values you do not want to change. Returns whether successful.
display_test_all(w,h,frequency,coldepth) Tests whether the indicated settings are allowed. It does not change the settings. Use -1 for values you do not want to change. Returns whether the settings are allowed.
display_reset() Resets the display settings to the ones when the program was started.


window_default()
window_get_cursor()
window_set_color(color)
window_get_color()
window_set_region_scale(scale,adaptwindow)
window_get_region_scale()
window_set_showborder(show)
window_get_showborder()
window_set_showicons(show)
window_get_showicons()
window_set_stayontop(stay)
window_get_stayontop()
window_set_sizeable(sizeable)
window_get_sizeable()*/

