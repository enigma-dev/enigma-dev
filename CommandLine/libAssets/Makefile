OS := $(shell uname -s)
ifeq ($(OS), Linux)
	EXT := .so
else ifeq ($(OS), Darwin)
	EXT := .dylib
else
	EXT := .dll
endif

GCCVER := $(shell gcc -dumpversion | cut -c 1)
LIBRARY   := ../../libAssets$(EXT)
PROTO_DIR := ../protos
SRC_DIR   := .
OBJ_DIR   := .eobjs

SHARED_SRC_DIR := ../../shared
include $(SHARED_SRC_DIR)/Makefile
include $(SHARED_SRC_DIR)/bettersystem/Makefile

CXXFLAGS  := -I$(SRC_DIR) -I../../CompilerSource -I$(SHARED_SRC_DIR) -I$(SHARED_SRC_DIR)/lodepng -I$(SHARED_SRC_DIR)/bettersystem -I$(PROTO_DIR) -I$(PROTO_DIR)/codegen -std=c++11 -Wall -Wextra -Wpedantic -g -fPIC
LDFLAGS   := -lz -L$(SHARED_SRC_DIR)/lodepng -llodepng -L../../ -lProtocols -lprotobuf

rwildcard=$(wildcard $1/$2) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))

SOURCES := $(call rwildcard,$(SRC_DIR),*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(patsubst %.cpp, %.o, $(SOURCES)))

OBJDIRS = $(sort $(OBJ_DIR) $(dir $(OBJECTS)))
DEPENDS = $(OBJECTS:.o=.d)

CXXFLAGS += -I$(SHARED_SRC_DIR) $(addprefix -I$(SHARED_SRC_DIR)/, $(SHARED_INCLUDES))
SOURCES += $(addprefix $(SHARED_SRC_DIR),$(SHARED_SOURCES))
OBJECTS += $(addprefix $(OBJ_DIR)/shared/,$(SHARED_SOURCES:.cpp=.o))
DEPENDS += $(addprefix $(OBJ_DIR)/shared/,$(SHARED_SOURCES:.cpp=.d))

lib:
	$(MAKE) $(LIBRARY)

# Old make has a bug which requires this nonsense...
obj_dirs: $(OBJDIRS)

clean:
	rm -rf $(LIBRARY) $(OBJ_DIR)

$(LIBRARY): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LDFLAGS)

# Create the object directories
$(OBJDIRS):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | obj_dirs
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<
$(OBJ_DIR)/shared/%.o: $(SHARED_SRC_DIR)/%.cpp | obj_dirs
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

.PHONY: lib clean obj_dirs

# Include rules for known (previously-built) files
-include $(DEPENDS)
.SUFFIXES:
