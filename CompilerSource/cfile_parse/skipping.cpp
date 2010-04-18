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

#include <stack>
#include <string>
#include <iostream>
using namespace std;
#include "../general/darray.h"
#include "../general/implicit_stack.h"
#include "../externs/externs.h"
#include "cfile_parse_constants.h"

#include "cfile_pushing.h"
#include "macro_functions.h"
#include "../general/parse_basics.h"

#include "expression_evaluator.h"
#include "template_handlers.h"

pt handle_skip()
{
  if (is_letter(cfile[pos]))
  {
    if (specializing and pos-specialize_start)
      specialize_string += cfile.substr(specialize_start,pos-specialize_start);
    
    //This segment is completely stolen from below and should be functionized ASAP
    pt sp = pos;
    while (is_letterd(cfile[++pos]));
    string n = cfile.substr(sp,pos-sp); //This is the word we're looking at.
    
    //This piece is also stolen from below, but is simple enough as-is
    const pt cm = handle_macros(n);
    if (cm == pt(-2)) // Was a macro
    {
      if (specializing)
        specialize_start = pos;
      return pt(-1);
    }
    if (cm != pt(-1)) return cm;
    
    //Not a macro
    if (specializing) {
      specialize_start = pos;
      specialize_string += n;
    }
    //skiping_last_id = n;
    return pt(-1);
  }
  
  /*if (is_useless(cfile[pos])) continue; //If anything ever needs to skip to whitespace, change "continue;" to "else"
  skiping_last_id = "";*/
  
  
  if (skipto == ';' and skipto2 == ';') //Special case.
  {
    if (cfile[pos] == ';' or cfile[pos] == ',')
    {
      if (skip_depth == 0)
        skipto = skipto2 = skip_inc_on = 0;
    }
    else if (cfile[pos] == '{')
      skip_depth++;
    else if (cfile[pos] == '(')
      skip_depth++;
    else if (cfile[pos] == ')')
    {
      if (skip_depth)
        skip_depth--;
      else
        skipto = skipto2 = skip_inc_on = 0;
    }
    else if (cfile[pos] == '}')
    {
      if (skip_depth)
        skip_depth--;
      else
        skipto = skipto2 = skip_inc_on = 0;
    }
    //TODO / FIXME: If there becomes need, this section needs redone in a manner that will allow proper skipping of template parameter sets.
  }
  else
  {
    if (cfile[pos] == skipto or cfile[pos] == skipto2)
    {
      if (skip_depth == 0)
        skipto = skipto2 = skip_inc_on = 0;
      else if (cfile[pos] == skipto) //Only skip if the primary is met. For instance, in "s = '>'; s2 = ','; sincon = '<';", we want to dec only on >
        skip_depth--;
      //if (cfile[pos] == ')')
        //pos++;
    }
    else if (cfile[pos] == skip_inc_on)
    {
      skip_depth++;
      /*if (specializing) {
        cferr = "Unexpected start of new template parameters in specialization of others";
        return pos;
      }*/
    }
  }
  
  if (skipto == 0)
  {
    if (specializing)
    {
      specialize_string += cfile.substr(specialize_start,pos-specialize_start);
      if ((last_named & ~LN_TYPEDEF) == LN_TEMPARGS)
      {
        last_named = LN_DECLARATOR | (last_named & LN_TYPEDEF);
        if (!access_specialization(last_type,specialize_string))
          return pos;
      }
      else if ((last_named & ~LN_TYPEDEF) == LN_TEMPLATE)
      {
        //template<whatever = a<...(*)> >
        if (last_named_phase == TMP_SIMPLE)
        {
          last_value = evaluate_expression(specialize_string);
          if (rerrpos != -1) {
            cout<< "Ignoring error in specialization expression at position " + tostring(rerrpos) + ": " + rerr << endl << endl;
            //return pos;
          }
          last_named_phase = TMP_SIMPLE_DEFAULTED;
          pos--;
        }
        else 
        {
          if (!access_specialization(last_type,specialize_string))
            return pos;
        }
      }
      else if ((last_named & ~LN_TYPEDEF) == LN_DECLARATOR and last_named_phase == DEC_FULL)
      {
        last_named = LN_DECLARATOR | (last_named & LN_TYPEDEF);
        last_type = TemplateSpecialize(last_type,specialize_string);
        if (!last_type)
          return pos;
      }
      else if ((last_named & ~LN_TYPEDEF) == LN_ENUM)
      {
        last_named_phase = EN_DEFAULTED;
        last_value = evaluate_expression(specialize_string);
        if (rerrpos != -1) {
          cferr = rerr;
          return pos;
        }
        //cout << "Evaluated \"" << specialize_string << "\" as " << last_value << endl;
      }
      else if ((last_named & ~LN_TYPEDEF) == LN_USING)
      {
        if (!access_specialization(last_type,specialize_string)) //Scope we're using
          return pos;
      }
      else if ((last_named & ~LN_TYPEDEF) == LN_STRUCT
           or  (last_named & ~LN_TYPEDEF) == LN_CLASS
           or  (last_named & ~LN_TYPEDEF) == LN_STRUCT_DD)
      {
        if (last_named_phase == SP_PUBLIC or last_named_phase == SP_PRIVATE or last_named_phase == SP_PROTECTED)
        {
          if (ihc <= 0) {
            cferr = "Error in instantiating inherited template: this should have been reported earlier";
            return pos;
          }
          if (!access_specialization(inheritance_types[ihc-1].parent,specialize_string)) //Parent we're inheriting from
            return pos;
        }
      }
      else if ((last_named & ~LN_TYPEDEF) == LN_IMPLEMENT)
      {
        if (!access_specialization(argument_type,specialize_string)) //Nothing to do with functions; we reused arg_type
          return pos;
        last_named_phase = IM_SPECD;
      }
      else if ((last_named & ~LN_TYPEDEF) == LN_OPERATOR) //operator cast_type<...> ();
      {
        string ne = temp_parse_list(last_type,specialize_string);
        if (ne == "") return pos;
        last_identifier += ne;
      }
      else if ((last_named & ~LN_TYPEDEF) == LN_TYPENAME or (last_named & ~LN_TYPEDEF) == LN_TYPENAME_P) //operator cast_type<...> ();
      {
        if (last_type == NULL) {
          cferr = "Internal error: `typename' turned up NULL somehow.";
          return pos;
        }
        if (!access_specialization(last_type,specialize_string)) //Nothing to do with functions; we reused arg_type
          return pos;
      }
      else {
        cferr = "Nothing to do with freshly parsed string. This shouldn't happen. ln = " + tostring(last_named);
        return pos;
      }
      specializing = false;
      specialize_string = "";
      if (skippast)
        pos++;
    }
    else if (skippast)
      pos++;
    skippast = 0;
  }
  else pos++;
  return pt(-1);
}
