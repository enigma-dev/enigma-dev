/** Copyright (C) 2020 Greg Williamson
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

#ifndef EGM_FILEFORMAT_H
#define EGM_FILEFORMAT_H

#include "project.pb.h"
#include "event_reader/event_parser.h"

#include <iostream>
#include <streambuf>
#include <filesystem>
#include <optional>
#include <unordered_map>

namespace egm {

namespace fs = std::filesystem;
using namespace buffers;
using namespace buffers::resources;

// Generic interface for reading and writing projects. All file readers/writers inherit this class.
class FileFormat {
public:
  FileFormat(const EventData* event_data) : _event_data(event_data) {}
  // Read
  virtual std::unique_ptr<Project> LoadProject(const fs::path& /*fName*/) const { return {}; }
  virtual std::optional<Background> LoadBackground(const fs::path& /*fName*/) const { return {}; }
  virtual std::optional<Sound> LoadSound(const fs::path& /*fName*/) const { return {}; }
  virtual std::optional<Sprite> LoadSprite(const fs::path& /*fName*/) const { return {}; }
  virtual std::optional<Shader> LoadShader(const fs::path& /*fName*/) const { return {}; }
  virtual std::optional<Font> LoadFont(const fs::path& /*fName*/) const { return {}; }
  virtual std::optional<Object> LoadObject(const fs::path& /*fName*/) const { return {}; }
  virtual std::optional<Timeline> LoadTimeLine(const fs::path& /*fName*/) const { return {}; }
  virtual std::optional<Room> LoadRoom(const fs::path& /*fName*/) const { return {}; }
  virtual std::optional<Path> LoadPath(const fs::path& /*fName*/) const { return {}; }
  virtual std::optional<Script> LoadScript(const fs::path& /*fName*/) const { return {}; }
  // Write
  
protected:
  const EventData* _event_data;
};

// Access file formats based on extensions
extern std::unordered_map<std::string, std::unique_ptr<FileFormat>> fileFormats;
void LibEGMInit(const EventData* event_data);
std::unique_ptr<Project> LoadProject(const fs::path& /*fName*/);

// Debugging output streams for file formats
extern std::ostream outStream;
extern std::ostream errStream;
void BindOutputStreams(std::ostream &out, std::ostream &err);

} //namespce egm

#endif
