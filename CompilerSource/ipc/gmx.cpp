#include <string>
#include <iostream>
#include <iomanip>

#include "flatbuffers/reflection.h"
#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/util.h"

#include "codegen/Project_generated.h"

Project *load_gmx(const char *fileName) {
	XMLDocument doc;
	doc.LoadFile(filename);

	flatbuffers::FlatBufferBuilder builder;
	ProjectBuilder projectBuilder(builder);

	std::string schemaFile;
	flatbuffers::LoadFile("codegen/Project.bfbs", true, &schemaFile);
	auto &schema = *reflection::GetSchema(schemaFile.c_str());
	auto root_table = schema.root_table();
	std::cout << root_table->name()->c_str() << std::endl;

	auto &fields = *root_table->fields();
	for (auto field : fields) {
		// loop fields of root Project table...
	}

	auto projectbuffer = projectBuilder.Finish();
	FinishTestBuffer(builder, projectbuffer);

	return builder.ReleaseBufferPointer().data();
}

int main() {

	return 0;
}
