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

extern unsigned int strc; // Number of strings we removed from the code.

extern map<string,char> edl_tokens;
void parser_remove_whitespace(string& code,const int use_cpp_strings=0);
void parser_buffer_syntax_map(string &code,string &syntax,const int use_cpp_numbers = 0);
void parser_add_semicolons(string &code,string &synt);
void print_the_fucker(string code,string synt);
int parser_ready_input(string&,string&);
int parser_reinterpret(string&,string&);

int dropscope();
int quickscope();
int initscope(string name);
int quicktype(unsigned flags, string name);

/*
int max(int a,int b);
int min(int a,int b);


#define ERR_NONTERMSTR     1
#define ERR_STRCATREAL     2
#define ERR_UNEXPECTED     3
#define ERR_SEMICOLON      4
#define ERR_NONTERMPAR     5
#define ERR_ASSOP          6
#define ERR_PAREXPECTED    7
#define ERR_NONTERMINATING 8
#define ERR_MALTERMINATING 9

int parser_init();

int is_letterd(string argument0,int argument1=0);
int is_letterd(char argument0,int argument1=0);
int is_digit(string argument0);
int is_digit(char argument0);
int is_letter(string argument0);
int is_letter(char argument0);
void parser_buffer_syntax_map();
//void parser_buffer_UNCcode(int textfileread);
int parser_end_of_braces(pt pos);
int parser_end_of_brackets(pt pos);
int parser_end_of_parenths(pt pos);
//void parser_fix_typecasts();
int parser_potential_error(int argument0, int argument1=0, int argument2=0);
//void parser_recast_unify();
void parser_recode_repeat();
void parser_remove_for_newlines();
void string_replace_stack(string argument0, string argument1);
void parser_remove_spaces();
int parser_remove_strings();
pt findnotstr(string findthis,pt poss);
string parse_asm(string code);
void parser_store_other_languages();
void parser_replace(string argument0,string argument1);
void parser_restore_strings();
void parser_concatenate_strings();
void parser_restore_languages();
int parser_start_of_braces(pt pos);
int parser_start_of_brackets(pt pos);
int parser_start_of_parenths(pt pos);
int string_isnt_operator(int argument0,int argument1);
void parser_convert_switch(pt pos,int argument1);
void parser_check_for_loop(pt strpos,int argument1=0,int argument2=0);
void parser_add_semicolons();
void parser_identify_statement(string argument0);
void parser_infinite_arguments(string argument0);
void parser_num_arguments(string argument0);
void parser_parse_dots();
void parser_parse_multidimensional(int argument0);
void parser_manage_multidimensional();
int parser_parse_statement(int argument0,int argument2);
int parser_patch_inconsistency(string argument0, string argument1);
//void parser_export(string argument0)
void parser_replace_until();
void parser_handle_new();
void parser_unhandle_new();
void parser_remove_newlines();
void parser_destroy_who();
void parser_get_varnamelist(string& code,string& synt,string*& vns,string*& vnss,string*& vnts,string*& vnas,int &vnc);
void parser_develop_token_stream();
void parser_buffer_remove_linecomments(string code);
void parser_get_varnamelist(string& code,string& synt,string*& vns,string*& vnss,string*& vnts,string*& vnas,int &vnc);
//void parser_get_withvars();
void parser_get_withcodes();*/

