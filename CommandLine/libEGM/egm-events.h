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

#include "filesystem.h"
#include "event_reader/event_parser.h"
#include "Object.pb.h"

namespace egm {

void LoadObjectEvents(const fs::path& fPath, google::protobuf::Message *m, const google::protobuf::FieldDescriptor *field, const EventData* event_data);
void WriteObjectEvents(const fs::path& fPath, const google::protobuf::RepeatedPtrField<buffers::resources::Object_EgmEvent>& events, const EventData* event_data);

}
