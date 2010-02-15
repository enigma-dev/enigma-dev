/*********************************************************************************\
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
\*********************************************************************************/

#include <string>
using namespace std;

#include "../externs/externs.h"
#include "cfile_parse_constants.h"

char skipto, skipto2, skip_inc_on;
bool skippast;

string cferr;
//string tostring(int val);

string cfile;
unsigned int pos = 0;
unsigned int len = cfile.length();
string cfile_top;

int skip_depth;
int specialize_start;
string specialize_string;
bool specializing;

externs *last_type = NULL;
externs *argument_type = NULL;
string last_identifier = "";
int last_named = LN_NOTHING;
int last_named_phase = 0;
long long last_value = 0;

bool cfile_debug = 0;
