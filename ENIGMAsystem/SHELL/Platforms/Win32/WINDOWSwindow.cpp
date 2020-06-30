/** Copyright (C) 2008-2017 Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
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

#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/PFmain.h" // For those damn vk_ constants.
#include "Platforms/General/PFwindow.h"

#include "WINDOWSicon.h"

#include "Widget_Systems/widgets_mandatory.h"

#include "strings_util.h" // For string_replace_all
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h" // room_caption
#include "Universal_System/globalupdate.h"

#define byte __windows_byte_workaround
#include <windows.h>
#undef byte
// mingw32 cross-compile bug workaround below
#ifndef MAPVK_VK_TO_VSC_EX
#define MAPVK_VK_TO_VSC_EX 0x04
#endif

#include <stdio.h>
#include <string>
using namespace std;

namespace {

std::string current_caption = "";

} // anonymous namespace

namespace enigma
{

extern HWND hWnd;
HCURSOR currentCursor = LoadCursor(NULL, IDC_ARROW);

void configure_devmode(DEVMODE &devMode, int w, int h, int freq, int bitdepth) {
  if (w != -1) {
    devMode.dmFields |= DM_PELSWIDTH;
    devMode.dmPelsWidth = w;
  }

  if (h != -1) {
    devMode.dmFields |= DM_PELSHEIGHT;
    devMode.dmPelsHeight = h;
  }

  if (freq != -1) {
    devMode.dmFields |= DM_DISPLAYFREQUENCY;
    devMode.dmDisplayFrequency = freq;
  }

  if (bitdepth != -1) {
    devMode.dmFields |= DM_BITSPERPEL;
    devMode.dmBitsPerPel = bitdepth;
  }
}

}

namespace enigma_user {

window_t window_handle() {
  return reinterpret_cast<window_t>(enigma::hWnd);
}

// returns an identifier for the HWND window
// this string can be used in shell scripts
wid_t window_identifier() {
  return std::to_string(reinterpret_cast<unsigned long long>(window_handle()));
}

// returns an identifier for certain window
// this string can be used in shell scripts
wid_t window_get_identifier(window_t hwnd) {
  return std::to_string(reinterpret_cast<unsigned long long>(hwnd));
}

static int currentIconIndex = -1;
static unsigned currentIconFrame;

int window_get_icon_index() {
  return currentIconIndex;
}

unsigned window_get_icon_subimg() {
  return currentIconFrame;
}

void window_set_icon(int ind, unsigned subimg) {
  // the line below prevents glitchy minimizing when 
  // icons are changed rapidly (i.e. for animation).
  if (window_get_minimized()) return;

  // needs to be visible first to prevent segfault
  if (!window_get_visible()) window_set_visible(true);
  enigma::SetIconFromSprite(enigma::hWnd, ind, subimg);

  currentIconIndex = ind;
  currentIconFrame = subimg;
}

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

void window_set_caption(const string &caption)
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
  SetWindowLongPtr(enigma::hWnd, GWL_EXSTYLE,
      GetWindowLongPtr(enigma::hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

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
  if (window_get_fullscreen()) return;
  enigma::windowX = x;
  enigma::windowY = y;
  SetWindowPos(enigma::hWnd, HWND_TOP, enigma::windowX, enigma::windowY, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE);
}

void window_set_size(unsigned int width, unsigned int height)
{
  if (window_get_fullscreen()) return;
  window_set_rectangle(enigma::windowX, enigma::windowY, width, height);
}

void window_set_rectangle(int x, int y, int width, int height) {
  if (window_get_fullscreen()) return;
  RECT c;
  c.left = (enigma::windowX = x); c.top = (enigma::windowY = y); c.right = enigma::windowX + (enigma::windowWidth = width); c.bottom = enigma::windowY + (enigma::windowHeight = height);
  AdjustWindowRect(&c, GetWindowLongPtr(enigma::hWnd, GWL_STYLE), false);
  SetWindowPos(enigma::hWnd, HWND_TOP, c.left, c.top, c.right-c.left, c.bottom-c.top, SWP_NOZORDER|SWP_FRAMECHANGED);
}

namespace {

bool prefer_borderless = !enigma::showBorder;
bool prefer_sizeable = enigma::isSizeable;
bool prefer_fullscreen = false;
bool prefer_stayontop = false;
int tmpWidth = enigma::windowWidth;
int tmpHeight = enigma::windowHeight;

}  // anonymous namespace

void window_set_fullscreen(bool full) {
  if (window_get_fullscreen() == full) return;
  enigma::isFullScreen = full;
  prefer_fullscreen = full;
  // tweak the style first to remove or restore the window border
  if (full) {
    prefer_borderless = !window_get_showborder();
    prefer_sizeable = window_get_sizeable();
    prefer_stayontop = window_get_stayontop();
    window_set_maximized(false);
    tmpWidth = window_get_width();
    tmpHeight = window_get_height();
    window_set_stayontop(false);
    LONG_PTR style = GetWindowLongPtr(enigma::hWnd, GWL_STYLE);
    style &= ~(WS_CAPTION | WS_BORDER | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX);
    SetWindowLongPtr(enigma::hWnd, GWL_STYLE, style);
    window_set_maximized(full);
  } else {
    window_set_maximized(full);
    window_set_showborder(!prefer_borderless);
    window_set_sizeable(prefer_sizeable);
    window_set_stayontop(prefer_stayontop);
    window_set_size(tmpWidth, tmpHeight);
  }
  enigma::compute_window_scaling();
}

bool window_get_fullscreen() {
  return prefer_fullscreen;
}

void window_set_sizeable(bool sizeable) {
  if (window_get_maximized()) return;
  if (window_get_fullscreen()) return;
  if (window_get_sizeable() == sizeable) return;
  prefer_sizeable = sizeable;
  enigma::isSizeable = sizeable;
  int tmp2Width = window_get_width();
  int tmp2Height = window_get_height();
  DWORD style = GetWindowLongPtr(enigma::hWnd, GWL_STYLE);
  if (sizeable) style |= WS_SIZEBOX | WS_MAXIMIZEBOX;
  else style &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);
  style |= WS_MINIMIZEBOX;
  SetWindowLongPtr(enigma::hWnd, GWL_STYLE, style);
  window_set_size(tmp2Width, tmp2Height);
}

bool window_get_sizeable() {
  return (GetWindowLongPtr(enigma::hWnd, GWL_STYLE) & WS_THICKFRAME) == WS_THICKFRAME;
}

void window_set_showborder(bool show) {
  if (window_get_maximized()) return;
  if (window_get_fullscreen()) return;
  if (window_get_showborder() == show) return;
  prefer_borderless = !show;
  enigma::showBorder = show;
  int tmp2Width = window_get_width();
  int tmp2Height = window_get_height();
  DWORD style = GetWindowLongPtr(enigma::hWnd, GWL_STYLE);
  if (show) style |= WS_CAPTION | WS_BORDER | WS_MINIMIZEBOX;
  else style &= ~(WS_CAPTION | WS_BORDER);
  if (show && prefer_sizeable) style |= WS_MAXIMIZEBOX;
  SetWindowLongPtr(enigma::hWnd, GWL_STYLE, style);
  window_set_size(tmp2Width, tmp2Height);
}

bool window_get_showborder() {
  return (GetWindowLongPtr(enigma::hWnd, GWL_STYLE) & WS_CAPTION) == WS_CAPTION;
}

void window_set_showicons(bool show) {
  SetWindowLongPtr(enigma::hWnd, GWL_EXSTYLE,
      GetWindowLongPtr(enigma::hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
}

bool window_get_showicons() {
  return (GetWindowLongPtr(enigma::hWnd, GWL_STYLE) & WS_THICKFRAME) == WS_THICKFRAME;
}

void window_set_visible(bool visible) {
  ShowWindow(enigma::hWnd, visible? SW_SHOW : SW_HIDE);
}

int window_get_visible() {
  return IsWindowVisible(enigma::hWnd);
}

void window_set_minimized(bool minimized) {
  ShowWindow(enigma::hWnd, minimized? SW_MINIMIZE : SW_RESTORE);
}

void window_set_maximized(bool maximized) {
  ShowWindow(enigma::hWnd, maximized? SW_MAXIMIZE : SW_RESTORE);
}

bool window_get_minimized() {
  return IsIconic(enigma::hWnd);
}

bool window_get_maximized() {
  return IsZoomed(enigma::hWnd);
}

void window_set_stayontop(bool stay)
{
  if (stay) {
    SetWindowPos(enigma::hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
  } else {
    SetWindowPos(enigma::hWnd,HWND_BOTTOM,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE);
    SetWindowPos(enigma::hWnd,HWND_TOP,   0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
  }
}

bool window_get_stayontop() {
    return (GetWindowLongPtr(enigma::hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST) == WS_EX_TOPMOST;
}

int display_mouse_get_x() {
  POINT mouse;
  GetCursorPos(&mouse);
  return mouse.x;
}

int display_mouse_get_y() {
  POINT mouse;
  GetCursorPos(&mouse);
  return mouse.y;
}

void display_mouse_set(int x,int y) {
    SetCursorPos(x,y);
}

int display_get_x() {
  // Windows is different than our Unix-like platforms in that this value is always zero...
  return 0;
}

int display_get_y() {
  // Windows is different than our Unix-like platforms in that this value is always zero...
  return 0;
}

int display_get_width() {
   return GetSystemMetrics(SM_CXSCREEN);
}

int display_get_height() {
   return GetSystemMetrics(SM_CYSCREEN);
}

int display_get_colordepth() {
  return GetDeviceCaps(GetDC(enigma::hWnd), BITSPIXEL);
}

int display_get_frequency() {
  return GetDeviceCaps(GetDC(enigma::hWnd), VREFRESH);
}

unsigned display_get_dpi_x() {
  return GetDeviceCaps(GetDC(enigma::hWnd), LOGPIXELSX);
}

unsigned display_get_dpi_y() {
  return GetDeviceCaps(GetDC(enigma::hWnd), LOGPIXELSY);
}

void display_reset()
{
	ChangeDisplaySettings(NULL, 0);
}

bool display_set_size(int w, int h)
{
	DEVMODE devMode;

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		return false;

	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	devMode.dmPelsWidth = w;
	devMode.dmPelsHeight = h;

	return (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL);
}

bool display_set_frequency(int freq)
{
	DEVMODE devMode;

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		return false;

	devMode.dmFields = DM_DISPLAYFREQUENCY;
	devMode.dmDisplayFrequency = freq;

	return (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL);
}

bool display_set_colordepth(int depth)
{
	DEVMODE devMode;

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		return true;

	devMode.dmFields = DM_BITSPERPEL;
	devMode.dmBitsPerPel = depth;

  return (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL);
}

bool display_set_all(int w, int h, int freq, int bitdepth)
{
	DEVMODE devMode;

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		return false;

	enigma::configure_devmode(devMode, w, h, freq, bitdepth);

	return (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL);
}

bool display_test_all(int w, int h, int freq, int bitdepth)
{
	DEVMODE devMode;

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		return false;

	enigma::configure_devmode(devMode, w, h, freq, bitdepth);

	return (ChangeDisplaySettings(&devMode, CDS_TEST) == DISP_CHANGE_SUCCESSFUL);
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

}

static void keyboard_set_direct(int key, bool down) {
  UINT scancode = MapVirtualKey(key, MAPVK_VK_TO_VSC_EX);
  DWORD flags = ((scancode >> 8) == 0xE0) ? KEYEVENTF_EXTENDEDKEY : 0;
  if (!down) flags |= KEYEVENTF_KEYUP;
  keybd_event(key, scancode, flags, 0);
}

namespace enigma_user
{

int window_set_cursor(int c)
{
  enigma::cursorInt = c;
  char* cursor = NULL;
  // this switch statement could be replaced with an array aligned to the constants
  switch (c)
  {
      case cr_default:
      case cr_arrow:
          cursor = IDC_ARROW;
          break;
      case cr_none:
          cursor = NULL;
          break;
      case cr_cross:
          cursor = IDC_CROSS;
          break;
      case cr_beam:
          cursor = IDC_IBEAM;
          break;
      case cr_size_nesw:
          cursor = IDC_SIZENESW;
          break;
      case cr_size_ns:
          cursor = IDC_SIZENS;
          break;
      case cr_size_nwse:
          cursor = IDC_SIZENWSE;
          break;
      case cr_size_we:
          cursor = IDC_SIZEWE;
          break;
      case cr_uparrow:
          cursor = IDC_UPARROW;
          break;
      case cr_hourglass:
          cursor = IDC_WAIT;
          break;
      case cr_drag:
          // Delphi-made?
          cursor = IDC_HAND;
          break;
      case cr_nodrop:
          cursor = IDC_NO;
          break;
      case cr_hsplit:
          // Delphi-made?
          cursor = IDC_SIZENS;
          break;
      case cr_vsplit:
          // Delphi-made?
          cursor = IDC_SIZEWE;
          break;
      case cr_multidrag:
          cursor = IDC_SIZEALL;
          break;
      case cr_sqlwait:
          // DEAR GOD WHY
          cursor = IDC_WAIT;
          break;
      case cr_no:
          cursor = IDC_NO;
          break;
      case cr_appstart:
          cursor = IDC_APPSTARTING;
          break;
      case cr_help:
          cursor = IDC_HELP;
          break;
      case cr_handpoint:
          cursor = IDC_HAND;
          break;
      case cr_size_all:
          cursor = IDC_SIZEALL;
          break;
  }
  enigma::currentCursor = LoadCursor(NULL, cursor);
  return SendMessage(enigma::hWnd, WM_SETCURSOR, (WPARAM)enigma::hWnd, MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
}

int window_get_cursor()
{
  return enigma::cursorInt;
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
  keyboard_set_direct(key, true);
}

void keyboard_key_release(int key) {
  keyboard_set_direct(key, false);
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
  text = string_replace_all(text, "\n", "\r\n"); //Otherwise newlines don't work
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
