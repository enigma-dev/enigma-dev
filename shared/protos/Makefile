OS := $(shell uname -s)
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= $(shell which grpc_cpp_plugin)
ifeq ($(OS), Linux)
	EXT := .so
else ifeq ($(OS), Darwin)
	EXT := .dylib
else
	EXT := .dll
	GRPC_CPP_PLUGIN_PATH := $(GRPC_CPP_PLUGIN_PATH).exe
endif

LIBRARY := ../../libProtocols$(EXT)
PROTO_DIR := codegen/
CXXFLAGS := -I$(PROTO_DIR) -std=c++11 -Wall -Wextra -Wpedantic -g -fPIC
SRC_DIR := .
OBJ_DIR := .eobjs

rwildcard=$(wildcard $1/$2) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))
PROTOS := $(call rwildcard,$(SRC_DIR),*.proto)
ifneq ($(CLI_ENABLE_SERVER), TRUE)
	# older protobuf does not even support the RPC syntax
	# so don't even run protoc on server.proto when off
	PROTOS := $(filter-out ./server.proto,$(PROTOS))
endif
PROTO_OUT := $(patsubst $(SRC_DIR)/%, $(PROTO_DIR)/%, $(patsubst %.proto, %.pb.cc, $(PROTOS)))

ifeq ($(CLI_ENABLE_SERVER), TRUE)
	PROTO_OUT += $(PROTO_DIR)/server.grpc.pb.cc
	PROTO_OBJECTS += $(OBJ_DIR)/codegen/server.o
	LIBLDFLAGS := -lgrpc++
	CXXFLAGS += -D_WIN32_WINNT=0x0600
endif

$(PROTO_DIR)/%.grpc.pb.cc: $(SRC_DIR)/server.proto
	protoc -I=. -I=/usr/include --cpp_out=$(PROTO_DIR) server.proto
	protoc -I=. -I=/usr/include --grpc_out=$(PROTO_DIR) --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) server.proto

$(PROTO_DIR)/%.pb.cc: $(SRC_DIR)/%.proto
	protoc -I=. -I=/usr/include --cpp_out=$(PROTO_DIR) $<

SOURCES := $(call rwildcard,$(SRC_DIR),*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(patsubst %.cpp, %.o, $(SOURCES)))

PROTO_SOURCES = $(call rwildcard,$(SRC_DIR),*.pb.cc)
PROTO_OBJECTS = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(patsubst %.pb.cc, %.o, $(PROTO_SOURCES)))

OBJDIRS = $(sort $(OBJ_DIR) $(dir $(PROTO_OBJECTS)) $(dir $(OBJECTS)))
DEPENDS = $(PROTO_OBJECTS:.o=.d) $(OBJECTS:.o=.d)

lib:
	$(MAKE) codegen
	$(MAKE) $(LIBRARY)

codegen: protodir $(PROTO_OUT)

# Old make has a bug which requires this nonsense...
obj_dirs: $(OBJDIRS)

clean:
	rm -rf $(LIBRARY) $(OBJ_DIR) $(PROTO_DIR)

$(LIBRARY): $(PROTO_OBJECTS) $(OBJECTS)
	$(CXX) -shared -o $@ $^ $(LIBLDFLAGS) -lprotobuf

# Create the object directories
$(OBJDIRS):
	mkdir -p $@

protodir:
	mkdir -p $(PROTO_DIR)

# Generate rules for new (unbuilt) files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.pb.cc | obj_dirs
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | obj_dirs
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

# Include rules for known (previously-built) files
-include $(DEPENDS)
.SUFFIXES:

.PHONY: lib codegen clean obj_dirs
