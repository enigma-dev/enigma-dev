GCCVER := $(shell gcc -dumpversion | cut -c 1)
OS := $(shell uname -s)

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

ifeq ($(UNIX_BASED), true)
	ARCH := $(shell uname -m)
	SUFFIX := -$(OS)-$(ARCH)
else
	ARCH := $(shell gcc -dumpmachine | sed 's/-w.*//')
	SUFFIX := -Windows-$(ARCH)
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
	CXXFLAGS += -I/opt/local/include
	CFLAGS   += -I/opt/local/include
	LDFLAGS  += -L/opt/local/lib
endif

# FreeBSD include and lib folders
ifeq ($(OS), FreeBSD)
	CXXFLAGS += -I/usr/local/include
	CFLAGS   += -I/usr/local/include
	LDFLAGS  += -L/usr/local/lib
endif

# DragonFlyBSD include and lib folders
ifeq ($(OS), DragonFly)
	CXXFLAGS += -I/usr/local/include
	CFLAGS   += -I/usr/local/include
	LDFLAGS  += -L/usr/local/lib
endif

# These will be relative to the file that includes this Makefile
SRC_DIR := .
OBJ_DIR := .eobjs

# This implements a recursive wildcard allowing us to iterate in subdirs
rwildcard=$(wildcard $1/$2) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))
