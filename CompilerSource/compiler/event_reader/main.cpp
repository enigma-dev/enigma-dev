//Copyright (C) 2010 Josh Ventura
//This code is released under the GNU General Public License, version 3.

#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

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

inline bool letter(char x) {
  return ((x >= 'a' and x <= 'z') or (x >= 'A' and x <= 'Z') or (x == '_'));
}
inline bool letterh(char x) {
  return ((x >= 'a' and x <= 'z') or (x >= 'A' and x <= 'Z') or (x == '_' or x == '-' or x == ' '));
}

inline bool digit(char x) {
  return (x >= '0' and x <= '9');
}

inline bool white(char x) {
  return x==' ' or x=='\t' or x == '\n' or x == '\r';
}

enum e_type {
  et_inline,
  et_stacked,
  et_special,
  et_spec_sys,
  et_system,
  et_none,
  et_error
  
};
const char *e_type_strs[] = {
  "inline",
  "stacked",
  "special",
  "spec-sys",
  "system",
  "none"
};

enum p_type {
  p2t_sprite,
  p2t_sound,
  p2t_background,
  p2t_path,
  p2t_script,
  p2t_font,
  p2t_timeline,
  p2t_object,
  p2t_room,
  p2t_key,
  p2t_error
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

struct event_class
{
  string name;
  int    gmid;
  
  string humanname;
  p_type par2type;
  
  e_type mode;
  int    mmod;
  
  string def;
  string cons;
  string super;
  string sub;
  
  void reassign(string nn, int nid) {
    name = nn, gmid = nid;
    mode = et_error, mmod = 0;
    def = cons = sub = super = "";
  }
  event_class() { reassign("",0); }
};

struct main_event
{
  string name;
  bool is_group;
  varray<event_class> specs;
}

int parse_event_res()
{
  FILE* events = fopen("events.res","rt");
  if (!events) {
    puts("events.res: File not found");
    return -1;
  }
  
  char line[4096];
  
  // Parse modes
  int evid = -1;    // If we are in an event, this is its ID. Otherwise, it must be -1.
  event_class last; // What we're dealing with thus far
  int linenum = 0;  // What line we're on, for error reporting
  int braces = 0;   // Number of curly braces we are in
  
  enum { exp_default, exp_constant, exp_sub, exp_super } last_exp; // Whether last EXPRESSION was sub or super
  
  while (!feof(events) and fgets(line,4096,events))
  {
    linenum++;
    bool lw = false;
    int pos = 0;
    
    if (braces) // We're still reading inside {}
    {
      int i;
      for (i = 1; line[i]; i++) {
        braces += (line[i] == '{') - (line[i] == '}');
        if (!braces) break;
      }
      switch (last_exp) {
        case exp_default:  last.def   += string(line,i); continue;
        case exp_constant: last.cons  += string(line,i); continue;
        case exp_sub:      last.super += string(line,i); continue;
        case exp_super:    last.sub   += string(line,i); continue;
        default: puts("THIS ERROR IS IMPOSSIBLE"); return 2;
      }
    }
    
    // Skip to end of leading whitespace
    // Lead whitespace means we're adding attributes
    if (white(line[pos])) {
      lw = true;
      while (white(line[++pos]));
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
    if (!letter(*nbegin)) {
      printf("Error on line %d: Expected attribute name.\n",linenum);
      return 1;
    }
    
    // Find the end of the word
    if(lw) while (letterh(line[++pos]));
    else   while (letter (line[++pos]));
    if (line[pos] != ':') {
      printf("Error on line %d: Expected colon following attribute name.\n",linenum);
      return 1;
    }
    
    line[pos] = 0;
    string str = nbegin;
    line[pos] = ':';
    
    //Skip whitespace after :
    while (white(line[++pos]));
    
    if (lw) // If the leading character was white
    {
      char *begin = line + pos++;
      while (line[pos] and line[pos] != '#') pos++;
      while (white(line[--pos])); pos++;
      
      string v(begin,line+pos-begin);
      
      if (lc(str,"Mode"))
      {
        for (int i=0; i != et_error; i++)
          if (lc(v,e_type_strs[i]))
            last.mode = e_type(i);
        if (last.mode == et_error) {
          printf("Error on line %d: Unrecognized mode `%s`.\n",linenum,v.c_str());
          return 1;
        }
      }
      
      else if (lc(str,"Name"))
        last.humanname = v;
      else if (lc(str,"Type"))
      {
        for (int i=0; i != p2t_error; i++)
          if (lc(v,p_type_strs[i]))
            last.par2type = p_type(i);
        if (last.par2type == p2t_error) {
          printf("Error on line %d: Unrecognized parameter type `%s`.\n",linenum,v.c_str());
          return 1;
        }
      }
        
      else if (lc(str,"Default")){
        last.def = v, last_exp = exp_default;
        goto EXPRESSION;
      }
      else if (lc(str,"Constant")){
        last.cons = v, last_exp = exp_constant;
        goto EXPRESSION;
      }
      else if (lc(str,"Sub check")) {
        last.sub = v, last_exp = exp_sub;
        goto EXPRESSION;
      }
      else if (lc(str,"Super check")) {
        last.super = v, last_exp = exp_super;
        goto EXPRESSION;
      }
      else if (lc(str,"Case"))
      {
        if (last.mode != et_special and last.mode != et_spec_sys) {
          printf("Error on line %d: Conditional attribute `case` incompatible with mode `%s`.\n",linenum,e_type_strs[last.mode]);
          return 1;
        } last.mmod = atol(v.c_str());
      }
      else {
        printf("Warning, line %d: Ignoring unknown attribute `%s`.\n",linenum,str.c_str());
      }
      
      continue;
      EXPRESSION:
        if (v[0] == '{')
        {
          braces = 1;
          for (size_t i=1; i<v.length(); i++)
            braces += (v[i] == '{') - (v[i] == '}');
        }
      continue;
    }
    else
    {
      // This is the declaration of a new event.
      // So far we have "eventname: "; we now expect an integer
      
      char *num = line + pos;
      
      if (!digit(line[pos])) {
        printf("Error on line %d: Expected integer ID following colon.\n",linenum);
        return 1;
      }
      
      while (digit(line[++pos]));
      
      if (!white(line[pos]) and line[pos] != '#' and line[pos]) {
        printf("Error on line %d: Expected end of line following integer.\n",linenum);
        return 1;
      }
      
      line[pos] = 0;
      evid = atol(num);
      
      last.reassign(str,evid);
    }
  }
  return 0;
}


int main(int,char**)
{
  int a = parse_event_res();
  if (a) printf("Error %d\n",a);
  else   puts  ("Parse completed successfully");
}
