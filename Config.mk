GCCVER := $(shell gcc -dumpversion | cut -c 1)

OS := $(shell uname -s)
ifeq ($(OS), Linux)
	LIB_EXT := .so
	BIN_EXT :=
else ifeq ($(OS), Darwin)
	LIB_EXT := .dylib
	BIN_EXT :=
else
	LIB_EXT := .dll
	BIN_EXT := .exe
endif

# Global g++ flags
CXXFLAGS := -Wall -Wextra -Wpedantic -g -I.
LDFLAGS := -g

# For older travis instances
ifeq ($(shell expr $(GCCVER) \<= 6), 1)
  CXXFLAGS += -std=c++11
endif

# These will be relative to the file that includes this Makefile
SRC_DIR := .
OBJ_DIR := .eobjs

# This implements a recursive wildcard allowing us to iterate in subdirs
rwildcard=$(wildcard $1/$2) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))
