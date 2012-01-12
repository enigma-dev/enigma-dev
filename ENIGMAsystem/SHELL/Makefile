#################
# configuration #
#################
# TODO: better config process based on features rather than OS

# PLATFORM { Platforms/* }
OS := $(shell uname -s)
ifeq ($(OS), Linux)
	PLATFORM := xlib
else ifeq ($(OS), Darwin)
	PLATFORM := Cocoa
else
	PLATFORM := Win32
endif

# GMODE { Run, Build, Debug, Compile }
GMODE := Run

# GRAPHICS { Graphics_Systems/* }
GRAPHICS := OpenGL

# AUDIO { Audio_Systems/* }
AUDIO := OpenAL

# COLLISION { Collision_Systems/* }
COLLISION := BBox

# WIDGETS { Widget_Systems/* }
WIDGETS := None

# Default windres
WINDRES = windres.exe

SYSTEMS := Platforms/$(PLATFORM) Graphics_Systems/$(GRAPHICS) Audio_Systems/$(AUDIO) Collision_Systems/$(COLLISION) Widget_Systems/$(WIDGETS) Universal_System

OBJDIR := .eobjs/$(COMPILEPATH)/$(GMODE)

###########
# options #
###########

CXX := g++
ifeq ($(GMODE), Debug)
	CXXFLAGS := -Wall -g -DDEBUG_MODE
else
	CXXFLAGS := -Wall -s -O3
endif

CC := gcc
ifeq ($(GMODE), Debug)
	CFLAGS := -Wall -g -DDEBUG_MODE
else
	CFLAGS := -Wall -g
endif

LDFLAGS := 

ifeq ($(GMODE), Compile)
	CXXFLAGS += -fdata-sections -ffunction-sections
	CFLAGS += -fdata-sections -ffunction-sections
	LDFLAGS += -gc-sections
endif

ifeq ($(PLATFORM), Win32)
	LDFLAGS += -static-libstdc++ -static-libgcc
	ifeq ($(GMODE), Compile)
		LDFLAGS += -static-libgcc -Wl,-subsystem,windows
	endif
endif

# CPPFLAGS needs these include dirs unconditionally
override CPPFLAGS += $(SYSTEMS:%=-I%/Info)

SOURCES := $(wildcard *.cpp)
include $(addsuffix /Makefile,$(SYSTEMS) $(EXTENSIONS))

OBJECTS := $(addprefix $(OBJDIR)/,$(patsubst %.cpp, %.o, $(patsubst %.c, %.o, $(SOURCES))))
RCFILES := $(addprefix $(OBJDIR)/,$(RESOURCES))
DEPENDS := $(OBJECTS:.o=.d)

OBJDIRS := $(sort $(dir $(OBJECTS) $(RCFILES)))

ifeq ($(RESOURCES),)
else
	RESOURCEBINARY := $(OBJDIR)/resources.res
endif

############
# building #
############

.PHONY: all clean

all: $(OUTPUTNAME)

clean:
	$(RM) $(OUTPUTNAME) $(OBJECTS) $(RESOURCEBINARY) $(DEPENDS)

$(OUTPUTNAME): $(OBJECTS) $(RCFILES) $(RESOURCEBINARY)
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) $(RESOURCEBINARY) $(LDLIBS)
	@echo Built to "$(OUTPUTNAME)"

# GCC will figure out dependencies and write out makefile rules in %.d when they change
# -MMD outputs dependencies to %.d as a side effect of compilation, ignoring system headers
# -MP gives phony rules for non-target files, avoiding problems with missing files
$(OBJDIR)/%.o $(OBJDIR)/%.d: %.cpp | $(OBJDIRS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -MP -c -o $(OBJDIR)/$*.o $<

$(OBJDIR)/%.o $(OBJDIR)/%.d: %.c | $(OBJDIRS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -c -o $(OBJDIR)/$*.o $<

$(OBJDIR)/resources.res: $(RCFILES) GENERATED_FILE
	echo "// GENERATED RESOURCE FILE FRONTEND" > $(OBJDIR)/resources.rc
	for res in $(RESOURCES); do echo "#include \"$$res\"" >> $(OBJDIR)/resources.rc; done
	$(WINDRES) -o $@ -i $(OBJDIR)/resources.rc -O coff
GENERATED_FILE:

$(OBJDIR)/%.rc: %.rc
	windres -o $@ -i $^ -O rc

$(OBJDIRS):
	mkdir -p $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif
