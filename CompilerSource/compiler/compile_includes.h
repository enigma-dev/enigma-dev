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

externs *globals_scope;
externs *enigma_type__var = builtin_type__int;
externs *enigma_type__variant = builtin_type__int;

extern string tostring(int val);

void quickmember_variable(externs* scope, externs* type, string name)
{
  externs* n = scope->members[name] = new externs;
  n->name = name;
  n->type = type;
  n->flags = 0;
  n->value_of = 0;
}
void quickmember_script(externs* scope, string name)
{
  externs* n = scope->members[name] = new externs;
  n->name = name;
  n->type = enigma_type__var;
  n->flags = 0;
  n->value_of = 0;
  
  rf_stack rfs;
  rfs += referencer('(',0,16,true);
  n->refstack = rfs.dissociate();
}

string format_error(string code,string err,int pos)
{
  if (pos == -1)
    return err;
  
  int line = 1, lp = 1;
  for (int i=0; i<pos; i++,lp++) {
    if (code[i] =='\n')
      line++, lp = 0, i += code[i+1] == '\n';
    else if (code[i] == '\n') line++, lp = 0;
  }
  return "Line " + tostring(line) + ", position " + tostring(lp) + " (absolute " + tostring(pos) + "): " + err;
}

void* lgmRoomBGColor(int c)
{
  return (void*)((c & 0xFF)?(((c & 0x00FF0000) >> 8) | ((c & 0x0000FF00) << 8) | ((c & 0xFF000000) >> 24)):0xFFFFFFFF);
}


inline string tdefault(string t) {
  return (t != "" ? t : "var");
}

//Error codes
enum {
  E_ERROR_NO_ERROR_LOL,
  E_ERROR_PLUGIN_FUCKED_UP,
  E_ERROR_SYNTAX,
  E_ERROR_WUT
};

#include "event_ids.h"

string event_get_subname_other(int id)
{
  switch (id)
  {
    case EV_OUTSIDE:        return "outside";
    case EV_BOUNDARY:       return "boundary";
    case EV_GAME_START:     return "game_start";
    case EV_GAME_END:       return "game_end";
    case EV_ROOM_START:     return "room_start";
    case EV_ROOM_END:       return "room_end";
    case EV_NO_MORE_LIVES:  return "no_more_lives";
    case EV_NO_MORE_HEALTH: return "no_more_health";
    case EV_ANIMATION_END:  return "animation_end";
    case EV_END_OF_PATH:    return "end_of_path";
    
    case EV_USER0:  return "user0";
    case EV_USER1:  return "user1";
    case EV_USER2:  return "user2";
    case EV_USER3:  return "user3";
    case EV_USER4:  return "user4";
    case EV_USER5:  return "user5";
    case EV_USER6:  return "user6";
    case EV_USER7:  return "user7";
    case EV_USER8:  return "user8";
    case EV_USER9:  return "user9";
    case EV_USER10: return "user10";
    case EV_USER11: return "user11";
    case EV_USER12: return "user12";
    case EV_USER13: return "user13";
    case EV_USER14: return "user14";
    case EV_USER15: return "user15";
  }
  return "lol_error";
}

string event_get_enigma_main_name(int mid, int id)
{
  switch (mid)
  {
    case EV_CREATE:     return "create";
    case EV_DESTROY:    return "destroy";
    case EV_ALARM:      return "alarm";
    case EV_STEP:
      return (id == STEP_BEGIN) ? "step_begin" : (id == STEP_END) ? "step_end" : "step";
    case EV_COLLISION:  return "collision";
    case EV_KEYBOARD:   return "keyboard";
    case EV_MOUSE:      return "mouse";
    case EV_OTHER:      return "other";
    case EV_DRAW:       return "draw";
    case EV_KEYPRESS:   return "keypress";
    case EV_KEYRELEASE: return "keyrelease";
    case EV_TRIGGER:    return "trigger";
  }
  return "error_not_main";
}


//Hey, it's that license from above!
const char* license = 
"/********************************************************************************\\\n"
"**                                                                              **\n"
"**  Copyright (C) 2008 Josh Ventura                                             **\n"
"**                                                                              **\n"
"**  This file is a part of the ENIGMA Development Environment.                  **\n"
"**                                                                              **\n"
"**                                                                              **\n"
"**  ENIGMA is free software: you can redistribute it and/or modify it under the **\n"
"**  terms of the GNU General Public License as published by the Free Software   **\n"
"**  Foundation, version 3 of the license or any later version.                  **\n"
"**                                                                              **\n"
"**  This application and its source code is distributed AS-IS, WITHOUT ANY      **\n"
"**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **\n"
"**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **\n"
"**  details.                                                                    **\n"
"**                                                                              **\n"
"**  You should have recieved a copy of the GNU General Public License along     **\n"
"**  with this code. If not, see <http://www.gnu.org/licenses/>                  **\n"
"**                                                                              **\n"
"**  ENIGMA is an environment designed to create games and other programs with a **\n"
"**  high-level, fully compilable language. Developers of ENIGMA or anything     **\n"
"**  associated with ENIGMA are in no way responsible for its users or           **\n"
"**  applications created by its users, or damages caused by the environment     **\n"
"**  or programs made in the environment.                                        **\n"                   
"**                                                                              **\n"
"\\********************************************************************************/\n"
"\n//This file was generated by the ENIGMA Development Environment."
"\n//Editing it is a sign of a certain medical condition. We're not sure which one.\n\n";


/*
#include <string>
#include <stdio.h>
using namespace std;

/ *
 * These functions are designed to do all the file garbage for you, making the
 * code look a little less messy, and a lot simpler.
 */
/*
extern char* writehere;
extern FILE* enigma_file;
extern int EXPECTNULL;


void space(int size);
string reads(int size);
string readSTR();

//string readNAME();
int readi();
unsigned char readb();
char* readv(int size);
void writes(FILE* fn,string str);
void writeDefine(FILE* fn,string varname,int value);
void transfer(int size,FILE* in,FILE* out);
int fileout(char* name,char* data,int size);
int transi(FILE* out);
void transSTR(FILE* out);
*/
