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
    *    TODO: implement this system.
    *
    *********************************************/

  std::string format_human_readable_error_message(std::string& message);

  /********************************************
    *
    *    Main system.
    * 
    *    '[' means start of a new level, and ']' is the end of that new level.
    * 
    *    New levels are considered "array of objects" by default. If a "key/value pair" is found
    *    in current level then state of current level is changed.
    * 
    *           "array of objects" ----> single JSON "object"
    *
    *********************************************/
  enum class levelState { arrayOfObjectsState, objectState };

  /*
    This pointer iterate over the whole string and doesn't ignore spaces.
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
    Stores state of the current level (array of objects/single JSON "object").
  */
  std::stack<levelState> levels_states_;

  /*
    If current level is a JSON "object", we need to keep track of entities' indices for the accumulated keys
    in current level.

    First entity   --> 0
    Second entity  --> 1
    ... entity     --> ...
    Last entity    --> N-1

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
  bool accumulate_null_value();

  /********************************************
    * 
    *    Stores boundaries for each level.
    *
    *    For example:
    *
    *        0  1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16  --> indices
    *        |  |  |  |  |  |  |  |  |  |  |   |   |   |   |   |   |
    *        |  |  |  |  |  |  |  |  |  |  |   |   |   |   |   |   |
    *        [  [  [  1  ,  4  ]  ,  4  ]  ,   [   1   ,   4   ]   ]   --> short json example
    *           |                       |
    *           |-----------------------|    --> this level has boundaries [1,9].
    *
    *********************************************/
  std::stack<std::pair<size_t, size_t>> levels_boundaries_pointers_;

  /*
    Stores entities' boundaries for each level.

    Entities can be "object" or "array of objects".

    Ignore "key/value pair" entity as it has constant length which is 4 + (size of the value).

    For example: "0":"test" --> length of key is 4, and length of value is 6.
  */
  std::stack<std::queue<std::pair<size_t, size_t>>> levels_entities_boundaries_pointers_;

  /************************************************************************
    * 
    *        Stores offsets for "objects" and "arrays of objects" in current level to prevent interating on them more than once.
    * 
    *        Ignore "key/value pair" entity as it has constant offset which is 4 + (size of the value).
    *
    *        For example:
    *
    *                                                                    
    *               |-------   9   ---------|      |-----  5  -----|       --> skip 9 then skip 5.
    *               |                       |      |               |
    *            [  [  [  1  ,  4  ]  ,  4  ]  ,   [   1   ,   4   ]   ]
    *
    *        - Note that storing offsets is stopped when we meet a "key/value pair",
    *            because current level is an "object" and we won't need to
    *            accumulate indices anymore. Indices are accumulated automatically
    *            if current level is an "object", see accumulate_string_value(), 
    *            accumulate_number_value(), and accumulate_boolean_value() functions.
    *
    *************************************************************************/
  std::stack<std::queue<size_t>> levels_entities_offsets_;

  /*
    When storing offsets is stopped if current level is a single JSON "object", 
    we continue accumulating offsets of current level.
  */
  std::stack<size_t> levels_cumulative_offsets_;

  /*
    This function maps short JSON indices to normal JSON indices.

    For example:

        0           4         0                       12
        |           |         |                       |
        |           |         |                       |
        [  1  ,  4  ]  ---->  {  "0":  1  ,  "1":  4  }
  */
  void map_short_json_indices();

  /*
    If current level is an "array of objects", keys aren't accumulated by default. A "key/value pair" means that
    current level is a single JSON "object". A "key/value pair" may come at the end of current level, which means 
    that there are some missing keys that must be accumulated.
  */
  void accumulate_missing_keys();

  /********************************************
    *
    *    Short JSON syntax validation system.
    *
    *********************************************/
  enum class shortJSONToken { leftSquareBracket, rightSquareBracket, comma, value, invalid };

  shortJSONToken next_token_[2] = {shortJSONToken::leftSquareBracket, shortJSONToken::invalid};

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
  ShortJSONConverter() = default;
  ~ShortJSONConverter() = default;

  /*
    Parse short JSON and put the output in the buffer.
  */
  bool parse_into_buffer(const std::string& data, std::string* buffer);
};

}  // namespace enigma

#endif
