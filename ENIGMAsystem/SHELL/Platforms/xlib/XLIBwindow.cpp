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

#include "../../Universal_System/CallbackArrays.h" // For those damn vk_ constants.
#include "../platforms_mandatory.h" // For type insurance
#include "../../GameSettings.h" // ABORT_ON_ALL_ERRORS (MOVEME: this shouldn't be needed here)
#include "XLIBwindow.h"
#include "XLIBmain.h"

#include <X11/Xlib.h>
#define uint unsigned int

using namespace std;
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

void sleep(int ms)
{
	if(ms>1000) sleep(ms/1000);
	if(ms>0)	usleep(ms%1000*1000);
}

int visx = -1, visy = -1;

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

int display_mouse_get_x() { return getMouse(0); }
int display_mouse_get_y() { return getMouse(1); }
int window_mouse_get_x()  { return getMouse(2); }
int window_mouse_get_y()  { return getMouse(3); }

void window_mouse_set(double x,double y) {
	XWarpPointer(disp,None,win,0,0,0,0,(int)x,(int)y);
}
void display_mouse_set(double x,double y) {
	XWarpPointer(disp,None,DefaultRootWindow(disp),0,0,0,0,(int)x,(int)y);
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

////////////////
// FULLSCREEN //
////////////////
enum {
  _NET_WM_STATE_REMOVE,
  _NET_WM_STATE_ADD,
  _NET_WM_STATE_TOGGLE
};

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

                 //default    +   -5   I    \    |    /    -    ^   ...  drg  no  -    |  drg3 ...  X  ...  ?   url  +
short curs[] = { 68, 68, 68, 130, 52, 152, 135, 116, 136, 108, 114, 150, 90, 68, 108, 116, 90, 150, 0, 150, 92, 60, 52};
void window_set_cursor(int c)
{
	XUndefineCursor(disp,win);
	XDefineCursor(disp, win, (c == -1) ? NoCursor : XCreateFontCursor(disp,curs[-c]));
}

// FIXME: MOVEME: I can't decide where the hell to put this.
void screen_refresh() {
	glXSwapBuffers(disp,win);
}

namespace enigma
{
  char keymap[256];
  char usermap[256];
  void initkeymap()
  {
    // Pretend this part doesn't exist
    keymap[0x51] = vk_left;
    keymap[0x53] = vk_right;
    keymap[0x52] = vk_up;
    keymap[0x54] = vk_down;
    keymap[0xE3] = vk_control;
    keymap[0xE4] = vk_control;
    keymap[0xE9] = vk_alt;
    keymap[0xEA] = vk_alt;
    keymap[0xE1] = vk_shift;
    keymap[0xE2] = vk_shift;
    keymap[0x0D] = vk_enter;
    keymap[0x85] = vk_lsuper;
    keymap[0x86] = vk_rsuper;
    keymap[0x17] = vk_tab;
    keymap[0x42] = vk_caps;
    keymap[0x4E] = vk_scroll;
    keymap[0x7F] = vk_pause;
    keymap[0x9E] = vk_numpad0;
    keymap[0x9C] = vk_numpad1;
    keymap[0x99] = vk_numpad2;
    keymap[0x9B] = vk_numpad3;
    keymap[0x96] = vk_numpad4;
    keymap[0x9D] = vk_numpad5;
    keymap[0x98] = vk_numpad6;
    keymap[0x95] = vk_numpad7;
    keymap[0x97] = vk_numpad8;
    keymap[0x9A] = vk_numpad9;
    keymap[0xAF] = vk_divide;
    keymap[0xAA] = vk_multiply;
    keymap[0xAD] = vk_subtract;
    keymap[0xAB] = vk_add;
    keymap[0x9F] = vk_decimal;
    keymap[0xBE] = vk_f1;
    keymap[0xBF] = vk_f2;
    keymap[0xC0] = vk_f3;
    keymap[0xC1] = vk_f4;
    keymap[0xC2] = vk_f5;
    keymap[0xC3] = vk_f6;
    keymap[0xC4] = vk_f7;
    keymap[0xC5] = vk_f8;
    keymap[0xC6] = vk_f9;
    keymap[0xC7] = vk_f10;
    keymap[0xC8] = vk_f11;
    keymap[0xC9] = vk_f12;
    keymap[0x08] = vk_backspace;
    keymap[0x1B] = vk_escape;
    keymap[0x50] = vk_home;
    keymap[0x57] = vk_end;
    keymap[0x55] = vk_pageup;
    keymap[0x56] = vk_pagedown;
    keymap[0xFF] = vk_delete;
    keymap[0x63] = vk_insert;
    
    // Set up identity map...
    for (int i = 0; i < 'a'; i++)
      usermap[i] = i;
    for (int i = 'a'; i <= 'z'; i++) // 'a' to 'z' wrap to 'A' to 'Z'
      usermap[i] = i + 'A' - 'a';
    for (int i = 'z'+1; i < 255; i++)
      usermap[i] = i;
   }
}

#include <sys/time.h>

extern double fps;
namespace enigma {
  string* parameters;
  unsigned int parameterc;
  void windowsystem_write_exename(char* x)
  {
    unsigned irx = 0;
    if (enigma::parameterc)
      for (irx = 0; enigma::parameters[0][irx] != 0; irx++)
        x[irx] = enigma::parameters[0][irx];
    x[irx] = 0;
  }
  #define hielem 9
  static int last_second[hielem+1] = {0},last_microsecond[hielem+1] = {0};
  void sleep_for_framerate(int rs)
  {
    timeval tv;
    
    for (int i=1; i<hielem+1; i++) {
      last_microsecond[i-1] = last_microsecond[i];
      last_second[i-1] = last_second[i];
    }
    
    //How many microseconds since 1970? herp
    gettimeofday(&tv, NULL);
    
    // I'm feeling hacky, so we'll give the processor a millisecond to take care
    // of these calculations and hop threads. I'd rather be fast than slow.
    int sdur = 1000000/rs - 1000 - (tv.tv_sec - last_second[hielem]) * 1000000 - (tv.tv_usec - last_microsecond[hielem]);
    if (sdur > 0 and sdur < 1000000) usleep(sdur);
    
    // Store this time for diff next time
    gettimeofday(&tv, NULL);
    last_second[hielem] = tv.tv_sec, last_microsecond[hielem] = tv.tv_usec;
    fps = (hielem+1)*1000000 / ((last_second[hielem] - last_second[0]) * 1000000 + (last_microsecond[hielem] - last_microsecond[0]));
  }
}

#include "../../Universal_System/globalupdate.h"
void io_handle()
{
  enigma::input_push();
  while(XQLength(disp)) {
    printf("processing an event...\n");
    if(handleEvents() > 0)
      exit(0);
  }
  enigma::update_globals();
}
void io_clear()
{
  for (int i = 0; i < 255; i++)
    enigma::keybdstatus[i] = enigma::last_keybdstatus[i] = 0;
  for (int i = 0; i < 3; i++)
    enigma::mousestatus[i] = enigma::last_mousestatus[i] = 0;
}
void keyboard_wait()
{
  for (;;)
  {
    io_handle();
    for (int i = 0; i < 255; i++)
      if (enigma::keybdstatus[i])
        return;
    usleep(10000); // Sleep 1/100 second
  }
}

string parameter_string(unsigned num) {
  return num < enigma::parameterc ? enigma::parameters[num] : "";
}
int parameter_count() {
  return enigma::parameterc;
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
