/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Josh Ventura                                             **
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

// As is typical of Win32 code, this code is fuck-ugly. Refer to the GTK version for
// porting to competent widget systems. Use this only for low-level APIs.

#define WINVER 9001
#include <windows.h>
#define _WIN32_IE 9001
#include <commctrl.h>
#include <windowsx.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>

typedef HWND ui_handle;
#include "gtklike.h"
typedef gtkl_widget enigma_widget;
#define getWidget(x) ((enigma_widget*)(widgets[x]))
#define getContainer(x) ((gtkl_container*)(widgets[x]))
#define getTable(x) ((gtkl_table*)(widgets[x]))

using namespace std;

vector<gtkl_object*> widgets;
static unsigned widget_idmax = 0;

namespace enigma {
  extern HWND hWndParent;
  extern HINSTANCE hInstance;
  bool widget_system_initialize()
  {
    INITCOMMONCONTROLSEX iccex;
    iccex.dwSize = sizeof(iccex);
    iccex.dwICC  = ICC_STANDARD_CLASSES | ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&iccex);
    return true;
  }
}

void enigma_widget::resize(int x,int y,int w,int h) { MoveWindow(me,x,y,w,h,1); }

struct enigma_widget_alignment: gtkl_container
{
  enigma_widget *child;
  enigma_widget_alignment(int id, int w, int h):  gtkl_container(id,w,h,false), child(NULL) {}
  enigma_widget_alignment(int id, int w, int h, enigma_widget* c):  gtkl_container(id,w,h,false), child(c) {}
  void insert(enigma_widget* whom) { child = whom; }
  
  void clear()   { delete child; }
  void resolve() { if (child) child->resolve(), srw = child->srw+8, srh = child->srh+8; }
  void resize(int x,int y,int w,int h)  { if (child) child->resize(x+4,y+4,w-8,h-8); }
};


bool wgt_exists(int id) {
  return unsigned(id) < widgets.size() and widgets[id] and (widgets[id]->type != o_widget or IsWindow(getWidget(id)->me));
}

#define enigma_window_generic_class "enigma_window_generic_class"

#define getID(hwnd) GetWindowLong((HWND)(hwnd),GWL_USERDATA)
#define setID(hwnd,id) SetWindowLong((HWND)(hwnd),GWL_USERDATA,(id))
#define fixFont(hwnd) SendMessage(hwnd,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),0); 

struct enigma_window: enigma_widget {
  int layout_id;
  void resolve() {}
  enigma_window(int id,HWND hwnd, int w, int h): gtkl_widget(id,hwnd,w,h), layout_id(-1) {}
};

static LRESULT CALLBACK GenWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{ 
  switch (message)
  {
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
          getWidget((int)getID(lParam))->actions++;
      break;
    case WM_SIZE:
      {
        if (!widget_idmax) break;
        const int lid = ((enigma_window*)widgets[wParam])->layout_id;
        if (lid != -1) getContainer(lid)->resize(4,4,LOWORD(lParam)-8,HIWORD(lParam)-8);
      }
      break;
    case WM_GETMINMAXINFO:
      if (widget_idmax)
      {
        const int lid = ((enigma_window*)widgets[wParam])->layout_id;
        if (lid != -1) {
          getContainer(lid)->resolve();
          LPMINMAXINFO mmi = (LPMINMAXINFO)lParam;
          RECT r;
          r.left = 0, r.right  = getContainer(lid)->srw;
          r.top  = 0, r.bottom = getContainer(lid)->srh;
          AdjustWindowRect(&r,WS_CAPTION|WS_THICKFRAME,FALSE);
          mmi->ptMinTrackSize.x = r.right- r.left;
          mmi->ptMinTrackSize.y = r.bottom - r.top;
        }
      }
    default:
      return DefWindowProc (hwnd, message, wParam, lParam);
  }
  return 0;
}

void log_enigma_widget(gtkl_object* wgt)
{
  /*const int ind = widgets.size();*/  widgets.push_back(wgt);
  //g_signal_connect(wgt->me, "destroy", G_CALLBACK(widget_destroy), (void*)ind);
}

struct Sgeneric_window
{
  WNDCLASSEX wincl;        /* Data structure for the window class */
  
  Sgeneric_window()
  {
    /* The Window structure */
    wincl.hInstance = enigma::hInstance;
    wincl.lpszClassName = enigma_window_generic_class;
    wincl.lpfnWndProc = GenWindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);
    
    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    
    wincl.hbrBackground = (HBRUSH)COLOR_WINDOW;
    
    RegisterClassEx (&wincl);
  }
} generic_window;



// Windows

int wgt_window_create(int w, int h)
{
  HWND win = CreateWindowEx(0, enigma_window_generic_class, "caption", WS_POPUP | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, w, h, enigma::hWndParent, (HMENU)widget_idmax, enigma::hInstance, (LPVOID)widget_idmax);
  if (!win)
    return -1;
  setID(win,widget_idmax); fixFont(win);
  log_enigma_widget(new enigma_window(widget_idmax,win,w,h));
  return widget_idmax++;
}

void wgt_window_show(int wgt) {
  RECT r;
  GetWindowRect(getWidget(wgt)->me, &r);
  AdjustWindowRect(&r, WS_CAPTION|WS_THICKFRAME, FALSE);
  SetWindowPos(getWidget(wgt)->me, NULL, 0, 0, r.right - r.left, r.bottom - r.top, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOREPOSITION);
  ShowWindow(getWidget(wgt)->me,SW_SHOW);
}



// Layouts

int wgt_layout_create(int win, string layout, int hpad, int vpad)
{
  const char * const lyt = layout.c_str();
  vector<const char*> rows;
  rows.push_back(lyt);
  unsigned cc = 1;
  
  for (unsigned i = 0, tr = 1; lyt[i]; i++, tr++)
  {
    if (lyt[i] == '\r' and lyt[i+1] == '\n') i++;
    if (lyt[i] == '\r' or  lyt[i]   == '\n')
      rows.push_back(lyt+i+1), tr = 0;
    if (tr > cc)
      cc = tr;
  }
  
  enigma_window *window = (enigma_window*)getWidget(win);
  gtkl_table *table = new gtkl_table(rows.size(),cc);
  table->parent_id = win;
  
  for (unsigned y = 0; y < rows.size(); y++)
  for (unsigned x = 0; x < cc; x++)
  {
    // Pick a char
    char cat = rows[y][x];
    unsigned cs = 1, rs = 1;
    
    // Determine cell dimensions
    while (cs < cc - x          and rows[y][x + cs] == cat) cs++;
    while (rs < rows.size() - y and rows[y + rs][x] == cat) rs++;
    
    enigma_widget_alignment *wa = new enigma_widget_alignment(-1,2,2);
    if (!table->attach(wa,x,x+cs,y,y+rs,cat))
      delete wa;
  }
  log_enigma_widget(table);
  window->layout_id = widget_idmax;
  RECT wr; GetClientRect(window->me, &wr);
  table->resize(4,4,wr.right-wr.left-8,wr.bottom-wr.top-8);
  return widget_idmax++;
}

void wgt_layout_insert_widget(int layout, string cell, int wgt) {
  ((enigma_widget_alignment*)getTable(layout)->atts[cell[0]].child)->insert(getWidget(wgt));
  SetParent(getWidget(wgt)->me,getWidget(getContainer(layout)->parent_id)->me);
  SetWindowLong(getWidget(wgt)->me, GWL_STYLE, WS_VISIBLE | WS_CHILD | getWidget(wgt)->attribs);
  getTable(layout)->resolve();
}


// Buttons

struct enigma_button: enigma_widget {
  void resolve();
  enigma_button(int id,HWND hwnd, int at, int w, int h): gtkl_widget(id,hwnd,at,w,h) {}
};

void enigma_button::resolve()
{
  HDC dc = GetDC(me);
  int tl = Button_GetTextLength(me); 
  char t[tl+1]; Button_GetText(me,t,tl+1);
  SIZE d; GetTextExtentPoint32(dc,t,tl,&d);
  ReleaseDC(me,dc);
  srw = d.cx+8, srh = d.cy+8;
}

int wgt_button_create(string text)
{
  HWND butn = CreateWindow("button", text.c_str(), BS_PUSHBUTTON, 0, 0, 24, 24, NULL, (HMENU)NULL, NULL, (LPVOID*)widget_idmax);
  if (!butn) return -1;
  setID(butn,widget_idmax); fixFont(butn);
  log_enigma_widget(new enigma_button(widget_idmax,butn,BS_PUSHBUTTON,24,24));
  return widget_idmax++;
}

int wgt_button_get_pressed(int butn)
{
  int &clicked = getWidget(butn)->actions;
  const int ret = clicked; clicked = 0;
  return ret;
}


// Combo boxes

struct enigma_combobox: enigma_widget
{
  void resolve() { srw = 24, srh = 24; }
  void resize(int xr, int yr, int wr, int hr) { x=xr, y=yr, w=wr, h=24; MoveWindow(me,x,y,w,h+128,1); }
  enigma_combobox(int id,HWND me,int at,int w,int h): enigma_widget(id,me,at,w,h) {}
};

int wgt_combobox_create(string contents)
{
  HWND cbox = CreateWindow("combobox", "", CBS_DROPDOWNLIST | CBS_HASSTRINGS , 0, 0, 24, 144, NULL, (HMENU) NULL, NULL, NULL);
  setID(cbox,widget_idmax); fixFont(cbox);
  const char *st = contents.c_str();
  char *mt;
  for (mt = (char*)st; *mt; mt++)
  if (*mt == '|')
  {
    *mt = 0;
    ComboBox_AddString(cbox, st);
    st = mt + 1;
    *mt = '|';
  }
  if (mt > st) ComboBox_AddString(cbox, st);
  log_enigma_widget(new enigma_combobox(widget_idmax,cbox,CBS_DROPDOWNLIST | CBS_HASSTRINGS,24,24));
  return widget_idmax++;
}

int wgt_combobox_get_selection(int cbbox) {
  return ComboBox_GetCurSel(getWidget(cbbox)->me);
}

string wgt_combobox_get_selected_text(int cbbox) {
  HWND cbox = getWidget(cbbox)->me;
  const int tl = ComboBox_GetTextLength(cbox);
  char et[tl];
  return string(et,ComboBox_GetText(cbox,et,tl));
}


// Checkboxes

struct enigma_checkbox: enigma_button {
  void resolve()
  {
    HDC dc = GetDC(me);
    int tl = Button_GetTextLength(me); 
    char t[tl+1]; Button_GetText(me,t,tl+1);
    SIZE d; GetTextExtentPoint32(dc,t,tl,&d);
    ReleaseDC(me,dc);
    srw = d.cx+8, srh = d.cy+8;
    srw = 24, srh = 24;
  }
  enigma_checkbox(int id,HWND me,int at,int w,int h): enigma_button(id,me,at,w,h) {}
};

int wgt_checkbox_create(string text)
{
  HWND chbox = CreateWindow("button", text.c_str(), BS_AUTOCHECKBOX, 0, 0, 24, 144, NULL, (HMENU) NULL, NULL, NULL);
  setID(chbox,widget_idmax); fixFont(chbox);
  log_enigma_widget(new enigma_checkbox(widget_idmax,chbox,BS_AUTOCHECKBOX,24,24));
  return widget_idmax++;
}
bool wgt_checkbox_get_checked(int cbox) {
  return Button_GetCheck(getWidget(cbox)->me);
}


// Text entry line

struct enigma_textline: enigma_widget {
  void resolve();
  enigma_textline(int id,HWND hwnd, int at, int w, int h): gtkl_widget(id,hwnd,at,w,h) {}
};

void enigma_textline::resolve() { 
  TEXTMETRIC tm; 
  HDC dc = GetDC(me);
  GetTextMetrics(dc,&tm);
  ReleaseDC(me,dc);
  int cc = SendMessage(me,EM_GETLIMITTEXT,0,0);
  srw = tm.tmMaxCharWidth * (cc > 20 ? 20 : cc);
  srh = 16;
}

int wgt_textline_create(string text, int numchars)
{
  HWND ent = CreateWindowEx(WS_EX_CLIENTEDGE,"edit", text.c_str(), 0, 0, 0, 8, 16, NULL, (HMENU) NULL, NULL, NULL);
  Edit_LimitText(ent, numchars);
  setID(ent,widget_idmax); fixFont(ent);
  log_enigma_widget(new enigma_textline(widget_idmax,ent,0,12,24));
  return widget_idmax++;
}

string wgt_textline_get_text(int tline)
{
  HWND ent = getWidget(tline)->me;
  const int bl = Edit_GetTextLength(ent);
  char str[bl];
  return string(str,Edit_GetText(ent,str,bl));
}


/*/ Radio buttons
int wgt_radio_create(string text)
{
  HWND rb = gtk_radio_button_new_with_label(NULL, text.c_str());
  log_enigma_widget(new enigma_widget(rb));
  return widget_idmax++;
}
                      
int wgt_radio_add(string text, int rbo)
{
  HWND rb = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(widgets[rbo]), text.c_str());
  log_enigma_widget(new enigma_widget(rb));
  return widget_idmax++;
}
*/
