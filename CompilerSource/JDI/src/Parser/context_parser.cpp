/**
 * @file context_parser.cpp
 * Source implementing context_parser creation and destruction.
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include "context_parser.h"
#include <API/AST.h>

#include <iostream>
using std::cerr; using std::endl;

namespace jdip {
  context_parser::context_parser(context *ctex_i, lexer *lex_i, error_handler *herr_i): ctex_alloc(NULL), ctex(ctex_i), lex(lex_i), herr(herr_i), astbuilder(new AST_Builder(this)) {
    if (ctex->parse_open) {
      cerr << "Another parser is already active on this context." << endl;
      abort();
    }
    else
      ctex->parse_open = true;
  }
  context_parser::context_parser(lexer *lex_i, error_handler *herr_i): ctex_alloc(new context(0)), ctex(ctex_alloc), lex(lex_i), herr(herr_i), astbuilder(new AST_Builder(this)) {
    ctex->parse_open = true;
  }
  context_parser::~context_parser() {
    delete astbuilder;
    if (ctex) {
      if (ctex->parse_open)
        ctex->parse_open = false;
      else {
        cerr << "Someone has freed this parse context prematurely." << endl;
        abort();
      }
    }
    delete ctex_alloc;
  }
}
