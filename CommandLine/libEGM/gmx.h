/** Copyright (C) 2018-2020 Greg Williamson, Robert B. Colton
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

#ifndef EGM_GMX_H
#define EGM_GMX_H

#include "file-format.h"

namespace egm {

class GMXFileFormat : public FileFormat {
 public:
  GMXFileFormat(const EventData* event_data) : FileFormat(event_data) {}
  virtual std::unique_ptr<Project> LoadProject(const fs::path& fName) const override;
  virtual std::optional<Background> LoadBackground(const fs::path& fName) const override;
  virtual std::optional<Sound> LoadSound(const fs::path& fName) const override;
  virtual std::optional<Sprite> LoadSprite(const fs::path& fName) const override;
  virtual std::optional<Shader> LoadShader(const fs::path& fName) const override;
  virtual std::optional<Font> LoadFont(const fs::path& fName) const override;
  virtual std::optional<Object> LoadObject(const fs::path& fName) const override;
  virtual std::optional<Timeline> LoadTimeLine(const fs::path& fName) const override;
  virtual std::optional<Room> LoadRoom(const fs::path& fName) const override;
  virtual std::optional<Path> LoadPath(const fs::path& fName) const override;
  virtual std::optional<Script> LoadScript(const fs::path& fName) const override;
};

} //namespace egm

#endif
