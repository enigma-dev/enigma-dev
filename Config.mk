GCCVER := $(shell gcc -dumpversion | cut -c 1)

SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

OS := $(shell uname -s)
ifeq ($(OS), Linux)
	LIB_EXT := .so
	BIN_EXT :=
	UNIX_BASED := true
else ifeq ($(OS), FreeBSD)
	LIB_EXT := .so
	BIN_EXT :=
	UNIX_BASED := true
else ifeq ($(OS), Darwin)
	LIB_EXT := .dylib
	BIN_EXT :=
	UNIX_BASED := true
else 
	LIB_EXT := .dll
	BIN_EXT := .exe
	UNIX_BASED := false
endif

# Global g++ flags
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g -I.
LDFLAGS := -g

# These will be relative to the file that includes this Makefile
SRC_DIR := .
OBJ_DIR := .eobjs

# This implements a recursive wildcard allowing us to iterate in subdirs
rwildcard=$(wildcard $1/$2) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))
