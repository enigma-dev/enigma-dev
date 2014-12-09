/** Copyright (C) 2008-2011 IsmAvatar <cmagicj@nni.com>, Josh Ventura
*** Copyright (C) 2014 Seth N. Hetu
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

#include <stdio.h> //printf, NULL
#include <stdlib.h> //malloc
#include <unistd.h> //usleep
#include <time.h> //clock
#include <string> //Return strings without needing a GC
#include <map>
#include <climits>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

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

namespace enigma {
  bool isVisible = true, isMinimized = false, stayOnTop = false, windowAdapt = true;
  int regionWidth = 0, regionHeight = 0, windowWidth = 0, windowHeight = 0;
  double scaledWidth = 0, scaledHeight = 0;
  extern bool isSizeable, showBorder, showIcons, freezeOnLoseFocus, isFullScreen;
  extern int viewScale, windowColor;
    
  void setwindowsize(int forceX=-1, int forceY=-1)
  {
      if (!regionWidth)
          return;

      Screen *screen = DefaultScreenOfDisplay(disp);
      int parWidth = isFullScreen?XWidthOfScreen(screen):windowWidth, parHeight = isFullScreen?XHeightOfScreen(screen):windowHeight;
      if (viewScale > 0)  //Fixed Scale
      {
          double viewDouble = viewScale/100.0;
          scaledWidth = regionWidth*viewDouble;
          scaledHeight = regionHeight*viewDouble;
      }
      else if (viewScale == 0)  //Full Scale
      {
          scaledWidth = parWidth;
          scaledHeight = parHeight;
      }
      else  //Keep Aspect Ratio
      {
          double fitWidth = parWidth/double(regionWidth), fitHeight = parHeight/double(regionHeight);
          if (fitWidth < fitHeight)
          {
              scaledWidth = parWidth;
              scaledHeight = regionHeight*fitWidth;
          }
          else
          {
              scaledWidth = regionWidth*fitHeight;
              scaledHeight = parHeight;
          }
      }

      if (!isFullScreen)
      {
          if (windowAdapt && viewScale > 0) // If the window is to be adapted and Fixed Scale
          {
              if (scaledWidth > windowWidth)
                  windowWidth = scaledWidth;
              if (scaledHeight > windowHeight)
                  windowHeight = scaledHeight;
          }
          //Now actually set the window's position and size:
          if (forceX==-1 || forceY==-1) {
            enigma_user::window_set_size(windowWidth, windowHeight);
          } else {
            enigma_user::window_set_rectangle(forceX, forceY, windowWidth, windowHeight);
          }
      } else {
          enigma_user::window_set_rectangle(0, 0, windowWidth, windowHeight);
      }
  }
}

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

void window_set_visible(bool visible)
{
  if (visible) {
    XMapRaised(disp,win);
    //TODO: Move to bridges or some shit this is the last remaining GL call in XLIB
    //#include <GL/glx.h>
    //GLXContext glxc = glXGetCurrentContext();
    //glXMakeCurrent(disp,win,glxc);
    if(visx != -1 && visy != -1)
      window_set_position(visx,visy);
  } else {
    XUnmapWindow(disp, win);
  }
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

enum {
  _NET_WM_STATE_REMOVE,
  _NET_WM_STATE_ADD,
  _NET_WM_STATE_TOGGLE
};

typedef struct
            {
              unsigned long   flags;
              unsigned long   functions;
              unsigned long   decorations;
              long            inputMode;
              unsigned long   status;
            } Hints;


namespace enigma_user
{

int display_mouse_get_x() { return getMouse(0); }
int display_mouse_get_y() { return getMouse(1); }
int window_mouse_get_x()  { return getMouse(2); }
int window_mouse_get_y()  { return getMouse(3); }

void window_set_stayontop(bool stay) {
  if (enigma::stayOnTop == stay) return;
  enigma::stayOnTop = stay;
  
  Atom wmState = XInternAtom(disp, "_NET_WM_STATE", False);
  Atom aStay = XInternAtom(disp,"_NET_WM_STATE_ABOVE", False);
  XEvent xev;
  xev.xclient.type=ClientMessage;
  xev.xclient.serial = 0;
  xev.xclient.send_event=True;
  xev.xclient.window=win;
  xev.xclient.message_type=wmState;
  xev.xclient.format=32;
  xev.xclient.data.l[0] = (stay ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE);
  xev.xclient.data.l[1] = aStay;
  xev.xclient.data.l[2] = 0;
  XSendEvent(disp,DefaultRootWindow(disp),False,SubstructureRedirectMask|SubstructureNotifyMask,&xev);
}

bool window_get_stayontop() {
  return enigma::stayOnTop;
}

void window_set_sizeable(bool sizeable) {
  if (enigma::isSizeable == sizeable) return;
  enigma::isSizeable = sizeable;
  
  XSizeHints hints;
  hints.min_width = 640;
  hints.min_height = 480;
  hints.max_width = 641;
  hints.max_height = 481;
  XSetWMNormalHints(disp, win, &hints);
}

bool window_get_sizeable() {
  return enigma::isSizeable;
}

void window_set_showborder(bool show) {
  if (enigma::showBorder == show) return;
  enigma::showBorder = show;
  
  Atom property = XInternAtom(disp,"_MOTIF_WM_HINTS",True);
  if (!show) {
    Hints   hints;
    hints.flags = 2;        // Specify that we're changing the window decorations.
    hints.decorations = 0;  // 0 (false) means that window decorations should go bye-bye.
    XChangeProperty(disp,win,property,property,32,PropModeReplace,(unsigned char *)&hints,5);
  } else {
    XDeleteProperty(disp,win,property);
  }
}

bool window_get_showborder() {
  return enigma::showBorder;
}

void window_set_showicons(bool show) {
  if (enigma::showIcons == show) return;
  enigma::showIcons = show;

  Atom wmState = XInternAtom(disp, "_NET_WM_WINDOW_TYPE", False);
  Atom aShow = XInternAtom(disp,"_NET_WM_WINDOW_TYPE_TOOLBAR", False);
  XEvent xev;
  xev.xclient.type=ClientMessage;
  xev.xclient.serial = 0;
  xev.xclient.send_event=True;
  xev.xclient.window=win;
  xev.xclient.message_type=wmState;
  xev.xclient.format=32;
  xev.xclient.data.l[0] = (show ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE);
  xev.xclient.data.l[1] = aShow;
  xev.xclient.data.l[2] = 0;
  XSendEvent(disp,DefaultRootWindow(disp),False,SubstructureRedirectMask|SubstructureNotifyMask,&xev);
}

bool window_get_showicons() {
  return enigma::showIcons;
}

void window_set_minimized(bool minimized) {
  if (enigma::isMinimized == minimized) return;
  enigma::isMinimized = minimized;

  XClientMessageEvent ev;
  Atom prop;

  prop = XInternAtom(disp, "WM_CHANGE_STATE", False);
  if (prop == None) return;

  // TODO: When restored after a minimize the window may not have focus.
  ev.type = ClientMessage;
  ev.window = win;
  ev.message_type = prop;
  ev.format = 32;
  ev.data.l[0] = minimized ? IconicState : NormalState;
  XSendEvent(disp, RootWindow(disp, 0), False, SubstructureRedirectMask|SubstructureNotifyMask,(XEvent *)&ev);
}

bool window_get_minimized() { 
	return enigma::isMinimized;
}

void window_default(bool center_size)
{
  int xm = room_width, ym = room_height;
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
  } else {
    // By default if the room is too big instead of creating a gigantic ass window
    // make it not bigger than the screen to full screen it, this is what 8.1 and Studio
    // do, if the user wants to manually override this they can using
    // views/screen_set_viewport or window_set_size/window_set_region_size
    // We won't limit those functions like GM, just the default.
    Screen *screen = DefaultScreenOfDisplay(disp);
    if (xm > screen->width) xm = screen->width;
    if (ym > screen->height) ym = screen->height;
  }
  bool center = true;
  if (center_size) {
    center = (xm != window_get_width() || ym != window_get_height());
  }
  enigma::windowWidth = enigma::regionWidth = xm;
  enigma::windowHeight = enigma::regionHeight = ym;

  int forceX = -1;
  int forceY = -1;
  if (center) {
    forceX = display_get_width()/2 - enigma::windowWidth/2;
    forceY = display_get_height()/2 - enigma::windowHeight/2;
    //window_center();
  }
  
  enigma::setwindowsize(forceX, forceY);
}

void window_mouse_set(int x,int y) {
	XWarpPointer(disp,None,win,0,0,0,0,(int)x,(int)y);
}

void display_mouse_set(int x,int y) {
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
  //enigma::windowWidth = w;
  //enigma::windowHeight = h;
  //enigma::setwindowsize(); //NOTE: I think this will also infinitely loop.
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
   int windowX = s->width/2-w/2;
   int windowY = s->height/2-h/2;
	XMoveWindow(disp,win,windowX,windowY);
}

}

////////////////
// FULLSCREEN //
////////////////

namespace enigma_user
{

void window_set_freezeonlosefocus(bool freeze)
{
    enigma::freezeOnLoseFocus = freeze;
}

bool window_get_freezeonlosefocus()
{
    return enigma::freezeOnLoseFocus;
}

void window_set_fullscreen(bool full)
{
  if (enigma::isFullScreen == full) return;
  enigma::isFullScreen = full;
  
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

	enigma::setwindowsize();
}

bool window_get_fullscreen()
{
	//return enigma::isFullScreen;
	Atom wmState = XInternAtom(disp, "_NET_WM_STATE", False);
	Atom aFullScreen = XInternAtom(disp,"_NET_WM_STATE_FULLSCREEN", False);
	bool res = false;

	//Return types, not really used.
	Atom actualType;
	int actualFormat;
	unsigned long bytesAfterReturn;

	//These are used.
	unsigned long numItems;
	unsigned char* data = NULL;

	if (XGetWindowProperty(disp, win, wmState, 0, LONG_MAX, False, AnyPropertyType, &actualType, &actualFormat, &numItems, &bytesAfterReturn, &data) == Success) {
		for (unsigned long i=0; i<numItems; ++i) {
			if (aFullScreen == reinterpret_cast<unsigned long *>(data)[i]) {
				res = true;
			}
		}
	}

	//Reclaim memory.
	if (data) {
		XFree(data);
	}
	return res;
}

}

                 //default    +   -5   I    \    |    /    -    ^   ...  drg  no  -    |  drg3 ...  X  ...  ?   url  +
short curs[] = { 68, 68, 68, 130, 52, 152, 135, 116, 136, 108, 114, 150, 90, 68, 108, 116, 90, 150, 0, 150, 92, 60, 52};

namespace enigma
{
  std::map<int,int> keybdmap;

  inline unsigned short highbyte_allornothing(short x) { 
    return x & 0xFF00? x | 0xFF00 : x; 
  }
  
  unsigned char keymap[512];
  unsigned short keyrmap[256];
  void initkeymap()
  {
    using namespace enigma_user;
    
    for (size_t i = 0; i < 512; ++i) keymap[i] = 0;
    for (size_t i = 0; i < 256; ++i) keyrmap[i] = 0;
    
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
    //for (int i = 0; i < 255; i++)
    //  usermap[i] = i;

    for (int i = 0; i < 'a'; i++)
      keymap[i] = i;
    for (int i = 'a'; i <= 'z'; i++) // 'a' to 'z' wrap to 'A' to 'Z'
      keymap[i] = i + 'A' - 'a';
    for (int i = 'z'+1; i < 255; i++)
      keymap[i] = i;
      
    for (size_t i = 0; i < 512; ++i) keyrmap[keymap[i]] = highbyte_allornothing(i);
   }
}

#include <sys/time.h>

namespace enigma_user {
  extern double fps;
}

namespace enigma {
  string* parameters;
  int parameterc;
  int current_room_speed;
  int cursorInt;
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
  enigma::cursorInt = c;
	XUndefineCursor(disp,win);
	XDefineCursor(disp, win, (c == -1) ? NoCursor : XCreateFontCursor(disp,curs[-c]));
	return 0;
}

int window_get_cursor()
{
  return enigma::cursorInt;
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

void keyboard_set_map(int key1, int key2)
{
  std::map< int, int >::iterator it = enigma::keybdmap.find( key1 );
  if ( enigma::keybdmap.end() != it ) {
    it->second = key2;
  } else {
    enigma::keybdmap.insert( map< int, int >::value_type(key1, key2) );
  }
}

int keyboard_get_map(int key)
{
  std::map< int, int >::iterator it = enigma::keybdmap.find( key );
  if ( enigma::keybdmap.end() != it ) {
    return it->second;
  } else {
    return key;
  }
}

void keyboard_unset_map()
{
  enigma::keybdmap.clear();
}

void keyboard_clear(const int key)
{
  enigma::keybdstatus[key] = enigma::last_keybdstatus[key] = 0;
}

bool keyboard_check_direct(int key)
{
  char keyState[32];

  if ( XQueryKeymap(enigma::x11::disp, keyState) )  {
    //for (int x = 0; x < 32; x++)
    //keyState[x] = 0;
  } else {
    //TODO: print error message.
    return 0;
  }

  if (key == vk_anykey) {
    // next go through each member of keyState array
    for (unsigned i = 0; i < 32; i++ )
    {
      const char& currentChar = keyState[i];

      // iterate over each bit and check if the bit is set
      for (unsigned j = 0; j < 8; j++ )
      {
        // AND current char with current bit we're interested in
        bool isKeyPressed = ((1 << j) & currentChar) != 0;

        if (isKeyPressed)
        {
          return 1;
        }
      }
    }
    return 0;
  }
  if (key == vk_nokey) {
    // next go through each member of keyState array
    for (unsigned i = 0; i < 32; i++ )
    {
      const char& currentChar = keyState[i];

      // iterate over each bit and check if the bit is set
      for (unsigned j = 0; j < 8; j++ )
      {
        // AND current char with current bit we're interested in
        bool isKeyPressed = ((1 << j) & currentChar) != 0;

        if (isKeyPressed)
        {
          return 0;
        }
      }
    }
    return 1;
  }

  key = XKeysymToKeycode(enigma::x11::disp, enigma::keyrmap[key]);
  return (keyState[key >> 3] & (1 << (key & 7)));
}

void window_set_region_scale(double scale, bool adaptwindow)
{
    enigma::viewScale = int(scale*100);
    enigma::windowAdapt = adaptwindow;
    enigma::setwindowsize();
}

double window_get_region_scale()
{
    return enigma::viewScale/100.0;
}

void window_set_region_size(int w, int h, bool adaptwindow)
{
    if (w <= 0 || h <= 0) return;

    enigma::regionWidth = w;
    enigma::regionHeight = h;
    enigma::windowAdapt = adaptwindow;
    enigma::setwindowsize();
    window_center();
}

int window_get_region_width()
{
    return enigma::regionWidth;
}

int window_get_region_height()
{
    return enigma::regionHeight;
}

int window_get_region_width_scaled()
{
    return enigma::scaledWidth;
}

int window_get_region_height_scaled()
{
    return enigma::scaledHeight;
}

void window_set_color(int color)
{
    enigma::windowColor = color;

    //Inform xlib
    int revColor = (color & 0xFF000000) | ((color & 0xFF0000) >> 16) | (color & 0xFF00) | ((color & 0xFF) << 16);
    XSetWindowBackground(disp, win, revColor);
}

int window_get_color()
{
    return enigma::windowColor;
}

void clipboard_set_text(string text)
{
  Atom XA_UTF8 = XInternAtom(disp, "UTF8", 0);
  Atom XA_CLIPBOARD = XInternAtom(disp, "CLIPBOARD", False);
  XChangeProperty(disp, RootWindow(disp, 0), XA_CLIPBOARD, XA_UTF8, 8, PropModeReplace, reinterpret_cast<unsigned char*>(const_cast<char*>(text.c_str())), text.length() + 1);
}

string clipboard_get_text()
{
  Atom XA_UTF8 = XInternAtom(disp, "UTF8", 0);
  Atom XA_CLIPBOARD = XInternAtom(disp, "CLIPBOARD", False);
  //Atom XA_UNICODE = XInternAtom(disp, "UNICODE", 0);
  Atom actual_type;
  int actual_format;
  unsigned long nitems, leftover;
  unsigned char* buf;

  if (XGetWindowProperty(disp, RootWindow(disp,0), XA_CLIPBOARD, 0, 10000000L, False, XA_UTF8, &actual_type, &actual_format, &nitems, &leftover, &buf) == Success) {;
    if (buf != NULL) {
      //free(buf);
      return string(reinterpret_cast<char*>(buf));
    } else {
      return "";
    }
  } else {
    return "";
  }
}

bool clipboard_has_text() {
  Atom XA_UTF8 = XInternAtom(disp, "UTF8", 0);
  Atom XA_CLIPBOARD = XInternAtom(disp, "CLIPBOARD", False);
  //Atom XA_UNICODE = XInternAtom(disp, "UNICODE", 0);
  Atom actual_type;
  int actual_format;
  unsigned long nitems, leftover;
  unsigned char* buf;

  if (XGetWindowProperty(disp, RootWindow(disp,0), XA_CLIPBOARD, 0, 10000000L, False, XA_UTF8, &actual_type, &actual_format, &nitems, &leftover, &buf) == Success) {;
    return buf != NULL;
  } else {
    return false;
  }
}

}
