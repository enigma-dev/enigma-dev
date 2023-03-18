/**
 *  @file short_json_converter.h
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

#include "short_json.h"

namespace enigma {
class ShortToJSONConverter {
 private:
  /*
          Level theory here means that: whenever we find '[' we push a new level into the stack,
          and if we find ']' we pop the new level from the stack.

          We always assume that: the current level is an array of objects until otherwise.
      */
  enum levelState { arrayState, objectState };

  /*
          This is our string data iterator.
      */
  size_t pointer_{0};

  /*
          This variable will contain our output if it succeded
      */
  std::string json_;

  /*
          Whenever we are in a level, we need to accumulate values, objects, and arrays.
      */
  std::stack<std::string> levels_accumulators_;

  /*
          Whenever we are in a level, we need to keep track of this level state.
      */
  std::stack<levelState> levels_states_;

  /*
          This stack keeps track of current object's index in each level.

          Entities can be key/value pair or object or array.
      */
  std::stack<size_t> levels_entities_indices_;

  /*
          Stores boundaries of each level.
      */
  std::stack<std::pair<size_t, size_t>> levels_boundaries_pointers_;

  /*
          This queue stores boundaries of each object/array and number of objects/array inside it in a specific level.

          Entities can be object or array.

          For example:

                  0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  --> indices
                  | | | | | | | | | | |  |  |  |  |  |  |
                  [ [ [ 1 , 4 ] , 4 ] ,  [  1  ,  4  ]  ]   --> this is short json example
                    |               |
                    |---------------|--> this object has boundaries {1,9} and containes only 1 object

      */
  std::stack<std::queue<std::pair<size_t, size_t>>> levels_entities_boundaries_pointers_;

  /*
          Stores offsets needed to pass every object inside a specific level.

          Entities can be object or array of objects.

          For example:

                            9                  5           --> offset that we need to skip (those are offsets relative to short jsons
                                                               which means that for normal jsons they are larger and that's what
                                                               correct_pointers_positions() function about).
                    |---------------|    |-----------|
                    |               |    |           |
                  [ [ [ 1 , 4 ] , 4 ] ,  [  1  ,  4  ]  ]  --> this is short json example
                  |                                     |
                  |-------------------------------------|  --> this is our specific level

      */
  std::stack<std::queue<size_t>> levels_entities_offsets_;

  std::stack<size_t> levels_cumulative_offsets_;

 public:
  ShortToJSONConverter() = default;

  enum resultState { successState, errorState };

  resultState parse(std::string &data);

  resultState push_level();

  resultState pop_level();

  resultState accumulate_level();

  void accumulate_index();

  resultState map_short_json_indices();

  resultState accumulate_missing_indices();

  resultState read_key_value_pair(std::string &data);

  std::pair<bool, std::string> read(std::string data);
};
}  // namespace enigma

#endif
