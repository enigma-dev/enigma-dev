/** Copyright (C) 2008-2011 IsmAvatar <cmagicj@nni.com>, Josh Ventura
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

 //File consists of Ism's code glued together and set to work with ENIGMA
 //(Josh's doing)

#include <stdio.h> //printf, NULL
#include <stdlib.h> //malloc
#include <unistd.h> //usleep
#include <time.h> //clock
#include <string> //Return strings without needing a GC
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/Xos.h>
//#include <X11/Xatom.h>
//#include <X11/keysym.h>
#include <GL/glx.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

using namespace std;

#include "Universal_System/CallbackArrays.h" // For those damn vk_ constants, and io_clear().
#include "Universal_System/roomsystem.h"
#include "Platforms/platforms_mandatory.h" // For type insurance
#include "XLIBwindow.h" // Type insurance for non-mandatory functions
#include "GameSettings.h" // ABORT_ON_ALL_ERRORS (MOVEME: this shouldn't be needed here)
#include "XLIBwindow.h"
#include "XLIBmain.h"
#undef sleep

#include <X11/Xlib.h>
#define uint unsigned int

using namespace enigma::x11;

//////////
// INIT //
//////////
Cursor NoCursor,DefCursor;
void gmw_init()
{
	//Default cursor
	DefCursor = XCreateFontCursor(disp,68);
	//Blank cursor
	XColor dummy;
	Pixmap blank = XCreateBitmapFromData(disp,win,"",1,1);
	if(blank == None) { //out of memory
		printf("Failed to create no cursor. lulz\n");
		NoCursor = DefCursor;
	}
	else {
		NoCursor = XCreatePixmapCursor(disp,blank,blank,&dummy,&dummy,0,0);
		XFreePixmap(disp,blank);
	}
}

void Sleep(int ms)
{
	if(ms>=1000) sleep(ms/1000);
	if(ms>0)	usleep(ms%1000*1000);
}

int visx = -1, visy = -1;

namespace enigma_user
{

int window_set_visible(bool visible)
{
	if(visible)
	{
		XMapRaised(disp,win);
		GLXContext glxc = glXGetCurrentContext();
		glXMakeCurrent(disp,win,glxc);
		if(visx != -1 && visy != -1)
			window_set_position(visx,visy);
	}
	else
	  XUnmapWindow(disp, win);
	return 0;
}
int window_get_visible()
{
	XWindowAttributes wa;
	XGetWindowAttributes(disp,win,&wa);
	return wa.map_state != IsUnmapped;
}

void window_set_caption(string caption) {
	XStoreName(disp,win,caption.c_str());
}
string window_get_caption()
{
	char *caption;
	XFetchName(disp,win,&caption);
	string r=caption;
	return r;
}

}

inline int getMouse(int i)
{
	Window r1,r2;
	int rx,ry,wx,wy;
	unsigned int mask;
	XQueryPointer(disp,win,&r1,&r2,&rx,&ry,&wx,&wy,&mask);
	switch(i)
	{
    case 0:  return rx;
    case 1:  return ry;
    case 2:  return wx;
    case 3:  return wy;
    default: return -1;
	}
}

namespace enigma_user
{

int display_mouse_get_x() { return getMouse(0); }
int display_mouse_get_y() { return getMouse(1); }
int window_mouse_get_x()  { return getMouse(2); }
int window_mouse_get_y()  { return getMouse(3); }
int window_views_mouse_get_x()  { return getMouse(2); } // this should be changed, it needs to be in respect to all views, look at Win32
int window_views_mouse_get_y()  { return getMouse(3); } // this should be changed, it needs to be in respect to all views, look at Win32

void window_set_stayontop(bool stay) {}
bool window_get_stayontop() {return false;}
void window_set_sizeable(bool sizeable) {}
bool window_get_sizeable() {return false;}
void window_set_showborder(bool show) {}
bool window_get_showborder() {return true;}
void window_set_showicons(bool show) {}
bool window_get_showicons() {return true;}

void window_default()
{
    unsigned int xm = room_width, ym = room_height;
    if (view_enabled)
    {
      int tx = 0, ty = 0;
      for (int i = 0; i < 8; i++)
        if (view_visible[i])
        {
          if (view_xport[i]+view_wport[i] > tx)
            tx = (int)(view_xport[i]+view_wport[i]);
          if (view_yport[i]+view_hport[i] > ty)
            ty = (int)(view_yport[i]+view_hport[i]);
        }
      if (tx and ty)
        xm = tx, ym = ty;
    }
    window_set_size(xm, ym);
}

void window_mouse_set(double x,double y) {
	XWarpPointer(disp,None,win,0,0,0,0,(int)x,(int)y);
}
void display_mouse_set(double x,double y) {
	XWarpPointer(disp,None,DefaultRootWindow(disp),0,0,0,0,(int)x,(int)y);
}

}

////////////
// WINDOW //
////////////
static int getWindowDimension(int i)
{
	XFlush(disp);
	XWindowAttributes wa;
	XGetWindowAttributes(disp,win,&wa);
	if(i == 2) return wa.width;
	if(i == 3) return wa.height;
	Window root, parent, *child;
	uint children;
	XQueryTree(disp,win,&root,&parent,&child,&children);
	XWindowAttributes pwa;
	XGetWindowAttributes(disp,parent,&pwa);
	return i?(i==1?pwa.y+wa.y:-1):pwa.x+wa.x;
}

namespace enigma_user
{

//Getters
int window_get_x()      { return getWindowDimension(0); }
int window_get_y()      { return getWindowDimension(1); }
int window_get_width()  { return getWindowDimension(2); }
int window_get_height() { return getWindowDimension(3); }

//Setters
void window_set_position(int x,int y)
{
	XWindowAttributes wa;
	XGetWindowAttributes(disp,win,&wa);
	XMoveWindow(disp,win,(int) x  - wa.x,(int) y - wa.y);
}
void window_set_size(unsigned int w,unsigned int h) {
	XResizeWindow(disp,win, w, h);
}
void window_set_rectangle(int x,int y,int w,int h) {
	XMoveResizeWindow(disp, win, x, y, w, h);
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

}

////////////////
// FULLSCREEN //
////////////////

enum {
  _NET_WM_STATE_REMOVE,
  _NET_WM_STATE_ADD,
  _NET_WM_STATE_TOGGLE
};

namespace enigma_user
{

void window_set_freezeonlosefocus(bool freeze)
{
}

void window_set_fullscreen(bool full)
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
	XSendEvent(disp,DefaultRootWindow(disp),False,SubstructureRedirectMask|SubstructureNotifyMask,&xev);
}
// FIXME: Looks like I gave up on this one
bool window_get_fullscreen()
{
	Atom aFullScreen = XInternAtom(disp,"_NET_WM_STATE_FULLSCREEN",False);
	Atom ra;
	int ri;
	unsigned long nr, bar;
	unsigned char *data;
	int stat = XGetWindowProperty(disp,win,aFullScreen,0L,0L,False,AnyPropertyType,&ra,&ri,&nr,&bar,&data);
	if (stat != Success) {
		printf("Status: %d\n",stat);
		//return 0;
	}
	/*printf("%d %d %d %d\n",ra,ri,nr,bar);
	for (int i = 0; i < nr; i++) printf("%02X ",data[i]);
	printf("\n");*/
	return 0;
}

}

                 //default    +   -5   I    \    |    /    -    ^   ...  drg  no  -    |  drg3 ...  X  ...  ?   url  +
short curs[] = { 68, 68, 68, 130, 52, 152, 135, 116, 136, 108, 114, 150, 90, 68, 108, 116, 90, 150, 0, 150, 92, 60, 52};

namespace enigma
{
  unsigned char keymap[512];
  unsigned char usermap[256];
  void initkeymap()
  {
    using namespace enigma_user;
    // Pretend this part doesn't exist
    keymap[0x151] = vk_left;
    keymap[0x153] = vk_right;
    keymap[0x152] = vk_up;
    keymap[0x154] = vk_down;
    keymap[0x1E3] = vk_control;
    keymap[0x1E4] = vk_control;
    keymap[0x1E9] = vk_alt;
    keymap[0x1EA] = vk_alt;
    keymap[0x1E1] = vk_shift;
    keymap[0x1E2] = vk_shift;
    keymap[0x10D] = vk_enter;
    keymap[0x185] = vk_lsuper;
    keymap[0x186] = vk_rsuper;
    keymap[0x117] = vk_tab;
    keymap[0x142] = vk_caps;
    keymap[0x14E] = vk_scroll;
    keymap[0x17F] = vk_pause;
    keymap[0x19E] = vk_numpad0;
    keymap[0x19C] = vk_numpad1;
    keymap[0x199] = vk_numpad2;
    keymap[0x19B] = vk_numpad3;
    keymap[0x196] = vk_numpad4;
    keymap[0x19D] = vk_numpad5;
    keymap[0x198] = vk_numpad6;
    keymap[0x195] = vk_numpad7;
    keymap[0x197] = vk_numpad8;
    keymap[0x19A] = vk_numpad9;
    keymap[0x1AF] = vk_divide;
    keymap[0x1AA] = vk_multiply;
    keymap[0x1AD] = vk_subtract;
    keymap[0x1AB] = vk_add;
    keymap[0x19F] = vk_decimal;
    keymap[0x1BE] = vk_f1;
    keymap[0x1BF] = vk_f2;
    keymap[0x1C0] = vk_f3;
    keymap[0x1C1] = vk_f4;
    keymap[0x1C2] = vk_f5;
    keymap[0x1C3] = vk_f6;
    keymap[0x1C4] = vk_f7;
    keymap[0x1C5] = vk_f8;
    keymap[0x1C6] = vk_f9;
    keymap[0x1C7] = vk_f10;
    keymap[0x1C8] = vk_f11;
    keymap[0x1C9] = vk_f12;
    keymap[0x108] = vk_backspace;
    keymap[0x11B] = vk_escape;
    keymap[0x150] = vk_home;
    keymap[0x157] = vk_end;
    keymap[0x155] = vk_pageup;
    keymap[0x156] = vk_pagedown;
    keymap[0x1FF] = vk_delete;
    keymap[0x163] = vk_insert;

    // Set up identity map...
    for (int i = 0; i < 255; i++)
      usermap[i] = i;

    for (int i = 0; i < 255; i++)
      keymap[i] = i;
    for (int i = 'a'; i <= 'z'; i++) // 'a' to 'z' wrap to 'A' to 'Z'
      keymap[i] = i + 'A' - 'a';
    for (int i = 'z'+1; i < 255; i++)
      keymap[i] = i;
   }
}

#include <sys/time.h>

namespace enigma_user {
  extern double fps;
}

namespace enigma {
  string* parameters;
  unsigned int parameterc;
  int current_room_speed;
  void windowsystem_write_exename(char* x)
  {
    unsigned irx = 0;
    if (enigma::parameterc)
      for (irx = 0; enigma::parameters[0][irx] != 0; irx++)
        x[irx] = enigma::parameters[0][irx];
    x[irx] = 0;
  }
  #define hielem 9
  void set_room_speed(int rs)
  {
    current_room_speed = rs;
  }
}

#include "Universal_System/globalupdate.h"

namespace enigma_user
{

void io_handle()
{
  enigma::input_push();
  while(XQLength(disp)) {
    printf("processing an event...\n");
    if(handleEvents() > 0)
      exit(0);
  }
  enigma::update_mouse_variables();
}

int window_set_cursor(int c)
{
	XUndefineCursor(disp,win);
	XDefineCursor(disp, win, (c == -1) ? NoCursor : XCreateFontCursor(disp,curs[-c]));
	return 0;
}

void keyboard_wait()
{
  io_clear();
  for (;;)
  {
    io_handle();
    for (int i = 0; i < 255; i++)
      if (enigma::keybdstatus[i])
        return;
    usleep(10000); // Sleep 1/100 second
  }
}

void window_set_region_scale(double scale, bool adaptwindow) {}
bool window_get_region_scale() {return 1;}
void window_set_region_size(int w, int h, bool adaptwindow) {}

int window_get_region_width()
{
    return window_get_width();
}

int window_get_region_height()
{
    return window_get_height();
}

int window_get_region_width_scaled()
{
    return window_get_width();
}

int window_get_region_height_scaled()
{
    return window_get_height();
}

string parameter_string(unsigned num) {
  return num < enigma::parameterc ? enigma::parameters[num] : "";
}
int parameter_count() {
  return enigma::parameterc;
}

}
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
window_get_sizeable()
*/
