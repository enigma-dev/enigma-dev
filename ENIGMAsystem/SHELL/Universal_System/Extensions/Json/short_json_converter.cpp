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

bool ShortJSONConverter::parse_into_buffer(std::string& data, std::string* buffer) {
  std::string current_level_json_;  // holds the result of each level after each level.

  data_ = data;  // save our data to be accessible by all functions.

  if (data_.length() == 0) return false;

  while (1) {
    skip_spaces();

    if (pointer_ >= data_.length()) {
      /*
                    If it's end of the string and we have more levels, return syntax error, otherwise return success.
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
        if (next_token_[1] != invalid && levels_accumulators_.empty()) return false;

        if (!validate_next_token(leftSquareBracket)) return false;

        push_level();
        break;
      }
      case ']': {
        if (levels_accumulators_.empty()) return false;

        if (!validate_next_token(rightSquareBracket)) return false;

        /*
                                          For each level we pushed 0 for its end but now we know the index of that end so we set it.
                                      */
        levels_boundaries_pointers_.top().second = virtual_pointer_;

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

        /*
                                        If it's an array, wrap with array square brackets; else wrap with object parenthesis;
                                    */
        switch (levels_states_.top()) {
          case arrayOfObjectsState:
            current_level_json_ += '[';
            current_level_json_ += levels_accumulators_.top();
            current_level_json_ += ']';
            break;
          case objectState:
            current_level_json_ += '{';
            current_level_json_ += levels_accumulators_.top();
            current_level_json_ += '}';
            break;
          default:
            return false;
        }

        pop_level();

        /*
                    If it's not end of the string and we don't have more levels, don't accumulate, otherwise accumulate the saved json in `current_level_json_`.
                  */
        if (levels_accumulators_.empty()) break;

        if (levels_states_.top() != objectState) levels_entities_offsets_.top().push(levels_cumulative_offsets_.top());
        size_t current_level_offset_{levels_cumulative_offsets_.top()};
        levels_cumulative_offsets_.pop();
        levels_cumulative_offsets_.top() += current_level_offset_;

        if (levels_states_.top() == objectState) {
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
      case 'f': {
        if (levels_accumulators_.empty()) return false;  // all levels are done, what is this value?

        if (!validate_next_token(value)) return false;

        if (levels_states_.top() != objectState) {
          levels_states_.top() = objectState;  // then it's object
        }

        levels_cumulative_offsets_.top() += 4;
        if (!accumulate_value()) return false;
        break;
      }
      case ',': {
        if (levels_accumulators_.empty()) return false;  // all levels are done, what is this comma?

        if (!validate_next_token(comma)) return false;

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
*********************************************/

std::string ShortJSONConverter::format_human_readable_error_message(std::string& message) { return ""; }

/********************************************
*
*    Main system.
*
*********************************************/

void ShortJSONConverter::push_level() {
  levels_accumulators_.push("");
  levels_states_.push(arrayOfObjectsState);  // We assume worst case which is "array of objects".

  levels_entities_keys_indices_.push(0);

  /*
              Push this new level start index.
              Note that we have no idea where the end of the new level is so we push start index in both start and end indices.
            */
  levels_boundaries_pointers_.push({virtual_pointer_, virtual_pointer_});

  std::queue<std::pair<size_t, size_t>> level_entities_boundaries_pointers_;
  levels_entities_boundaries_pointers_.push(level_entities_boundaries_pointers_);

  std::queue<size_t> level_entities_offsets_;
  levels_entities_offsets_.push(level_entities_offsets_);

  levels_cumulative_offsets_.push(0);
}  // push_level

void ShortJSONConverter::pop_level() {
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

  levels_entities_keys_indices_.pop();
}  // pop_level

void ShortJSONConverter::accumulate_single_key() {
  levels_accumulators_.top() += '\"';
  levels_accumulators_.top() += std::to_string(levels_entities_keys_indices_.top());
  levels_accumulators_.top() += '\"';
  levels_accumulators_.top() += ':';
}  // accumulate_single_key

bool ShortJSONConverter::accumulate_value() {
  size_t second_pointer_{pointer_};

  switch (data_.at(second_pointer_)) {
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
    default:
      return false;
  }

  return true;
}  // accumulate_value

bool ShortJSONConverter::accumulate_string_value() {
  size_t second_pointer_{pointer_};

  second_pointer_++;
  while (1) {
    if (second_pointer_ >= data_.length()) break;
    switch (data_.at(second_pointer_)) {
      case '\"': {
        second_pointer_++;  // advance the pointer again to set it after the close double quote.

        if (second_pointer_ >= data_.length()) return false;

        accumulate_single_key();
        levels_accumulators_.top() += data_.substr(pointer_, second_pointer_ - pointer_);
        virtual_pointer_ += second_pointer_ - pointer_ - 1;
        pointer_ += second_pointer_ - pointer_ - 1;
        return true;
      }
      default:
        break;
    }
    second_pointer_++;
  }

  return false;
}  // accumulate_string_value

bool ShortJSONConverter::accumulate_number_value() {
  size_t second_pointer_{pointer_};

  bool is_double_{false};

  if (data_.at(second_pointer_) == '-') second_pointer_++;  // skip the negative sign if exists.

  while (1) {
    if (second_pointer_ >= data_.length()) break;
    switch (data_.at(second_pointer_)) {
      case ' ':
      case '\r':
      case '\t':
      case '\n':
      case ',':
      case ']': {
        accumulate_single_key();
        levels_accumulators_.top() += data_.substr(pointer_, second_pointer_ - pointer_);
        virtual_pointer_ += second_pointer_ - pointer_ - 1;
        pointer_ += second_pointer_ - pointer_ - 1;
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
    second_pointer_++;
  }

  return false;
}  // accumulate_number_value

bool ShortJSONConverter::accumulate_boolean_value() {
  size_t second_pointer_{pointer_};

  switch (data_.at(second_pointer_)) {
    case 't':
      second_pointer_ += 4;  // true length is 4.
      if (second_pointer_ >= data_.length()) return false;
      if (data_.substr(pointer_, second_pointer_ - pointer_) != "true") return false;
      break;
    case 'f':
      second_pointer_ += 5;  // false length is 5.
      if (second_pointer_ >= data_.length()) return false;
      if (data_.substr(pointer_, second_pointer_ - pointer_) != "false") return false;
      break;
    default:
      return false;
  }

  accumulate_single_key();
  levels_accumulators_.top() += data_.substr(pointer_, second_pointer_ - pointer_);
  virtual_pointer_ += second_pointer_ - pointer_ - 1;
  pointer_ += second_pointer_ - pointer_ - 1;

  return true;
}  // accumulate_boolean_value

/********************************************
*
*    The next part is for increasing effeciency of the algorithm.
*
*********************************************/

void ShortJSONConverter::map_short_json_indices() {
  size_t number_of_entities_{levels_entities_boundaries_pointers_.top().size()};
  size_t shift_left_{levels_entities_boundaries_pointers_.top().front().first};

  size_t offset_{0};

  /*
                          This loop loops number of entities in current level times

                          levels_entities_keys_indices_.top() + 1
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

  accumulate_missing_keys();
}  // map_short_json_indices

void ShortJSONConverter::accumulate_missing_keys() {
  size_t number_of_entities_{levels_entities_boundaries_pointers_.top().size()};
  size_t next_entity_index{0};
  std::string corrected_json_;

  size_t breakdown_index_{levels_entities_boundaries_pointers_.top().front().second + 1};

  /*
                          This loop loops number of entities in current level times

                          levels_entities_keys_indices_.top() + 1
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

  // Rest of the string
  corrected_json_ += levels_accumulators_.top().substr(breakdown_index_);

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
    case comma:
      next_token_[0] = leftSquareBracket;
      next_token_[1] = value;
      break;
    case value:
      next_token_[0] = rightSquareBracket;
      next_token_[1] = comma;
      break;
    case rightSquareBracket:
      next_token_[0] = rightSquareBracket;
      next_token_[1] = comma;
      break;
    case leftSquareBracket:
      next_token_[0] = leftSquareBracket;
      next_token_[1] = value;
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
