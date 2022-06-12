/**
 * @file read_operator_cast_type.cpp
 * @brief Source handling reading cast operator types.
 * 
 * This is a recurring problem thanks to templates, and a nontrivial problem
 * do to the way \c read_fulltype is set up. The other helper functions do not
 * stop reading at function parameters; in fact, there's no way to have them do
 * so without lookahead. However, that isn't ultimately a problem, as we can
 * simply read the function parameters in as part of the cast type, verify that
 * the outermost referencer on the stack is an empty set of parameters, and
 * finally just remove it from the final cast type.
 *
 * While the process isn't overly brilliant, it's sufficiently convoluted to
 * constitute its own black-box function.
 * 
 * @section License
 * 
 * Copyright (C) 2014 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include "Parser/context_parser.h"
#include "General/debug_macros.h"
#include "API/compile_settings.h"
#include "Storage/definition.h"

using namespace jdi;

full_type context_parser::read_operatorkw_cast_type(token_t &token, definition_scope *scope)
{
  token = lex->get_token_in_scope(scope);
  full_type ft = read_fulltype(token, scope);
  if (!ft.def) {
    token.report_errorf(herr, "Cast type needed before %s");
    return full_type();
  }
  
  if (ft.refs.empty() || ft.refs.top().type != ref_stack::RT_FUNCTION || ft.refs.top().paramcount()) {
    token.report_error(herr, "Cast operator must be a function accepting zero parameters");
    return full_type();
  }
  
  ft.refs.pop();
  ref_stack nfunc;
  ref_stack::parameter_ct funcparams;
  ref_stack::parameter oneparam(ft, nullptr);
  funcparams.throw_on(oneparam);
  nfunc.push_func(funcparams);
  ft.refs.append_c(nfunc);
  
  return ft;
}
