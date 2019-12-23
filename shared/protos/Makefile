OS := $(shell uname -s)
ifeq ($(OS), Linux)
	LIB_EXT := .so
	BIN_EXT :=
else ifeq ($(OS), Darwin)
	LIB_EXT := .dylib
	BIN_EXT :=
else
	LIB_EXT := .dll
	BIN_EXT := .exe
endif

LIBRARY := ../../libProtocols$(LIB_EXT)
OBJDIR := .eobjs

CXXFLAGS := -I$(OBJDIR) -std=c++11 -Wall -Wextra -Wpedantic -g -fPIC
LDFLAGS := -shared
LDLIBS := -lprotobuf

.PHONY: all clean

all: $(LIBRARY)

clean:
	$(RM) -r $(LIBRARY) $(OBJDIR)

SOURCES := $(wildcard *.proto) $(wildcard Configuration/*.proto)
SOURCES_GRPC :=
ifeq ($(CLI_ENABLE_SERVER), TRUE)
	SOURCES_GRPC += server.proto
	LDLIBS += -lgrpc++
	CXXFLAGS += -D_WIN32_WINNT=0x0600

	GRPC_CPP_PLUGIN_PATH ?= $(shell which grpc_cpp_plugin)$(BIN_EXT)
else
	# older protobuf does not even support the RPC syntax
	# so don't even run protoc on server.proto when off
	SOURCES := $(filter-out server.proto,$(SOURCES))
endif

GENERATED := $(addprefix $(OBJDIR)/,$(SOURCES:.proto=.pb.cc) $(SOURCES:.proto=.pb.h) $(SOURCES_GRPC:.proto=.grpc.pb.cc) $(SOURCES_GRPC:.proto=.grpc.pb.h))
OBJECTS := $(addprefix $(OBJDIR)/,$(SOURCES:.proto=.pb.o) $(SOURCES_GRPC:.proto=.grpc.pb.o))
DEPENDS = $(OBJECTS:.o=.d)

OBJDIRS = $(sort $(dir $(OBJECTS)))

$(LIBRARY): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# The order-only dependency on $(GENERATED) is an over-approximation to force all protos to be built
# before any object files. This matters on the first run when Make can't yet know about #includes.
$(OBJDIR)/%.pb.o $(OBJDIR)/%.pb.d: $(OBJDIR)/%.pb.cc | $(GENERATED)
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $(OBJDIR)/$*.pb.o $(OBJDIR)/$*.pb.cc

$(OBJDIR)/%.pb.cc $(OBJDIR)/%.pb.h: %.proto | $(OBJDIRS)
	protoc -I. --cpp_out=$(OBJDIR) $<

$(OBJDIR)/%.grpc.pb.cc $(OBJDIR)/%.grpc.pb.h: %.proto | $(OBJDIRS)
	protoc -I. --grpc_out=$(OBJDIR) --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

$(OBJDIRS):
	mkdir -p $@

.PRECIOUS: $(GENERATED)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif
