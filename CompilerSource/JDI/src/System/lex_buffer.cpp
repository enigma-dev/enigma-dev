/**
 * @file lex_buffer.cpp
 * @brief Source implementing a token buffer which can be used as a lexer.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2012 Josh Ventura
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

#include "lex_buffer.h"

namespace jdip {
  token_t& lex_buffer::push(const token_t &token) {
    token_t &ret = *cur_token = token;
    if (--modulo)
      ++cur_token;
    else {
      cur_chunk = new chunk(cur_chunk);
      cur_token = cur_chunk->data;
      modulo = chunk_size;
    }
    ++total;
    ++at_index;
    return ret;
  }
  
  bool lex_buffer::empty() {
    return at_index >= total;
  }
  
  token_t lex_buffer::get_token(error_handler* herr) {
    if (at_index >= total)
      return fallback_lexer->get_token(herr);
    token_t &ret = *cur_token;
    if (--modulo)
      ++cur_token;
    else{
      if ((cur_chunk = cur_chunk->next))
        cur_token = cur_chunk->data;
      else cur_token = NULL;
      modulo = chunk_size;
    }
    ++at_index;
    return ret;
  }
  
  void lex_buffer::reset() {
    cur_token = base.data;
    cur_chunk = &base;
    at_index = 0;
    modulo = chunk_size;
  }
  
  lex_buffer::lex_buffer(lexer* fallback): cur_token(base.data), base(), cur_chunk(&base), at_index(0), total(0), modulo(chunk_size), fallback_lexer(fallback) {}
  lex_buffer::~lex_buffer() { for (chunk *n = base.next; n;) { __REGISTER chunk *const delme = n; n = n->next; delete delme; } }
  
  lex_buffer::chunk::chunk(): next(NULL) {}
  lex_buffer::chunk::chunk(chunk* prev): next(NULL) { prev->next = this; }
}
