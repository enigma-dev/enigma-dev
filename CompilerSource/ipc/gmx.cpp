#include <string>
#include <iostream>
#include <iomanip>

#include <vector>

#include "flatbuffers/reflection.h"
#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/util.h"

#include "codegen/Project_generated.h"

namespace gmx {

using namespace tinyxml2;

void load_tree(XMLNode* root, TreeNodeBuilder *rootBuilder) {
  std::vector<flatbuffers::Offset<TreeNode>> children;

  // loop the child elements of the current XML element
  for (const XMLElement* child = root.FirstChildElement();
       child != NULL;
       child = child.NextSiblingElement()) {
    // grab the name attribute for this child element
    const char* nameAttribute = child->Attribute("name");

    // create a builder for a tree node corresponding to this child
    flatbuffers::FlatBufferBuilder childFBB;
    TreeNodeBuilder childBuilder(childFBB);

    // if this child has the name attribute, then GMX considers it a directory
    // otherwise, it is a resource
    if (nameAttribute) {
      childuilder.add_file_name(childFBB.CreateString(nameAttribute));
      childuilder.add_is_directory(true);
      load_tree(child, childBuilder);
    } else {
      childBuilder.add_file_name(childFBB.CreateString(nameAttribute));
      childuilder.add_is_directory(false);
    }

    // finish the buffer for this child's tree node
    auto childBuffer = childBuilder.Finish();
    FinishTreeNodeBuffer(childFBB, childBuffer);
    children.push_back(childBuffer);
  }

  // finish the children vector for this tree root
  rootBuilder->add_children(rootBuilder->CreateVector(children));
}

Project *load_project(const char *fileName) {
  // load the gmx manifest
  XMLDocument doc;
  doc.LoadFile(fileName);

  // create project builder
  flatbuffers::FlatBufferBuilder projectFBB;
  ProjectBuilder projectBuilder(projectFBB);

  projectBuilder.add_file_name(fileName);

  // create builder for the project tree's root node
  flatbuffers::FlatBufferBuilder rootFBB;
  TreeNodeBuilder rootBuilder(rootFBB);

  rootBuilder.add_file_name(rootFBB.CreateString(fileName));
  rootBuilder.add_is_directory(false);

  // recursively build the project tree using DFS
  load_tree(&doc);

  // finish the root tree node buffer
  auto rootBuffer = rootBuilder.Finish();
  FinishTreeNodeBuffer(rootFBB, rootBuffer);

  // add the finished tree root to the project
  projectBuilder.add_tree_root(rootBuffer);

  // finish the project buffer
  auto projectBuffer = projectBuilder.Finish();
  FinishProjectBuffer(projectFBB, projectBuffer);

  return GetProject(projectBuffer);
}

int main() {

  return 0;
}

} // end namespace gmx
