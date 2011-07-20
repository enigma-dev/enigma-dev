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

inline bool is_wordop(string name)
{
  switch (name[0]) {
    case 'a':
      return name == "and";
    case 'd':
      return name == "div";
    case 'm':
      return name == "mod";
    case 'n':
      return name == "not";
    case 'o':
      return name == "or";
    case 'x':
      return name == "xor";
  }
  return false;
}

inline bool is_statement(string name)
{
  switch (name[0])
  {
    case 'b':
      return name == "break";
    case 'c':
      return name == "case" or name == "catch" or name == "continue";
    case 'd':
      return name == "default" or name == "do";
    case 'e':
      return name == "else" or name == "exit";
    case 'f':
      return name == "for";
    case 'i':
      return name == "if";
    case 'r':
      return name == "return" or name == "repeat";
    case 's':
      return name == "switch";
    case 't':
      return name == "try";
    case 'u':
      return name == "until";
    case 'w':
      return name == "while" or name == "with";
  }
  return false;
}

inline TT statement_type(string name)
{
  switch (name[0])
  {
    case 'b':
      if (name == "break") return TT_TINYSTATEMENT;
      break;
    case 'c':
      if (name == "case") return TT_S_CASE;
      if (name == "catch") return TT_S_CATCH; 
      if (name == "continue") return TT_TINYSTATEMENT;
      break;
    case 'd':
      if (name == "default") return TT_S_DEFAULT;
      if (name == "do") return TT_S_DO;
      break;
    case 'e':
      if (name == "else") return TT_S_ELSE;
      if (name == "exit") return TT_TINYSTATEMENT;
      break;
    case 'f':
      if (name == "for") return TT_S_FOR;
      break;
    case 'i':
      if (name == "if") return TT_S_IF;
    case 'r':
      if (name == "return") return TT_SHORTSTATEMENT;
      if (name == "repeat") return TT_GEN_STATEMENT;
      break;
    case 's':
      if (name == "switch") return TT_S_SWITCH;
    case 't':
      if (name == "try") return TT_S_TRY;
    case 'u':
      if (name == "until") return TT_SHORTSTATEMENT;
    case 'w':
      if (name == "while") return TT_SHORTSTATEMENT;
      if (name == "with") return TT_SHORTSTATEMENT;
  }
  return TT_ERROR;
}
