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

#include "../externs/externs.h"
#include "../parser/object_storage.h"

struct onode
{
  string op; // What would follow the word "operator" if overloaded
  externs *type; // The type we would return if this were it for the stack: this tracks all referencing!
  unsigned otype; // The kind of operator this is; binary or unary, for example
  unsigned short prec;  // The precedence integer of this level.
  unsigned short pad;  // The number of & referencers on top of our existing ref stack
  rf_node *deref;     // The rf_node of the next dereference needed, or NULL if conked out
  
  onode();
  onode(externs* t);
  onode(string oN,unsigned oT,short pr,externs *e);
  void operator <= (const onode &t);
};

extern void exp_typeof_init();
extern string externs_name(onode e);
extern onode exp_typeof(string exp,map<string,dectrip>** lvars = NULL,int lvarc = 0);
