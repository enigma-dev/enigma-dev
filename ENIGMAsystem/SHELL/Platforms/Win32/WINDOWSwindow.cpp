/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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
#include "WINDOWSwindow.h"

bool resizeableWindow = false;
char* currentCursor = IDC_ARROW;

namespace enigma
{
  extern HWND hWnd,hWndParent;
  extern int windowcolor; extern double viewscale; extern bool windowIsTop;

  void clampparent()
  {
    RECT c;
    GetWindowRect(hWnd,&c);

    if(resizeableWindow) {
        AdjustWindowRect(&c, WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE | WS_SIZEBOX, FALSE);
    } else {
        AdjustWindowRect(&c, WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE, FALSE);
    }

    SetWindowPos(hWndParent,NULL,c.left,c.top,c.right-c.left,c.bottom-c.top,SWP_NOZORDER);
  }
}


int show_message(string str)
{
  MessageBox(enigma::hWnd,str.c_str(),"Message",MB_OK);
  return 0;
}

int window_get_x()
{
  RECT window;
  GetWindowRect(enigma::hWnd,&window);
  return window.left;
}

int window_get_y()
{
  RECT window;
  GetWindowRect(enigma::hWnd,&window);
  return window.top;
}

int window_get_width()
{
  RECT window;
  GetWindowRect(enigma::hWnd,&window);
  return window.right-window.left;
}

int window_get_height()
{
  RECT window;
  GetWindowRect(enigma::hWnd,&window);
  return window.bottom-window.top;
}

void window_set_visible(bool visible)
{
  ShowWindow(enigma::hWnd,visible);
}

int window_get_visible()
{
   return IsWindowVisible(enigma::hWnd);
}



void window_set_stayontop(const bool stay)
{
   if ((bool)stay)
   {
     SetWindowPos(enigma::hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
     enigma::windowIsTop=1;
   } else
   {
     SetWindowPos(enigma::hWnd,HWND_BOTTOM,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE);
     SetWindowPos(enigma::hWnd,HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
     enigma::windowIsTop=0; //Cheap hax, but every windows program I've experimented with does it,
     //though GM changes it back every step if you try to make it stay on top. It does not notice if you send it back.
   }
}
int window_get_stayontop()
{
  return enigma::windowIsTop;
}

void window_set_caption(char* caption)
{
  SetWindowText(enigma::hWnd,caption);
}

void window_set_caption(string caption) {
  SetWindowText(enigma::hWndParent,(char*) caption.c_str());
}

string window_get_caption()
{
  char text_buffer[513];
  GetWindowText(enigma::hWnd, text_buffer, 512);
  return text_buffer;
}

void window_set_color(int color) {
  enigma::windowcolor=(int)color;
}

int window_get_color() {
  return enigma::windowcolor;
}

int window_set_region_scale(double scale, const bool adaptwindow)
{
  enigma::viewscale=scale;
  if ((bool)adaptwindow)
  {
    RECT window; GetWindowRect(enigma::hWnd,&window);
    SetWindowPos(enigma::hWnd, HWND_TOP, window.right, window.top, (int)(room_width*scale), (int)(room_height*scale), SWP_SHOWWINDOW);
  }
  return 0;
}
double window_get_region_scale()
{
  return enigma::viewscale;
}

void window_set_position(int x, int y)
{
  SetWindowPos(enigma::hWnd, HWND_TOP, (int)x, (int)y, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE);
}
void window_set_size(unsigned int width, unsigned int height)
{
    SetWindowPos(enigma::hWnd, HWND_TOP, 0, 0, (int)width, (int)height, SWP_NOMOVE|SWP_NOACTIVATE);
    enigma::clampparent();
}

void window_set_rectangle(int x, int y, int width, int height)
{
    SetWindowPos(enigma::hWnd, HWND_TOP, x, y, width, height, SWP_SHOWWINDOW);
    enigma::clampparent();
}

void window_center()
{
    RECT window;
    GetWindowRect(enigma::hWndParent,&window);

    int tmp_wind_width = window.right - window.left;
    int tmp_wind_height = window.bottom - window.top;
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(enigma::hWndParent, HWND_TOP, (screen_width-tmp_wind_width)/2, (screen_height-tmp_wind_height)/2, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE|SWP_DRAWFRAME);
}

void window_default()
{
    RECT window;
    GetWindowRect(enigma::hWnd,&window);

    int xm=0, ym=0;
    if (view_enabled) for (int i=0;i<7;i++)
    {
      if (view_visible[i])
      { if (view_xview[i]+view_wview[i]>xm) xm=(int)(view_xview[i]+view_wview[i]);
        if (view_yview[i]+view_hview[i]>ym) ym=(int)(view_yview[i]+view_hview[i]); }
    } else { xm=(int)room_width; ym=(int)room_height; }

    int screen_width = GetSystemMetrics(SM_CXBORDER);
    int screen_height = GetSystemMetrics(SM_CYBORDER);
    SetWindowPos(enigma::hWnd, HWND_TOP, screen_width - xm/2, screen_height - ym/2, xm, ym, SWP_SHOWWINDOW);
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

/*
int window_views_mouse_get_x()   //NOTE: mousex/y should be set to these so they are relative to the view
{
    RECT window;
    GetWindowRect(enigma::hWnd,&window);
    POINT mouse;
	GetCursorPos(&mouse);
    if (!view_enabled)
    {
        return mouse.x-window.left;
    }
    else
    {
        int mp = -1, vp;
        for (int v = 6; v >= 0; v--)
        {
            if (!view_visible[v])
                continue;
            if (view_xview[v] < mp || mp == -1)
                mp = view_xview[v];
        }
        double xpos = mouse.x - window.left;
        if (xpos < 0)
        {
            return mp + xpos;
        }
        if (mouse.x > window.right)
        {
            mp = room_width;
            for (int v = 6; v >= 0; v--)
            {
                if (!view_visible[v])
                    continue;
                vp = view_xview[v] + view_wview[v];
                if (vp > mp)
                    mp = vp;
            }
            return mp + (mouse.x - window.right);
        }
        for (int v = 6; v >= 0; v--)
        {
            if (!view_visible[v])
                continue;
            vp = xpos + view_xview[v] - mp;
            if (vp >= 0)
            {
                return vp;
            }
        }
        return mouse.x-window.left;
    }
}

int window_views_mouse_get_y()
{
    RECT window;
    GetWindowRect(enigma::hWnd,&window);
    POINT mouse;
	GetCursorPos(&mouse);

    if (!view_enabled)
    {
        return mouse.y-window.top;
    }
    else
    {
        int mp = -1, vp;
        for (int v = 6; v >= 0; v--)
        {
            if (!view_visible[v])
                continue;
            if (view_yview[v] < mp || mp == -1)
                mp = view_yview[v];
        }
        double ypos = mouse.y - window.top;
        if (ypos < 0)
        {
            return mp + ypos;
        }
        if (mouse.y > window.bottom)
        {
            mp = room_height;
            for (int v = 6; v >= 0; v--)
            {
                if (!view_visible[v])
                    continue;
                vp = view_yview[v] + view_hview[v];
                if (vp > mp)
                    mp = vp;
            }
            return mp + (mouse.y - window.bottom);
        }
        for (int v = 6; v >= 0; v--)
        {
            if (!view_visible[v])
                continue;
            vp = ypos + view_yview[v] - mp;
            if (vp >= 0)
            {
                return vp;
            }
        }
        return mouse.y-window.top;
    }
}
void window_views_mouse_set(int x, int y)
{
    RECT window;
    GetWindowRect(enigma::hWnd,&window);

    if (!view_enabled)
    {
        SetCursorPos(window.left + x,window.top + y);
    }
    else
    {
        SetCursorPos(window.left + x,window.top + y);
    }
}*/

namespace enigma {
  long int laststyle;
  int isFullScreen=0;
}
void window_set_fullscreen(const bool full)
{
  if (full)
  {
    if (!enigma::isFullScreen)
    {
      enigma::laststyle=SetWindowLongPtr(enigma::hWndParent,GWL_STYLE,WS_POPUP);
      ShowWindow(enigma::hWndParent,SW_MAXIMIZE);
      ShowWindow(enigma::hWnd,SW_MAXIMIZE);
      enigma::isFullScreen=1;
    }
  }
  else
  {
    if (enigma::isFullScreen)
    {
      SetWindowLongPtr(enigma::hWndParent,GWL_STYLE,enigma::laststyle);
      ShowWindow(enigma::hWndParent,SW_RESTORE);
      ShowWindow(enigma::hWnd,SW_RESTORE);
      enigma::isFullScreen=0;
    }
  }
}
int window_get_fullscreen() { return enigma::isFullScreen;  }

/*
window_set_showborder(show)
window_get_showborder()
window_set_showicons(show)
window_get_showicons()
window_set_sizeable(sizeable)
window_get_sizeable()

// section 5.10

view_angle[0..7]
view_hspeed[0..7]
view_vspeed[0..7]
window_set_region_size(w,h,adaptwindow)
window_get_region_width()
window_get_region_height()
window_views_mouse_get_x()
window_views_mouse_get_y()
window_views_mouse_set(x,y)
*/


int window_set_cursor(int curs)
{
    switch (curs)
    {
        case cr_default:
            currentCursor = IDC_ARROW; return 1;
            break;
        case cr_none:
            currentCursor = NULL; return 1;
            break;
        case cr_arrow:
            currentCursor = IDC_ARROW; return 1;
            break;
        case cr_cross:
            currentCursor = IDC_CROSS; return 1;
            break;
        case cr_beam:
            currentCursor = IDC_IBEAM; return 1;
            break;
        case cr_size_nesw:
            currentCursor = IDC_SIZENESW; return 1;
            break;
        case cr_size_ns:
            currentCursor = IDC_SIZENS; return 1;
            break;
        case cr_size_nwse:
            currentCursor = IDC_SIZENWSE; return 1;
            break;
        case cr_size_we:
            currentCursor = IDC_SIZEWE; return 1;
            break;
        case cr_uparrow:
            currentCursor = IDC_UPARROW; return 1;
            break;
        case cr_hourglass:
            currentCursor = IDC_WAIT; return 1;
            break;
        case cr_drag:
            // Delphi-made?
            break;
        case cr_nodrop:
            currentCursor = IDC_NO; return 1;
            break;
        case cr_hsplit:
            // Delphi-made?
            currentCursor = IDC_SIZENS; return 1;
            break;
        case cr_vsplit:
            // Delphi-made?
            currentCursor = IDC_SIZEWE; return 1;
            break;
        case cr_multidrag:
            currentCursor = IDC_SIZEALL; return 1;
            break;
        case cr_sqlwait:
            // DEAR GOD WHY
            currentCursor = IDC_WAIT; return 1;
            break;
        case cr_no:
            currentCursor = IDC_NO; return 1;
            break;
        case cr_appstart:
            currentCursor = IDC_APPSTARTING; return 1;
            break;
        case cr_help:
            currentCursor = IDC_HELP; return 1;
            break;
        case cr_handpoint:
            currentCursor = IDC_HAND; return 1;
            break;
        case cr_size_all:
            currentCursor = IDC_SIZEALL; return 1;
            break;
    }
    return 0;
}

/*
int window_get_curor()
{
    //Returns C++'s current cursor
    return GetCursor();
}
*/


void game_end() { PostQuitMessage(0); }
void action_end_game() { game_end(); }

#include "Universal_System/globalupdate.h"
#include "WINDOWScallback.h"

void io_handle()
{
  MSG msg;
  enigma::input_push();
  while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage (&msg);
    DispatchMessage (&msg);
  }
  enigma::update_globals();
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

void mouse_clear(const int button)
{
    enigma::mousestatus[button] = enigma::last_mousestatus[button] = 0;
}
