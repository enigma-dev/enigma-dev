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

/* All common rules:
 *
 *   Following:
 *     assignment_must_be_closed(name)
 *     error_if_previous_operator(name)
 *     error_if_no_value_named(name)
 *     error_if_no_reference_named(name)
 *     declaration_must_be_closed(name)
 *     requires_assignment_op()
 *     error_if_previous_value()
 *     error_if_not_expecting_value()
 *     no_out_of_class_declaration()
 *     doesnt_belong_in_if(name)
 *
 *   Location:
 *     must_be_out_of_parenthesis(name)
 *
 *
 *   Operation:
 *     terminate_declaration_list()
 *     separate_statement_outside_lists()
 *     close_all_open()
 *     treat_as_varname()
 *     treat_as_number()
 *     treat_as_math_operator()
 *     merge_math_to_assignment()
 */



#define must_be_out_of_parenthesis(name)\
if (cnt==1) { error="Expected closing parenthesis before "+string(name); return pos; }\
else if (cnt>0) { error="Expected "+tostring(cnt)+" closing parentheses before "+string(name); return pos; }

#define assignment_must_be_closed(name)\
if ((assop && !varnamed[cnt] && !numnamed[cnt] && !fnamed[cnt])||mathop[cnt]) \
{ error="Expected secondary expression before "+string(name); return pos; }

#define error_if_previous_value()\
if (varnamed[cnt] || numnamed[cnt]) { error="Operator expected at this point"; return pos; }

#define error_if_previous_operator(name)\
if (mathop[cnt]) { error="Secondary expression expected before"+string(name); return pos; }

#define error_if_no_value_named(name)\
if (!(varnamed[cnt] || numnamed[cnt] || fnamed[cnt])) \
{ error="Expected primary expression before "+string(name); return pos; }

#define error_if_no_reference_named(name)\
if (!varnamed[cnt]) { error="Variable name expected before "+string(name); return pos; }

#define terminate_declaration_list()\
if (inDecList && !(varnamed[cnt] || numnamed[cnt] || assop))\
{ error="Declaration doesn't declare anything"; return pos; }\
inDecList=0;

#define declaration_must_be_closed(name)\
if (inDecList) { error="Expected declaration to terminate before "+string(name); return pos; }

#define requires_assignment_op() if (ford[lvl]==3) assop=1;\
if (!(assop || cnt || infunction[cnt])) { error="Assignment operator expected"; return pos; }

//#define declaration_must_be_closed(name)

#define separate_statement_outside_lists()\
{ if ((assop || (cnt>0)) && (varnamed[cnt] || numnamed[cnt] || fnamed[cnt])) {\
if (cnt>0) { if (!infunction[cnt]) { error="Operator expected at this point: Cannot have multiple statements inside parentheses"; return pos; }\
else { error="Comma or operator expected at this point"; return pos; } }\
if (inDecList) { error="Comma or operator expected at this point"; return pos; }\
varnamed[cnt]=numnamed[cnt]=fnamed[cnt]=0; opsep(); if (!cnt) assop=0; } }

#define error_if_not_expecting_value() if (ford[lvl]==3) assop=1;\
if (!(assop || (cnt>0)) || (assop &&  (varnamed[cnt] || numnamed[cnt] || fnamed[cnt]) ))\
{ if (varnamed[cnt]) { error="Operator expected at this point"; return pos; }\
  else               { error="Statement expected before this point"; return pos; } }

#define error_if_fragment() if (ford[lvl]==3) assop=1;\
if (varnamed[cnt] && !(assop||inDecList||infunction[cnt])) { error="Assignment operator expected"; return pos; }


#define doesnt_belong_in_if(name)


#define close_all_open()\
if ((assop || (cnt>0)) && (varnamed[cnt] || numnamed[cnt] || fnamed[cnt])) opsep();\
varnamed[cnt]=0; if (!cnt) assop=0; mathop[cnt]=varnamed[cnt]=numnamed[cnt]=fnamed[cnt]=0;

#define treat_as_varname()\
fnamed[cnt]=numnamed[cnt]=0; mathop[cnt]=0; varnamed[cnt]=1;

#define treat_as_number()\
fnamed[cnt]=varnamed[cnt]=0; mathop[cnt]=0; numnamed[cnt]=1;

#define treat_as_math_operator()\
varnamed[cnt]=numnamed[cnt]=fnamed[cnt]=0; mathop[cnt]=1;


#define no_out_of_class_declaration()\
if (inDecList && !assop) { error="Cannot declare a variable in another instance"; return pos; }


#define merge_math_to_assignment()\
if (mathop[cnt]) { mathop[cnt]=0; if (!cnt) assop=1; varnamed[cnt]=numnamed[cnt]=0; pos++; continue; }
