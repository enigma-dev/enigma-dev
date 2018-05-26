OS := $(shell uname -s)
ifeq ($(OS), Linux)
	EXT := .so
else ifeq ($(OS), Darwin)
	EXT := .dylib
else
	EXT := .dll
endif

LIBRARY   := ../../libEGM$(EXT)
PROTO_DIR := ../protos
CXXFLAGS  := -I/mingw64/include/pugixml-1.8/ -I$(PROTO_DIR) -I$(PROTO_DIR)/codegen -std=c++14 -Wall -Wextra -Wpedantic -g -fPIC
SRC_DIR   := .
OBJ_DIR   := .eobjs

rwildcard=$(wildcard $1/$2) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))

SOURCES := $(call rwildcard,$(SRC_DIR),*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(patsubst %.cpp, %.o, $(SOURCES)))

OBJDIRS = $(sort $(OBJ_DIR) $(dir $(OBJECTS)))
DEPENDS = $(OBJECTS:.o=.d)

lib:
	$(MAKE) $(LIBRARY)

# Old make has a bug which requires this nonsense...
obj_dirs: $(OBJDIRS)

clean:
	rm -rf $(LIBRARY) $(OBJ_DIR)

$(LIBRARY): $(OBJECTS)
	$(CXX) -shared -o $@ $^ -lz -L/mingw64/lib/pugixml-1.8/ -lpugixml -L../../ -lProtocols -lprotobuf

# Create the object directories
$(OBJDIRS):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | obj_dirs
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

.PHONY: lib clean obj_dirs

# Include rules for known (previously-built) files
-include $(DEPENDS)
.SUFFIXES:
