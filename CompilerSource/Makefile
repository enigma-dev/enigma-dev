# "Shared" includes.
SHARED_SRC_DIR := ../shared
include $(SHARED_SRC_DIR)/Makefile
include $(SHARED_SRC_DIR)/eyaml/Makefile
include $(SHARED_SRC_DIR)/event_reader/Makefile
include $(SHARED_SRC_DIR)/rectpacker/Makefile

#################
# configuration #
#################
# TODO: better config process based on features rather than OS

BASE = compileEGMf
OS := $(shell uname -s)
ifeq ($(OS), Linux)
	TARGET := ../lib$(BASE).so
	CXXFLAGS += -fPIC
	MKDIR := mkdir
else ifeq ($(OS), Darwin)
	TARGET := ../lib$(BASE).dylib
	CXXFLAGS += -fPIC
	MKDIR := mkdir
else
	TARGET := ../$(BASE).dll
	LDFLAGS += -static-libgcc
	MKDIR := mkdir.exe
endif

###########
# options #
###########

CXX := g++
CXXFLAGS += -std=c++11 -Wall -O3 -g -I./JDI/src
LDFLAGS += -shared -O3 -g -L../ -Wl,-rpath,./
LDLIBS += -lProtocols -lprotobuf -lz -L$(SHARED_SRC_DIR)/libpng-util -lpng-util -lpng

# This implements a recursive wildcard allowing us to iterate in subdirs
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SOURCES := $(filter-out ./standalone_main.cpp, $(call rwildcard,./,*.cpp))
OBJECTS := $(addprefix .eobjs/,$(SOURCES:.cpp=.o))
DEPENDS := $(OBJECTS:.o=.d)
PROTO_DIR := $(SHARED_SRC_DIR)/protos

CXXFLAGS += -I$(SHARED_SRC_DIR) -I$(SHARED_SRC_DIR)/libpng-util -I$(PROTO_DIR)/codegen $(addprefix -I$(SHARED_SRC_DIR)/, $(SHARED_INCLUDES))
SOURCES += $(addprefix $(SHARED_SRC_DIR),$(SHARED_SOURCES))
OBJECTS += $(addprefix .eobjs/shared/,$(SHARED_SOURCES:.cpp=.o))
DEPENDS += $(addprefix .eobjs/shared/,$(SHARED_SOURCES:.cpp=.d))

# sort has the nice side effect of removing duplicates. this may or may not be faster.
OBJDIRS := $(sort $(dir $(OBJECTS)))
override CPPFLAGS += -I.

############
# building #
############

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(OBJECTS) $(DEPENDS)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

# GCC will figure out dependencies and write out makefile rules in %.d when they change
# -MMD outputs dependencies to %.d as a side effect of compilation, ignoring system headers
# -MP gives phony rules for non-target files, avoiding problems with missing files
.eobjs/%.o .eobjs/%.d: %.cpp | $(OBJDIRS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -MP -c -o .eobjs/$*.o $<
.eobjs/shared/%.o .eobjs/shared/%.d: $(SHARED_SRC_DIR)/%.cpp | $(OBJDIRS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -MP -c -o .eobjs/shared/$*.o $<

$(OBJDIRS):
	$(MKDIR) -p $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif
