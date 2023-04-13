/**
 *  @file short_json_converter.cpp
 *  @section License
 *
 *      Copyright (C) 2023 Saif Kandil
 *
 *      This file is a part of the ENIGMA Development Environment.
 *
 *      This program is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef SHORT_JSON_CONVERTER_H
#define SHORT_JSON_CONVERTER_H

#include <iostream>
#include <queue>
#include <stack>
#include <string>

namespace enigma {

class ShortJSONConverter {
 private:
  /********************************************
    *
    *    Error reporting system.
    *
    *********************************************/

  std::string format_human_readable_error_message(std::string& message);

  /********************************************
    *
    *    Main system.
    * 
    *    Theory: Whenever we find '[', we push a new level into the stack,
    *            and if we find ']', we pop that new level from the stack.
    *
    *    Assumptions:
    *           - Any new level is considered "array of objects" until we find
    *             a "key/value pair" which means that this new level is a single JSON "object".
    *
    *********************************************/
  enum levelState { arrayOfObjectsState, objectState };

  /*
                  This is our string data iterator.
              */
  size_t pointer_{0};

  /*
        This variable stores our input data to be easily accessible by all functions.
    */
  std::string data_;

  /*
                  Accumulates "key/value pairs", "objects", and "arrays of objects" for current level.
              */
  std::stack<std::string> levels_accumulators_;

  /*
                  is current level considered an "array of objects" or a single JSON "object"?
              */
  std::stack<levelState> levels_states_;

  /*
                If current level is a JSON "object", we need to keep track of keys' indices for each value in current level.

                Entities can be "key/value pair" or "object" or "array of objects".
              */
  std::stack<size_t> levels_entities_keys_indices_;

  void push_level();

  void pop_level();

  void accumulate_single_key();

  bool accumulate_value();

  bool accumulate_string_value();
  bool accumulate_number_value();
  bool accumulate_boolean_value();

  /********************************************
    *
    *    The next part is for increasing effeciency of the algorithm.
    *
    *    Stores boundaries for each level.
    *
    *    For example:
    *
    *        0  1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16  --> indices
    *        |  |  |  |  |  |  |  |  |  |  |   |   |   |   |   |   |
    *        |  |  |  |  |  |  |  |  |  |  |   |   |   |   |   |   |
    *        [  [  [  1  ,  4  ]  ,  4  ]  ,   [   1   ,   4   ]   ]   --> this is short json example
    *           |                       |
    *           |-----------------------|    --> this level has boundaries [1,9].
    *
    *********************************************/
  std::stack<std::pair<size_t, size_t>> levels_boundaries_pointers_;

  /*
            Stores entities' boundaries for each level.

            Entities can be "object" or "array of objects".
    */
  std::stack<std::queue<std::pair<size_t, size_t>>> levels_entities_boundaries_pointers_;

  /************************************************************************
    * 
    *        Stores offsets for "objects" and "arrays of objects" in current level, so that we don't iterate over them again.
    *
    *        Entities can be "object" or "array of objects".
    *
    *        For example:
    *
    *                                                                    
    *               |-------   9   ---------|      |-----  5  -----|       --> offsets that we need to skip.
    *               |                       |      |               |
    *            [  [  [  1  ,  4  ]  ,  4  ]  ,   [   1   ,   4   ]   ]
    *
    *        - note that: these offsets are relative to short jsons. For normal jsons,
    *            we need to map between them before proceeding.
    *            map_short_json_indices() will do that.
    *
    *        - note that we stop storing offsets when we meet a "key/value pair",
    *            which means that, current level is an "object" and we won't need to
    *            accumulate indices anymore. Indices are accumulated automatically
    *            if current level is an "object".
    *
    *************************************************************************/
  std::stack<std::queue<size_t>> levels_entities_offsets_;

  /*
            When we stop storing indices if current level is an "object", we continue accumulating offsets of current level.
    */
  std::stack<size_t> levels_cumulative_offsets_;

  /*
            This function maps short JSON indices to normal JSON indices.

            For example:

                0           4        0                       12
                |           |        |                       |
                |           |        |                       |
                [  1  ,  4  ]  --->  {  "0":  1  ,  "1":  4  }
    */
  void map_short_json_indices();

  void accumulate_missing_keys();

  /********************************************
    *
    *    Short JSON syntax validation system.
    *
    *********************************************/
  enum shortJSONToken { leftSquareBracket, rightSquareBracket, comma, value, invalid };

  shortJSONToken next_token_[2] = {leftSquareBracket, invalid};

  bool validate_next_token(shortJSONToken token);

  /********************************************
    *
    *    Spaces removal system.
    * 
    *    This pointer ignores spaces, new lines (\n), tabs (\t), and carriage returns (\r).
    *
    *********************************************/
  size_t virtual_pointer_{0};

  void skip_spaces();

 public:
  ShortJSONConverter();
  ~ShortJSONConverter();

  /*
            Parse short JSON and put the output in the buffer.
            */
  bool parse_into_buffer(std::string& data, std::string* buffer);
};

}  // namespace enigma

#endif
