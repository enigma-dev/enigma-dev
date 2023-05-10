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

bool ShortJSONConverter::parse_into_buffer(const std::string& data, std::string* buffer) {
  /*
    Holds the accumulated JSON of last popped level.
  */
  std::string current_level_json_;

  data_ = data;  // save our data to be accessible by all functions.

  if (data_.length() == 0) return false;

  while (1) {
    skip_spaces();

    if (pointer_ >= data_.length()) {
      /*
        If there are more levels available and no more tokens available, return syntax error, otherwise return success.
      */
      if (!levels_accumulators_.empty()) return false;
      *buffer = current_level_json_;
      return true;
    }

    switch (data_.at(pointer_)) {
      case '[': {
        /*
          Return error if a new level exists when all levels are done.
          For example: [1,[1,4]],[1,4]
        */
        if (next_token_[1] != shortJSONToken::invalid && levels_accumulators_.empty()) return false;

        if (!validate_next_token(shortJSONToken::leftSquareBracket)) return false;

        push_level();
        break;
      }
      case ']': {
        if (levels_accumulators_.empty()) return false;

        if (!validate_next_token(shortJSONToken::rightSquareBracket)) return false;

        /*
          Set the end boundary of current level.
        */
        levels_boundaries_pointers_.top().second = virtual_pointer_;

        /*
          Accumulates missing indicies if exists.

          Checks:

            - "levels_entities_boundaries_pointers_" contains boundaries of entities for each level, and it 
              must contain some entities to be fixed.

            - At the end of any level, this level must contain entities inside its boundaries because this
              level maybe considered an "object" inside level.

              For example:

                                    l       m       ---> this must be true ((x < l) && (m < y))
                        |-------|   |-------|
                        |       |   |       |
                        |       |   |       |
                      [ [ 1 , 4 ] , [ 1 , 4 ] , 4 ]
                      |                           |
                      |---------------------------|
                      x                           y

            - Current level must be an "object".

        */
        if (!(levels_entities_boundaries_pointers_.top().empty()) &&
            ((levels_boundaries_pointers_.top().first < levels_entities_boundaries_pointers_.top().front().first) &&
             (levels_boundaries_pointers_.top().second > levels_entities_boundaries_pointers_.top().front().second)) &&
            (levels_states_.top() == levelState::objectState)) {
          map_short_json_indices();
          accumulate_missing_keys();
        }

        /*
          If it's an array, wrap with array square brackets; else wrap with object parenthesis;
        */
        switch (levels_states_.top()) {
          case levelState::arrayOfObjectsState:
            current_level_json_ += '[';
            current_level_json_ += levels_accumulators_.top();
            current_level_json_ += ']';
            break;
          case levelState::objectState:
            current_level_json_ += '{';
            current_level_json_ += levels_accumulators_.top();
            current_level_json_ += '}';
            break;
          default:
            return false;
        }

        pop_level();

        if (levels_accumulators_.empty()) break;

        if (levels_states_.top() != levelState::objectState) levels_entities_offsets_.top().push(levels_cumulative_offsets_.top());
        size_t current_level_offset_{levels_cumulative_offsets_.top()};
        levels_cumulative_offsets_.pop();
        levels_cumulative_offsets_.top() += current_level_offset_;

        if (levels_states_.top() == levelState::objectState) {
          levels_cumulative_offsets_.top() += 4;
          accumulate_single_key();
        }
        levels_accumulators_.top() += current_level_json_;
        current_level_json_.clear();

        break;
      }
      case '\"':
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
      case 'f':
      case 'n': {
        if (levels_accumulators_.empty()) return false;  // all levels are done, what is this value?

        if (!validate_next_token(shortJSONToken::value)) return false;

        if (levels_states_.top() != levelState::objectState) {
          levels_states_.top() = levelState::objectState;  // then it's object
        }

        levels_cumulative_offsets_.top() += 4;
        if (!accumulate_value()) return false;
        break;
      }
      case ',': {
        if (levels_accumulators_.empty()) return false;  // all levels are done, what is this comma?

        if (!validate_next_token(shortJSONToken::comma)) return false;

        levels_accumulators_.top() += ',';
        levels_entities_keys_indices_.top()++;
        break;
      }
      default:
        return false;
    }
    pointer_++;
    virtual_pointer_++;
  }
}  // parse_into_buffer

/********************************************
*
*    Error reporting system.
*
*    TODO: implement this system.
*
*********************************************/

std::string ShortJSONConverter::format_human_readable_error_message(std::string& message) { return ""; }

/********************************************
*
*    Main system.
*
*********************************************/

void ShortJSONConverter::push_level() {
  levels_accumulators_.push("");
  levels_states_.push(levelState::arrayOfObjectsState);  // Assume worst case which is "array of objects".

  levels_entities_keys_indices_.push(0);

  levels_boundaries_pointers_.push({virtual_pointer_, virtual_pointer_});

  std::queue<std::pair<size_t, size_t>> level_entities_boundaries_pointers_;
  levels_entities_boundaries_pointers_.push(level_entities_boundaries_pointers_);

  std::queue<size_t> level_entities_offsets_;
  levels_entities_offsets_.push(level_entities_offsets_);

  levels_cumulative_offsets_.push(0);
}  // push_level

void ShortJSONConverter::pop_level() {
  levels_accumulators_.pop();
  levels_states_.pop();

  /*
    Pop "levels_entities_boundaries_pointers_" and "levels_entities_offsets_" current level, 
    so that current level boundaries are added to previous level entities as well as offsets.

    For example:

                1       5   7       11
                |       |   |       |
              [ [ 1 , 4 ] , [ 1 , 4 ] , 4 ]  ---> this level entities boundaries should be {{1 , 5 } , { 7 , 11}}.
  */
  levels_entities_boundaries_pointers_.pop();
  levels_entities_offsets_.pop();

  /*
    As long as it's an array, keep pushing because it maybe an "object" eventually.
    
    If it's an "object", then no need to continue pushing.
  */
  if (!(levels_entities_boundaries_pointers_.empty()) && !(levels_states_.empty())) {
    if (levels_states_.top() != levelState::objectState)
      levels_entities_boundaries_pointers_.top().push({levels_boundaries_pointers_.top()});
  }

  levels_boundaries_pointers_.pop();

  levels_entities_keys_indices_.pop();
}  // pop_level

void ShortJSONConverter::accumulate_single_key() {
  levels_accumulators_.top() += '\"';
  levels_accumulators_.top() += std::to_string(levels_entities_keys_indices_.top());
  levels_accumulators_.top() += '\"';
  levels_accumulators_.top() += ':';
}  // accumulate_single_key

bool ShortJSONConverter::accumulate_value() {
  size_t temp_pointer_{pointer_};

  switch (data_.at(temp_pointer_)) {
    case '\"': {
      if (!accumulate_string_value()) return false;
      break;
    }
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
    case '-': {
      if (!accumulate_number_value()) return false;
      break;
    }
    case 't':
    case 'f': {
      if (!accumulate_boolean_value()) return false;
      break;
    }
    case 'n': {
      if (!accumulate_null_value()) return false;
      break;
    }
    default:
      return false;
  }

  return true;
}  // accumulate_value

bool ShortJSONConverter::accumulate_string_value() {
  size_t temp_pointer_{pointer_};

  temp_pointer_++;
  while (1) {
    if (temp_pointer_ >= data_.length()) break;
    switch (data_.at(temp_pointer_)) {
      case '\"': {
        temp_pointer_++;  // Advance the pointer again to set it after the end double quote.

        if (temp_pointer_ >= data_.length()) return false;

        accumulate_single_key();
        levels_accumulators_.top() += data_.substr(pointer_, temp_pointer_ - pointer_);
        virtual_pointer_ += temp_pointer_ - pointer_ - 1;
        pointer_ = temp_pointer_ - 1;
        return true;
      }
      default:
        break;
    }
    temp_pointer_++;
  }

  return false;
}  // accumulate_string_value

bool ShortJSONConverter::accumulate_number_value() {
  size_t temp_pointer_{pointer_};

  bool is_double_{false};

  if (data_.at(temp_pointer_) == '-') temp_pointer_++;  // Skip the negative sign if exists.

  while (1) {
    if (temp_pointer_ >= data_.length()) break;
    switch (data_.at(temp_pointer_)) {
      case ' ':
      case '\r':
      case '\t':
      case '\n':
      case ',':
      case ']': {
        accumulate_single_key();
        levels_accumulators_.top() += data_.substr(pointer_, temp_pointer_ - pointer_);
        virtual_pointer_ += temp_pointer_ - pointer_ - 1;
        pointer_ = temp_pointer_ - 1;
        return true;
      }
      case '.': {
        if (is_double_)
          return false;
        else
          is_double_ = true;
        break;
      }
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
        break;
      default:
        return false;
    }
    temp_pointer_++;
  }

  return false;
}  // accumulate_number_value

bool ShortJSONConverter::accumulate_boolean_value() {
  size_t temp_pointer_{pointer_};

  switch (data_.at(temp_pointer_)) {
    case 't':
      if (temp_pointer_ + 3 >= data_.length()) return false;

      if (data_.at(temp_pointer_ + 1) != 'r' || data_.at(temp_pointer_ + 2) != 'u' ||
          data_.at(temp_pointer_ + 3) != 'e')
        return false;

      temp_pointer_ += 4;  // "true" length is 4.

      break;
    case 'f':
      if (temp_pointer_ + 4 >= data_.length()) return false;

      if (data_.at(temp_pointer_ + 1) != 'a' || data_.at(temp_pointer_ + 2) != 'l' ||
          data_.at(temp_pointer_ + 3) != 's' || data_.at(temp_pointer_ + 4) != 'e')
        return false;

      temp_pointer_ += 5;  // "false" length is 5.

      break;
    default:
      return false;
  }

  if (temp_pointer_ >= data_.length()) return false;

  accumulate_single_key();
  levels_accumulators_.top() += data_.substr(pointer_, temp_pointer_ - pointer_);
  virtual_pointer_ += temp_pointer_ - pointer_ - 1;
  pointer_ = temp_pointer_ - 1;

  return true;
}  // accumulate_boolean_value

bool ShortJSONConverter::accumulate_null_value() {
  size_t temp_pointer_{pointer_};

  if (temp_pointer_ + 3 >= data_.length()) return false;

  if (data_.at(temp_pointer_ + 1) != 'u' || data_.at(temp_pointer_ + 2) != 'l' || data_.at(temp_pointer_ + 3) != 'l')
    return false;

  temp_pointer_ += 4;  // "null" length is 4.

  if (temp_pointer_ >= data_.length()) return false;

  accumulate_single_key();
  levels_accumulators_.top() += data_.substr(pointer_, temp_pointer_ - pointer_);
  virtual_pointer_ += temp_pointer_ - pointer_ - 1;
  pointer_ = temp_pointer_ - 1;

  return true;
}  // accumulate_null_value

void ShortJSONConverter::map_short_json_indices() {
  size_t number_of_entities_{levels_entities_boundaries_pointers_.top().size()};
  size_t shift_left_{levels_entities_boundaries_pointers_.top().front().first};

  size_t offset_{0};

  /*
    1. Map the front boundaries.
    2. Calculate the new offset, save it in "offset_", pop it from front.
    3. Push the mapped boundaries to the back of the queue, pop it from front.
    4. Repeat steps 1 - 3.
  */
  for (size_t i{0}; i < number_of_entities_; i++) {
    auto &top = levels_entities_boundaries_pointers_.top();
    top.front().first -= shift_left_;
    top.front().second -= shift_left_;

    top.front().first += offset_;
    top.front().second += offset_ + levels_entities_offsets_.top().front();

    offset_ += levels_entities_offsets_.top().front();

    levels_entities_offsets_.top().pop();

    top.push(levels_entities_boundaries_pointers_.top().front());
    top.pop();
  }
}  // map_short_json_indices

void ShortJSONConverter::accumulate_missing_keys() {
  size_t number_of_entities_{levels_entities_boundaries_pointers_.top().size()};
  size_t next_entity_index{0};
  std::string corrected_json_;

  size_t breakdown_index_{levels_entities_boundaries_pointers_.top().front().second + 1};

  /*
    1. Accumulate key/value pair in "corrected_json_".
    2. Pop "levels_entities_boundaries_pointers_.top()".
    3. Repeat 1 - 2.
  */
  for (size_t i{0}; i < number_of_entities_; i++) {
    corrected_json_ += '\"';
    corrected_json_ += std::to_string(next_entity_index);
    corrected_json_ += '\"';
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

  corrected_json_ += levels_accumulators_.top().substr(breakdown_index_);  // Rest of the string

  levels_accumulators_.top() = corrected_json_;
}  // accumulate_missing_keys

/********************************************
*
*    Short JSON syntax validation system.
*
*********************************************/

bool ShortJSONConverter::validate_next_token(shortJSONToken current_token) {
  if (next_token_[0] != current_token && next_token_[1] != current_token) return false;

  switch (current_token) {
    case shortJSONToken::comma:
      next_token_[0] = shortJSONToken::leftSquareBracket;
      next_token_[1] = shortJSONToken::value;
      break;
    case shortJSONToken::value:
      next_token_[0] = shortJSONToken::rightSquareBracket;
      next_token_[1] = shortJSONToken::comma;
      break;
    case shortJSONToken::rightSquareBracket:
      next_token_[0] = shortJSONToken::rightSquareBracket;
      next_token_[1] = shortJSONToken::comma;
      break;
    case shortJSONToken::leftSquareBracket:
      next_token_[0] = shortJSONToken::leftSquareBracket;
      next_token_[1] = shortJSONToken::value;
      break;
    default:
      return false;
  }

  return true;
}  // validate_next_token

/********************************************
*
*    Spaces removal system.
*
*********************************************/

void ShortJSONConverter::skip_spaces() {
  while (pointer_ < data_.length()) {
    char c = data_.at(pointer_);
    if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
      pointer_++;
    else
      break;
  }
}  // skip_spaces

}  // namespace enigma
