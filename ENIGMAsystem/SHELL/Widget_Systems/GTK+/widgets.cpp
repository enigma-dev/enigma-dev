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

#include <gtk/gtk.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

const GtkAttachOptions GTK_EXPAND_FILL = GtkAttachOptions(GTK_FILL | GTK_EXPAND);

typedef pair<string,GtkWidget*> gwpair;
typedef map<string,GtkWidget*> swmap;
struct enigma_widget
{
  GtkWidget* me; // This item's GTK widget
  swmap occc;   // Occupiable Child widgets
  int actions;
  bool exists;
  enigma_widget(GtkWidget* x): me(x), actions(0), exists(1) {}
};
vector<enigma_widget*> widgets;
static unsigned widget_idmax = 0;

struct threadsafety
{
  inline  threadsafety() { gdk_threads_enter(); }
  inline ~threadsafety() { gdk_threads_leave(); }
};

static void widget_destroy(GtkObject *who, gpointer wgtid)
{
  printf("Destroyed widget #%d\n",(int)long(wgtid));
  widgets[(int)long(wgtid)]->exists = false;
  if (widgets[(int)long(wgtid)]->occc.size())
    printf("  (and %lu children)\n",widgets[(int)long(wgtid)]->occc.size()),
    widgets[(int)long(wgtid)]->occc.clear();
}

inline void log_enigma_widget(enigma_widget* wgt)
{
  const int ind = widgets.size(); widgets.push_back(wgt);
  g_signal_connect(wgt->me, "destroy", G_CALLBACK(widget_destroy), (void*)ind);
}

bool wgt_exists(int id) {
  return unsigned(id) < widgets.size() and widgets[id]->exists;
}


// Windows

int wgt_window_create(int w, int h)
{
  threadsafety insurance;
  GtkWidget* win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(win), w, h);
  log_enigma_widget(new enigma_widget(win));
  return widget_idmax++;
}
void wgt_window_show(int wgt)
{
  threadsafety insurance;
  gtk_widget_show_all(widgets[wgt]->me);
}



// Layouts

int wgt_layout_create(int win, string layout, int hpad, int vpad)
{
  threadsafety insurance;
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
  
  GtkWidget *tbl = gtk_table_new(rows.size(),cc,0);
  enigma_widget *ww = widgets[win];
  
  enigma_widget *tblew = new enigma_widget(tbl);
  for (unsigned y = 0; y < rows.size(); y++)
  for (unsigned x = 0; x < cc; x++)
  {
    char cat = rows[y][x];
    unsigned cs = 1, rs = 1;
    while (cs < cc - x          and rows[y][x + cs] == cat) cs++;
    while (rs < rows.size() - y and rows[y + rs][x] == cat) rs++;
    pair<swmap::iterator, bool> insd = tblew->occc.insert(gwpair(string(1,cat),NULL));
    if (insd.second)
    {
      //printf("Inserted cell of ID '%c' at (%d, %d)[%d, %d]\n", cat, x,y,cs,rs);
      insd.first->second = gtk_alignment_new(0,0,1,1),
      gtk_table_attach(GTK_TABLE(tbl),insd.first->second,x,x+cs,y,y+rs,GTK_EXPAND_FILL,GTK_EXPAND_FILL,hpad,vpad);
    }
  }
  
  gtk_container_add(GTK_CONTAINER(ww->me), tbl);
  log_enigma_widget(tblew);
  return widget_idmax++;
}

void wgt_layout_insert_widget(int layout, string cell, int wgt) {
  threadsafety insurance;
  gtk_container_add(GTK_CONTAINER(widgets[layout]->occc[cell]), widgets[wgt]->me);
}


// Buttons

static void button_callback(GtkButton *button, void* user_data) {
  widgets[(int)long(user_data)]->actions++;
}

int wgt_button_create(string text)
{
  threadsafety insurance;
  GtkWidget *butn = gtk_button_new_with_label(text.c_str());
  g_signal_connect(butn, "clicked", G_CALLBACK(button_callback), (void*)widget_idmax);
  log_enigma_widget(new enigma_widget(butn));
  return widget_idmax++;
}

int wgt_button_get_pressed(int butn)
{
  int &clicked = widgets[butn]->actions;
  int ret = clicked; clicked = 0;
  return ret;
}


// Combo boxes

int wgt_combobox_create(string contents)
{
  threadsafety insurance;
  GtkWidget *cbox = gtk_combo_box_new_text();
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
  threadsafety insurance;
  return gtk_combo_box_get_active(GTK_COMBO_BOX(widgets[cbbox]->me));
}

string wgt_combobox_get_selected_text(int cbbox) {
  threadsafety insurance;
  const char* et = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widgets[cbbox]->me));
  return et ? et : "";
}


// Checkboxes

int wgt_checkbox_create(string text)
{
  threadsafety insurance;
  log_enigma_widget(new enigma_widget(gtk_check_button_new_with_label(text.c_str())));
  return widget_idmax++;
}
bool wgt_checkbox_get_checked(int cbox) {
  threadsafety insurance;
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets[cbox]->me));
}


// Text entry line

int wgt_textline_create(string text, int numchars)
{
  threadsafety insurance;
  GtkWidget *ent = gtk_entry_new_with_buffer(gtk_entry_buffer_new(text.c_str(),text.length()));
  gtk_entry_set_width_chars(GTK_ENTRY(ent), numchars);
  log_enigma_widget(new enigma_widget(ent));
  return widget_idmax++;
}

string wgt_textline_get_text(int tline)
{
  threadsafety insurance;
  GtkEntryBuffer *buf = gtk_entry_get_buffer(GTK_ENTRY(widgets[tline]->me));
  return string(gtk_entry_buffer_get_text(buf),gtk_entry_buffer_get_bytes(buf));
}


// Radio buttons
int wgt_radio_create(string text)
{
  threadsafety insurance;
  GtkWidget* rb = gtk_radio_button_new_with_label(NULL, text.c_str());
  log_enigma_widget(new enigma_widget(rb));
  return widget_idmax++;
}
                      
int wgt_radio_add(string text, int rbo)
{
  threadsafety insurance;
  GtkWidget* rb = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(widgets[rbo]), text.c_str());
  log_enigma_widget(new enigma_widget(rb));
  return widget_idmax++;
}
