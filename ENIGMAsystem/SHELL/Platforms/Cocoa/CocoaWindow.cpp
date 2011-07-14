/*
 * Copyright (C) 2008 IsmAvatar <cmagicj@nni.com>
 * Copyright (C) 2010 Alasdair Morrison <ali@alasdairmorrison.com>
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

#include <stdio.h> //printf, NULL
#include <stdlib.h> //malloc
#include <unistd.h> //usleep
#include <time.h> //clock
#include <string> //Return strings without needing a GC
#include <X11/Xlib.h>
#include "ObjectiveC.h"

std::string working_directory;

Display *disp;
Window win;
GC gc;
Atom wm_delwin;

#include <X11/Xlib.h>
#define uint unsigned int

using namespace std;







//////////
// INIT //
//////////
Cursor NoCursor,DefCursor;
void gmw_init()
{
}


void sleep(int ms){
    if (ms > 1000) sleep(ms/1000);
    usleep((ms % 1000) *1000);
    };


int visx = -1, visy = -1;

int window_set_visible(int visible)
{
	return cocoa_window_set_visible(visible);
}

int window_get_visible()
{
	/*XWindowAttributes wa;
	XGetWindowAttributes(disp,win,&wa);
	return wa.map_state != IsUnmapped;*/
}

int window_set_caption(string caption)
{
	cocoa_window_set_caption(caption.c_str());
}
string window_get_caption()
{
	cocoa_window_get_caption();
}



int display_mouse_get_x() { return getMouse(0); }
int display_mouse_get_y() { return getMouse(1); }
int window_mouse_get_x()  { return getMouse(2); }
int window_mouse_get_y()  { return getMouse(3); }

void window_mouse_set(double x,double y) {
	//XWarpPointer(disp,None,win,0,0,0,0,(int)x,(int)y);
}
void display_mouse_set(double x,double y) {
	//XWarpPointer(disp,None,DefaultRootWindow(disp),0,0,0,0,(int)x,(int)y);
}

////////////
// WINDOW //
////////////

//Getters
int window_get_x()      { return 0;}//return getWindowDimension(0); }
int window_get_y()      { return 0;}//return getWindowDimension(1); }
int window_get_width()  { return getWindowDimension(2); }
int window_get_height() { return getWindowDimension(3); }

//Setters
void window_set_position(int x,int y)
{
	/*XWindowAttributes wa;
	XGetWindowAttributes(disp,win,&wa);
	XMoveWindow(disp,win,(int) x  - wa.x,(int) y - wa.y);*/
}

int window_set_size(unsigned int w,unsigned int h)
{
	cocoa_window_set_size(w,h);
}

//Center
void window_center()
{
	/*Window r;
	int x,y;
	uint w,h,b,d;
	XGetGeometry(disp,win,&r,&x,&y,&w,&h,&b,&d);
	Screen *s = DefaultScreenOfDisplay(disp);
	XMoveWindow(disp,win,s->width/2-w/2,s->height/2-h/2);*/
}

////////////////
// FULLSCREEN //
////////////////
enum {
	_NET_WM_STATE_REMOVE,
	_NET_WM_STATE_ADD,
	_NET_WM_STATE_TOGGLE
};

void window_set_fullscreen(const bool full)
{
	/*Atom wmState = XInternAtom(disp, "_NET_WM_STATE", False);
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
	XSendEvent(disp,DefaultRootWindow(disp),False,SubstructureRedirectMask|SubstructureNotifyMask,&xev);*/
}
int window_get_fullscreen()
{
	/*Atom aFullScreen = XInternAtom(disp,"_NET_WM_STATE_FULLSCREEN",False);
	Atom ra;
	int ri;
	unsigned long nr, bar;
	unsigned char *data;
	int stat = XGetWindowProperty(disp,win,aFullScreen,0L,0L,False,AnyPropertyType,&ra,&ri,&nr,&bar,&data);
	if (stat != Success) {
		printf("Status: %d\n",stat);
		//return 0;
	}*/
	/*printf("%d %d %d %d\n",ra,ri,nr,bar);
	 for (int i = 0; i < nr; i++) printf("%02X ",data[i]);
	 printf("\n");*/
	return 0;
}

//default    +   -5   I    \    |    /    -    ^   ...  drg  no  -    |  drg3 ...  X  ...  ?   url  +
short curs[] = { 68, 68, 68, 130, 52, 152, 135, 116, 136, 108, 114, 150, 90, 68, 108, 116, 90, 150, 0, 150, 92, 60, 52};
int window_set_cursor(double c)
{
	/*XUndefineCursor(disp,win);
	XDefineCursor(disp, win, (c == -1) ? NoCursor : XCreateFontCursor(disp,curs[-c]));
	return 0;*/
}


void window_set_showicons(bool show) {
    
}

void window_set_color(int color) {}

int window_view_mouse_get_x(int wid) {}
int window_view_mouse_get_y(int wid) {}
void window_view_mouse_set(int wid, int x, int y) {}
int window_views_mouse_get_x() {}
int window_views_mouse_get_y() {}
void window_views_mouse_set(int x, int y) {}


int window_get_region_width() { return cocoa_window_get_region_width();}
int window_get_region_height() { return cocoa_window_get_region_height();}


void screen_refresh() {
	cocoa_screen_refresh();
    cocoa_flush_opengl();
}

namespace enigma {
    extern char keybdstatus[256];
}

extern char cocoa_keybdstatus[256];
void io_clear() {
    for(int i=0; i<255; i++) {
        enigma::keybdstatus[i]=0;
        cocoa_keybdstatus[i]=0;
    }
}


namespace enigma
{	
	char keymap[256];
	void initkeymap() // To create this table print the values passed to keypress put it in the array indice and set the value to vk_whatever
	{
		keymap[0] = 65; //vk_A
		keymap[0x01] = 83; //vk_S
		keymap[0x02] = 68; //vk_D
		keymap[0x03] = 70; //vk_F
		keymap[0x04] = 72; //vk_H
		keymap[0x05] = 71; //vk_G
		keymap[0x06] = 90; //vk_Z
		keymap[0x07] = 88; //vk_X
		keymap[0x08] = 67; //vk_C
		keymap[0x09] = 86; //vk_V
		keymap[0x0B] = 66; //vk_B
		keymap[0x0C] = 81; //vk_Q
		keymap[0x0D] = 87; //vk_W
		keymap[0x0E] = 69; //vk_E
		keymap[0x0F] = 82; //vk_R
		keymap[0x10] = 89; //vk_Y
		keymap[0x11] = 84; //vk_T
		keymap[0x12] = 49; //vk_1
		keymap[0x13] = 50; //vk_2
		keymap[0x14] = 51; //vk_3
		keymap[0x15] = 52; //vk_4
		keymap[0x16] = 54; //vk_6
		keymap[0x17] = 53; //vk_5
		keymap[0x18] = 187; //Equal
		keymap[0x19] = 57; //vk_9
		keymap[0x1A] = 55; //vk_7
		keymap[0x1B] = 189; //Minus
		keymap[0x1C] = 56; //vk_8
		keymap[0x1D] = 48; //vk_0
		keymap[0x1E] = 221; //RightBracket
		keymap[0x1F] = 79; //vk_O
		keymap[0x20] = 85; //vk_U
		keymap[0x21] = 219; //LeftBracket
		keymap[0x22] = 73; //vk_I
		keymap[0x23] = 80; //vk_P
		keymap[0x25] = 76; //vk_L
		keymap[0x26] = 74; //vk_J
		keymap[0x28] = 75; //vk_K
		keymap[0x2D] = 78; //vk_N
		keymap[0x2E] = 77; //vk_M
		keymap[0x27] = 192; //quote
		keymap[0x29] = 186; //semicolon
		keymap[0x2B] = 188; //comma
		keymap[123] = 37;  //vk_left;
		keymap[124] = 39;  //vk_right;
		keymap[126] = 38;  //vk_up;
		keymap[125] = 40;  //vk_down;
		keymap[0xE4] = 17;  //vk_control; //doesn't work mac
		keymap[0xEA] = 18;  //vk_alt;
		keymap[0xE2] = 16;  //vk_shift;
		keymap[36] = 13;  //vk_enter;
		keymap[0x52] = 96;  //vk_numpad0;
		keymap[0x53] = 97;  //vk_numpad1;
		keymap[0x54] = 98;  //vk_numpad2;
		keymap[0x55] = 99;  //vk_numpad3;
		keymap[0x56] = 100; //vk_numpad4;
		keymap[0x57] = 101; //vk_numpad5;
		keymap[0x58] = 102; //vk_numpad6;
		keymap[0x59] = 103; //vk_numpad7;
		keymap[0x5B] = 104; //vk_numpad8;
		keymap[0x5C] = 105; //vk_numpad9;
		keymap[0x4B] = 111; //vk_divide;
		keymap[0x43] = 106; //vk_multiply;
		keymap[0x4E] = 109; //vk_subtract;
		keymap[0x45] = 107; //vk_add;
		keymap[0x41] = 110; //vk_decimal;
		keymap[122] = 112; //vk_f1;
		keymap[120] = 113; //vk_f2;
		keymap[99] = 114; //vk_f3;
		keymap[118] = 115; //vk_f4;
		keymap[96] = 116; //vk_f5;
		keymap[97] = 117; //vk_f6;
		keymap[98] = 118; //vk_f7;
		keymap[100] = 119; //vk_f8;
		keymap[101] = 120; //vk_f9;
		keymap[109] = 121; //vk_f10;
		keymap[103] = 122; //vk_f11;
		keymap[111] = 123; //vk_f12;
		keymap[51] = 8;   //vk_backspace;
		keymap[53] = 27;  //vk_escape;
		keymap[0x50] = 36;  //vk_home; //no home button
		keymap[0x57] = 35;  //vk_end; //no end button
		keymap[0x55] = 33;  //vk_pageup; //no pageup
		keymap[0x56] = 34;  //vk_pagedown;
		keymap[0xFF] = 46;  //vk_delete; //no delete
		keymap[0x63] = 45;  //vk_insert; //no ins
        keymap[49] = 32; //vk_space
        //keymap[mac] = gm;
	}
}

extern void ABORT_ON_ALL_ERRORS();
void show_error(string err, const bool fatal)
{
	printf("ERROR: %s\n",err.c_str());
	if (fatal) exit(0);
	ABORT_ON_ALL_ERRORS();
}

#include <sys/time.h>
#include <iostream>

extern double fps;
namespace enigma {
	char** parameters;
	void windowsystem_write_exename(char* x)
	{
        const char* resourcefile = enigma::parameters[0];
		unsigned irx;
		for (irx = 0; resourcefile[irx] != 0; irx++)
			x[irx] = resourcefile[irx];
        x[irx]= '.';
        x[irx+1]= 'r';
        x[irx+2]= 'e';
        x[irx+3]= 's';
		x[irx+4] = 0;
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
#include "CocoaFunctions.h"
extern void cocoa_io_handle();
void io_handle() {
    
    cocoa_io_handle();
}

bool keyboard_check(int key);


void keyboard_wait() {
    io_clear();
    while(!keyboard_check(1/*vk_anykey*/)) {
        io_handle();
    }
}


void game_end() {
    //audiosystem_cleanup();
    exit(0);
}
void action_end_game() {
    game_end();
} 