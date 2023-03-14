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

#include "short_json.h"

namespace enigma {
class ShortJSONReader {
 private:
  enum ShortJSONState { arrayState, objectState, valueState, successState, errorState };
  int pointer_{-1};
  ShortJSONState currentState;
  std::stack<std::pair<std::string, bool>> levels;
  std::stack<int> indices;

  ShortJSONState pushLevel(std::string data) {
    while (1) {
      pointer_++;
      if (!(pointer_ < ((int)(data.length())))) return successState;
      switch (data.at(pointer_)) {
        case '[':
          levels.push({"", true});  // we assume worst case which is array of objects until otherwise
          indices.push(0);
          currentState = pushLevel(data);
          switch (currentState) {
            case errorState:
              return errorState;
            default:
              break;
          }
          continue;
        case ']':
          indices.pop();
          currentState = popLevel();
          switch (currentState) {
            case errorState:
              return errorState;
            default:
              break;
          }
          continue;
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
        case 'f':
          levels.top().second = false;  // then it's object
          currentState = read_value(data);
          switch (currentState) {
            case errorState:
              return errorState;
            default:
              break;
          }
          continue;
        case ',':
          levels.top().first += ',';
          indices.top()++;
          continue;
        default:
          return errorState;
      }
    }
  }  // pushLevel

  ShortJSONState popLevel() {
    if (levels.top().second)
      return array_to_json(levels.top().first);
    else
      return object_to_json(levels.top().first);
  }  // popLevel()

  ShortJSONState read_value(std::string data) {
    switch (data.at(pointer_)) {
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
        int temp_pointer_{pointer_};
        while (((temp_pointer_ + 1) < ((int)(data.length()))) && ((data.at(temp_pointer_ + 1)) != ',') &&
               ((data.at(temp_pointer_ + 1) != ']')))
          temp_pointer_++;
        if (!(temp_pointer_ < ((int)(data.length())))) return errorState;
        add_index();
        levels.top().first.append(data.substr(pointer_, temp_pointer_ - pointer_ + 1));
        pointer_ += temp_pointer_ - pointer_;
        return valueState;
      }
      default:
        return errorState;
    }
  }  // read_value()

  ShortJSONState array_to_json(std::string data) {
    std::string json_array_{"["};
    json_array_.append(data);
    json_array_ += ']';
    levels.pop();
    add_index();
    levels.top().first.append(json_array_);
    return arrayState;
  }  // array_to_json()

  ShortJSONState object_to_json(std::string data) {
    std::string json_object_{"{"};
    json_object_.append(data);
    json_object_ += '}';
    levels.pop();
    add_index();
    levels.top().first.append(json_object_);
    add_index_if_not_exist();  // iterate over last object and fix it
    return objectState;
  }  // object_to_json()

  void add_index() {
    if ((!(indices.empty())) && (!(levels.top().second))) {
      levels.top().first += '"';
      levels.top().first += std::to_string(indices.top());
      levels.top().first += '"';
      levels.top().first += ':';
    }
  }  // add_index()

  ShortJSONState add_index_if_not_exist() {
    int json_length_{(int)(levels.top().first.length())}, next_object_index{0};
    std::string defective_json_{levels.top().first}, perfect_json_{"{"};
    int temp_pointer_ = 1;  // ignore the first parenthesis because it's already added in the initialization step
    while (1) {
      switch (defective_json_.at(temp_pointer_)) {
        case '{': {
          perfect_json_ += '"';
          perfect_json_ += std::to_string(next_object_index);
          perfect_json_ += '"';
          perfect_json_ += ':';

          int temp_temp_pointer_ = temp_pointer_;

          // this stack for skipping the current object/array
          std::stack<char> temp_stack_;

          while (1) {
            if (defective_json_.at(temp_temp_pointer_) == '{')
              temp_stack_.push('{');
            else if (defective_json_.at(temp_temp_pointer_) == '}')
              temp_stack_.pop();

            temp_temp_pointer_++;

            // when stack is empty this means parenthesis has been balanced
            if (!(temp_temp_pointer_ < (json_length_ - 1)) || temp_stack_.empty()) break;
          }

          perfect_json_.append(defective_json_.substr(temp_pointer_, temp_temp_pointer_ - temp_pointer_));
          next_object_index++;
          temp_pointer_ = temp_temp_pointer_;
        }
          continue;
        case '[': {
          perfect_json_ += '"';
          perfect_json_ += std::to_string(next_object_index);
          perfect_json_ += '"';
          perfect_json_ += ':';

          int temp_temp_pointer_ = temp_pointer_;

          // this stack for skipping the current object/array
          std::stack<char> temp_stack_;

          while (1) {
            if (defective_json_.at(temp_temp_pointer_) == '[')
              temp_stack_.push('[');
            else if (defective_json_.at(temp_temp_pointer_) == ']')
              temp_stack_.pop();

            temp_temp_pointer_++;

            // when stack is empty this means parenthesis has been balanced
            if (!(temp_temp_pointer_ < (json_length_ - 1)) || temp_stack_.empty()) break;
          }

          perfect_json_.append(defective_json_.substr(temp_pointer_, temp_temp_pointer_ - temp_pointer_));
          next_object_index++;
          temp_pointer_ = temp_temp_pointer_;
        }
          continue;
        case ',':
          perfect_json_ += ',';
          temp_pointer_++;
          continue;
        case '"':
          // finding this double quote means that, we found the first added index so no need to continue adding indices
          if ((perfect_json_.length()) >
              1) {  // correctedJSON var has default length equal to 1, that's because the open parenthesis we added
            perfect_json_.append(
                defective_json_.substr(temp_pointer_));  // the rest of the string including the last parenthesis
            bool temp_bool_ = levels.top().second;       // we save our level state
            levels.pop();                                // remove wrong top and push the right one
            levels.push({perfect_json_, temp_bool_});
          }
          return successState;
        default:
          return errorState;
      }
    }
  }  // add_index_if_not_exist()

 public:
  ShortJSONReader() = default;

  std::pair<bool,std::string> read(std::string data) {
    // This is pre-first level which will contains full json.
    // Note that, true and false here has no meaning because we only interseted in the string (see return statement).
    levels.push({"", true});

    currentState = pushLevel(data);

    switch (currentState) {
      case errorState:
        return {false, ""};
      default:
        return {true, levels.top().first};
    }
  }  // read()
};
}  // namespace enigma
