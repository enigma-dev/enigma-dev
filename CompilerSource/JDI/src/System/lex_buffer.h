/**
 * @file lex_buffer.h
 * @brief Header declaring a token buffer which can be used as a lexer.
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

#include <API/lexer_interface.h>

namespace jdip {
  /**
    A queue-like buffer class for reading in tokens and passing them around as a lexer.
    This is useful for conditional delegation of blocks of tokens, such as 
  */
  class lex_buffer: public lexer {
    const static size_t chunk_size = 10;
    token_t *cur_token; ///< The token that will be returned when polled
    struct chunk {
      token_t data[chunk_size]; ///< The tokens to be polled for.
      chunk *next; ///< The next chunk of tokens in the list.
      /** Convenience constructor: construct with a previous node. */
      chunk(chunk* prev);
      /** Default constructor. */
      chunk();
    } base;
    chunk *cur_chunk; ///< Pointer to the current data chunk
    size_t at_index; ///< The absolute index of the token we are at
    size_t total; ///< The total number of tokens
    unsigned modulo; ///< The number of tokens left in this chunk
    
    public:
      /** The fallback lexer; the lexer which will be pulled after we're empty. */
      lexer *fallback_lexer;
      /** Push a token, consuming it. This token will not be
          extractable until you call reset().
          @param token  The token to push onto our lex queue. */
      token_t& push(const token_t &token);
      /** Returns true if all tokens have been read */
      bool empty();
      /** Prepare for read after pushing or already reading. */
      void reset();
      /** Return the first unread token in this buffer */
      token_t get_token(error_handler *herr);
      /** Construct with a fallback lexer */
      lex_buffer(lexer* l);
      /** Destruct, cleaning up chunks */
      ~lex_buffer();
  };
}
