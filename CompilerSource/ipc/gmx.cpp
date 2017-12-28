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

	for (const XMLElement* child = root.FirstChildElement();
		 child != NULL;
		 child = child.NextSiblingElement()) {
		const char* nameAttribute = child->Attribute("name");

		flatbuffers::FlatBufferBuilder childFBB;
		TreeNodeBuilder childBuilder(childFBB);

		if (nameAttribute) {
			childuilder.add_file_name(childFBB.CreateString(nameAttribute));
			childuilder.add_is_directory(true);
			load_tree(child, childBuilder);
		} else {
			childBuilder.add_file_name(childFBB.CreateString(nameAttribute));
			childuilder.add_is_directory(false);
		}

		auto childBuffer = childBuilder.Finish();
		FinishTreeNodeBuffer(childFBB, childBuffer);
		children.push_back(childBuffer);
	}

	rootBuilder->add_children(rootBuilder->CreateVector(children));
}

Project *load_project(const char *fileName) {
	XMLDocument doc;
	doc.LoadFile(fileName);

	flatbuffers::FlatBufferBuilder projectFBB;
	ProjectBuilder projectBuilder(projectFBB);

	flatbuffers::FlatBufferBuilder rootFBB;
	TreeNodeBuilder rootBuilder(rootFBB);
	rootBuilder.add_file_name(rootFBB.CreateString(fileName));
	rootBuilder.add_is_directory(false);

	load_tree(&doc);

	auto rootBuffer = rootBuilder.Finish();
	FinishTreeNodeBuffer(rootFBB, rootBuffer);

	projectBuilder.add_tree_root(rootBuffer);

	auto projectBuffer = projectBuilder.Finish();
	FinishProjectBuffer(projectFBB, projectBuffer);

	return GetProject(projectBuffer);
}

int main() {

	return 0;
}

} // end namespace gmx
