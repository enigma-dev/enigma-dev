OS := $(shell uname -s)
ifeq ($(OS), Linux)
	EXT := .so
else ifeq ($(OS), Darwin)
	EXT := .dylib
else
	EXT := .dll
endif

GCCVER := $(shell gcc -dumpversion | cut -c 1)
SHARED_SOURCES := ../../shared
LIBRARY   := ../../libEGM$(EXT)
PROTO_DIR := ../../shared/protos
SRC_DIR   := .
OBJ_DIR   := .eobjs

CXXFLAGS  := -I$(SRC_DIR) $(shell pkg-config --cflags pugixml) -I$(PROTO_DIR) -I$(PROTO_DIR)/codegen -I$(SHARED_SOURCES)/lodepng -Wall -Wextra -Wpedantic -g -fPIC
LDFLAGS   := -lz $(shell pkg-config --libs-only-L pugixml) -lpugixml -lyaml-cpp -L../../ -lProtocols -lprotobuf -lstdc++fs -L$(SHARED_SOURCES)/lodepng -llodepng

#if gcc >= 8 use std::filesystem instead of boost
ifeq ($(shell expr $(GCCVER) \>= 8), 1)
	override CXXFLAGS += -std=c++17
	FS_LIBS=-lstdc++fs
else
	override CXXFLAGS += -std=c++14 -DUSE_BOOST_FS
	ifeq ($(OS), Linux)
		FS_LIBS=-lboost_system -lboost_filesystem
	else ifeq ($(OS), Darwin)
		FS_LIBS=-lboost_system -lboost_filesystem
	else
		FS_LIBS=-lboost_system-mt -lboost_filesystem-mt
	endif
endif

LDFLAGS += $(FS_LIBS)

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

# --libs-only-L is specified because the Trusty Tahr package for pugixml does not have a proper pkg-config
# hence we only use pkg-config for the lib dirs and just always link pugixml whether in travis, on ubuntu, or in msys2
$(LIBRARY): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LDFLAGS)

# Create the object directories
$(OBJDIRS):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | obj_dirs
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

.PHONY: lib clean obj_dirs

# Include rules for known (previously-built) files
-include $(DEPENDS)
.SUFFIXES:
