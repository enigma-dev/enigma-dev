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

int is_lettersdigits(std::string argument0,int argument1=0);
int is_lettersdigits(char argument0,int argument1=0);
int is_of_digits(std::string argument0);
int is_of_digits(char argument0);
int is_of_letters(std::string argument0);
int is_of_letters(char argument0);
void parser_buffer_syntax_map();
//void parser_buffer_UNCcode(int textfileread);
int parser_end_of_braces(unsigned int pos);
int parser_end_of_brackets(unsigned int pos);
int parser_end_of_parenths(unsigned int pos);
//void parser_fix_typecasts();
int parser_potential_error(int argument0, int argument1=0, int argument2=0);
//void parser_recast_unify();
void parser_recode_repeat();
void parser_remove_for_newlines();
void string_replace_stack(std::string argument0, std::string argument1);
void parser_remove_spaces();
int parser_remove_strings();
unsigned int findnotstr(std::string findthis,unsigned int poss);
std::string parse_asm(std::string code);
void parser_store_other_languages();
void parser_replace(std::string argument0,std::string argument1);
void parser_restore_strings();
void parser_concatenate_strings();
void parser_restore_languages();
int parser_start_of_braces(unsigned int pos);
int parser_start_of_brackets(unsigned int pos);
int parser_start_of_parenths(unsigned int pos);
int string_isnt_operator(int argument0,int argument1);
void parser_convert_switch(unsigned int pos,int argument1);
void parser_check_for_loop(unsigned int strpos,int argument1=0,int argument2=0);
void parser_add_semicolons();
void parser_identify_statement(std::string argument0);
void parser_infinite_arguments(std::string argument0);
void parser_num_arguments(std::string argument0);
void parser_parse_dots();
void parser_parse_multidimensional(int argument0);
void parser_manage_multidimensional();
int parser_parse_statement(int argument0,int argument2);
int parser_patch_inconsistency(std::string argument0, std::string argument1);
//void parser_export(std::string argument0)
void parser_replace_until();
void parser_handle_new();
void parser_unhandle_new();
void parser_remove_newlines();
void parser_destroy_who();
void parser_get_varnamelist(string& code,string& synt,string*& vns,string*& vnss,string*& vnts,string*& vnas,int &vnc);
void parser_develop_token_stream();
void parser_buffer_remove_linecomments(std::string code);
void parser_get_varnamelist(string& code,string& synt,string*& vns,string*& vnss,string*& vnts,string*& vnas,int &vnc);
//void parser_get_withvars();
int varname_onlists(std::string vname);
int varname_oninheriteds(std::string vname);
int varname_onnotinheriteds(std::string vname);
void parser_get_withcodes();

