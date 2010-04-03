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

#include "expression_evaluator_settings.h"
#include "../general/parse_basics.h"

#define RTYPE_NONE -1
#define RTYPE_INT 0
#define RTYPE_DOUBLE 1
#define RTYPE_STRING 2
#define RTYPE_UINT 3

#define OP_NONE -1
#define OP_ADD 0
#define OP_SUB 1
#define OP_MUL 2
#define OP_DIV 3
#define OP_IDIV 4
#define OP_MOD 5
#define OP_AND 6
#define OP_OR 7
#define OP_XOR 8
#define OP_BOOLAND 9
#define OP_BOOLOR 10
#define OP_BOOLXOR 11
#define OP_LSH 12
#define OP_RSH 13
#define OP_MORE 14
#define OP_LESS 15
#define OP_EQUAL 16
#define OP_NEQUAL 17
#define OP_MOREOREQ 18
#define OP_LESSOREQ 19

#define PRECEDENCE {2,2,1,1,1,1,5,5,5,5,5,5,3,3,4,4,4,4,4};

#define UNARY_NONE -1
#define UNARY_MINUS 0
#define UNARY_PLUS 1
#define UNARY_NEGATE 2
#define UNARY_NOT 3
#define UNARY_BOOL 4
#define UNARY_CHAR 5
#define UNARY_SHORT 6
#define UNARY_INT 7
#define UNARY_LONG 8
#define UNARY_FLOAT 9
#define UNARY_DOUBLE 10
#define UNARY_LONGLONG 11
#define UNARY_SIGNED 12
#define UNARY_UNSIGNED 13
#define UNARY_DECNOW 14
#define UNARY_DECLATER 15
#define UNARY_INCNOW 16
#define UNARY_INCLATER 17
#define UNARY_COUNT 18


//--------------------------------------------------------------
// This section defines functions that are specific to each type
//   Meaning they assume the first, but then check at the second.
//   They are defined this way so they only apply to supported types.
//-------------------------------------------------------------------

// int, same as/applies to long long
//------------------------------------
#if USETYPE_INT
  #define perform_operatorii(val1, operator, val2)\
    if (val2.type==RTYPE_INT) val1.real.i operator##= val2.real.i; else
  
  #if USETYPE_DOUBLE
    #define perform_operatorid(val1, operator, val2)\
      if (val2.type==RTYPE_DOUBLE)\
      {\
        val1.real.d=val1.real.i operator val2.real.d;\
        val1.type=RTYPE_DOUBLE;\
      } else
  #else
    #define perform_operatorid(val1, operator, val2)
  #endif
  
  #if USETYPE_UINT
    #define perform_operatoriu(val1, operator, val2)\
      if (val2.type==RTYPE_UINT)\
      {\
        val1.real.u = val1.real.i operator val2.real.u;\
        val1.type=RTYPE_UINT;\
      } else
  #else
    #define perform_operatoriu(val1, operator, val2)
  #endif
#else
  #define perform_operatorii(val1, operator, val2)
  #define perform_operatorid(val1, operator, val2)
  #define perform_operatoriu(val1, operator, val2)
#endif



// double, long double, float
//------------------------------------
#if USETYPE_DOUBLE
  #define perform_operatordd(val1, operator, val2)\
    if (val2.type==RTYPE_DOUBLE)\
     val1.real.d operator##= val2.real.d; else
  
  #if USETYPE_INT
    #define perform_operatordi(val1, operator, val2)\
      if (val2.type==RTYPE_INT)\
        val1.real.d operator##= val2.real.i; else
  #else
    #define perform_operatordi(val1, operator, val2)
  #endif
  
  #if USETYPE_UINT
    #define perform_operatordu(val1, operator, val2)\
      if (val2.type==RTYPE_UINT)\
        val1.real.d operator##= val2.real.u; else
  #else
    #define perform_operatordu(val1, operator, val2)
  #endif
#else
  #define perform_operatordi(val1, operator, val2)
  #define perform_operatordd(val1, operator, val2)
  #define perform_operatordu(val1, operator, val2)
#endif



// unsigned long long. For when it REALLY has to stay an integer.
//----------------------------------------------------------------
#if USETYPE_UINT
  #define perform_operatoruu(val1, operator, val2)\
    if (val2.type==RTYPE_UINT) val1.real.u operator##= val2.real.u; else
  
  #if USETYPE_INT
    #define perform_operatorui(val1, operator, val2)\
      if (val2.type==RTYPE_INT)\
      {\
        val1.real.u operator##= val2.real.i;\
      } else
  #else
    #define perform_operatorui(val1, operator, val2)
  #endif
  
  #if USETYPE_DOUBLE
    #define perform_operatorud(val1, operator, val2)\
      if (val2.type==RTYPE_DOUBLE)\
      {\
        val1.real.d=val1.real.u operator val2.real.d;\
        val1.type=RTYPE_DOUBLE;\
      } else
  #else
    #define perform_operatorud(val1, operator, val2)
  #endif
#else
  #define perform_operatorui(val1, operator, val2)
  #define perform_operatorud(val1, operator, val2)
  #define perform_operatoruu(val1, operator, val2)
#endif



//--------------------------------------------------------------
// This section defines functions that are unspecific to a type.
//   They and their routines do all type checking, just call them.
//-------------------------------------------------------------------

#define perform_operatori(val1, operator, val2)\
  if (val1.type==RTYPE_INT)\
  {\
    perform_operatorii(val1, operator, val2)\
    perform_operatorid(val1, operator, val2)\
    perform_operatoriu(val1, operator, val2);\
  } else

#define perform_operatord(val1, operator, val2)\
  if (val1.type==RTYPE_DOUBLE)\
  {\
    perform_operatordi(val1, operator, val2)\
    perform_operatordd(val1, operator, val2)\
    perform_operatordu(val1, operator, val2);\
  } else

#define perform_operatoru(val1, operator, val2)\
  if (val1.type==RTYPE_UINT)\
  {\
    perform_operatorui(val1, operator, val2)\
    perform_operatorud(val1, operator, val2)\
    perform_operatoruu(val1, operator, val2);\
  } else



//---------------------------------------------------------------------------
// This function is the big kahuna. Call it, and it'll take care of the rest.
//---------------------------------------------------------------------------
#define perform_operator(val1, operator, val2)\
    perform_operatori(val1, operator, val2)\
    perform_operatord(val1, operator, val2)\
    perform_operatoru(val1, operator, val2);





//--------------------------------------------------------------
// This section defines functions that are specific to each type
//   Meaning they assume the first, but then check at the second.
//   They are defined this way so they only apply to supported types.
//-------------------------------------------------------------------

#if USETYPE_INT
#define comparison_set(val1,x) { val1.real.i = (x); val1.type=RTYPE_INT; }
#else
  #if USETYPE_UINT
  #define comparison_set(val1,x) { val1.real.u = (x); val1.type=RTYPE_UINT; }
  #else
    #if USETYPE_DOUBLE
    #define comparison_set(val1,x) { val1.real.d = (x); val1.type=RTYPE_DOUBLE; }
    #else
      #define comparison_set(val1,x)
    #endif
  #endif
#endif

// int, same as/applies to long long
//------------------------------------
#if USETYPE_INT
  #define perform_comparisonii(val1, operator, val2)\
    if (val2.type==RTYPE_INT) comparison_set(val1,val1.real.i operator val2.real.i) else
  
  #if USETYPE_DOUBLE
    #define perform_comparisonid(val1, operator, val2)\
      if (val2.type==RTYPE_DOUBLE)\
        comparison_set(val1,val1.real.i operator val2.real.d) else
  #else
    #define perform_comparisonid(val1, operator, val2)
  #endif
  
  #if USETYPE_UINT
    #define perform_comparisoniu(val1, operator, val2)\
      if (val2.type==RTYPE_UINT)\
        comparison_set(val1,val1.real.i operator (UTYPE_INT)val2.real.u) else
  #else
    #define perform_comparisoniu(val1, operator, val2)
  #endif
#else
  #define perform_comparisonii(val1, operator, val2)
  #define perform_comparisonid(val1, operator, val2)
  #define perform_comparisoniu(val1, operator, val2)
#endif



// double, long double, float
//------------------------------------
#if USETYPE_DOUBLE
  #define perform_comparisondd(val1, operator, val2)\
    if (val2.type==RTYPE_DOUBLE)\
     comparison_set(val1,val1.real.d operator val2.real.d) else
  
  #if USETYPE_INT
    #define perform_comparisondi(val1, operator, val2)\
      if (val2.type==RTYPE_INT)\
        comparison_set(val1,val1.real.d operator val2.real.i) else
  #else
    #define perform_comparisondi(val1, operator, val2)
  #endif
  
  #if USETYPE_UINT
    #define perform_comparisondu(val1, operator, val2)\
      if (val2.type==RTYPE_UINT)\
        comparison_set(val1,val1.real.d operator val2.real.u) else
  #else
    #define perform_comparisondu(val1, operator, val2)
  #endif
#else
  #define perform_comparisondi(val1, operator, val2)
  #define perform_comparisondd(val1, operator, val2)
  #define perform_comparisondu(val1, operator, val2)
#endif



// unsigned long long. For when it REALLY has to stay an integer.
//----------------------------------------------------------------
#if USETYPE_UINT
  #define perform_comparisonuu(val1, operator, val2)\
    if (val2.type==RTYPE_UINT) comparison_set(val1,val1.real.u operator val2.real.u) else
  
  #if USETYPE_INT
    #define perform_comparisonui(val1, operator, val2)\
      if (val2.type==RTYPE_INT)\
        comparison_set(val1,val1.real.u operator (UTYPE_UINT)val2.real.i) else
  #else
    #define perform_comparisonui(val1, operator, val2)
  #endif
  
  #if USETYPE_DOUBLE
    #define perform_comparisonud(val1, operator, val2)\
      if (val2.type==RTYPE_DOUBLE)\
        comparison_set(val1,val1.real.u operator val2.real.d) else
  #else
    #define perform_comparisonud(val1, operator, val2)
  #endif
#else
  #define perform_comparisonui(val1, operator, val2)
  #define perform_comparisonud(val1, operator, val2)
  #define perform_comparisonuu(val1, operator, val2)
#endif

//---------------------------------------
//String comparison.
//---------------------------------------

#if USETYPE_STRING
  #define compstrings(val1,operator,val2)\
    if (val1.type==RTYPE_STRING and val2.type==RTYPE_STRING)\
      comparison_set(val1,val1.str operator val2.str)
#else
  #define compstrings(val1,operator,val2)
#endif

//--------------------------------------------------------------
// This section defines functions that are unspecific to a type.
//   They and their routines do all type checking, just call them.
//-------------------------------------------------------------------

#define perform_comparisoni(val1, operator, val2)\
  if (val1.type==RTYPE_INT)\
  {\
    perform_comparisonii(val1, operator, val2)\
    perform_comparisonid(val1, operator, val2)\
    perform_comparisoniu(val1, operator, val2);\
  } else

#define perform_comparisond(val1, operator, val2)\
  if (val1.type==RTYPE_DOUBLE)\
  {\
    perform_comparisondi(val1, operator, val2)\
    perform_comparisondd(val1, operator, val2)\
    perform_comparisondu(val1, operator, val2);\
  } else

#define perform_comparisonu(val1, operator, val2)\
  if (val1.type==RTYPE_UINT)\
  {\
    perform_comparisonui(val1, operator, val2)\
    perform_comparisonud(val1, operator, val2)\
    perform_comparisonuu(val1, operator, val2);\
  } else



//---------------------------------------------------------------------------
// This function is the big kahuna. Call it, and it'll take care of the rest.
//---------------------------------------------------------------------------

#define perform_comparison(val1, operator, val2)\
    perform_comparisoni(val1, operator, val2)\
    perform_comparisond(val1, operator, val2)\
    perform_comparisonu(val1, operator, val2)\
    compstrings(val1, operator, val2);






//--------------------------------------------------------------
// This section defines functions that are specific to each type
//   Meaning they assume the first, but then check at the second.
//   They are defined this way so they only apply to supported types.
//-------------------------------------------------------------------

// int, same as/applies to long long
//------------------------------------
#if USETYPE_INT
  #define perform_operator_noeii(val1, operator, val2)\
    if (val2.type==RTYPE_INT) val1.real.i = (val1.real.i!=0) operator (val2.real.i!=0); else
  
  #if USETYPE_DOUBLE
    #define perform_operator_noeid(val1, operator, val2)\
      if (val2.type==RTYPE_DOUBLE)\
      {\
        val1.real.d = (val1.real.i!=0) operator (val2.real.d!=0);\
        val1.type=RTYPE_DOUBLE;\
      } else
  #else
    #define perform_operator_noeid(val1, operator, val2)
  #endif
  
  #if USETYPE_UINT
    #define perform_operator_noeiu(val1, operator, val2)\
      if (val2.type==RTYPE_UINT)\
      {\
        val1.real.u = (val1.real.i!=0) operator (val2.real.u!=0);\
        val1.type=RTYPE_UINT;\
      } else
  #else
    #define perform_operator_noeiu(val1, operator, val2)
  #endif
#else
  #define perform_operator_noeii(val1, operator, val2)
  #define perform_operator_noeid(val1, operator, val2)
  #define perform_operator_noeiu(val1, operator, val2)
#endif



// double, long double, float
//------------------------------------
#if USETYPE_DOUBLE
  #define perform_operator_noedd(val1, operator, val2)\
    if (val2.type==RTYPE_DOUBLE)\
     val1.real.d = (val1.real.d!=0) operator (val2.real.d!=0); else
  
  #if USETYPE_INT
    #define perform_operator_noedi(val1, operator, val2)\
      if (val2.type==RTYPE_INT)\
        val1.real.d = (val1.real.d!=0) operator (val2.real.i!=0); else
  #else
    #define perform_operator_noedi(val1, operator, val2)
  #endif
  
  #if USETYPE_UINT
    #define perform_operator_noedu(val1, operator, val2)\
      if (val2.type==RTYPE_UINT)\
        val1.real.d = (val1.real.d!=0) operator (val2.real.u!=0); else
  #else
    #define perform_operator_noedu(val1, operator, val2)
  #endif
#else
  #define perform_operator_noedi(val1, operator, val2)
  #define perform_operator_noedd(val1, operator, val2)
  #define perform_operator_noedu(val1, operator, val2)
#endif



// unsigned long long. For when it REALLY has to stay an integer.
//----------------------------------------------------------------
#if USETYPE_UINT
  #define perform_operator_noeuu(val1, operator, val2)\
    if (val2.type==RTYPE_UINT) val1.real.u = (val1.real.u!=0) operator (val2.real.u!=0); else
  
  #if USETYPE_INT
    #define perform_operator_noeui(val1, operator, val2)\
      if (val2.type==RTYPE_INT)\
      {\
        val1.real.u = (val1.real.u!=0) operator (val2.real.i!=0);\
      } else
  #else
    #define perform_operator_noeui(val1, operator, val2)
  #endif
  
  #if USETYPE_DOUBLE
    #define perform_operator_noeud(val1, operator, val2)\
      if (val2.type==RTYPE_DOUBLE)\
      {\
        val1.real.d = (val1.real.u!=0) operator (val2.real.d!=0);\
        val1.type=RTYPE_DOUBLE;\
      } else
  #else
    #define perform_operator_noeud(val1, operator, val2)
  #endif
#else
  #define perform_operator_noeui(val1, operator, val2)
  #define perform_operator_noeud(val1, operator, val2)
  #define perform_operator_noeuu(val1, operator, val2)
#endif



//--------------------------------------------------------------
// This section defines functions that are unspecific to a type.
//   They and their routines do all type checking, just call them.
//-------------------------------------------------------------------

#define perform_operator_noei(val1, operator, val2)\
  if (val1.type==RTYPE_INT)\
  {\
    perform_operator_noeii(val1, operator, val2)\
    perform_operator_noeid(val1, operator, val2)\
    perform_operator_noeiu(val1, operator, val2);\
  } else

#define perform_operator_noed(val1, operator, val2)\
  if (val1.type==RTYPE_DOUBLE)\
  {\
    perform_operator_noedi(val1, operator, val2)\
    perform_operator_noedd(val1, operator, val2)\
    perform_operator_noedu(val1, operator, val2);\
  } else

#define perform_operator_noeu(val1, operator, val2)\
  if (val1.type==RTYPE_UINT)\
  {\
    perform_operator_noeui(val1, operator, val2)\
    perform_operator_noeud(val1, operator, val2)\
    perform_operator_noeuu(val1, operator, val2);\
  } else



//---------------------------------------------------------------------------
// This function is the big kahuna. Call it, and it'll take care of the rest.
//---------------------------------------------------------------------------
#define perform_operator_noe(val1, operator, val2)\
    perform_operator_noei(val1, operator, val2)\
    perform_operator_noed(val1, operator, val2)\
    perform_operator_noeu(val1, operator, val2);







//--------------------------------------------------------------
// This section defines functions that are specific to each type
//   Meaning they assume the first, but then check at the second.
//   They are defined this way so they only apply to supported types.
//-------------------------------------------------------------------


// int, same as/applies to long long
//------------------------------------
#if USETYPE_INT
  #define perform_intoperatorii(val1, operator, val2)\
    if (val2.type==RTYPE_INT) val1.real.i operator##= val2.real.i; else
  
  #if USETYPE_DOUBLE
    #if ALLOW_FLOAT_BITOPS
      #define perform_intoperatorid(val1, operator, val2)\
        if (val2.type==RTYPE_DOUBLE)\
          val1.real.i operator##= (long long)val2.real.d; else
    #else
      #define perform_intoperatorid(val1, operator, val2) { rerr="Performing a bitwise operator on a float"; rerrpos=pos; return 0; }
    #endif
  #else
    #define perform_intoperatorid(val1, operator, val2)
  #endif
  
  #if USETYPE_UINT
    #define perform_intoperatoriu(val1, operator, val2)\
      if (val2.type==RTYPE_UINT)\
        val1.real.i operator##= val2.real.u; else
  #else
    #define perform_intoperatoriu(val1, operator, val2)
  #endif
#else
  #define perform_intoperatorii(val1, operator, val2)
  #define perform_intoperatorid(val1, operator, val2)
  #define perform_intoperatoriu(val1, operator, val2)
#endif



// double, long double, float
//------------------------------------
#if ALLOW_FLOAT_BITOPS
    #if USETYPE_DOUBLE
      #define perform_intoperatordd(val1, operator, val2)\
        if (val2.type==RTYPE_DOUBLE)\
         val1.real.d=(long long)val1.real.d operator (long long)val2.real.d; else
      
      #if USETYPE_INT
        #define perform_intoperatordi(val1, operator, val2)\
          if (val2.type==RTYPE_INT)\
            val1.real.d=(long long)val1.real.d operator val2.real.i; else
      #else
        #define perform_intoperatordi(val1, operator, val2)
      #endif
      
      #if USETYPE_UINT
        #define perform_intoperatordu(val1, operator, val2)\
          if (val2.type==RTYPE_UINT)\
            val1.real.d=(long long)val1.real.d operator val2.real.u; else
      #else
        #define perform_intoperatordu(val1, operator, val2)
      #endif
    #else
      #define perform_intoperatordi(val1, operator, val2)
      #define perform_intoperatordd(val1, operator, val2)
      #define perform_intoperatordu(val1, operator, val2)
    #endif
#else
    #if USETYPE_DOUBLE
      #define perform_intoperatordd(val1, operator, val2)\
        if (val2.type==RTYPE_DOUBLE)\
           { rerr="Performing a bitwise operator on a float"; rerrpos=pos; return 0; } else
      
      #if USETYPE_INT
        #define perform_intoperatordi(val1, operator, val2)\
          if (val2.type==RTYPE_INT)\
            { rerr="Performing a bitwise operator on a float"; rerrpos=pos; return 0; } else
      #else
        #define perform_intoperatordi(val1, operator, val2)
      #endif
      
      #if USETYPE_UINT
        #define perform_intoperatordu(val1, operator, val2)\
          if (val2.type==RTYPE_UINT)\
            { rerr="Performing a bitwise operator on a float"; rerrpos=pos; return 0; } else
      #else
        #define perform_intoperatordu(val1, operator, val2)
      #endif
    #else
      #define perform_intoperatordi(val1, operator, val2)
      #define perform_intoperatordd(val1, operator, val2)
      #define perform_intoperatordu(val1, operator, val2)
    #endif
#endif

// unsigned long long. For when it REALLY has to stay an integer.
//----------------------------------------------------------------
#if USETYPE_UINT
  //#define val1.real.u=x) { val1.real.u = (x); val1.type=RTYPE_UINT; }
  
  #define perform_intoperatoruu(val1, operator, val2)\
    if (val2.type==RTYPE_UINT) val1.real.u operator##= val2.real.u; else
  
  #if USETYPE_INT
    #define perform_intoperatorui(val1, operator, val2)\
      if (val2.type==RTYPE_INT)\
        val1.real.u operator##= val2.real.i; else
  #else
    #define perform_intoperatorui(val1, operator, val2)
  #endif
  
  #if USETYPE_DOUBLE
    #if ALLOW_FLOAT_BITOPS
      #define perform_intoperatorud(val1, operator, val2)\
        if (val2.type==RTYPE_DOUBLE)\
          val1.real.u operator##= (long long)val2.real.d; else
    #else
      #define perform_intoperatorud(val1, operator, val2) { rerr="Performing a bitwise operator on a float"; rerrpos=pos; return 0; }
    #endif
  #else
    #define perform_intoperatorud(val1, operator, val2)
  #endif
#else
  #define perform_intoperatorui(val1, operator, val2)
  #define perform_intoperatorud(val1, operator, val2)
  #define perform_intoperatoruu(val1, operator, val2)
#endif



//--------------------------------------------------------------
// This section defines functions that are unspecific to a type.
//   They and their routines do all type checking, just call them.
//-------------------------------------------------------------------

#define perform_intoperatori(val1, operator, val2)\
  if (val1.type==RTYPE_INT)\
  {\
    perform_intoperatorii(val1, operator, val2)\
    perform_intoperatorid(val1, operator, val2)\
    perform_intoperatoriu(val1, operator, val2);\
  } else

#define perform_intoperatord(val1, operator, val2)\
  if (val1.type==RTYPE_DOUBLE)\
  {\
    perform_intoperatordi(val1, operator, val2)\
    perform_intoperatordd(val1, operator, val2)\
    perform_intoperatordu(val1, operator, val2);\
  } else

#define perform_intoperatoru(val1, operator, val2)\
  if (val1.type==RTYPE_UINT)\
  {\
    perform_intoperatorui(val1, operator, val2)\
    perform_intoperatorud(val1, operator, val2)\
    perform_intoperatoruu(val1, operator, val2);\
  } else



//---------------------------------------------------------------------------
// This function is the big kahuna. Call it, and it'll take care of the rest.
//---------------------------------------------------------------------------

#define perform_intoperator(val1, operator, val2)\
    perform_intoperatori(val1, operator, val2)\
    perform_intoperatord(val1, operator, val2)\
    perform_intoperatoru(val1, operator, val2);





//-------------------------------------------------------------------------------------------------
// Define a routine for adding strings. If they're supported.
//-------------------------------------------------------------------------------------------------

#if USETYPE_STRING
  #define addstrings(val1,val2)\
    if (val1.type==RTYPE_STRING and val2.type==RTYPE_STRING) val1.str+=val2.str;
#else
  #define addstrings(val1,val2)
#endif


//-------------------------------------------------------------------------------------------------
// This function is as high as it goes. It handles an operator of any sort, knowing nothing at all.
// It is the only thing here we'll actually be calling through the course of the parsing.
//-------------------------------------------------------------------------------------------------

//Pop and Evaluate
#define opstack_pae(q)\
    switch (op[level][--opc[level]])\
    {\
      case OP_ADD: perform_operator(regval[level][opc[level]],+,regval[level][opc[level]+1]); addstrings(regval[level][opc[level]],regval[level][opc[level]+1]); break;\
      case OP_SUB: perform_operator(regval[level][opc[level]],-,regval[level][opc[level]+1]); break;\
      case OP_MUL: perform_operator(regval[level][opc[level]],*,regval[level][opc[level]+1]); break;\
      case OP_BOOLAND: perform_operator_noe(regval[level][opc[level]],&,regval[level][opc[level]+1]); break;\
      case OP_BOOLOR: perform_operator_noe(regval[level][opc[level]],|,regval[level][opc[level]+1]); break;\
      case OP_BOOLXOR: perform_operator_noe(regval[level][opc[level]],^,regval[level][opc[level]+1]); break;\
      case OP_DIV: if(nz(regval[level][opc[level]+1])) { perform_operator(regval[level][opc[level]],/,regval[level][opc[level]+1]); } break;\
      case OP_IDIV: if(nz(regval[level][opc[level]+1])) { perform_intoperator(regval[level][opc[level]],/,regval[level][opc[level]+1]); } break;\
      case OP_MOD: perform_intoperator(regval[level][opc[level]],%,regval[level][opc[level]+1]); break;\
      case OP_AND: perform_intoperator(regval[level][opc[level]],&,regval[level][opc[level]+1]); break;\
      case OP_OR:  perform_intoperator(regval[level][opc[level]],|,regval[level][opc[level]+1]); break;\
      case OP_XOR: perform_intoperator(regval[level][opc[level]],^,regval[level][opc[level]+1]); break;\
      case OP_LSH: perform_intoperator(regval[level][opc[level]],<<,regval[level][opc[level]+1]); break;\
      case OP_RSH: perform_intoperator(regval[level][opc[level]],>>,regval[level][opc[level]+1]); break;\
      case OP_MORE: perform_comparison(regval[level][opc[level]],>,regval[level][opc[level]+1]); break;\
      case OP_LESS: perform_comparison(regval[level][opc[level]],<,regval[level][opc[level]+1]); break;\
      case OP_EQUAL: perform_comparison(regval[level][opc[level]],==,regval[level][opc[level]+1]); break;\
      case OP_NEQUAL: perform_comparison(regval[level][opc[level]],!=,regval[level][opc[level]+1]); break;\
      case OP_MOREOREQ: perform_comparison(regval[level][opc[level]],>=,regval[level][opc[level]+1]); break;\
      case OP_LESSOREQ: perform_comparison(regval[level][opc[level]],<=,regval[level][opc[level]+1]); break;\
    }

#define flush_opstack()\
  while (opc[level] > 0)\
  {\
    opstack_pae()\
  }






