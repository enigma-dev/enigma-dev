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

#include "short_json_converter.h"

namespace enigma {
ShortJSONConverter::ShortJSONConverter() = default;
ShortJSONConverter::~ShortJSONConverter() = default;

ShortJSONConverter::resultState ShortJSONConverter::push_level() {
  levels_accumulators_.push("");
  levels_states_.push(arrayOfObjectsState);  // We assume worst case which is "array of objects".

  levels_entities_indices_.push(0);

  /*
          Push this new level start index.
          Note that we have no idea where the end of the new level is so we push start index in both start and end indices.
        */
  levels_boundaries_pointers_.push({pointer_, pointer_});

  std::queue<std::pair<size_t, size_t>> level_entities_boundaries_pointers_;
  levels_entities_boundaries_pointers_.push(level_entities_boundaries_pointers_);

  std::queue<size_t> level_entities_offsets_;
  levels_entities_offsets_.push(level_entities_offsets_);

  levels_cumulative_offsets_.push(0);

  return successState;
}  // push_level

ShortJSONConverter::resultState ShortJSONConverter::pop_level() { return accumulate_level(); }  // pop_level

ShortJSONConverter::resultState ShortJSONConverter::accumulate_level() {
  /*
                      We pop our accumulator level and stack level.
                  */
  levels_accumulators_.pop();
  levels_states_.pop();

  /*
                      We pop current level, so that we add current level boundaries to previous level entities as well as offsets.

                      For example:

                                  1       5   7       11
                                  |       |   |       |
                                [ [ 1 , 4 ] , [ 1 , 4 ] , 4 ]  ---> this level entities boundaries should be {{1 , 5 } , { 7 , 11}}.
                  */
  levels_entities_boundaries_pointers_.pop();
  levels_entities_offsets_.pop();

  /*
                      as long as it's an array we keep pushing because it maybe an object eventually
                      if it's object, then no need to continue pushing
                  */
  if (!(levels_entities_boundaries_pointers_.empty()) && !(levels_states_.empty())) {
    if (levels_states_.top() != objectState)
      levels_entities_boundaries_pointers_.top().push({levels_boundaries_pointers_.top()});
  }

  levels_boundaries_pointers_.pop();

  levels_entities_indices_.pop();

  return successState;
}  // accumulate_level

void ShortJSONConverter::accumulate_index() {
  levels_accumulators_.top() += '"';
  levels_accumulators_.top() += std::to_string(levels_entities_indices_.top());
  levels_accumulators_.top() += '"';
  levels_accumulators_.top() += ':';
}  // accumulate_index

ShortJSONConverter::resultState ShortJSONConverter::map_short_json_indices() {
  size_t number_of_entities_{levels_entities_boundaries_pointers_.top().size()};
  size_t shift_left_{levels_entities_boundaries_pointers_.top().front().first};

  size_t offset_{0};

  /*
                      This loop loops number of entities in current level times

                      levels_entities_indices_.top() + 1
                  */
  for (size_t i{0}; i < number_of_entities_; i++) {
    levels_entities_boundaries_pointers_.top().front().first -= shift_left_;
    levels_entities_boundaries_pointers_.top().front().second -= shift_left_;

    levels_entities_boundaries_pointers_.top().front().first += offset_;
    levels_entities_boundaries_pointers_.top().front().second += offset_ + levels_entities_offsets_.top().front();

    offset_ += levels_entities_offsets_.top().front();

    levels_entities_offsets_.top().pop();

    levels_entities_boundaries_pointers_.top().push(levels_entities_boundaries_pointers_.top().front());
    levels_entities_boundaries_pointers_.top().pop();
  }

  return accumulate_missing_indices();
}  // map_short_json_indices

ShortJSONConverter::resultState ShortJSONConverter::accumulate_missing_indices() {
  size_t number_of_entities_{levels_entities_boundaries_pointers_.top().size()};
  size_t next_entity_index{0};
  std::string corrected_json_;

  size_t breakdown_index_{levels_entities_boundaries_pointers_.top().front().second + 1};

  /*
                      This loop loops number of entities in current level times

                      levels_entities_indices_.top() + 1
                  */
  for (size_t i{0}; i < number_of_entities_; i++) {
    corrected_json_ += '"';
    corrected_json_ += std::to_string(next_entity_index);
    corrected_json_ += '"';
    corrected_json_ += ':';
    corrected_json_ +=
        levels_accumulators_.top().substr(levels_entities_boundaries_pointers_.top().front().first,
                                          levels_entities_boundaries_pointers_.top().front().second -
                                              levels_entities_boundaries_pointers_.top().front().first + 1);

    levels_cumulative_offsets_.top() += 4;

    levels_entities_boundaries_pointers_.top().pop();

    next_entity_index++;

    if (i != number_of_entities_ - 1) {
      breakdown_index_ = levels_entities_boundaries_pointers_.top().front().second + 1;
      corrected_json_ += ',';
    }
  }

  // Rest of the string
  corrected_json_ += levels_accumulators_.top().substr(breakdown_index_);

  levels_accumulators_.top() = corrected_json_;

  return successState;
}  // accumulate_missing_indices

ShortJSONConverter::resultState ShortJSONConverter::read_key_value_pair(std::string& data) {
  size_t new_pointer_{pointer_};

  while (1) {
    if (new_pointer_ >= data.length()) return errorState;
    switch (data.at(new_pointer_)) {
      case ',':
      case ']':
        accumulate_index();
        levels_accumulators_.top() += data.substr(pointer_, new_pointer_ - pointer_);
        pointer_ += new_pointer_ - pointer_ - 1;
        return successState;
      default:
        break;
    }
    new_pointer_++;
  }
}  // read_key_value_pair

// void ShortJSONConverter::skip_spaces(std::string &data) {
//   while (pointer_ < data.length())
//   {
//     char c = data.at(pointer_);
//     if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
//       pointer_++;
//     else
//       break;
//   }
// }

bool ShortJSONConverter::parse_into_buffer(std::string& data, std::string* buffer) {
  if (data.length() == 0) return false;

  while (1) {
    if (pointer_ >= data.length()) return true;

    // skip_spaces(data);
    switch (data.at(pointer_)) {
      case '[': {
        if (next_token_[0] != leftSquareBracket && next_token_[1] != leftSquareBracket) return false;
        next_token_[0] = leftSquareBracket;
        next_token_[1] = value;
        if (push_level() == errorState) return false;
        break;
      }
      case ']': {
        if ((next_token_[0] != rightSquareBracket && next_token_[1] != rightSquareBracket) ||
            levels_accumulators_.empty())
          return false;
        next_token_[0] = rightSquareBracket;
        next_token_[1] = comma;

        /*
                                  For each level we pushed 0 for its end but now we know the index of that end so we set it.
                              */
        levels_boundaries_pointers_.top().second = pointer_;

        /*
                                  This snippet fixes current level if we found that it's an object.

                                  Checks:

                                   - Our code assumes that: current level is an array until we find key/value pair which means that:
                                     current level is an object, so we start add indices to each key/value pair, objects, and arrays
                                     when we find our first key/value pair in current level which means that we may have entities without indices before our
                                     first key/value pair which also means that we need to fix them. Our first key/value pair maybe preceding objects or arrays so
                                     if there's no objects/arrays before our first key/value pair, then nothing need to be fixed. Our "levels_entities_boundaries_pointers_"
                                     contains boundaries of entities for each level so for current level we must make sure that it contains some entities to be fixed.

                                   - When we get out of a specific level, we must make sure that this level contains entities inside its boundaries because this
                                     level maybe considered an object inside level.

                                     For example:

                                                            l       m       ---> we must make sure that ((x < l) && (m < y))
                                                |-------|   |-------|
                                                |       |   |       |
                                                |       |   |       |
                                              [ [ 1 , 4 ] , [ 1 , 4 ] , 4 ]  --> this is short json example
                                              |                           |
                                              |---------------------------|  --> this is our specific level
                                              x                           y

                                   - Array of objects doesn't need any kind of indices for its objects so we must make sure that our level is an object.

                              */
        if (!(levels_entities_boundaries_pointers_.top().empty()) &&
            ((levels_boundaries_pointers_.top().first < levels_entities_boundaries_pointers_.top().front().first) &&
             (levels_boundaries_pointers_.top().second > levels_entities_boundaries_pointers_.top().front().second)) &&
            (levels_states_.top() == objectState)) {
          map_short_json_indices();
        }

        std::string level_json_;

        /*
                                If it's an array, wrap with array square brackets; else wrap with object parenthesis;
                            */
        if (levels_states_.top() == arrayOfObjectsState) {
          level_json_ += '[';
          if (!(levels_accumulators_.empty())) level_json_ += levels_accumulators_.top();
          level_json_ += ']';
        } else {
          level_json_ += '{';
          if (!(levels_accumulators_.empty())) level_json_ += levels_accumulators_.top();
          level_json_ += '}';
        }

        if (pop_level() == errorState) return false;

        if (pointer_ == data.length() - 1) {
          if (!levels_accumulators_.empty()) return false;
          *buffer = level_json_;
          break;
        }

        if (levels_accumulators_.empty()) return false;

        if (levels_states_.top() != objectState) levels_entities_offsets_.top().push(levels_cumulative_offsets_.top());
        size_t current_level_offset_{levels_cumulative_offsets_.top()};
        levels_cumulative_offsets_.pop();
        levels_cumulative_offsets_.top() += current_level_offset_;

        if (levels_states_.top() == objectState) {
          levels_cumulative_offsets_.top() += 4;
          accumulate_index();
        }
        levels_accumulators_.top() += level_json_;

        break;
      }
      case '"':
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '-':
      case 't':
      case 'f': {
        if (next_token_[0] != value && next_token_[1] != value) return false;
        next_token_[0] = rightSquareBracket;
        next_token_[1] = comma;

        if (levels_states_.top() != objectState) {
          levels_states_.top() = objectState;  // then it's object
        }
        levels_cumulative_offsets_.top() += 4;
        if (read_key_value_pair(data) == errorState) return false;
        break;
      }
      case ',': {
        if (next_token_[0] != comma && next_token_[1] != comma) return false;
        next_token_[0] = leftSquareBracket;
        next_token_[1] = value;

        levels_accumulators_.top() += ',';
        levels_entities_indices_.top()++;
        break;
      }
      default:
        return false;
    }
    pointer_++;
  }
}  // parse_into_buffer

}  // namespace enigma
