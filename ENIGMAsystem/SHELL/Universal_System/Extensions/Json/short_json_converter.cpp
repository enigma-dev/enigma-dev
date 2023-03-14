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

class ShortJSON {

private: 
    enum ShortJSONState {
        arrayState,
        objectState,
        valueState,
        errorState
    };
    std::stack<std::pair<ShortJSONState, int>> shortStack;
    int pointer_ = -1;
    int old_pointer_ = -1;
    int object_index = 0;
    int value_index = 0;
    std::string json;


public: 

    ShortJSON();

    std::string read(std::string data) {
        // remove all spaces and new lines inside data
        data.erase(std::remove_if(data.begin(), data.end(), isspace), data.end());

        auto first_state = readArray(data);

        switch (first_state) {
        case objectState:
            first_state = readObject(data);
            break;
        case arrayState:
            break;
        }

        return json;
    }

    ShortJSONState readArray(std::string data) {
        
        pointer_++;

        switch (data.at(pointer_))
        {
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
            return objectState;
        case '[':
        {
            int u = pointer_;

            shortStack.push({ arrayState, pointer_ });
            auto state = readArray(data);
            switch (state) {
            case arrayState:
            {
                auto temp_state = readArray(data);

                return temp_state;
            }
            case objectState:
            {
                old_pointer_ = pointer_;
                pointer_ = shortStack.top().second;
                shortStack.pop();
                auto temp_state = readObject(data);


                return temp_state;
            }
            default:

                return errorState;
            }
        }
        default:
            return errorState;
        }
    }

    ShortJSONState readObject(std::string data) {
        

        if (data.at(pointer_) == ',') pointer_++;

        int temp_pointer;

        std::vector<std::string> object_json;

        if (json != "") object_json.emplace_back(json);

        switch (data.at(pointer_))
        {
        case '[':
        {
            shortStack.push({ objectState, pointer_ });
            if (old_pointer_ != -1) pointer_ = old_pointer_;

            int u = pointer_;

            while (1) {
                auto state = readValue(data);
                object_json.emplace_back(json);
                if (pointer_ < (data.length()) && data.at(pointer_) == ',') pointer_++;
                if (pointer_ < (data.length()) && data.at(pointer_) == ']') {
                    pointer_++;
                    break;
                }
            }

            auto state = objectToJSON(object_json);

            shortStack.pop();

            return shortStack.top().first;
        }

        default:
            return errorState;
        }
    }

    ShortJSONState readValue(std::string data) {

        if (data.at(pointer_) == ',') pointer_++;

        int temp_pointer;

        std::string value_json;

        switch (data.at(pointer_))
        {
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
        {
            shortStack.push({ valueState, pointer_ });
            temp_pointer = pointer_;
            while (((temp_pointer + 1) < (data.length())) && ((data.at(temp_pointer + 1)) != ',') && ((data.at(temp_pointer + 1) != ']'))) temp_pointer++;
            if (temp_pointer >= (data.length())) return errorState;
            value_json.append("\"");
            value_json.append(std::to_string(object_index));
            value_json.append("\"");
            value_json.append(":");
            value_json.append(data.substr(pointer_, temp_pointer - pointer_ + 1));
            object_index++;
            pointer_ += temp_pointer - pointer_ + 1;

            shortStack.pop();

            json = value_json;
            return valueState;
        }
        default:
            return errorState;
        }
    }

    ShortJSONState objectToJSON(std::vector<std::string> object_json) {
        std::string temp_object_json;

        temp_object_json += '{';

        for (int i{ 0 }; i < object_json.size(); i++) {
            temp_object_json.append(object_json.at(i));
            if (i != (object_json.size() - 1)) temp_object_json += ',';
        }

        temp_object_json += '}';

        json = temp_object_json;

        return objectState;
    }

};
