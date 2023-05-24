/**
 * @file is_potential_friend.cpp
 * @brief Header with two inlined functions for checking if a name can be used as a constructor.
 *
 * This header only exists to prevent nontrivial duplicate code.
 * The code is only "nontrivial" because it has to consider templates; it's still pretty simple.
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

#include <Parser/context_parser.h>
#include <API/compile_settings.h>

int jdi::context_parser::handle_friend(definition_scope *scope, token_t& token, definition_class *recipient) {
  token = read_next_token(scope);
  bool friendclass = token.type == TT_CLASS || token.type == TT_STRUCT;
  if (friendclass)
    token = read_next_token(scope);
  else if (token.type != TT_DEFINITION && token.type != TT_DECLARATOR && token.type != TT_DECFLAG) {
    token.report_errorf(herr, "Expected friend identifier before %s");
    return 1;
  }
  full_type ft = read_fulltype(token, scope);
  definition *rd = friendclass? ft.def : ft.refs.ndef;
  if (!rd) {
    token.report_error(herr, "Cannot friend " + (ft.refs.name.empty()? "anonymous type" : "`" + ft.refs.name + "'") + ": no affiliated definition");
    return 1;
  }
  recipient->friends.insert(rd);
  return 0;
}
