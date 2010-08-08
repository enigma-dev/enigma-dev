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


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
using namespace std;

#include "../../general/darray.h"
#include "../../general/parse_basics.h"
inline bool is_letterh(char x) { return is_letter(x) or x == '-' or x == ' '; }

#include "event_parser.h"

inline bool lc(string s, string ss)
{
  const size_t l = s.length();
  if (l != ss.length())
    return false;
  for (size_t i=0; i<l; i++)
    if (  (s[i]>='A'&&s[i]<='Z'?s[i]-('A'-'a'):s[i])  !=  (ss[i]>='A'&&ss[i]<='Z'?ss[i]-('A'-'a'):ss[i])  )
      return false;
  return true;
}

const char *e_type_strs[] = {
  "Inline",
  "Stacked",
  "Special",
  "Spec-sys",
  "System",
  "None"
};
const char *p_type_strs[] = {
  "Sprite",
  "Sound",
  "Background",
  "Path",
  "Script",
  "Font",
  "Timeline",
  "Object",
  "Room",
  "Key"
};

event_info::event_info():               name(),  gmid(0), humanname(), par2type(p2t_error), mode(et_error), mmod(0), def(), cons(), super(), sub(), instead() { }
event_info::event_info(string n,int i): name(n), gmid(i), humanname(), par2type(p2t_error), mode(et_error), mmod(0), def(), cons(), super(), sub(), instead() { }

main_event_info::main_event_info(): name(), is_group(false), specs() { }

varray<main_event_info> main_event_infos;
typedef pair<int, int> evpair;
vector<evpair> event_sequence;

inline void event_add(int evid,event_info* last)
{
  int lid;
  if (last->mode == et_special or last->mode == et_spec_sys or last->mmod)
    lid = last->mmod;
  else
    lid = main_event_infos[evid].specs.rbegin() != main_event_infos[evid].specs.rend() ? main_event_infos[evid].specs.rbegin()->first + 1 : 0;
  if (lid) main_event_infos[evid].is_group = true;
  main_event_infos[evid].specs[lid] = last;
  event_sequence.push_back(evpair(evid,lid));
}

int event_parse_resourcefile()
{
  FILE* events = fopen("events.res","rt");
  if (!events) {
    puts("events.res: File not found");
    return -1;
  }
  
  char line[4096];
  
  // Parse modes
  int evid = -1;            // If we are in an event, this is its ID. Otherwise, it must be -1.
  event_info *last = NULL; // What we're dealing with thus far
  int linenum = 0;   // What line we're on, for error reporting
  int braces = 0;   // Number of curly braces we are in
  int iq = 0;      // In a quote in an expression
  
  enum { exp_default, exp_constant, exp_sub, exp_super, exp_instead } last_exp = exp_default; // Whether last EXPRESSION was sub or super
  
  while (!feof(events) and fgets(line,4096,events))
  {
    linenum++;
    bool lw = false;
    int pos = 0;
    
    if (braces) // We're still reading inside {}
    {
      int i;
      for (i = 0; line[i]; i++)
        if (!iq) { braces += (line[i] == '{') - (line[i] == '}');
          iq = (line[i] == '"') + ((line[i] == '\'') << 1);
        } else { iq = (iq == 1 and line[i] != '"') + ((iq == 2 and line[i] != '\'') << 1);
          i += (line[i] == '\\');
        }
      switch (last_exp) {
        case exp_default:  last->def     += string(line,i); continue;
        case exp_constant: last->cons    += string(line,i); continue;
        case exp_sub:      last->super   += string(line,i); continue;
        case exp_super:    last->sub     += string(line,i); continue;
        case exp_instead:  last->instead += string(line,i); continue;
        default: puts("THIS ERROR IS IMPOSSIBLE"); return 2;
      }
    }
    
    // Skip to end of leading whitespace
    // Lead whitespace means we're adding attributes
    if (is_useless(line[pos])) {
      lw = true;
      while (is_useless(line[++pos]));
    }
    
    if (line[pos] == '#' or !line[pos])
      continue;
  
    // If we're not adding to an event yet, error.
    if (lw and evid == -1) {
      printf("Error on line %d: No event name declared to which attributes should be added.\n",linenum);
      return 1;
    }
    
    // Adding an attribute to the current event, we hope
    char* nbegin = line + pos;
    if (!is_letter(*nbegin)) {
      printf("Error on line %d: Expected attribute name.\n",linenum);
      return 1;
    }
    
    // Find the end of the word
    if(lw) while (is_letterh(line[++pos]));
    else   while (is_letter (line[++pos]));
    if (line[pos] != ':') {
      printf("Error on line %d: Expected colon following attribute name.\n",linenum);
      return 1;
    }
    
    line[pos] = 0;
    string str = nbegin;
    line[pos] = ':';
    
    //Skip whitespace after :
    while (is_useless(line[++pos]));
    
    if (lw) // If the leading character was white
    {
      char *begin = line + pos++;
      while (line[pos] and line[pos] != '#') pos++;
      while (is_useless(line[--pos])); pos++;
      
      string v(begin,line+pos-begin);
      
      if (lc(str,"Mode"))
      {
        for (int i=0; i != et_error; i++)
          if (lc(v,e_type_strs[i]))
            last->mode = e_type(i);
        if (last->mode == et_error) {
          printf("Error on line %d: Unrecognized mode `%s`.\n",linenum,v.c_str());
          return 1;
        }
      }
      
      else if (lc(str,"Group"))
        main_event_infos[last->gmid].name = v;
      else if (lc(str,"Name"))
        last->humanname = v;
      else if (lc(str,"Type"))
      {
        for (int i=0; i != p2t_error; i++)
          if (lc(v,p_type_strs[i]))
            last->par2type = p_type(i);
        if (last->par2type == p2t_error) {
          printf("Error on line %d: Unrecognized parameter type `%s`.\n",linenum,v.c_str());
          return 1;
        }
      }
        
      else if (lc(str,"Default")){
        last->def = v, last_exp = exp_default;
        goto EXPRESSION;
      }
      else if (lc(str,"Constant")){
        last->cons = v, last_exp = exp_constant;
        goto EXPRESSION;
      }
      else if (lc(str,"Sub check")) {
        last->sub = v, last_exp = exp_sub;
        goto EXPRESSION;
      }
      else if (lc(str,"Super check")) {
        last->super = v, last_exp = exp_super;
        goto EXPRESSION;
      }
      else if (lc(str,"Instead")) {
        last->instead = v, last_exp = exp_instead;
        goto EXPRESSION;
      }
      else if (lc(str,"Case"))
      {
        if (last->mode != et_special and last->mode != et_spec_sys) {
          printf("Error on line %d: Conditional attribute `case` incompatible with mode `%s`.\n",linenum,e_type_strs[last->mode]);
          return 1;
        } last->mmod = atol(v.c_str());
      }
      else {
        printf("Warning, line %d: Ignoring unknown attribute `%s`.\n",linenum,str.c_str());
      }
      
      continue;
      EXPRESSION:
        if (v[0] == '{')
        {
          braces = 1;
          int iq = 0;
          for (size_t i=1; i<v.length(); i++)
            if (!iq) { braces += (v[i] == '{') - (v[i] == '}');
              iq = (v[i] == '"') + ((v[i] == '\'') << 1);
            } else { iq = (iq == 1 and v[i] != '"') + ((iq == 2 and v[i] != '\'') << 1);
              i += (v[i] == '\\');
            }
        }
      continue;
    }
    else
    {
      // This is the declaration of a new event.
      // So far we have "eventname: "; we now expect an integer
      
      // First, take care of the old event
      if (last and evid != -1)
        event_add(evid,last);
        
      char *num = line + pos;
      
      if (!is_digit(line[pos])) {
        printf("Error on line %d: Expected integer ID following colon.\n",linenum);
        return 1;
      }
      
      while (is_digit(line[++pos]));
      
      if (!is_useless(line[pos]) and line[pos] != '#' and line[pos]) {
        printf("Error on line %d: Expected end of line following integer.\n",linenum);
        return 1;
      }
      
      line[pos] = 0;
      evid = atol(num);
      last = new event_info(str,evid);
    }
  }
  
  if (last and evid != -1)
    event_add(evid,last);
  
  for (size_t i=0; i<main_event_infos.size; i++)
  {
    for (main_event_info::iter ii = main_event_infos[i].specs.begin(); ii != main_event_infos[i].specs.end(); ii++)
      if (ii->second->humanname == "")
        ii->second->humanname = ii->second->name;
    
    main_event_info::iter ii = main_event_infos[i].specs.begin();
    if (main_event_infos[i].name == "" and ii != main_event_infos[i].specs.end())
      main_event_infos[i].name = main_event_infos[i].specs[0]->humanname;
  }
  
  return 0;
}

extern string tostring(int);
string format_lookup(int id, p_type t)
{
  switch (t)
  {
    case p2t_sprite:     return "spr_" + tostring(id);
    case p2t_sound:      return "snd_" + tostring(id);
    case p2t_background: return "bk_" + tostring(id);
    case p2t_path:       return "pth_" + tostring(id);
    case p2t_script:     return "scr_" + tostring(id);
    case p2t_font:       return "fnt_" + tostring(id);
    case p2t_timeline:   return "tl_" + tostring(id);
    case p2t_object:     return "obj_" + tostring(id);
    case p2t_room:       return "rm_" + tostring(id);
    case p2t_key:        return "key" + tostring(id);
    case p2t_error:      return "...";
  }
  return tostring(id);
}
string format_lookup_econstant(int id, p_type t)
{
  switch (t)
  {
    case p2t_sprite:     return tostring(id);
    case p2t_sound:      return tostring(id);
    case p2t_background: return tostring(id);
    case p2t_path:       return tostring(id);
    case p2t_script:     return tostring(id);
    case p2t_font:       return tostring(id);
    case p2t_timeline:   return tostring(id);
    case p2t_object:     return tostring(id);
    case p2t_room:       return tostring(id);
    case p2t_key:        return tostring(id);
    case p2t_error:      return tostring(id);
  }
  return tostring(id);
}

inline string autoparam(string x,string y)
{
  const size_t p = x.find("%1");
  if (p != string::npos) return x.replace(p,2,y);
  return x;
}

// Query for a name suitable for use as
// an identifier. No spaces or specials.
string event_get_function_name(int mid, int id)
{
  main_event_info &mei = main_event_infos[mid];
  if (mei.is_group or mei.specs[0]->mode != et_stacked) {
    main_event_info::iter i = main_event_infos[mid].specs.find(id);
    return i != main_event_infos[mid].specs.end() ? i->second->name : "undefinedEventERROR";
  }
  string buf = mei.specs[0]->name;
  size_t pp = buf.find("%1");
  if (pp != string::npos)
    buf.replace(pp,2,format_lookup(id, mei.specs[0]->par2type));
  else
    buf += "_" + tostring(id);
  return buf;
}

string event_stacked_get_root_name(int mid) {
  main_event_info &mei = main_event_infos[mid];
  return autoparam(mei.specs[0]->name,"stackroot");
}

// Fetch a user-friendly name for the event
// with the given credentials.
string event_get_human_name(int mid, int id)
{
  main_event_info &mei = main_event_infos[mid];
  if (mei.is_group) {
    char buf[64];
    main_event_info::iter i = main_event_infos[mid].specs.find(id);
    return i != main_event_infos[mid].specs.end() ? i->second->humanname : (sprintf(buf,"Undefined or Unsupported (%d,%d)",mid,id),buf);
  }
  string buf = mei.specs[0]->humanname;
  size_t pp = buf.find("%1");
  if (pp != string::npos)
    buf.replace(pp,2,format_lookup(id, mei.specs[0]->par2type));
  return buf;
}
string event_get_human_name_min(int mid, int id)
{
  main_event_info &mei = main_event_infos[mid];
  if (mei.is_group) {
    char buf[64];
    main_event_info::iter i = main_event_infos[mid].specs.find(id);
    return i != main_event_infos[mid].specs.end() ? i->second->humanname : (sprintf(buf,"Undefined or Unsupported (%d,%d)",mid,id),buf);
  }
  if (mei.specs[0]->mode == et_stacked)
    return mei.name;
  string buf = mei.specs[0]->humanname;
  size_t pp = buf.find("%1");
  if (pp != string::npos)
    buf.replace(pp,2,format_lookup(id, mei.specs[0]->par2type));
  return buf;
}

// Used by the rest of these functions
event_info *event_access(int mid, int id)
{
  main_event_info &mei = main_event_infos[mid];
  return (mei.is_group) ? mei.specs[id] : mei.specs[0];
}

// Test whether there is code that will remain
// active if a user has not declared this event.
bool event_has_default_code(int mid, int id) {
  return event_access(mid,id)->def != "" or event_access(mid,id)->cons != "";
}

string event_get_default_code(int mid, int id) {
  return event_access(mid,id)->def + event_access(mid,id)->cons;
}


// Test whether there is code that will remain
// active whether or not a user has declared this event.
bool event_has_const_code(int mid, int id) {
  return event_access(mid,id)->cons != "";
}

string event_get_const_code(int mid, int id) {
  return event_access(mid,id)->cons;
}

// Some events have special behavior as placeholders, instead of simple iteration.
// These two functions will test for and return such.

bool event_has_instead(int mid, int id) {
  return event_access(mid,id)->instead != "";
}

string event_get_instead(int mid, int id) {
  return event_access(mid,id)->instead;
}


// The rest of these functions use this
string evres_code_substitute(string code, int sid, p_type t)
{
  for (size_t i = code.find("%1"); i != string::npos; i = code.find("%1"))
    code.replace(i, 2, format_lookup_econstant(sid, t));
  return code;
}

// Many events check things before executing, some before starting the loop. Deal with them.

bool event_has_super_check(int mid, int id) {
  return event_access(mid,id)->super != "";
}

bool event_has_sub_check(int mid, int id) {
  return event_access(mid,id)->sub != "";
}

string event_get_super_check_condition(int mid, int id) {
  event_info* ei = event_access(mid,id);
  return evres_code_substitute(ei->super, id, ei->par2type);
}

string event_get_super_check_function(int mid, int id) {
  event_info *e = event_access(mid,id);
  return (e->super != "" and e->super[0] == '{') ? "inline bool supercheck_" + e->name + "() " + e->super + "\n\n" : "";
}

string event_get_sub_check_condition(int mid, int id) {
  event_info* ei = event_access(mid,id);
  const string res = evres_code_substitute(ei->sub, id, ei->par2type);
  return res[0] == '{' ? res : "if (!(" + res + ")) return 0;";
}

// Does this event belong on the list of events to execute?
bool event_execution_uses_default(int mid, int id) {
  event_info *e = event_access(mid,id);
  return e->mode == et_inline or e->mode == et_special or e->mode == et_stacked;
}

// Clear all data from previous parses, save
// main events, which can just be overwritten.
void event_info_clear()
{
  for (unsigned i=0; i<main_event_infos.size; i++)
    main_event_infos[i].specs.clear();
  event_sequence.clear();
}

bool event_is_instance(int mid, int id) { // Returns if the event with the given ID pair is an instance of a stacked event
  main_event_info &mei = main_event_infos[mid];
  return !mei.is_group and mei.specs[0]->mode == et_stacked;
}

/*
int main(int,char**)
{
  int a = event_parse_resourcefile();
  if (a) printf("Event Parse: Error %d\n",a);
  else   puts  ("Event Parse: completed successfully");
  
  for (size_t i=0; i<main_event_infos.size; i++)
  {
    printf("%c %s\n",main_event_infos[i].is_group ? ']' : '+', main_event_infos[i].name.c_str());
    for (main_event_info::iter ii = main_event_infos[i].specs.begin(); ii != main_event_infos[i].specs.end(); )
      printf("%s── [%02d] %-25s [%s]\n",++ii != main_event_infos[i].specs.end()?"├":"╰",ii->first,ii->second->humanname.c_str(),ii->second->name.c_str());
  }
}
*/
