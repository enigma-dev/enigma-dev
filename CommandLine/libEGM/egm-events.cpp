/* Copyright (C) 2020 Greg Williamson, Josh Ventura
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
*/

#include "egm-events.h"
#include "strings_util.h"

namespace egm {

void LoadObjectEvents(const fs::path& fPath, google::protobuf::Message *m, const google::protobuf::FieldDescriptor *field, const EventData* event_data) {
    for(auto& f : fs::directory_iterator(fPath)) {
    if (f.path().extension() == ".edl") {
      const std::string eventIdString = f.path().stem().string();
      auto event = event_data->DecodeEventString(eventIdString);

      buffers::resources::Object::EgmEvent event_proto;
      event_proto.set_id(event.bare_id());
      event_proto.set_code(FileToString(f.u8string()));
      for (const auto &arg : event.arguments)
        event_proto.add_arguments(arg.name);

      const google::protobuf::Reflection *refl = m->GetReflection();

      google::protobuf::Message* msg = refl->AddMessage(m, field);
      msg->CopyFrom(event_proto);
    }
  }
}

void WriteObjectEvents(const fs::path& fPath, const google::protobuf::RepeatedPtrField<buffers::resources::Object_EgmEvent>& events, const EventData* event_data) {
  for (auto &e : events) {
    auto event = event_data->get_event(e.id(), {e.arguments().begin(), e.arguments().end()});
    auto edlFile = fPath/(event.IdString() + ".edl");
    std::ofstream fout{edlFile};
    fout << e.code();
  }
}

}
