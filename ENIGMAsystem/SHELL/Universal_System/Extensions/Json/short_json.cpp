/**
 *  @file short_json.cpp
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
#include "json.h"
#include "short_json_converter.h"

namespace enigma_user {

variant short_json_decode(std::string data) {
  std::string buffer;
  enigma::ShortJSONConverter shortJSONConverter;
  bool success = shortJSONConverter.parse_into_buffer(std::string(data), &buffer);
  if (!success) {
    DEBUG_MESSAGE("Failed to read configuration", MESSAGE_TYPE::M_ERROR);
    return -1;
  }

  DEBUG_MESSAGE("Short JSON converted successfully.\n", MESSAGE_TYPE::M_INFO);

  return enigma_user::json_decode(buffer);
}

std::string short_json_encode(variant ds_map) {
  // TODO: implement reverse operation

  return enigma_user::json_encode(ds_map);
}
}  // namespace enigma_user
