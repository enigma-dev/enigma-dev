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

// As is typical of Win32 code, this code is fuck-ugly. Refer to the GTK version for
// porting to competent widget systems. Use this only for low-level APIs.

#include <windows.h>
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

struct enigma_window: gtkl_widget {
  int layout_id;
  enigma_window(int id,HWND hwnd, int w, int h): gtkl_widget(id,hwnd,8,8), layout_id(-1) {}
};

struct enigma_button: gtkl_widget {
  enigma_button(int id,HWND hwnd): gtkl_widget(id,hwnd,8,8) {
    
  }
  void resolve() {}
  void resize(int x,int y,int w,int h) { MoveWindow(me,x,y,w,h,1); printf("Resize button to %d, %d\n",w,h); }
};

struct enigma_widget_alignment: gtkl_container
{
  enigma_widget *child;
  enigma_widget_alignment(int id, int w, int h):  gtkl_container(id,w,h,false), child(NULL) {}
  enigma_widget_alignment(int id, int w, int h, enigma_widget* c):  gtkl_container(id,w,h,false), child(c) {}
  void insert(enigma_widget* whom) { child = whom; }
  
  void clear()   { delete child; }
  void resolve() { if (child) srw = child->srw+8, srh = child->srh+8; }
  void resize(int x,int y,int w,int h)  { if (child) child->resize(x+4,y+4,w-8,h-8); }
};


bool wgt_exists(int id) {
  return unsigned(id) < widgets.size() and widgets[id] and (widgets[id]->type != o_widget or IsWindow(getWidget(id)->me));
}

extern HWND enigmaHwnd;
extern HINSTANCE enigmaHinstance;
#define enigma_window_generic_class "enigma_window_generic_class"

static LRESULT CALLBACK GenWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{ 
  switch (message)
  {
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
          printf("Command: clicked button %d\n", LOWORD(wParam));
      break;
    case WM_SIZE:
      {
        if (!widget_idmax) break;
        const int lid = ((enigma_window*)widgets[wParam])->layout_id;
        printf("Resize %d to %d x %d\n",wParam,LOWORD(lParam),HIWORD(lParam));
        if (lid != -1) getContainer(lid)->resize(0,0,LOWORD(lParam),HIWORD(lParam));
      }
      break;
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
    wincl.hInstance = enigmaHinstance;
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
    
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    
    RegisterClassEx (&wincl);
  }
} generic_window;



// Windows

int wgt_window_create(int w, int h)
{
  HWND win = CreateWindowEx(0, enigma_window_generic_class, "caption", WS_POPUP | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, w, h, enigmaHwnd, (HMENU)widget_idmax, enigmaHinstance, (LPVOID)widget_idmax);
  log_enigma_widget(new enigma_window(widget_idmax,win,w,h));
  return widget_idmax++;
}

void wgt_window_show(int wgt)
{
  ShowWindow(getWidget(wgt)->me, true);
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
  table->resize(0,0,wr.right-wr.left,wr.bottom-wr.top);
  return widget_idmax++;
}

void wgt_layout_insert_widget(int layout, string cell, int wgt) {
  ((enigma_widget_alignment*)getTable(layout)->atts[cell[0]].child)->insert(getWidget(wgt));
  SetParent(getWidget(wgt)->me,getWidget(getContainer(layout)->parent_id)->me);
  SetWindowLong(getWidget(wgt)->me,GWL_STYLE,WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON);
  getTable(layout)->resolve();
}


// Buttons

int wgt_button_create(string text)
{
  HWND butn = CreateWindow("button", text.c_str(), BS_PUSHBUTTON, 0, 0, 90, 20, NULL, (HMENU) NULL, NULL, NULL);
  if (!butn) return -1;
  
  log_enigma_widget(new enigma_button(widget_idmax,butn));
  return widget_idmax++;
}

int wgt_button_get_pressed(int butn)
{
  int &clicked = getWidget(butn)->actions;
  const int ret = clicked; clicked = 0;
  return ret;
}


/*/ Combo boxes

int wgt_combobox_create(string contents)
{
  HWND *cbox = gtk_combo_box_new_text();
  const char *st = contents.c_str();
  for (char *mt = (char*)st; *mt; mt++)
  if (*mt == '|')
  {
    *mt = 0;
    gtk_combo_box_append_text(GTK_COMBO_BOX(cbox), st);
    st = mt + 1;
    *mt = '|';
  }
  log_enigma_widget(new enigma_widget(cbox));
  return widget_idmax++;
}

int wgt_combobox_get_selection(int cbbox) {
  return gtk_combo_box_get_active(GTK_COMBO_BOX(widgets[cbbox]->me));
}

string wgt_combobox_get_selected_text(int cbbox) {
  const char* et = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widgets[cbbox]->me));
  return et ? et : "";
}


// Checkboxes

int wgt_checkbox_create(string text)
{
  log_enigma_widget(new enigma_widget(gtk_check_button_new_with_label(text.c_str())));
  return widget_idmax++;
}
bool wgt_checkbox_get_checked(int cbox) {
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets[cbox]->me));
}


// Text entry line

int wgt_textline_create(string text, int numchars)
{
  HWND *ent = gtk_entry_new_with_buffer(gtk_entry_buffer_new(text.c_str(),text.length()));
  gtk_entry_set_width_chars(GTK_ENTRY(ent), numchars);
  log_enigma_widget(new enigma_widget(ent));
  return widget_idmax++;
}

string wgt_textline_get_text(int tline)
{
  GtkEntryBuffer *buf = gtk_entry_get_buffer(GTK_ENTRY(widgets[tline]->me));
  return string(gtk_entry_buffer_get_text(buf),gtk_entry_buffer_get_bytes(buf));
}


// Radio buttons
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
