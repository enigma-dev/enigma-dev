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


enum
{
  LN_NOTHING,      //Nothing named
  //LN_IDENTIFIER, //Identifier (antiquated)
  LN_DECLARATOR,   //int, struct {}, you name it
  LN_TEMPLATE,     //template <>
  LN_CLASS,        //class
  LN_STRUCT,       //struct
  LN_ENUM,         //enum
  LN_UNION,        //union
  LN_NAMESPACE,    //namespace whatever
  LN_OPERATOR,     //as in the operator keyword, not a random operator
  LN_USING,        //using whatever
  LN_TYPEDEF=128,  //typedef whatever
};

//No worries. In typedef struct a b;, a is already defined.
//Grab a pointer to it, and move on.


enum
{
  SP_EMPTY,       //
  SP_IDENTIFIER,  //
  SP_COLON,       //
  SP_PUBLIC,      //
  SP_PRIVATE,     //
  SP_PROTECTED    //
};

enum
{
  OP_EMPTY,       //operator...
  OP_BRACKET,     //operator[] ...
  OP_PARENTH,     //operator() ...
  OP_NEWORDELETE, //operator new..., operator delete...
  OP_PARAMS       //operator whatever (...)
};

enum
{
  DEC_NOTHING_YET,  //not sure when this'd happen
  DEC_GENERAL_FLAG, //short, unsigned... anything but long
  DEC_LONG,         //long...
  DEC_LONGLONG,     //long long...
  DEC_FULL,         //completed type; ie, "int"
  DEC_IDENTIFIER,   //completed type and identifier. Recurses back to DEC_FULL on comma or semi.
  DEC_THROW         //int a() throw(...);
};

enum
{
  NS_NOTHING,        //namespace...
  NS_IDENTIFIER,     //namespace a...
  NS_EQUALS,         //namespace a = ...;
  NS_COMPLETE_ASSIGN //namespace a = b;
};

enum
{
  USE_NOTHING,              //using
  USE_NAMESPACE,            //using namespace
  USE_NAMESPACE_IDENTIFIER, //using namespace
  USE_SINGLE_IDENTIFIER,    //using namespace
};

enum
{
  TMP_NOTHING,
  TMP_PSTART,
  TMP_TYPENAME,
  TMP_IDENTIFIER,
  TMP_EQUALS,
  TMP_DEFAULTED
};
