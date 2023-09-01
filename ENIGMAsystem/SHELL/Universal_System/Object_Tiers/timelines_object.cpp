/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2011-2012 polygone
*** Copyright (C) 2014 Robert B Colton, canthelp
*** Copyright (C) 2014 Seth N. Hetu
*** Copyright (C) 2023 Fares Atef
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

/**
  @file  timelines_object.cpp
  @brief Adds a timelines tier following the planar tier.
*/

#include "timelines_object.h"

#include "../Serialization/serialization.h"
#include "Widget_Systems/widgets_mandatory.h"

namespace enigma
{
  std::vector< std::map<int, int> > object_timelines::timeline_moments_maps;

  object_timelines::object_timelines() {}
  object_timelines::object_timelines(unsigned _x, int _y): object_planar(_x,_y) {}

  //This just needs implemented virtually so instance_destroy works.
  object_timelines::~object_timelines() {}

  void object_timelines::advance_curr_timeline() 
  {
    //Find the next instant (it may be right now).
    //Note: If a map lookup each tick is a performance hit, this value can be cached in the object itself with a tuple <timeline_id, timeline_pos, next_moment_iterator>
    if (timeline_index<0 || timeline_index>=(int)timeline_moments_maps.size()) { return; }

    //Algorithm varies for +/- speed. Assume zero is positive (just for consistency).
    //Note that we *cannot* call these events as they are found, because they might change timeline_position (and GM does not work that way).
    std::vector<int> moment_ids;
    if (timeline_speed>=0) { //Positive
      std::map<int, int>::const_iterator next = timeline_moments_maps[timeline_index].lower_bound(ceil(timeline_position));

      //We now advance the timeline_position by timeline_speed, saving event information on the way.
      while (timeline_speed!=0) {
        //Landing *exactly* on the next moment will actually trigger it *next* turn.
        if (next==timeline_moments_maps[timeline_index].end() || timeline_position+timeline_speed<=next->first) {
           timeline_position += timeline_speed;
           break;
        }
        timeline_speed -= next->first - timeline_position;
        timeline_position = next->first;
        moment_ids.push_back(next->second);
        next++;
      }
    } else { //Negative
      //This will put us one ahead. The reverse_iterator will reverse it.
      std::map<int, int>::const_iterator nextTEMP = timeline_moments_maps[timeline_index].upper_bound(floor(timeline_position));
      std::map<int, int>::const_reverse_iterator next = std::reverse_iterator<std::map<int, int>::const_iterator>(nextTEMP);

      //The algorithm proceeds as in FWD, but with rev_iterators (so it's hard to share code).
      while (timeline_speed!=0) {
        //Landing *exactly* on the next moment will actually trigger it *next* turn.
        if (next==timeline_moments_maps[timeline_index].rend() || timeline_position+timeline_speed>=next->first) {
           timeline_position += timeline_speed;
           break;
        }
        timeline_speed += timeline_position - next->first;
        timeline_position = next->first;
        moment_ids.push_back(next->second);
        next++;
      }
    }

    //Now, trigger each moment that we've passed.
    for (std::vector<int>::iterator it=moment_ids.begin(); it!=moment_ids.end(); it++) {
      timeline_call_moment_script(timeline_index, *it);
    }
  }

  void object_timelines::loop_curr_timeline() 
  {
    //Determine if we're past the last event. Note that no residual movement carries over; this effectively "resets to 0".
    if (timeline_index<0 || timeline_index>=(int)timeline_moments_maps.size()) { return; }
    if (timeline_speed>=0) { //Positive
      if (timeline_position > timeline_moments_maps[timeline_index].rbegin()->first) { //If ==, it will trigger on the next time tick.
        timeline_position = 0;
      }
    } else { //Negative
      if (timeline_position < 0) { //If ==, it will trigger on the next time tick.
        timeline_position = timeline_moments_maps[timeline_index].rbegin()->first;
      }
    }
  }

  std::vector<std::byte> object_timelines::serialize() const {
    auto bytes = object_planar::serialize();
    std::size_t len = 0;

    enigma::bytes_serialization::enigma_serialize<unsigned char>(object_timelines::objtype, len, bytes);
    enigma::bytes_serialization::enigma_serialize(timeline_moments_maps.size(), len, bytes);
    for (auto &map : timeline_moments_maps) {
      enigma::bytes_serialization::enigma_serialize(map.size(), len, bytes);
      for (auto &[key, value]: map) {
        enigma::bytes_serialization::enigma_serialize(key, len, bytes);
        enigma::bytes_serialization::enigma_serialize(value, len, bytes);
      }
    }

    enigma::bytes_serialization::enigma_serialize_many(len, bytes, timeline_index, timeline_running, timeline_speed,
                                   timeline_position, timeline_loop);

    bytes.shrink_to_fit();
    return bytes;
  }

  std::size_t object_timelines::deserialize_self(std::byte *iter) {
    auto len = object_planar::deserialize_self(iter);

    unsigned char type;
    enigma::bytes_serialization::enigma_deserialize(type, iter, len);
    if (type != object_timelines::objtype) {
      DEBUG_MESSAGE("object_timelines::deserialize_self: Object type '" + std::to_string(type) +
                        "' does not match expected: " + std::to_string(object_timelines::objtype),
                    MESSAGE_TYPE::M_FATAL_ERROR);
    }
    std::size_t timeline_maps_len{};
    enigma::bytes_serialization::enigma_deserialize(timeline_maps_len, iter, len);
    timeline_moments_maps.resize(timeline_maps_len);
    for (auto &map: timeline_moments_maps) {
      std::size_t map_len{};
      enigma::bytes_serialization::enigma_deserialize(map_len, iter, len);
      for (std::size_t i = 0; i < map_len; i++) {
        int key{};
        int value{};
        enigma::bytes_serialization::enigma_deserialize(key, iter, len);
        enigma::bytes_serialization::enigma_deserialize(value, iter, len);
        map[key] = value;
      }
    }

    enigma::bytes_serialization::enigma_deserialize_many(iter, len, timeline_index, timeline_running, timeline_speed, timeline_position,
                                     timeline_loop);

    return len;
  }

  std::pair<object_timelines, std::size_t> object_timelines::deserialize(std::byte *iter) {
    object_timelines result;
    auto len = result.deserialize_self(iter);
    return {std::move(result), len};
  }

  std::string object_timelines::json_serialize() const {
    std::string json = "{";
    
    json += "\"object_type\":\"object_timelines\",";
    json += "\"parent\":" + object_planar::json_serialize() + ",";;
    json += "\"timeline_moments_maps\":" + enigma::JSON_serialization::enigma_serialize(timeline_moments_maps) + ","; 
    json += "\"timeline_index\":" + enigma::JSON_serialization::enigma_serialize(timeline_index) + ",";
    json += "\"timeline_running\":" + enigma::JSON_serialization::enigma_serialize(timeline_running) + ",";
    json += "\"timeline_speed\":" + enigma::JSON_serialization::enigma_serialize(timeline_speed) + ",";
    json += "\"timeline_position\":" + enigma::JSON_serialization::enigma_serialize(timeline_position) + ",";
    json += "\"timeline_loop\":" + enigma::JSON_serialization::enigma_serialize(timeline_loop);

    json += "}";

    return json;
  }

  void object_timelines::json_deserialize_self(const std::string &json) {
    std::string type = enigma::JSON_serialization::enigma_deserialize<std::string>(enigma::JSON_serialization::json_find_value(json,"object_type"));
    if (type != "object_timelines") {
      DEBUG_MESSAGE(
          "object_timelines::json_deserialize_self: Object type '" + type + "' does not match expected: object_timelines",
          MESSAGE_TYPE::M_FATAL_ERROR);
    } 

      object_planar::json_deserialize_self(enigma::JSON_serialization::json_find_value(json,"parent"));
    
      timeline_moments_maps = enigma::JSON_serialization::enigma_deserialize<std::vector<std::map<int, int>>>(enigma::JSON_serialization::json_find_value(json,"timeline_moments_maps"));
      timeline_index = enigma::JSON_serialization::enigma_deserialize<int>(enigma::JSON_serialization::json_find_value(json,"timeline_index"));
      timeline_running = enigma::JSON_serialization::enigma_deserialize<bool>(enigma::JSON_serialization::json_find_value(json,"timeline_running"));
      timeline_speed = enigma::JSON_serialization::enigma_deserialize<gs_scalar>(enigma::JSON_serialization::json_find_value(json,"timeline_speed"));
      timeline_position = enigma::JSON_serialization::enigma_deserialize<gs_scalar>(enigma::JSON_serialization::json_find_value(json,"timeline_position"));
      timeline_loop = enigma::JSON_serialization::enigma_deserialize<bool>(enigma::JSON_serialization::json_find_value(json,"timeline_loop"));
  }

  object_timelines object_timelines::json_deserialize(const std::string &json){
    object_timelines result;
    result.json_deserialize_self(json);
    return result;
  }
}
