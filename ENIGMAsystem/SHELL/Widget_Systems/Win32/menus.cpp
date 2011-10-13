/*********************************************************************************\
**                                                                              **
**  Copyright (C) 2009-2011 Josh Ventura                                        **
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
\*********************************************************************************/

#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

#include "menus.h"

static bool menu_add_item(HMENU menu,int iid,int id,string str,unsigned int type)
{
  MENUITEMINFO mii;
  mii.cbSize=sizeof(MENUITEMINFO);
  mii.fMask=MIIM_FTYPE|MIIM_STRING|MIIM_ID;
  mii.fType=type;
  mii.wID=id;
  mii.dwTypeData=(char*)str.c_str();
  mii.cch=str.length();
  
  return InsertMenuItem(menu,iid,0,&mii);
}
static bool menu_add_submenu(HMENU menu,HMENU smenu,int iid,int id,string str)
{
  MENUITEMINFO mii;
  mii.cbSize=sizeof(MENUITEMINFO);
  mii.fMask=MIIM_STRING|MIIM_FTYPE|MIIM_ID|MIIM_SUBMENU;
  mii.wID=id;
  mii.fType=MFT_STRING;
  mii.hSubMenu=smenu;
  mii.dwTypeData=(char*)str.c_str();
  mii.cch=str.length();
  
  return InsertMenuItem(menu,iid,0,&mii);
}
static bool menu_add_item_ext(HMENU menu,int iid,int id,string str,unsigned int type,unsigned int state,const bool checkmark=0)
{
  MENUITEMINFO mii;
  mii.cbSize=sizeof(MENUITEMINFO);
  mii.fMask=MIIM_ID|MIIM_FTYPE|MIIM_STRING|MIIM_STATE|(MIIM_CHECKMARKS*checkmark);
  mii.fType=type;
  mii.wID=id;
  mii.dwTypeData=(char*)str.c_str();
  mii.cch=str.length();
  mii.fState=state;
  mii.hbmpChecked=NULL;
  mii.hbmpUnchecked=NULL;
  
  return InsertMenuItem(menu,iid,0,&mii);
}
namespace enigma {
  extern HWND hWndParent;
}
int show_menu(int x, int y, string text)
{
  int i=0, is=0, id=1, iid=1;
  const int len=text.length();
  HMENU menu=CreatePopupMenu();
  if (menu==NULL) return -1;
  
  while (i<=len)
  {
    if (i>=len or text[i]=='|')
    {
      string itxt=text.substr(is,i-is);
      if (itxt=="-")
        menu_add_item(menu,iid,id,itxt,MFT_SEPARATOR);
      else
      {
        menu_add_item(menu,iid,id,itxt,MFT_STRING);
        id++;
      }
      is=i+1; iid++;
    }
    i++; 
  }
  
  int rv = TrackPopupMenuEx(menu,TPM_LEFTBUTTON|TPM_RETURNCMD|TPM_LEFTALIGN|TPM_TOPALIGN,x,y,enigma::hWndParent,NULL);
  SendMessage(enigma::hWndParent, WM_NULL, 0, 0);
  return rv;
}

struct hmenustack
{
  hmenustack *prev;
  HMENU menu;
  hmenustack():prev(NULL) {}
  
  hmenustack* push(HMENU n) { hmenustack* next=new hmenustack; next->prev=this; next->menu=n; return next; }
  hmenustack* pop() { if (prev==NULL) return this; hmenustack* r=prev; delete this; return r; }
};

int show_menu_ext(int x, int y, string text)
{
  int i=0, is=0, id=1, iid=1;
  const int len=text.length();
  hmenustack* ms=new hmenustack;
  ms->menu=CreatePopupMenu();
  if (ms->menu==NULL) return -1;
  
  while (i<=len)
  {
    if (i>=len or text[i]=='|')
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
          ms=ms->push(CreatePopupMenu());
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
          cout << "\"" << itxt.substr(1) << "\"" << endl;
          menu_add_item(ms->menu,iid,id,itxt.substr(1),MFT_STRING);
        }
        else
          menu_add_item(ms->menu,iid,id,itxt,MFT_STRING);
        id++;
      }
      is=i+1; iid++;
    }
    i++; 
  }
  
  while (ms->prev) ms=ms->pop();
  
  int rv = TrackPopupMenuEx(ms->menu,TPM_LEFTBUTTON|TPM_RETURNCMD|TPM_LEFTALIGN|TPM_TOPALIGN,x,y,enigma::hWndParent,NULL);
  SendMessage(enigma::hWndParent, WM_NULL, 0, 0);
  delete ms;
  return rv;
}

int show_menu_ext_nl(int x, int y, string text)
{
  int i=0, is=0, id=1, iid=1;
  const int len=text.length();
  hmenustack* ms=new hmenustack;
  ms->menu=CreatePopupMenu();
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
          ms=ms->push(CreatePopupMenu());
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
  
  int rv = TrackPopupMenuEx(ms->menu,TPM_LEFTBUTTON|TPM_RETURNCMD|TPM_LEFTALIGN|TPM_TOPALIGN,x,y,enigma::hWndParent,NULL);
  SendMessage(enigma::hWndParent, WM_NULL, 0, 0);
  delete ms;
  return rv;
}
