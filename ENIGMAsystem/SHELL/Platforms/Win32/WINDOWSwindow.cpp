/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2013 Robert B. Colton                                         **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <string>
#include <windows.h>
using namespace std;

#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <unistd.h> //usleep
#include "Universal_System/CallbackArrays.h" // For those damn vk_ constants.

#include "Widget_Systems/widgets_mandatory.h"
#include "../General/PFwindow.h"

#include "Universal_System/globalupdate.h"
#include "WINDOWScallback.h"

#include <stdio.h>

static int displayInitialResolutionWidth = 0, displayInitialResolutionHeight = 0, displayInitialBitdepth = 0, displayInitialFrequency = 0;

namespace enigma
{
    extern HWND hWnd,hWndParent;
    bool isVisible = true, windowIsTop = false, freezeWindow = false;
    int windowcolor = 0, cursorInt = 0, regionWidth = 0, regionHeight = 0, windowWidth = 0, windowHeight = 0, windowX = 0, windowY = 0;
    double scaledWidth = 0, scaledHeight = 0;
    char* currentCursor = IDC_ARROW;
    extern bool isSizeable, showBorder, showIcons, freezeOnLoseFocus, isFullScreen;
    extern int viewScale;

    LONG_PTR getparentstyle()
    {
        LONG_PTR newlong = WS_MINIMIZEBOX;
        if (isSizeable)
            newlong |= WS_SIZEBOX;
        if (showBorder)
            newlong |= WS_CAPTION;
        if (showIcons)
            newlong |= WS_POPUPWINDOW;
        if (isVisible)
            newlong |= WS_VISIBLE;

        return newlong;
    }

    void clampparent()
    {
        RECT c;
        c.left = enigma::windowX; c.top = enigma::windowY; c.right = enigma::windowX + enigma::windowWidth; c.bottom = enigma::windowY + enigma::windowHeight;
        AdjustWindowRect(&c, getparentstyle(), false);
        SetWindowPos(enigma::hWndParent, HWND_TOP, c.left, c.top, c.right-c.left, c.bottom-c.top, SWP_NOZORDER|SWP_FRAMECHANGED);
    }

    void setparentstyle()
    {
        SetWindowLongPtr(enigma::hWndParent,GWL_STYLE,getparentstyle());
        clampparent();
    }

    void centerchild()
    {
        int parWidth = isFullScreen?GetSystemMetrics(SM_CXSCREEN):windowWidth, parHeight = isFullScreen?GetSystemMetrics(SM_CYSCREEN):windowHeight;
        SetWindowPos(hWnd, HWND_TOP, (parWidth - scaledWidth)/2, (parHeight - scaledHeight)/2, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE);
    }

    void setchildsize(bool adapt)
    {
        if (!regionWidth)
            return;

        int parWidth = isFullScreen?GetSystemMetrics(SM_CXSCREEN):windowWidth, parHeight = isFullScreen?GetSystemMetrics(SM_CYSCREEN):windowHeight;

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

        SetWindowPos(hWnd, NULL, 0, 0, scaledWidth, scaledHeight, SWP_NOACTIVATE);
        if (!isFullScreen)
        {
            if (adapt)
            {
                windowWidth = scaledWidth;
                windowHeight = scaledHeight;
            }
            else
            {
                if (scaledWidth > windowWidth)
                    windowWidth = scaledWidth;
                if (scaledHeight > windowHeight)
                    windowHeight = scaledHeight;
            }
            clampparent();
        }

        SetWindowPos(hWnd, HWND_TOP, (parWidth - scaledWidth)/2, (parHeight - scaledHeight)/2, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE);  //center child
    }
}

namespace enigma_user
{

int window_get_x()
{
    return enigma::windowX;
}

int window_get_y()
{
    return enigma::windowY;
}

int window_get_width()
{
    return enigma::windowWidth;
}

int window_get_height()
{
    return enigma::windowHeight;
}

void window_set_caption(string caption)
{
/*  if (caption == "")
      if (score != 0)
        caption = "Score: " + string(score);  //GM does this but it's rather fucktarded */

    if (caption != current_caption)
    {
        SetWindowText(enigma::hWndParent,(char*) caption.c_str());
        current_caption = caption;
    }
}

string window_get_caption()
{
  return current_caption;
}

void window_set_color(int color)
{
    enigma::windowcolor = color;
}

int window_get_color()
{
    return enigma::windowcolor;
}

void window_set_position(int x, int y)
{
    enigma::windowX = x;
    enigma::windowY = y;
    SetWindowPos(enigma::hWnd, HWND_TOP, enigma::windowX, enigma::windowY, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE);
    enigma::centerchild();
}

void window_set_size(unsigned int width, unsigned int height)
{
    if (width > enigma::scaledWidth)
        enigma::windowWidth = width;
    if (height > enigma::scaledHeight)
        enigma::windowHeight = height;
    enigma::clampparent();
    enigma::centerchild();
}

void window_set_rectangle(int x, int y, int width, int height)
{
    enigma::windowX = x;
    enigma::windowY = y;
    enigma::windowWidth = width;
    enigma::windowHeight = height;
    enigma::clampparent();
    enigma::centerchild();
}

void window_center()
{
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    enigma::windowX = (screen_width - enigma::scaledWidth)/2;
    enigma::windowY = (screen_height - enigma::scaledHeight)/2;
    enigma::clampparent();
    enigma::centerchild();
}

void window_default()
{
    int xm = int(room_width), ym = int(room_height);
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

    enigma::windowWidth = enigma::regionWidth = xm;
    enigma::windowHeight = enigma::regionHeight = ym;
    enigma::setchildsize(true);
    window_center();
    if (enigma::isFullScreen)
    {
        SetWindowLongPtr(enigma::hWndParent,GWL_STYLE,WS_POPUP);
        ShowWindow(enigma::hWndParent,SW_MAXIMIZE);
    }
    else
    {
        enigma::setparentstyle();
        ShowWindow(enigma::hWndParent,SW_RESTORE);
    }
    enigma::setchildsize(true);
}

void window_set_fullscreen(bool full)
{
    if (enigma::isFullScreen == full)
        return;

    if ((enigma::isFullScreen = full))
    {
        SetWindowLongPtr(enigma::hWndParent,GWL_STYLE,WS_POPUP);
        ShowWindow(enigma::hWndParent,SW_MAXIMIZE);
    }
    else
    {
        enigma::setparentstyle();
        ShowWindow(enigma::hWndParent,SW_RESTORE);
    }
    enigma::setchildsize(true);
}

int window_get_fullscreen()
{
    return enigma::isFullScreen;
}

void window_set_sizeable(bool sizeable)
{
    if (enigma::isSizeable == sizeable)
        return;

    enigma::isSizeable = sizeable;
    enigma::setparentstyle();
}

bool window_get_sizeable()
{
    return enigma::isSizeable;
}

void window_set_showborder(bool show)
{
    if (enigma::showBorder == show)
        return;

    enigma::showBorder = show;
    enigma::setparentstyle();
}

bool window_get_showborder()
{
    return enigma::showBorder;
}

void window_set_showicons(bool show)
{
    if (enigma::showIcons == show)
        return;

    enigma::showIcons = show;
    enigma::setparentstyle();
}

bool window_get_showicons()
{
    return enigma::showIcons;
}

void window_set_visible(bool visible)
{
    if (enigma::isVisible == visible)
        return;

    enigma::isVisible = visible;
    enigma::setparentstyle();
}

int window_get_visible()
{
    return enigma::isVisible;
}

void window_set_stayontop(bool stay)
{
    if (enigma::windowIsTop == stay)
        return;

    if ((enigma::windowIsTop = stay))
    {
        SetWindowPos(enigma::hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
    }
    else
    {
        SetWindowPos(enigma::hWnd,HWND_BOTTOM,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE);
        SetWindowPos(enigma::hWnd,HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
    }
}

void window_set_freezeonlosefocus(bool freeze)
{
    enigma::freezeOnLoseFocus = freeze;
}

bool window_get_freezeonlosefocus()
{
    return enigma::freezeOnLoseFocus;
}

bool window_get_stayontop()
{
    return enigma::windowIsTop;
}

void window_set_region_scale(double scale, bool adaptwindow)
{
    enigma::viewScale = int(scale*100);
    enigma::setchildsize(adaptwindow);
    window_center();
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
    enigma::setchildsize(adaptwindow);
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

int display_mouse_get_x()
{
    POINT mouse;
	GetCursorPos(&mouse);
	return mouse.x;
}
int display_mouse_get_y()
{
    POINT mouse;
    GetCursorPos(&mouse);
	return mouse.y;
}
void display_mouse_set(int x,int y)
{
    SetCursorPos(x,y);
}
int display_get_width()
{
   return GetSystemMetrics(SM_CXSCREEN);
}
int display_get_height()
{
   return GetSystemMetrics(SM_CYSCREEN);
}

int display_get_colordepth()
{
	return GetDeviceCaps(GetDC(enigma::hWnd), BITSPIXEL);
}

int display_get_frequency()
{
	return GetDeviceCaps(GetDC(enigma::hWnd), VREFRESH);
}

unsigned display_get_dpi_x()
{
	return GetDeviceCaps(GetDC(enigma::hWnd), LOGPIXELSX);
}

unsigned display_get_dpi_y()
{
	return GetDeviceCaps(GetDC(enigma::hWnd), LOGPIXELSY);
}

void display_reset()
{
	DEVMODE devMode;

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		return;

	if (displayInitialBitdepth != 0)
	{
		devMode.dmFields |= DM_BITSPERPEL;
		devMode.dmBitsPerPel = displayInitialBitdepth;
	}

	if (displayInitialFrequency != 0)
	{
		devMode.dmFields |= DM_DISPLAYFREQUENCY;
		devMode.dmDisplayFrequency = displayInitialFrequency;
	}

	if (displayInitialResolutionWidth != 0)
	{
		devMode.dmFields |= DM_PELSWIDTH;
		devMode.dmPelsWidth = displayInitialResolutionWidth;
	}

	if (displayInitialResolutionHeight != 0)
	{
		devMode.dmFields |= DM_PELSHEIGHT;
		devMode.dmPelsHeight = displayInitialResolutionHeight;
	}

	ChangeDisplaySettings(&devMode, 0);
}

bool display_set_size(int w, int h)
{
	DEVMODE devMode;

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		return false;

	if (displayInitialResolutionWidth == 0)
		displayInitialResolutionWidth = devMode.dmPelsWidth;

	if (displayInitialResolutionHeight == 0)
		displayInitialResolutionHeight = devMode.dmPelsHeight;

	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	devMode.dmPelsWidth = w;
	devMode.dmPelsHeight = h;

	ChangeDisplaySettings(&devMode, 0);
	return true;
}

bool display_set_frequency(int freq)
{
	DEVMODE devMode;

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		return false;

	if (displayInitialFrequency == 0)
		displayInitialFrequency = devMode.dmBitsPerPel;

	devMode.dmFields = DM_DISPLAYFREQUENCY;
	devMode.dmDisplayFrequency = freq;

	ChangeDisplaySettings(&devMode, 0);
	return true;
}

bool display_set_colordepth(int depth)
{
	DEVMODE devMode;

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		return true;

	if (displayInitialBitdepth == 0)
		displayInitialBitdepth = devMode.dmBitsPerPel;

	devMode.dmFields = DM_BITSPERPEL;
	devMode.dmBitsPerPel = depth;

	ChangeDisplaySettings(&devMode, 0);
	return true;
}

bool display_set_all(int w, int h, int freq, int bitdepth)
{
	DEVMODE devMode;

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		return false;

	if (w != -1)
	{
		if (displayInitialResolutionWidth == 0)
			displayInitialResolutionWidth = devMode.dmPelsWidth;

		devMode.dmFields |= DM_PELSWIDTH;
		devMode.dmPelsWidth = w;
	}

	if (h != -1)
	{
		if (displayInitialResolutionHeight == 0)
			displayInitialResolutionHeight = devMode.dmPelsHeight;

		devMode.dmFields |= DM_PELSHEIGHT;
		devMode.dmPelsHeight = h;
	}

	if (freq != -1)
	{
		if (displayInitialFrequency == 0)
			displayInitialFrequency = devMode.dmDisplayFrequency;

		devMode.dmFields |= DM_DISPLAYFREQUENCY;
		devMode.dmDisplayFrequency = freq;
	}

	if (bitdepth != -1)
	{
		if (displayInitialBitdepth == 0)
			displayInitialBitdepth = devMode.dmBitsPerPel;

		devMode.dmFields |= DM_BITSPERPEL;
		devMode.dmBitsPerPel = bitdepth;
	}

	return ChangeDisplaySettings(&devMode, 0) == DISP_CHANGE_SUCCESSFUL;
}

bool display_test_all(int w, int h, int freq, int bitdepth)
{
	DEVMODE devMode;

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		return false;

	if (w != -1)
	{
		if (displayInitialResolutionWidth == 0)
			displayInitialResolutionWidth = devMode.dmPelsWidth;

		devMode.dmFields |= DM_PELSWIDTH;
		devMode.dmPelsWidth = w;
	}

	if (h != -1)
	{
		if (displayInitialResolutionHeight == 0)
			displayInitialResolutionHeight = devMode.dmPelsHeight;

		devMode.dmFields |= DM_PELSHEIGHT;
		devMode.dmPelsHeight = h;
	}

	if (freq != -1)
	{
		if (displayInitialFrequency == 0)
			displayInitialFrequency = devMode.dmDisplayFrequency;

		devMode.dmFields |= DM_DISPLAYFREQUENCY;
		devMode.dmDisplayFrequency = freq;
	}

	if (bitdepth != -1)
	{
		if (displayInitialBitdepth == 0)
			displayInitialBitdepth = devMode.dmBitsPerPel;

		devMode.dmFields |= DM_BITSPERPEL;
		devMode.dmBitsPerPel = bitdepth;
	}

	return ChangeDisplaySettings(&devMode, CDS_TEST) == DISP_CHANGE_SUCCESSFUL;
}

int window_mouse_get_x()
{
    RECT window;
    GetWindowRect(enigma::hWnd,&window);
    POINT mouse;
	GetCursorPos(&mouse);

	return mouse.x-window.left;
}

int window_mouse_get_y()
{
    RECT window;
    GetWindowRect(enigma::hWnd,&window);
    POINT mouse;
	GetCursorPos(&mouse);

	return mouse.y-window.top;
}

void window_mouse_set(int x, int y)
{
    RECT window;
    GetWindowRect(enigma::hWnd,&window);
    SetCursorPos(window.left + x,window.top + y);
}

int window_view_mouse_get_x(int id)
{
    RECT window;
    GetWindowRect(enigma::hWnd,&window);
    POINT mouse;
	GetCursorPos(&mouse);

	return mouse.x-window.left+view_xview[id];
}

int window_view_mouse_get_y(int id)
{
    RECT window;
    GetWindowRect(enigma::hWnd,&window);
    POINT mouse;
	GetCursorPos(&mouse);

	return mouse.y-window.top+view_yview[id];
}

void window_view_mouse_set(int id, int x, int y)
{
    RECT window;
    GetWindowRect(enigma::hWnd,&window);
    SetCursorPos(window.left + x + view_xview[id],window.top + y + view_yview[id]);
}

}

namespace enigma_user
{

int window_set_cursor(int c)
{
    switch (c)
    {
        enigma::cursorInt = c;
        case cr_default:
            enigma::currentCursor= IDC_ARROW; return 1;
            break;
        case cr_none:
            enigma::currentCursor = NULL; return 1;
            break;
        case cr_arrow:
            enigma::currentCursor = IDC_ARROW; return 1;
            break;
        case cr_cross:
            enigma::currentCursor = IDC_CROSS; return 1;
            break;
        case cr_beam:
            enigma::currentCursor = IDC_IBEAM; return 1;
            break;
        case cr_size_nesw:
            enigma::currentCursor = IDC_SIZENESW; return 1;
            break;
        case cr_size_ns:
            enigma::currentCursor = IDC_SIZENS; return 1;
            break;
        case cr_size_nwse:
            enigma::currentCursor = IDC_SIZENWSE; return 1;
            break;
        case cr_size_we:
            enigma::currentCursor = IDC_SIZEWE; return 1;
            break;
        case cr_uparrow:
            enigma::currentCursor = IDC_UPARROW; return 1;
            break;
        case cr_hourglass:
            enigma::currentCursor = IDC_WAIT; return 1;
            break;
        case cr_drag:
            // Delphi-made?
            break;
        case cr_nodrop:
            enigma::currentCursor = IDC_NO; return 1;
            break;
        case cr_hsplit:
            // Delphi-made?
            enigma::currentCursor = IDC_SIZENS; return 1;
            break;
        case cr_vsplit:
            // Delphi-made?
            enigma::currentCursor = IDC_SIZEWE; return 1;
            break;
        case cr_multidrag:
            enigma::currentCursor = IDC_SIZEALL; return 1;
            break;
        case cr_sqlwait:
            // DEAR GOD WHY
            enigma::currentCursor = IDC_WAIT; return 1;
            break;
        case cr_no:
            enigma::currentCursor = IDC_NO; return 1;
            break;
        case cr_appstart:
            enigma::currentCursor = IDC_APPSTARTING; return 1;
            break;
        case cr_help:
            enigma::currentCursor = IDC_HELP; return 1;
            break;
        case cr_handpoint:
            enigma::currentCursor = IDC_HAND; return 1;
            break;
        case cr_size_all:
            enigma::currentCursor = IDC_SIZEALL; return 1;
            break;
    }
    return 0;
}


int window_get_curor()
{
    return enigma::cursorInt;
}

void game_end() { PostQuitMessage(0); }
void action_end_game() { game_end(); }

void io_handle()
{
  MSG msg;
  enigma::input_push();
  while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage (&msg);
    DispatchMessage (&msg);
  }
  enigma::update_mouse_variables();
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

void mouse_wait()
{
  for (;;)
  {
    io_handle();
    for (int i = 0; i < 3; i++)
      if (enigma::mousestatus[i])
        return;
    usleep(10000); // Sleep 1/100 second
  }
}

void keyboard_clear(const int key)
{
    enigma::keybdstatus[key] = enigma::last_keybdstatus[key] = 0;
}

bool keyboard_check_direct(int key)
{
   BYTE keyState[256];
 
   if ( GetKeyboardState( keyState ) )  {
	  for (int x = 0; x < 256; x++)
		keyState[x] = (char) (GetKeyState(x) >> 8);
   } else {
      //TODO: print error message.
	  return 0;
   }

  if (key == vk_anykey) {
    for(int i = 0; i < 256; i++)
      if (keyState[i] == 1) return 1;
    return 0;
  }
  if (key == vk_nokey) {
    for(int i = 0; i < 256; i++)
      if (keyState[i] == 1) return 0;
    return 1; 
  }
  return keyState[key & 0xFF];
}

void keyboard_key_press(int key) {
    BYTE keyState[256];

    GetKeyboardState((LPBYTE)&keyState);
	
	// Simulate a key press
	 keybd_event( key,
				  keyState[key],
				  KEYEVENTF_EXTENDEDKEY | 0,
				  0 );
}

void keyboard_key_release(int key) {
    BYTE keyState[256];

    GetKeyboardState((LPBYTE)&keyState);

	// Simulate a key release
	keybd_event( key, keyState[key], KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}

bool keyboard_get_capital() {
	return (((unsigned short)GetKeyState(0x14)) & 0xffff) != 0;
}

bool keyboard_get_numlock() {
	return (((unsigned short)GetKeyState(0x90)) & 0xffff) != 0;
}

bool keyboard_get_scroll() {
	return (((unsigned short)GetKeyState(0x91)) & 0xffff) != 0;
}

void keyboard_set_capital(bool on) {
    BYTE keyState[256];

    GetKeyboardState((LPBYTE)&keyState);
	
	if( (on && !(keyState[VK_CAPITAL] & 1)) ||
	  (!on && (keyState[VK_CAPITAL] & 1)) )
	{
	// Simulate a key press
	 keybd_event( VK_CAPITAL, 0x14, KEYEVENTF_EXTENDEDKEY | 0, 0 );

	// Simulate a key release
	 keybd_event( VK_CAPITAL, 0x14, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}
}

void keyboard_set_numlock(bool on) {
    BYTE keyState[256];

    GetKeyboardState((LPBYTE)&keyState);
	
	if( (on && !(keyState[VK_NUMLOCK] & 1)) ||
	  (!on && (keyState[VK_NUMLOCK] & 1)) )
	{
	// Simulate a key press
	 keybd_event( VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );

	// Simulate a key release
	 keybd_event( VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}
}

void keyboard_set_scroll(bool on) {
    BYTE keyState[256];

    GetKeyboardState((LPBYTE)&keyState);
	
	if( (on && !(keyState[VK_SCROLL] & 1)) ||
	  (!on && (keyState[VK_SCROLL] & 1)) )
	{
	// Simulate a key press
	 keybd_event( VK_SCROLL, 0x91, KEYEVENTF_EXTENDEDKEY | 0, 0 );

	// Simulate a key release
	 keybd_event( VK_SCROLL, 0x91, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}
}

void keyboard_set_map(int key1, int key2) {
	map< int, int >::iterator it = enigma::keybdmap.find( key1 );
    if ( enigma::keybdmap.end() != it ) {
		it->second = key2;
    } else {
		enigma::keybdmap.insert( map< int, int >::value_type(key1, key2) );
	}
}

int keyboard_get_map(int key) {
	map< int, int >::iterator it = enigma::keybdmap.find( key );
    if ( enigma::keybdmap.end() != it ) {
		return it->second;
    } else {
		return key;
	}
}

void keyboard_unset_map() {
	enigma::keybdmap.clear();
}

void mouse_clear(const int button)
{
    enigma::mousestatus[button] = enigma::last_mousestatus[button] = 0;
}

string clipboard_get_text()
{
  if (!OpenClipboard(enigma::hWndParent)) return "";
  unsigned int format=EnumClipboardFormats(0);
  string res = "";
  while (format) {
    if (format == CF_TEXT)
    {
      HGLOBAL clip_data = GetClipboardData(format);
      char* clip_text = (char*)GlobalLock(clip_data);
      if (clip_text)
      {
        unsigned long clip_size = GlobalSize(clip_data) - 1;
        if (clip_size)
          res = string(clip_text,clip_size);
        GlobalUnlock(clip_data); //Give Windows back its clipdata
      }
    }
    format=EnumClipboardFormats(format);  //Next
  }
  CloseClipboard();
  return res;
}

void clipboard_set_text(string text)
{
	HGLOBAL hGlobal, hgBuffer;
	if (!OpenClipboard(enigma::hWndParent)) return;
	EmptyClipboard();
	hGlobal = GlobalAlloc(GMEM_DDESHARE, text.length() + 1);
	hgBuffer = GlobalLock(hGlobal);
    strcpy((char*)hgBuffer, text.c_str());
    GlobalUnlock(hGlobal);
	SetClipboardData(CF_TEXT, (HANDLE)hGlobal);
	CloseClipboard();
}

bool clipboard_has_text()
{
    if (!OpenClipboard(enigma::hWndParent))
        return false;

    bool value = GetClipboardData(CF_TEXT);
    CloseClipboard();
    return value;
}

}

