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

typedef size_t pt; //Use size_t as our pos type; this will prevent errors with size_t's like std::string::npos

#include "../general/darray.h"

extern map<string,char> edl_tokens;
int parser_ready_input(string&,string&,unsigned int&,varray<string>&);
void parser_remove_whitespace(string& code,const int use_cpp_strings=0);
void parser_buffer_syntax_map(string &code,string &syntax,const int use_cpp_numbers = 0);
void parser_fix_templates(string &code,pt pos,pt spos,string *synt);
void parser_add_semicolons(string &code,string &synt);
void print_the_fucker(string code,string synt);
int parser_reinterpret(string&,string&);

int dropscope();
int quickscope();
int initscope(string name);
int quicktype(unsigned flags, string name);


