OS := $(shell uname -s)
ifeq ($(OS), Linux)
	TARGET := ../emake.elf
	CXXFLAGS += -fPIC
	MKDIR := mkdir
else ifeq ($(OS), Darwin)
	TARGET := ../emake.elf
	CXXFLAGS += -fPIC
	MKDIR := mkdir
else
	TARGET := ../emake.exe
	LDFLAGS += -static-libgcc
	MKDIR := mkdir.exe
endif

###########
# options #
###########

CXX := g++
CXXFLAGS += -Wall -g -I. -I../../../CompilerSource -I../../../ENIGMAsystem/SHELL -I../../../ENIGMAsystem/Additional/i686-w64-mingw32/include -I../../../ENIGMAsystem/SHELL/Universal_System
LDFLAGS += -L../../../ENIGMAsystem/Additional/i686-w64-mingw32/lib
LDLIBS += -lz

SOURCES := $(shell find ./ -name "*.cpp") ../../../ENIGMAsystem/SHELL/Universal_System/lodepng.cpp ../../../ENIGMAsystem/SHELL/Universal_System/zlib.cpp
OBJECTS := $(addprefix .eobjs/,$(SOURCES:.cpp=.o))
DEPENDS := $(OBJECTS:.o=.d)

# sort has the nice side effect of removing duplicates. this may or may not be faster.
OBJDIRS := $(sort $(dir $(OBJECTS)))

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

$(OBJDIRS):
	$(MKDIR) -p $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif