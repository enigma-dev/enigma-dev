#include "gmx.hpp"

#include "flatbuffers/reflection.h"
#include "flatbuffers/minireflect.h"

#include "flatbuffers/util.h"

#include <iostream>
#include <iomanip>

namespace gmx {

using namespace tinyxml2;

flatbuffers::Offset load_resource(const XMLNode& root, const flatbuffers::TypeTable& typeTable) {
  flatbuffers::FlatBufferBuilder resourceFBB;
  resourceFBB.StartTable();
  for (size_t i = 0; i < typeTable->num_elements; ++i) {
    std::string name = typeTable->names[i];
    std::cout << name << std::endl;
  }
  return flatbuffers::Offset(resourceFBB.EndTable());
}

flatbuffers::Offset load_resource(std::string fileName, const flatbuffers::TypeTable& typeTable) {
  // load the gmx metadata
  XMLDocument doc;
  doc.LoadFile(fileName);
  return load_resource(doc, typeTable);
}

void load_tree(const XMLNode& root, const TreeNodeBuilder& rootBuilder, ResourceMap resources) {
  std::vector<flatbuffers::Offset<TreeNode>> children;

  // loop the child elements of the current XML element
  for (const XMLElement* child = root.FirstChildElement();
       child != NULL;
       child = child.NextSiblingElement()) {
    // grab the name attribute for this child element
    std::string nameAttribute = child->Attribute("name");

    // create a builder for a tree node corresponding to this child
    flatbuffers::FlatBufferBuilder childFBB;
    TreeNodeBuilder childBuilder(childFBB);

    // if this child has the name attribute, then GMX considers it a directory
    // otherwise, it is a resource
    if (nameAttribute) {
      childBuilder.add_file_name(childFBB.CreateString(nameAttribute));
      childBuilder.add_is_directory(true);
      load_tree(child, &childBuilder, resources);
    } else {
      std::string resourceType = child->Value();
      std::string fileName = std::string(child->GetText()) +
                             std::string(resourceType) +
                             ".gmx";
      childBuilder.add_file_name(childFBB.CreateString(fileName));
      childBuilder.add_is_directory(false);
      //TODO: lookup type table
      resources[root->Value()].push_back(load_resource(fileName, typeTable));
    }

    // finish the buffer for this child's tree node
    auto childBuffer = childBuilder.Finish();
    FinishTreeNodeBuffer(childFBB, childBuffer);
    children.push_back(childBuffer);
  }

  // finish the children vector for this tree root
  rootBuilder->add_children(rootBuilder->CreateVector(children));
}

std::unique_ptr<Project> load_project(std::string fileName) {
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
  ResourceMap resourceMap;
  load_tree(doc, rootBuilder, resourceMap);

  // use the resource map to create the flat vectors of the resource buffers themselves
  auto projectTypeTable = ProjectTypeTable();
  for (size_t i = 0; i < projectTypeTable->num_elements; ++i) {
    std::string fieldName = projectTypeTable->names[i];
    //std::string gmxAttribute = projectTypeTable->attrs[i];

    // see if the resource map contains an std::vector of resource buffers
    // with this resource type to create the flat vector from
    const auto resourceVector = resourceMap.find(fieldName);
    if (resourceVector != resourceMap.end()) {
        projectFBB.AddOffset(XXX, projectFBB.CreateVector(resourceVector->second));
    }
  }

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
  Project* project = load_project("C:/Users/Owner/Documents/GameMaker/Projects/fps6.gmx");
  std::cout << project->file_name()->c_str() << std::endl;
  return 0;
}

} // end namespace gmx
