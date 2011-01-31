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
#include <gtk/gtk.h>

using namespace std;


int get_color(int defcol)
{
  gdk_threads_enter();
  
    GtkWidget* colsel = gtk_color_selection_dialog_new("Pick a color");
    
    GdkColor col; 
      col.red   = ((defcol & 0xFF) | ((defcol & 0xFF) << 8));
      col.green = ((defcol & 0xFF00) >> 8) | (defcol & 0xFF00);
      col.blue  = ((defcol & 0xFF0000) >> 16) | ((defcol & 0xFF0000) >> 8);
    col.pixel = ((defcol & 0xFF) << 16) | (defcol & 0xFF00) | ((defcol & 0xFF0000) >> 16);
    
    gtk_color_selection_set_previous_color (GTK_COLOR_SELECTION(gtk_color_selection_dialog_get_color_selection(GTK_COLOR_SELECTION_DIALOG(colsel))), &col);
    gtk_color_selection_set_current_color(GTK_COLOR_SELECTION(gtk_color_selection_dialog_get_color_selection(GTK_COLOR_SELECTION_DIALOG(colsel))), &col);
    int resp = gtk_dialog_run(GTK_DIALOG(colsel));
    gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(gtk_color_selection_dialog_get_color_selection(GTK_COLOR_SELECTION_DIALOG(colsel))), &col);
    gtk_widget_destroy(colsel);
    
  gdk_threads_leave();
  
  if (resp == GTK_RESPONSE_DELETE_EVENT or resp == GTK_RESPONSE_NONE or resp == GTK_RESPONSE_CANCEL)
    return defcol;
  return ((col.red & 0xFF00) >> 8) | (col.green & 0xFF00) | ((col.blue & 0xFF00) << 8);
}

inline void parse_filter_string(GtkFileChooser *dialog, const string &filter)
{
  unsigned ts = 0; bool itn = 0; string filtname;
  for (unsigned i = 0; i <= filter.length(); i++)
  if (i == filter.length() or filter[i] == '|')
  {
    string fs = filter.substr(ts,i-ts);
    if (!itn)
      itn = 1,
      filtname = fs;
    else
    {
      itn = 0;
      GtkFileFilter *FF = gtk_file_filter_new();
      gtk_file_filter_set_name(FF,filtname.c_str());
      unsigned fb = 0; while (fs[fb] == ' ' or fs[fb] == '\t' or fs[fb] == '\r' or fs[fb] == '\n') fb++;
      for (unsigned ii = fb; ii < fs.length(); ii++)
        if (fs[ii] == ';') {
          gtk_file_filter_add_pattern(FF,fs.substr(fb,ii-fb).c_str());
          fb = ii + 1; while (fs[fb] == ' ' or fs[fb] == '\t' or fs[fb] == '\r' or fs[fb] == '\n') fb++;
          ii = fb - 1;
        }
      if (fb < fs.length())
        gtk_file_filter_add_pattern(FF,fs.substr(fb).c_str());
      gtk_file_chooser_add_filter(dialog, FF);
    }
    ts = i+1;
  }
}

string get_open_filename(string filter, string fname)
{
  string ret;
  gdk_threads_enter();
  
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new ("Open File", NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
             GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
    
    parse_filter_string(GTK_FILE_CHOOSER(dialog), filter);
    if (!gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog), fname.c_str()))
      gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), fname.substr(fname.find_last_of("\\/") + 1).c_str());
    
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char* rcs = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
      if (rcs)
        ret = rcs,
        g_free(rcs);
    }
    gtk_widget_destroy (dialog);
  gdk_threads_leave();
  
  return ret;
}

string get_save_filename(string filter, string fname)
{
  string ret;
  gdk_threads_enter();
  
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new ("Save File", NULL, GTK_FILE_CHOOSER_ACTION_SAVE,
             GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
    
    parse_filter_string(GTK_FILE_CHOOSER(dialog), filter);
    if (!gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog), fname.c_str())) {
      size_t a = fname.find_last_of("\\/");
      gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), fname.substr(a != string::npos ? a+1 : 0).c_str());
    }
    
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char* rcs = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
      if (rcs)
        ret = rcs,
        g_free(rcs);
    }
    gtk_widget_destroy (dialog);
  gdk_threads_leave();
  
  return ret;
}




static int* cmret;
static void menu_item_clicked(gpointer user_data) {
  *cmret = (int)long(user_data);
}

enum { MFT_STRING, MFT_SEPARATOR, MFT_RADIOCHECK };
enum { MFS_UNCHECKED, MFS_DISABLED, MFS_CHECKED, };

static void menu_add_item(GtkMenu *menu,int iid,int id,string str,unsigned int type)
{
  GtkWidget *nitem;
  if (type != MFT_SEPARATOR)
      nitem = gtk_menu_item_new_with_label(str.c_str()),
      g_signal_connect_swapped (G_OBJECT(nitem), "activate", G_CALLBACK(menu_item_clicked), (gpointer)id);
  else
    nitem = gtk_separator_menu_item_new();
  gtk_widget_show(nitem), gtk_menu_append(menu, nitem);
}
static void menu_add_submenu(GtkMenu* menu, GtkWidget* smenu,int iid,int id,string str)
{
  GtkWidget *nitem = gtk_menu_item_new_with_label(str.c_str());
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(nitem), smenu);
  gtk_widget_show(nitem), gtk_menu_append(menu, nitem);
}
static void menu_add_item_ext(GtkMenu* menu,int iid,int id,string str,unsigned int type,unsigned int state,const bool checkmark=0)
{
  GtkWidget *nitem;
  if (type != MFT_SEPARATOR)
  {
    if (checkmark)
      nitem = gtk_check_menu_item_new_with_label(str.c_str()),
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(nitem), bool(state & MFS_CHECKED)),
      gtk_check_menu_item_set_draw_as_radio(GTK_CHECK_MENU_ITEM(nitem), bool(type & MFT_RADIOCHECK));
    else
      nitem = gtk_menu_item_new_with_label(str.c_str());
    g_signal_connect_swapped (G_OBJECT(nitem), "activate", G_CALLBACK(menu_item_clicked), (gpointer)id);
  }
  else
    nitem = gtk_separator_menu_item_new();
  
  if (state & MFS_CHECKED) gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(nitem), true);
  if (state & MFS_DISABLED) gtk_widget_set_sensitive(nitem, false);
  gtk_widget_show(nitem), gtk_menu_append(menu, nitem);
}

struct menu_descriptor {
  int *result;
  int x, y;
};

static void menu_destroy(GtkObject *menu, gpointer mdv) {
  if (*cmret == -2) *cmret = -1;
  int *i = ((menu_descriptor*)mdv)->result;
  if (*i == -2) *i = -1;
}

void strict_positioner(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer user_data) {
  menu_descriptor *md = (menu_descriptor*)user_data;
  *x = md->x, *y = md->y;
}

int show_menu(int x, int y, string text)
{
  gdk_threads_enter();
  
    int i=0, is=0, id=1, iid=1;
    const int len=text.length();
    GtkWidget *menu = gtk_menu_new();
    
    while (i<=len)
    {
      if (i>=len or text[i]=='|')
      {
        string itxt=text.substr(is,i-is);
        if (itxt=="-")
          menu_add_item(GTK_MENU(menu),iid,id,itxt,MFT_SEPARATOR);
        else
        {
          menu_add_item(GTK_MENU(menu),iid,id,itxt,MFT_STRING);
          id++;
        }
        is = i+1; iid++;
      }
      i++; 
    }
    
    int rv = -2; cmret = &rv;
    menu_descriptor md = { &rv, x, y };
    g_signal_connect(G_OBJECT(menu), "destroy", G_CALLBACK(menu_destroy), (gpointer)&md);
    g_signal_connect(G_OBJECT(menu), "hide", G_CALLBACK(menu_destroy), (gpointer)&md);
    gtk_menu_popup(GTK_MENU(menu),NULL,NULL,strict_positioner,&md,0,0);
  
  gdk_threads_leave();

  while (rv == -2)
    g_usleep(25000);
  
  gdk_threads_enter();
    gtk_widget_destroy(menu);
  gdk_threads_leave();
  
  return rv;
}

struct hmenustack
{
  hmenustack *prev;
  GtkWidget *menu;
  hmenustack():prev(NULL) {}
  
  hmenustack* push(GtkWidget *n) { hmenustack* next=new hmenustack; next->prev=this; next->menu=n; return next; }
  hmenustack* pop() { if (prev==NULL) return this; hmenustack* r=prev; delete this; return r; }
};
#include <iostream>
int show_menu_ext(int x, int y, string text)
{
  gdk_threads_enter();
  
  int i=0, is=0, id=1, iid=1;
  const int len=text.length();
  hmenustack* ms=new hmenustack;
  ms->menu=gtk_menu_new();
  if (ms->menu==NULL) return -1;
  
  while (i<=len)
  {
    if (i>=len or text[i]=='|')
    {
      string itxt=text.substr(is,i-is);
      if (itxt=="-")
        menu_add_item(GTK_MENU(ms->menu),iid,id,itxt,MFT_SEPARATOR);
      else 
      {
        if (i-is>1 and itxt[0]=='/')
          menu_add_item_ext(GTK_MENU(ms->menu),iid,id,itxt.substr(1),MFT_STRING,MFS_DISABLED);
        else if (i-is>1 and itxt[0]=='[' and itxt[1]==']')
          menu_add_item_ext(GTK_MENU(ms->menu),iid,id,itxt.substr(2),MFT_STRING,MFS_UNCHECKED,1);
        else if (i-is>2 and itxt[0]=='[' and itxt[1]=='*' and itxt[2]==']')
          menu_add_item_ext(GTK_MENU(ms->menu),iid,id,itxt.substr(3),MFT_STRING,MFS_CHECKED,1);
        else if (i-is>1 and itxt[0]=='(' and itxt[1]==')')
          menu_add_item_ext(GTK_MENU(ms->menu),iid,id,itxt.substr(2),MFT_STRING|MFT_RADIOCHECK,MFS_UNCHECKED,1);
        else if (i-is>1 and itxt[0]=='(' and itxt[1]=='*' and itxt[2]==')')
          menu_add_item_ext(GTK_MENU(ms->menu),iid,id,itxt.substr(3),MFT_STRING|MFT_RADIOCHECK,MFS_CHECKED,1);
        else if (i-is>0 and itxt[0]=='>')
        {
          id--;
          ms=ms->push(gtk_menu_new());
          menu_add_submenu(GTK_MENU(ms->prev->menu),ms->menu,iid,0,itxt.substr(1));
        }
        else if (itxt=="<")
        {
          ms=ms->pop();
          id--; iid--; //This counts as nothing
        }
        else if (i-is>1 and itxt[0]=='<')
        {
          ms=ms->pop();
          cout << "\"" << itxt.substr(1) << "\"" << endl;
          menu_add_item(GTK_MENU(ms->menu),iid,id,itxt.substr(1),MFT_STRING);
        }
        else
          menu_add_item(GTK_MENU(ms->menu),iid,id,itxt,MFT_STRING);
        id++;
      }
      is=i+1; iid++;
    }
    i++; 
  }
  
  while (ms->prev) ms=ms->pop();
  
    int rv = -2; cmret = &rv;
    menu_descriptor md = { &rv, x, y };
    g_signal_connect(G_OBJECT(ms->menu), "destroy", G_CALLBACK(menu_destroy), (gpointer)&md);
    g_signal_connect(G_OBJECT(ms->menu), "hide", G_CALLBACK(menu_destroy), (gpointer)&md);
    gtk_menu_popup(GTK_MENU(ms->menu),NULL,NULL,strict_positioner,&md,0,0);
  
  gdk_threads_leave();

  while (rv == -2)
    g_usleep(25000);
  
  gdk_threads_enter();
    gtk_widget_destroy(ms->menu);
  gdk_threads_leave();
  
  delete ms;
  return rv;
}
/*
int show_menu_ext_nl(int x, int y, string text)
{
  int i=0, is=0, id=1, iid=1;
  const int len=text.length();
  hmenustack* ms=new hmenustack;
  ms->menu=gtk_menu_new();
  if (ms->menu==NULL) return -1;
  
  while (text[i]=='\r' or text[i]=='\n' or text[i]==' ')
    i++; 
  is=i;
  
  while (i<=len)
  {
    if (i>=len or text[i]=='\r'  or text[i]=='\n')
    {
      string itxt=text.substr(is,i-is);
      if (itxt=="-")
        menu_add_item(ms->menu,iid,id,itxt,MFT_SEPARATOR);
      else 
      {
        if (i-is>1 and itxt[0]=='/')
          menu_add_item_ext(ms->menu,iid,id,itxt.substr(1),MFT_STRING,MFS_DISABLED);
        else if (i-is>1 and itxt[0]=='[' and itxt[1]==']')
          menu_add_item_ext(ms->menu,iid,id,itxt.substr(2),MFT_STRING,MFS_UNCHECKED,1);
        else if (i-is>2 and itxt[0]=='[' and itxt[1]=='*' and itxt[2]==']')
          menu_add_item_ext(ms->menu,iid,id,itxt.substr(3),MFT_STRING,MFS_CHECKED,1);
        else if (i-is>1 and itxt[0]=='(' and itxt[1]==')')
          menu_add_item_ext(ms->menu,iid,id,itxt.substr(2),MFT_STRING|MFT_RADIOCHECK,MFS_UNCHECKED,1);
        else if (i-is>1 and itxt[0]=='(' and itxt[1]=='*' and itxt[2]==')')
          menu_add_item_ext(ms->menu,iid,id,itxt.substr(3),MFT_STRING|MFT_RADIOCHECK,MFS_CHECKED,1);
        else if (i-is>0 and itxt[0]=='>')
        {
          id--;
          ms=ms->push(gtk_menu_new());
          menu_add_submenu(ms->prev->menu,ms->menu,iid,0,itxt.substr(1));
        }
        else if (itxt=="<")
        {
          ms=ms->pop();
          id--; iid--; //This counts as nothing
        }
        else if (i-is>1 and itxt[0]=='<')
        {
          ms=ms->pop();
          menu_add_item(ms->menu,iid,id,itxt.substr(1),MFT_STRING);
        }
        else
          menu_add_item(ms->menu,iid,id,itxt,MFT_STRING);
        id++;
      }
      is=i+1; iid++;
      if (i<len)
      {
        while (text[i]=='\r' or text[i]=='\n' or text[i]==' ') i++;
        is=i;
        i--;
      }
    }
    i++;
  }
  
  while (ms->prev) ms=ms->pop();
  
  SetForegroundWindow(hwnd);
  int rv = TrackPopupMenuEx(ms->menu,TPM_LEFTBUTTON|TPM_RETURNCMD|TPM_LEFTALIGN|TPM_TOPALIGN,x,y,hwnd,NULL);
  SendMessage(hwnd, WM_NULL, 0, 0);
  delete ms;
  return rv;
}
*/
