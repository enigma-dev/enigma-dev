#include "codegen/game.pb.h"

#include <string>

namespace gmx {
  buffers::Project *LoadGMX(std::string fName, bool verbose);
} //namespace gmx
