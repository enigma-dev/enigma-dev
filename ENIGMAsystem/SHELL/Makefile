#################
# configuration #
#################
# TODO: better config process based on features rather than OS

# PLATFORM { Platforms/* }
OS := $(shell uname -s)
ifeq ($(OS), Linux)
	PLATFORM := xlib
	MKDIR := mkdir
else ifeq ($(OS), Darwin)
	PLATFORM := Cocoa
	MKDIR := mkdir
else
	PLATFORM := Win32
	MKDIR := mkdir.exe
endif

# GMODE { Run, Build, Debug, Compile }
GMODE ?= Run

# GRAPHICS { Graphics_Systems/* }
GRAPHICS ?= OpenGL1

# AUDIO { Audio_Systems/* }
AUDIO ?= OpenAL

# COLLISION { Collision_Systems/* }
COLLISION ?= BBox

# WIDGETS { Widget_Systems/* }
WIDGETS ?= None

# NETWORKING { Networking_Systems/* }
NETWORKING ?= None

# RESOURCE FILE WITH ICON AND VERSION INFO
ifeq ($(PLATFORM), Win32)
RESOURCES += Preprocessor_Environment_Editable/Resources.rc
endif

# Default windres
WINDRES = windres.exe

SYSTEMS := Platforms/$(PLATFORM) Graphics_Systems/$(GRAPHICS) Audio_Systems/$(AUDIO) Collision_Systems/$(COLLISION) Widget_Systems/$(WIDGETS) Networking_Systems/$(NETWORKING) Universal_System

OBJDIR := .eobjs/$(COMPILEPATH)/$(GMODE)

###########
# options #
###########

CXX := g++
ifeq ($(GMODE), Debug)
	CXXFLAGS += -Wall -g -DDEBUG_MODE
else
	CXXFLAGS += -Wall -s -O3
endif

CC := gcc
ifeq ($(GMODE), Debug)
	CFLAGS += -Wall -g -DDEBUG_MODE
else
	CFLAGS += -Wall -s -O3 
endif

DEPENDENCIES := 

ifeq ($(GMODE), Compile)
	CXXFLAGS += -fdata-sections -ffunction-sections
	CFLAGS += -fdata-sections -ffunction-sections
	LDFLAGS += -gc-sections
endif

ifeq ($(PLATFORM), Win32)
	LDFLAGS += -static-libgcc
	ifeq ($(GMODE), Compile)
		LDFLAGS += -static-libgcc -Wl,-subsystem,windows
	endif
endif

# CPPFLAGS needs these include dirs unconditionally
override CPPFLAGS += $(SYSTEMS:%=-I%/Info)
override CPPFLAGS += -I.

.PHONY: all clean

all: compile_game

clean:
	$(RM) $(OUTPUTNAME) $(OBJECTS) $(RESOURCEBINARY) $(DEPENDS)

SOURCES := $(wildcard *.cpp)
include $(addsuffix /Makefile,$(SYSTEMS) $(EXTENSIONS))
include Bridges/$(PLATFORM)-$(GRAPHICS)/Makefile

OBJECTS := $(addprefix $(OBJDIR)/,$(patsubst %.m, %.o, $(patsubst %.cpp, %.o, $(patsubst %.c, %.o, $(SOURCES)))))
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

compile_game: $(OBJECTS) $(RCFILES) $(RESOURCEBINARY) $(DEPENDENCIES)
	$(CXX) $(LDFLAGS) -o "$(OUTPUTNAME)" $(OBJECTS) $(RESOURCEBINARY) $(LDLIBS)
	@echo Built to "$(OUTPUTNAME)"

# GCC will figure out dependencies and write out makefile rules in %.d when they change
# -MMD outputs dependencies to %.d as a side effect of compilation, ignoring system headers
# -MP gives phony rules for non-target files, avoiding problems with missing files
$(OBJDIR)/%.o $(OBJDIR)/%.d: %.cpp | $(OBJDIRS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) -MMD -MP -c -o $(OBJDIR)/$*.o $<

$(OBJDIR)/%.o $(OBJDIR)/%.d: %.c | $(OBJDIRS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) -MMD -MP -c -o $(OBJDIR)/$*.o $<

$(OBJDIR)/%.o $(OBJDIR)/%.d: %.m | $(OBJDIRS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) -MMD -MP -c -o $(OBJDIR)/$*.o $<

$(OBJDIR)/resources.res: $(RCFILES) GENERATED_FILE
	echo "// GENERATED RESOURCE FILE FRONTEND" > $(OBJDIR)/resources.rc
	for res in $(RESOURCES); do echo "#include \"$$res\"" >> $(OBJDIR)/resources.rc; done
	$(WINDRES) -o $@ -i $(OBJDIR)/resources.rc -O coff
GENERATED_FILE:

$(OBJDIR)/%.rc: %.rc
	$(WINDRES) -o $@ -i $^ -O rc

$(OBJDIRS):
	$(MKDIR) -p $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif
