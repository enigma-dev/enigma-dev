BINARY := ../../emake-tests
CXXFLAGS := -std=c++17
DEPS := ../../shared/event_reader/event_parser.cpp ../../shared/ProtoYaml/proto-yaml.cpp
INCLUDES := -I../../shared/ -I../../shared/protos/.eobjs/ -I../libEGM
LDFLAGS := -Wl,-rpath,./ -L../..
LDLIBS := -lEGM -lProtocols -lgtest_main -lgtest -lprotobuf -lyaml-cpp -lpthread

$(BINARY): $(wildcard *.cpp) $(DEPS) ../../libEGM.so ../../libProtocols.so
	g++ $(CXXFLAGS) $^ $(INCLUDES) $(LDFLAGS) $(LDLIBS) -o $@

clean:
	rm -rf $(BINARY)
