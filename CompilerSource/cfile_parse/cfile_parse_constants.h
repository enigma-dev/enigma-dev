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

#ifndef _CPARSE_CONSTS_H
#define _CPARSE_CONSTS_H

enum c_last_named
{
  LN_NOTHING,     //Nothing named
  LN_DECLARATOR,  //int, struct {}, you name it
  LN_TEMPLATE,    //template <...>
  LN_CLASS,       //class
  LN_STRUCT,      //struct
  LN_ENUM,        //enum
  LN_UNION,       //union
  LN_NAMESPACE,   //namespace whatever
  LN_OPERATOR,    //as in the operator keyword, not a random operator
  LN_USING,       //using whatever
  LN_TEMPARGS,//10//std::map<...>
  LN_LABEL,       //Public, private, protected
  LN_IMPLEMENT,   //int templated_class<int>::...
  LN_DESTRUCTOR,  //Added to invoke errors where appropriate
  LN_TYPENAME,    //typename some_unimplemented_structure::doesntexist...
   LN_TYPENAME_P, //int a(typename some_unimplemented_structure::doesntexist...
  LN_STRUCT_DD,   //Previously declared struct/class
  LN_TYPEDEF=128, //typedef whatever
};

//No worries. In typedef struct a b;, a is already defined.
//Grab a pointer to it, and move on.


enum
{
  SP_EMPTY,        //Nothing but the keyword
  SP_IDENTIFIER,   //The class has been named
  SP_COLON,        //The class will now be given ancestors
  SP_PUBLIC,       //Inherits as-is
  SP_PRIVATE,      //Inherits into private scope
  SP_PROTECTED,    //Inherits into protected scope
  SP_PARENT_NAMED, //The parent's name was given
};

enum
{
  EN_NOTHING,          //enum...
  EN_IDENTIFIER,       //enum a...
  EN_WAITING,          //enum a { ... // or enum a { b, ...
  EN_CONST_IDENTIFIER, //enum a { b...
  EN_DEFAULTED         //enum a { b = 1,...
};

enum
{
  OP_EMPTY,       //operator...
  OP_BRACKET,     //operator[] ...
  OP_PARENTH,     //operator() ...
  OP_NEWORDELETE, //operator new..., operator delete...
  OP_PARAMS,      //operator whatever (...)
  OP_CAST         //; operator ...
};

enum
{
  DEC_NOTHING_YET,  //not sure when this'd happen
  DEC_GENERAL_FLAG, //const, static... Can modify anything
  DEC_ATOMIC_FLAG,  //short, unsigned... anything that can only modify atomics, except long
  DEC_LONG,         //long...
  DEC_LONGLONG,     //long long...
  DEC_FULL,         //completed type; ie, "int"
  DEC_IDENTIFIER,   //completed type and identifier. Recurses back to DEC_FULL on comma or semi.
  DEC_THROW,        //int a() throw(...);
  DEC_DESTRUCTOR    //~classname()
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
  USE_NOTHING,              //using...
  USE_NAMESPACE,            //using namespace...
  USE_NAMESPACE_IDENTIFIER, //using namespace std
  USE_SINGLE_IDENTIFIER,    //using std::string
};

enum
{
  IM_NOTHING, //Unused
  IM_SCOPE,   //int template_type...<
  IM_SPECD    //int template_type<int>... expect :
};

enum
{
  TMP_NOTHING,     //template
  TMP_PSTART,      //template <... , ...
  TMP_TYPENAME,    //template <typename...
  TMP_IDENTIFIER,  //template <typename a...
  TMP_EQUALS,      //template <typename a =...
  TMP_DEFAULTED,   //template <typename a = bool,...
  TMP_SIMPLE,      //template <bool...
  TMP_SIMPLE_DEFAULTED //template <bool = 1,...
};

enum
{
  TN_NOTHING,   //typename ...
  TN_TEMPLATE,  //typename anything::template ...
  TN_GIVEN,     //typename anything...
  TN_GIVEN_TEMP //typename anything::template anything...
};

//For efficiency

enum
{
  SH___CONST = 391,
  SH___INLINE__ = 400,
  SH_CLASS = 417,
  SH_CONST = 436,
  SH_DELETE = 416,
  SH_ENUM = 442,
  SH_EXPLICIT = 461,
  SH_EXTERN = 463,
  SH_FRIEND = 441,
  SH_INLINE = 439,
  SH_MUTABLE = 466,
  SH_NAMESPACE = 422,
  SH_NEW = 434,
  SH_OPERATOR = 444,
  SH_PRIVATE = 452,
  SH_PROTECTED = 460,
  SH_PUBLIC = 450,
  SH_STRUCT = 467,
  SH_TEMPLATE = 435,
  SH_THROW = 443,
  SH_TYPEDEF = 477,
  SH_TYPENAME = 478,
  SH_UNION = 447,
  SH_USING = 457,
  SH_VIRTUAL = 449,
};

inline short switch_hash(string x) {
  const int l = x.length();
  return l>2 ? (x[0]+x[1]+x[1]+x[2]+l) : 0;
}

#endif
