OS := $(shell uname -s)
ifeq ($(OS), Linux)
	EXT := .so
else ifeq ($(OS), Darwin)
	EXT := .dylib
else
	EXT := .dll
endif

LIBRARY := ../../libProtocols$(EXT)
PROTO_DIR := codegen/
CXXFLAGS := -I$(PROTO_DIR) -std=c++11 -Wall -Wextra -Wpedantic -g -fPIC
SRC_DIR := .
OBJ_DIR := .eobjs

rwildcard=$(wildcard $1/$2) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))
PROTOS := $(call rwildcard,$(SRC_DIR),*.proto)
PROTO_OUT := $(patsubst $(SRC_DIR)/%, $(PROTO_DIR)/%, $(patsubst %.proto, %.pb.h, $(PROTOS)))

$(PROTO_DIR)/%.pb.h: $(SRC_DIR)/%.proto
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
	$(CXX) -shared -o $@ $^ -lprotobuf

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
