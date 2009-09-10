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

struct referencer
{
  char symbol;
  unsigned short count;
  char completed;
  
  referencer(char s);
  referencer(char s,unsigned short c);
  referencer(char s,unsigned short c,char ch);
  referencer(const referencer &r);
};

struct rf_node
{
  rf_node* next;
  rf_node* prev;
  referencer ref;
  
  rf_node();
  rf_node(const referencer &r);
  rf_node(rf_node* Prev,const referencer &r);
  rf_node(const referencer &r,rf_node* Next);
  rf_node(rf_node* Prev,const referencer &r,rf_node* Next);
};
extern struct rf_stack
{
  rf_node* first;
  rf_node* last;
  rf_node* now;
  
  char nextsymbol();
  char currentsymbol();
  rf_stack &operator += (referencer r);
  rf_stack &operator++ (int nothing);
  
  //This will complete a set of function parameters,
  //Or move past the last opened set of parentheses
  rf_stack &operator-- (int nothing);
  
  rf_stack dissociate();
  void dump();
  
  bool empty();
  
} refstack;
