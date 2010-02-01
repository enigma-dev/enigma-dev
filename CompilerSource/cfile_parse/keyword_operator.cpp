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

#include <map>
#include <string>
#include <iostream>
#include "../general/darray.h"

using namespace std;

#include "value.h"
#include "../externs/externs.h"
#include "expression_evaluator.h"

#include "cfile_parse_constants.h"
#include "cparse_shared.h"

extern string cferr;

//110 lines of clutter.
//For operators new, delete, new[], and delete[], see the list of keywords in cfile_parse.cpp
int keyword_operator()
{
  if (last_named_phase==OP_EMPTY)
  {
    char o=cfile[pos];
    if (string("+-*/=<>%&^!|~[(,").find(o) != string::npos)
    {
      pos++;
      char o2=cfile[pos];
      if (o2 == '='  and  string("+-*/=!<>&|^").find(o2) != string::npos)
      {
        pos++;
        last_identifier=string("operator") + o + o2;
        last_named_phase=OP_PARAMS;
        return -1; //+=, -=, *=, /=, !=, <=, >=
      }
      else if (o2 == o and string("+-&|<>").find(o) != string::npos)
      {
        pos++;
        last_identifier=string("operator") + o + o2;
        if (cfile[pos]=='=' and (o == '<' or o == '>'))
        {
          pos++;
          last_identifier+="=";
        }
        last_named_phase=OP_PARAMS;
        return -1; //++ or --, &&, ||, <<, >>, <<=, >>=
      }
      else if (o == '-' and o2 == '>')
      {
        last_identifier = "operator->";
        last_named_phase = OP_PARAMS;
        pos++; return -1;
      }
      else if (o=='[')
      {
        last_named_phase=OP_BRACKET;
        last_identifier="operator[]";
        return -1;
      }
      else if (o=='(')
      {
        last_named_phase=OP_PARENTH;
        last_identifier="operator()";
        return -1;
      }
      last_identifier=string("operator") + o;
      last_named_phase = OP_PARAMS;
      return -1; //Single-character operator
    }
    else
    {
      ((cferr="Can't overload this: '")+=cfile[pos])+="'";
      return pos;
    }
  }
  if (last_named_phase==OP_BRACKET)
  {
    if (cfile[pos] != ']')
    {
      cferr="Symbol ']' expected";
      return pos;
    }
    last_named_phase=OP_PARAMS;
    pos++; return -1;
  }
  if (last_named_phase==OP_PARENTH)
  {
    if (cfile[pos] != ')')
    {
      cferr="Symbol ')' expected";
      return pos;
    }
    last_named_phase=OP_PARAMS;
    pos++; return -1;
  }
  if (last_named_phase==OP_NEWORDELETE)
  {
    if (cfile[pos] == '[')
    {
      last_identifier += "[]";
      last_named_phase=OP_BRACKET;
      pos++; return -1;
    }
    last_named_phase = OP_PARAMS;
    return -1;
  }
  if (last_named_phase == OP_CAST)
  {
    if (cfile[pos] != '(') {
      cferr = "Expected '(' at this point";
      return pos;
    }
    last_named_phase = OP_PARAMS;
    return -1;
  }
  cferr="Unexpected symbol";
  return pos;
}
