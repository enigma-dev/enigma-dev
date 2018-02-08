
#include "codegen/Project_generated.h"

#include "flatbuffers/flatbuffers.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace gmx {
  typedef std::unordered_map<std::string,std::vector<flatbuffers::Offset> > ResourceMap;

  void load_resource(const XMLNode& root, const flatbuffers::TypeTable& typeTable);
  void load_resource(std::string fileName, const flatbuffers::TypeTable& typeTable);
  void load_tree(const XMLNode& root, TreeNodeBuilder& rootBuilder, ResourceMap resources);
  std::unique_ptr<Project> load_project(std::string fileName);
}
