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

//This file is mostly documentation.
#include "../externs/externs.h"

/*Skipto and skipto2 tell the parser to ignore everything until it reaches a given symbol.
  "Everything" excludes comments and macros, which will still be handled accordingly.   */
extern char skipto, skipto2;
extern char skip_inc_on;
extern bool skippast;

extern bool is_tflag(string x);
extern bool tflag_atomic(string x);
extern externs *argument_type;

extern externs *last_type;
extern externs *argument_type;
extern string last_identifier;
extern int last_named;
extern int last_named_phase;
extern long long last_value;

extern int skip_depth;
extern int specialize_start;
extern string specialize_string;
extern bool specializing;
extern bool flag_extern;

extern string cferr;
extern string tostring(int val);

extern string cfile;
extern pt pos;
extern pt len;

extern unsigned id_would_err_at;

extern bool cfile_debug;

void print_scope_members(externs*,int indent = 2,int depth = 100);
#define tree_scope_members(x,y) print_scope_members(x,2,y);
string strace(externs*);
