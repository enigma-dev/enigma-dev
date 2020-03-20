

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

MAKE_TARGETS :=
ifeq ($(TARGET-PLATFORM), Android)
	MAKE_TARGETS += android_game
	BASENAME := $(shell basename $(OUTPUTNAME))
	SDL_CSTDIO := TRUE
	ifeq ($(GMODE), Compile)
		GRADLE_TARGET := build
		APK_OUT := $(WORKDIR)/android/enigma/outputs/apk/release
	else
		GRADLE_TARGET := installDebug
		APK_OUT := $(WORKDIR)/android/enigma/outputs/apk/debug
	endif
else
	MAKE_TARGETS += compile_game
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
ifeq ($(TARGET-PLATFORM), Windows)
RESOURCES += Preprocessor_Environment_Editable/Resources.rc
endif

# Default windres
WINDRES = windres.exe --codepage=65001

# Default find
FIND = find

SYSTEMS := Platforms/$(PLATFORM) Graphics_Systems/$(GRAPHICS) Audio_Systems/$(AUDIO) Collision_Systems/$(COLLISION) Widget_Systems/$(WIDGETS) Networking_Systems/$(NETWORKING) Universal_System

OBJDIR := $(WORKDIR).eobjs/$(COMPILEPATH)/$(GMODE)

SHARED_SRC_DIR := ../../shared/
include $(SHARED_SRC_DIR)/Makefile
include $(SHARED_SRC_DIR)/rectpacker/Makefile

###########
# options #
###########

CXX := g++
CC := gcc
DEPENDENCIES :=
override CXXFLAGS += -std=c++17
 
ifeq ($(GMODE), Debug)
	override CXXFLAGS += -Wall -g -DDEBUG_MODE
	override CFLAGS += -Wall -g -DDEBUG_MODE
else ifeq ($(GMODE), Compile)
	override CXXFLAGS += -Wall -s -O3 -fno-rtti -fno-exceptions -flto -fdata-sections -ffunction-sections
	override CFLAGS += -Wall -s -O3 -flto -fno-exceptions -fdata-sections -ffunction-sections
	override LDFLAGS += -flto -s -O3

	# Disable PIE when possible
	ifneq ($(shell $(CC) -dumpspecs 2>/dev/null | grep -e '[^f]no-pie'),)
		override CXXFLAGS += -fno-pie
		override CFLAGS += -fno-pie
		override LDLIBS += -no-pie
		override LDFLAGS += -no-pie
	endif
	ifneq ($(shell $(CC) -dumpspecs 2>/dev/null | grep -e '[^f]nopie'),)
		override CXXFLAGS += -fno-pie
		override CFLAGS += -fno-pie
		override LDLIBS += -nopie
		override LDFLAGS += -nopie
	endif
else
	override CXXFLAGS += -Wall -s -O3 -fno-rtti -fno-exceptions
	override CFLAGS += -Wall -s -O3 -fno-exceptions
endif

ifeq ($(PLATFORM), Win32)
	ifeq ($(GMODE), Compile)
		override LDFLAGS += -static-libgcc -Wl,-subsystem,windows
	endif
endif

# CPPFLAGS needs these include dirs unconditionally
override CPPFLAGS += $(SYSTEMS:%=-I%/Info)
override CPPFLAGS += -I. -I$(CODEGEN) -I$(SHARED_SRC_DIR)

.PHONY: all clean compile_game clean print_flags write_src_files android_game

all: $(MAKE_TARGETS)

print_flags:
	@echo -e "\n################ Compiler Info ################"
	@$(CXX) --version
	@echo "CXXFLAGS=\"$(CXXFLAGS) $(CPPFLAGS) $(INCLUDES)\""
	@echo "CFLAGS=\"$(CFLAGS) $(CPPFLAGS)\""
	@echo "LDFLAGS=\"$(LDFLAGS)\""
	@echo "LDLIBS=\"$(LDIBS)\""
	@echo -e "################################################\n"

clean:
	$(RM) $(OUTPUTNAME) $(RESOURCEBINARY)
	$(FIND) "$(WORKDIR).eobjs/$(COMPILEPATH)" -name "*.o" -exec $(RM) -rf {} \;
	$(FIND) "$(WORKDIR).eobjs/$(COMPILEPATH)" -name "*.d" -exec $(RM) -rf {} \;

SOURCES := $(wildcard *.cpp) $(wildcard Platforms/General/*.cpp)
include $(addsuffix /Makefile,$(SYSTEMS) $(EXTENSIONS))
include Bridges/$(PLATFORM)-$(GRAPHICS)/Makefile

#This does not work, use a for loop and prepend it to each one not the whole string
OBJECTS := $(addprefix $(OBJDIR)/,$(patsubst %.m, %.o, $(patsubst %.cpp, %.o, $(patsubst %.c, %.o, $(SOURCES)))))
#RCFILES := $(addprefix $(WORKDIR),$(RESOURCES))
DEPENDS := $(OBJECTS:.o=.d)

SOURCES += $(addprefix $(SHARED_SRC_DIR),$(SHARED_SOURCES))
OBJECTS += $(addprefix $(OBJDIR)/shared/,$(SHARED_SOURCES:.cpp=.o))
DEPENDS += $(addprefix $(OBJDIR)/shared/,$(SHARED_SOURCES:.cpp=.d))

OBJDIRS := $(sort $(dir $(OBJECTS) $(RCFILES)))

ifeq ($(RESOURCES),)
else
	RESOURCEBINARY := $(OBJDIR)/resources.res
endif

############
# building #
############

write_src_files:
	@echo "$(SOURCES)" > $(WORKDIR)/sources.list

android_game: write_src_files
	@cd ../../android && BASENAME=$(BASENAME) ./gradlew $(GRADLE_TARGET)
	@cp $(APK_OUT)/$(BASENAME) $(OUTPUTNAME)

compile_game: print_flags $(OBJECTS) $(RCFILES) $(RESOURCEBINARY) $(DEPENDENCIES)
	@echo "Linking $(OUTPUTNAME)"
	@$(CXX) $(LDFLAGS) -o "$(OUTPUTNAME)" $(OBJECTS) $(RESOURCEBINARY) $(LDLIBS)
	@echo Built to "$(OUTPUTNAME)"

# GCC will figure out dependencies and write out makefile rules in %.d when they change
# -MMD outputs dependencies to %.d as a side effect of compilation, ignoring system headers
# -MP gives phony rules for non-target files, avoiding problems with missing files
$(OBJDIR)/%.o: %.cpp | $(OBJDIRS)
	@echo [$(CXX)] $<
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDES) -MMD -MP -c -o $(OBJDIR)/$*.o $<

$(OBJDIR)/shared/%.o: $(SHARED_SRC_DIR)/%.cpp | $(OBJDIRS)
	@echo [$(CXX)] $<
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDES) -MMD -MP -c -o $(OBJDIR)/shared/$*.o $<

$(OBJDIR)/%.o: %.c | $(OBJDIRS)
	@echo [$(CC)] $<
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) -MMD -MP -c -o $(OBJDIR)/$*.o $<

$(OBJDIR)/%.o: %.m | $(OBJDIRS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) -MMD -MP -c -o $(OBJDIR)/$*.o $<

$(OBJDIR)/resources.res: $(RCFILES) GENERATED_FILE
	echo // GENERATED RESOURCE FILE FRONTEND > $(OBJDIR)/resources.rc
	for res in $(RESOURCES); do echo "#include \"$$res\"" >> $(OBJDIR)/resources.rc; done
	@$(WINDRES) -o $@ -I. -I$(WORKDIR) $(OBJDIR)/resources.rc -O coff $(RCFLAGS)
GENERATED_FILE:

$(OBJDIR)/%.rc: %.rc
	@echo [$(WINDRES)] $<
	@$(WINDRES) -o $@ -I. -I$(WORKDIR) $^ -O rc $(RCFLAGS)

$(OBJDIRS):
	@$(MKDIR) -p $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif
