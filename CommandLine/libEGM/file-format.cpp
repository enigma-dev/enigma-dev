#include "file-format.h"

// Debugging output streams for file formats
namespace egm {

std::ostream outStream(nullptr);
std::ostream errStream(nullptr);

void BindOutputStreams(std::ostream &out, std::ostream &err) { outStream.rdbuf(out.rdbuf()); errStream.rdbuf(err.rdbuf()); }

} //namespace egm