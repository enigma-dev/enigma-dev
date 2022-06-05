#ifndef EGM_TSX_H
#define EGM_TSX_H

#include "file-format.h"

namespace egm {

class TSXFileFormat : public FileFormat {
public:
  TSXFileFormat(const EventData* event_data) : FileFormat(event_data) {}
  virtual std::unique_ptr<Project> LoadProject(const fs::path& fPath) const override;
};

} // namespace egm

#endif
