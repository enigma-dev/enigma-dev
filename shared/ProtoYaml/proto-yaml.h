/** Copyright (C) 2019 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <yaml-cpp/yaml.h>
#include <google/protobuf/message.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <type_traits>
#include <map>
#include <set>

namespace egm {

bool DecodeYaml(const YAML::Node &yaml, google::protobuf::Message *out);

template<typename Proto, typename std::enable_if<
    std::is_base_of<google::protobuf::Message, Proto>::value, int>::type = 0>
Proto ReadYamlAs(std::istream &file) {
  Proto result;
  DecodeYaml(YAML::Load(file), &result);
  return result;
}

template<typename Proto, typename std::enable_if<
    std::is_base_of<google::protobuf::Message, Proto>::value, int>::type = 0>
Proto ReadYamlFileAs(const std::string &file) {
  Proto result;
  DecodeYaml(YAML::LoadFile(file), &result);
  return result;
}

} //namespace egm
