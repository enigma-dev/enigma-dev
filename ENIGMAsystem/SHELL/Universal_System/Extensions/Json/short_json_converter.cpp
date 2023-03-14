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


class ShortJSONReader {
 private:
  enum ShortJSONState { arrayState, objectState, valueState, successState, errorState };
  int pointer_ = -1;
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
          break;
        case ']':
          indices.pop();
          currentState = popLevel();
          switch (currentState) {
            case errorState:
              return errorState;
            default:
              break;
          }
          break;
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
          break;
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
    int temp_pointer;

    std::string value_json;

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
        temp_pointer = pointer_;
        while (((temp_pointer + 1) < ((int)(data.length()))) && ((data.at(temp_pointer + 1)) != ',') &&
               ((data.at(temp_pointer + 1) != ']')))
          temp_pointer++;
        if (temp_pointer >= ((int)(data.length()))) return errorState;
        add_index();
        levels.top().first.append(data.substr(pointer_, temp_pointer - pointer_ + 1));
        pointer_ += temp_pointer - pointer_;
        return valueState;
      }
      default:
        return errorState;
    }
  }  // read_value()

  ShortJSONState array_to_json(std::string data) {
    std::string array;
    array += '[';
    array.append(data);
    array += ']';
    levels.pop();
    add_index();
    levels.top().first.append(array);
    return arrayState;
  }  // array_to_json()

  ShortJSONState object_to_json(std::string data) {
    std::string object;
    object += '{';
    object.append(data);
    object += '}';
    levels.pop();
    add_index();
    levels.top().first.append(object);
    return objectState;
  }  // object_to_json()

  void add_index() {
    if (!(indices.empty())) {
      levels.top().first += '"';
      levels.top().first += std::to_string(indices.top());
      levels.top().first += '"';
      levels.top().first += ':';
    }
  }  // add_index()

 public:
  ShortJSONReader();

  bool read(std::string data, std::string *json) {
    // This is pre-first level which will contains full json.
    // Note that, true and false here has no meaning because we only interseted in the string (see return statement).
    levels.push({"", true});

    currentState = pushLevel(data);

    switch (currentState) {
      case errorState:
        return false;
      default:
        *json = levels.top().first;
        return true;
    }
  }  // read()
};
