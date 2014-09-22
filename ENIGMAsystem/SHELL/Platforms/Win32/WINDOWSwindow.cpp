/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
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
    extern HWND hWnd;
    bool isVisible = true, windowIsTop = false, windowAdapt = true, gameWindowFocused = false;
    int cursorInt = 0, regionWidth = 0, regionHeight = 0, windowWidth = 0, windowHeight = 0, windowX = 0, windowY = 0;
    double scaledWidth = 0, scaledHeight = 0;
    char* currentCursor = IDC_ARROW;
    extern bool isSizeable, showBorder, showIcons, freezeOnLoseFocus, isFullScreen;
    extern int viewScale, windowColor;

    LONG_PTR getwindowstyle()
    {
        LONG_PTR newlong = WS_MINIMIZEBOX;
        if (isSizeable)
            newlong |= WS_SIZEBOX;
        if (showBorder)
            newlong |= WS_CAPTION;
        if (showIcons)
            newlong |= WS_POPUPWINDOW;
        if (isVisible)
            newlong |= WS_VISIBLE | WS_EX_APPWINDOW;

        return newlong;
    }
    
    void centerwindow() {
      int screen_width = GetSystemMetrics(SM_CXSCREEN);
      int screen_height = GetSystemMetrics(SM_CYSCREEN);
      enigma::windowX = (screen_width - enigma::regionWidth)/2;
      enigma::windowY = (screen_height - enigma::regionHeight)/2;
    }

    void clampwindow()
    {
        RECT c;
        c.left = enigma::windowX; c.top = enigma::windowY; c.right = enigma::windowX + enigma::windowWidth; c.bottom = enigma::windowY + enigma::windowHeight;
        AdjustWindowRect(&c, getwindowstyle(), false);
        SetWindowPos(enigma::hWnd, HWND_TOP, c.left, c.top, c.right-c.left, c.bottom-c.top, SWP_NOZORDER|SWP_FRAMECHANGED);
    }

    void setwindowstyle()
    {
        SetWindowLongPtr(enigma::hWnd,GWL_STYLE,getwindowstyle());
        clampwindow();
    }

    void setwindowsize()
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

        if (!isFullScreen)
        {
            if (windowAdapt && viewScale > 0) // If the window is to be adapted and Fixed Scale
            {
                if (scaledWidth > windowWidth)
                    windowWidth = scaledWidth;
                if (scaledHeight > windowHeight)
                    windowHeight = scaledHeight;
            }
             clampwindow();
        } else {
          SetWindowPos(hWnd, NULL, 0, 0, parWidth, parHeight, SWP_NOACTIVATE); 
        }
      
    }
}

namespace enigma_user
{

// GM8.1 Used its own internal variables for these functions and reported the regular window dimensions when minimized,
// Studio uses the native functions and will tell you the dimensions of the window are 0 when it is minimized,
// I have decided to go with the Studio method because it is still backwards compatible but also because it saves us some variables.
// NOTE: X and Y are -32000 when the window is minimized.
int window_get_x()
{
  RECT rc;
  GetWindowRect(enigma::hWnd, &rc);
  return rc.left;
}

int window_get_y()
{
  RECT rc;
  GetWindowRect(enigma::hWnd, &rc);
  return rc.top;
}

int window_get_width()
{
  RECT rc;
  GetClientRect(enigma::hWnd, &rc);
  return rc.right-rc.left;
}

int window_get_height()
{
  RECT rc;
  GetClientRect(enigma::hWnd, &rc);
  return rc.bottom-rc.top;
}

void window_set_caption(string caption)
{
/*  if (caption == "")
      if (score != 0)
        caption = "Score: " + string(score);  //GM does this but it's rather fucktarded */

    if (caption != current_caption)
    {
        SetWindowText(enigma::hWnd,(char*) caption.c_str());
        current_caption = caption;
    }
}

string window_get_caption()
{
  return current_caption;
}

void window_set_alpha(double alpha) {
  // Set WS_EX_LAYERED on this window
  SetWindowLong(enigma::hWnd, GWL_EXSTYLE,
  GetWindowLong(enigma::hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

  // Make this window transparent
  SetLayeredWindowAttributes(enigma::hWnd, 0, (unsigned char)(alpha*255), LWA_ALPHA);
}

double window_get_alpha() {
	unsigned char alpha;
	// Make this window transparent
	GetLayeredWindowAttributes(enigma::hWnd, 0, &alpha, 0);
	return alpha/255;
}

void window_set_position(int x, int y)
{
    enigma::windowX = x;
    enigma::windowY = y;
    SetWindowPos(enigma::hWnd, HWND_TOP, enigma::windowX, enigma::windowY, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE);
}

void window_set_size(unsigned int width, unsigned int height)
{
    enigma::windowWidth = width;
    enigma::windowHeight = height;
    enigma::setwindowsize();
}

void window_set_rectangle(int x, int y, int width, int height)
{
    enigma::windowX = x;
    enigma::windowY = y;
    enigma::windowWidth = width;
    enigma::windowHeight = height;
    enigma::setwindowsize();
}

void window_center()
{
  enigma::centerwindow();
  enigma::clampwindow();
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
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    // By default if the room is too big instead of creating a gigantic ass window
    // make it not bigger than the screen to full screen it, this is what 8.1 and Studio
    // do, if the user wants to manually override this they can using
    // views/screen_set_viewport or window_set_size/window_set_region_size
    // We won't limit those functions like GM, just the default.
    if (xm > screen_width) xm = screen_width;
    if (ym > screen_height) ym = screen_height;
  }
  bool center = true;
  if (center_size) {
    center = (xm != enigma::windowWidth || ym != enigma::windowHeight);
  }
  
  enigma::windowWidth = enigma::regionWidth = xm;
  enigma::windowHeight = enigma::regionHeight = ym;
  if (center)
    enigma::centerwindow();

  if (enigma::isFullScreen)
  {
      SetWindowLongPtr(enigma::hWnd,GWL_STYLE,WS_POPUP);
      ShowWindow(enigma::hWnd,SW_MAXIMIZE);
  }
  else
  {
      enigma::setwindowstyle();
      ShowWindow(enigma::hWnd,SW_RESTORE);
  }

  enigma::setwindowsize();
}

void window_set_fullscreen(bool full)
{
    if (enigma::isFullScreen == full)
      return;

    if ((enigma::isFullScreen = full))
    {
      SetWindowLongPtr(enigma::hWnd,GWL_STYLE,WS_POPUP);
      ShowWindow(enigma::hWnd,SW_MAXIMIZE);
    }
    else
    {
      enigma::setwindowstyle();
      ShowWindow(enigma::hWnd,SW_RESTORE);
    }
    enigma::setwindowsize();
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
    enigma::setwindowstyle();
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
    enigma::setwindowstyle();
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
    enigma::setwindowstyle();
}

bool window_get_showicons()
{
    return enigma::showIcons;
}

void window_set_visible(bool visible)
{
    if (enigma::isVisible == visible)
        return;

    if (!visible) {
      ShowWindow(enigma::hWnd,SW_HIDE);
    } else {
      ShowWindow(enigma::hWnd,SW_SHOW);
    }
    enigma::isVisible = visible;
    enigma::setwindowstyle();
}

int window_get_visible()
{
    return enigma::isVisible;
}

void window_set_minimized(bool minimized)
{
    ShowWindow(enigma::hWnd,(minimized?SW_MINIMIZE:SW_RESTORE));
}

bool window_get_minimized()
{
    return IsIconic(enigma::hWnd);
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
  POINT mouse;
  GetCursorPos(&mouse);
  ScreenToClient(enigma::hWnd, &mouse);

  return mouse.x;
}

int window_mouse_get_y()
{
  POINT mouse;
  GetCursorPos(&mouse);
  ScreenToClient(enigma::hWnd, &mouse);

  return mouse.y;
}

void window_mouse_set(int x, int y)
{
  POINT pt;
  pt.x=x;
  pt.y=y;
  ClientToScreen(enigma::hWnd, &pt);
  SetCursorPos(pt.x, pt.y);
}

void window_set_color(int color)
{
    enigma::windowColor = color;
}

int window_get_color()
{
    return enigma::windowColor;
}

}

namespace enigma_user
{

int window_set_cursor(int c)
{
    // this switch statement could be replaced with an array aligned to the constants
    switch (c)
    {
        enigma::cursorInt = c;
        case cr_default:
            enigma::currentCursor= IDC_ARROW;
            break;
        case cr_none:
            enigma::currentCursor = NULL;
            break;
        case cr_arrow:
            enigma::currentCursor = IDC_ARROW;
            break;
        case cr_cross:
            enigma::currentCursor = IDC_CROSS;
            break;
        case cr_beam:
            enigma::currentCursor = IDC_IBEAM;
            break;
        case cr_size_nesw:
            enigma::currentCursor = IDC_SIZENESW;
            break;
        case cr_size_ns:
            enigma::currentCursor = IDC_SIZENS;
            break;
        case cr_size_nwse:
            enigma::currentCursor = IDC_SIZENWSE;
            break;
        case cr_size_we:
            enigma::currentCursor = IDC_SIZEWE;
            break;
        case cr_uparrow:
            enigma::currentCursor = IDC_UPARROW;
            break;
        case cr_hourglass:
            enigma::currentCursor = IDC_WAIT;
            break;
        case cr_drag:
            // Delphi-made?
            break;
        case cr_nodrop:
            enigma::currentCursor = IDC_NO;
            break;
        case cr_hsplit:
            // Delphi-made?
            enigma::currentCursor = IDC_SIZENS;
            break;
        case cr_vsplit:
            // Delphi-made?
            enigma::currentCursor = IDC_SIZEWE;
            break;
        case cr_multidrag:
            enigma::currentCursor = IDC_SIZEALL;
            break;
        case cr_sqlwait:
            // DEAR GOD WHY
            enigma::currentCursor = IDC_WAIT;
            break;
        case cr_no:
            enigma::currentCursor = IDC_NO;
            break;
        case cr_appstart:
            enigma::currentCursor = IDC_APPSTARTING;
            break;
        case cr_help:
            enigma::currentCursor = IDC_HELP;
            break;
        case cr_handpoint:
            enigma::currentCursor = IDC_HAND;
            break;
        case cr_size_all:
            enigma::currentCursor = IDC_SIZEALL;
            break;
    }
    
    return SendMessage(enigma::hWnd, WM_SETCURSOR, (WPARAM)enigma::hWnd, MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
}


int window_get_curor()
{
    return enigma::cursorInt;
}

unsigned long long window_handle()
{
    return (unsigned long long)enigma::hWnd;
}

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
      {
        io_clear();
        return;
      }
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
  if (!OpenClipboard(enigma::hWnd)) return "";
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
	if (!OpenClipboard(enigma::hWnd)) return;
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
  if (!OpenClipboard(enigma::hWnd))
      return false;

  bool value = GetClipboardData(CF_TEXT);
  CloseClipboard();
  return value;
}

}

