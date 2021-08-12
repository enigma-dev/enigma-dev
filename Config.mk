GCCVER := $(shell gcc -dumpversion | cut -c 1)
OS := $(shell uname -s)
ARCH := $(shell uname -m)
SUFFIX := -$(shell uname -n)-$(ARCH)

# Determine whether Unix-based
ifeq ($(OS), Darwin)
	UNIX_BASED := true
else ifeq ($(OS), Linux)
	UNIX_BASED := true
else ifeq ($(OS), FreeBSD)
	UNIX_BASED := true
else ifeq ($(OS), DragonFly)
	UNIX_BASED := true
else 
	UNIX_BASED := false
endif

# Determine current platform
ifeq ($(OS), Darwin)
	PLATFORM := Cocoa
	MKDIR := mkdir
	LIB_PFX := lib
	LIB_EXT := $(SUFFIX).dylib
	BIN_EXT := $(SUFFIX)
else ifeq ($(UNIX_BASED), true)
	PLATFORM := xlib
	MKDIR := mkdir
	LIB_PFX := lib
	LIB_EXT := $(SUFFIX).so
	BIN_EXT := $(SUFFIX)
else
	PLATFORM := Win32
	MKDIR := mkdir.exe
	LIB_PFX := lib
	LIB_EXT := $(SUFFIX).dll
	BIN_EXT := $(SUFFIX).exe
endif

# Global g++ flags
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g -I.
LDFLAGS := -g

# MacPorts include and lib folders
ifeq ($(OS), Darwin)
	SLIBPATH  = /opt/local/lib/
	CXXFLAGS += -I/opt/local/include
	CFLAGS   += -I/opt/local/include
	LDFLAGS  += -L/opt/local/lib
endif

# FreeBSD include and lib folders
ifeq ($(OS), FreeBSD)
	SLIBPATH  = /usr/local/lib/
	CXXFLAGS += -I/usr/local/include
	CFLAGS   += -I/usr/local/include
	LDFLAGS  += -L/usr/local/lib
endif

# DragonFlyBSD include and lib folders
ifeq ($(OS), DragonFly)
	SLIBPATH  = /usr/local/lib/
	CXXFLAGS += -I/usr/local/include
	CFLAGS   += -I/usr/local/include
	LDFLAGS  += -L/usr/local/lib
endif

ifeq ($(UNIX_BASED), true)
	ifeq ($(OS), Linux)
	else ifeq ($(GMODE), Compile)
		SOURCES += $(SLIBPATH)libX11.a $(SLIBPATH)libxcb.a $(SLIBPATH)libXau.a $(SLIBPATH)libXdmcp.a $(SLIBPATH)libXrandr.a $(SLIBPATH)libXrender.a $(SLIBPATH)libXinerama.a $(SLIBPATH)libXext.a $(SLIBPATH)libOpenGL.a $(SLIBPATH)libGLX.a $(SLIBPATH)libGLEW.a $(SLIBPATH)libGL.a $(SLIBPATH)libGLU.a $(SLIBPATH)libopenal.a $(SLIBPATH)libalure.a $(SLIBPATH)libvorbisfile.a $(SLIBPATH)libvorbis.a $(SLIBPATH)libogg.a $(SLIBPATH)libpng.a $(SLIBPATH)libz.a $(SLIBPATH)libpthread.a $(SLIBPATH)libm.a
	endif
endif

# These will be relative to the file that includes this Makefile
SRC_DIR := .
OBJ_DIR := .eobjs

# This implements a recursive wildcard allowing us to iterate in subdirs
rwildcard=$(wildcard $1/$2) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))
