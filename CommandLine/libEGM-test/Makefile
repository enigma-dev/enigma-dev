CXXFLAGS := -std=c++17
DEPS := ../../shared/event_reader/event_parser.cpp ../../shared/ProtoYaml/proto-yaml.cpp
INCLUDES := -I../../shared/ -I../../shared/protos/.eobjs/
LDFLAGS := -Wl,-rpath,../.. -L../..
LDLIBS := -lEGM -lProtocols -lgtest -lgtest_main -lprotobuf -lyaml-cpp -lpthread

event-reader-tests: event-reader-tests.cpp $(DEPS)
	g++ $(CXXFLAGS) $^ $(INCLUDES) $(LDFLAGS) $(LDLIBS) -o $@
