GCCVER := $(shell gcc -dumpversion | cut -c 1)
OS := $(shell uname -s)
ARCH := $(shell uname -m)

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
	LIB_EXT := -$(OS)-$(ARCH).dylib
	BIN_EXT := -$(OS)-$(ARCH)
else ifeq ($(UNIX_BASED), true)
	PLATFORM := xlib
	MKDIR := mkdir
	LIB_PFX := lib
	ifeq ($(OS), Linux)
	LIB_EXT := -$(OS)-$(ARCH).so
	else ifeq ($(OS), FreeBSD)
	LIB_EXT := -$(OS)-$(ARCH).so
	else ifeq ($(OS), DragonFly)
	LIB_EXT := -$(OS)-$(ARCH).so
	endif
	BIN_EXT := -$(OS)-$(ARCH)
else
	PLATFORM := Win32
	MKDIR := mkdir.exe
	LIB_PFX := lib
	LIB_EXT := -$(OS)-$(ARCH).dll
	BIN_EXT := -$(OS)-$(ARCH).exe
endif

# Global g++ flags
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g -I.
LDFLAGS := -g

# macOS brew include and lib folders
ifeq ($(OS), Darwin)
	CXXFLAGS += -I/usr/local/include
	CFLAGS   += -I/usr/local/include
	LDFLAGS  += -L/usr/local/lib
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
