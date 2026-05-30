GCCVER := $(shell gcc -dumpversion | cut -c 1)
OS := $(shell uname -s)

# MSYS2 MinGW reports MINGW64_NT-10.0-…, not "Windows" (see uname -s).
IS_WINDOWS :=
ifneq ($(findstring MINGW,$(OS)),)
	IS_WINDOWS := 1
else ifneq ($(findstring _NT-,$(OS)),)
	IS_WINDOWS := 1
endif

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
	LIB_EXT := .dylib
	BIN_EXT :=
else ifeq ($(UNIX_BASED), true)
	PLATFORM := xlib
	MKDIR := mkdir
	LIB_PFX := lib
	LIB_EXT := .so
	BIN_EXT :=
else
	PLATFORM := Win32
	MKDIR := mkdir.exe
	LIB_PFX := lib
	LIB_EXT := .dll
	BIN_EXT := .exe
endif

# Global g++ flags
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g -I.
LDFLAGS := -g

# Enable ASAN only if ENABLE_ASAN=1
# On Mac, ASAN is disabled by default unless explicitly enabled
ifeq ($(ENABLE_ASAN), 1)
	CXXFLAGS += -fsanitize=address
	LDFLAGS += -fsanitize=address
endif

# Code coverage (CI: COVERAGE=1)
ifeq ($(COVERAGE), 1)
	CXXFLAGS += --coverage
	LDFLAGS += --coverage
endif

# macOS: Homebrew LLVM Clang + libc++ (matches brew protobuf/grpc/abseil).
# Requires: brew install llvm  (see CI/deps/brew.packages.txt)
ifeq ($(OS), Darwin)
	CXXFLAGS += -isystem /usr/local/include -isystem /opt/homebrew/include/
	CFLAGS   += -isystem /usr/local/include -isystem /opt/homebrew/include/
	LDFLAGS  += -L/usr/local/lib -L/opt/homebrew/lib/
	# ASan with shared libraries on macOS needs dynamic symbol resolution
	ifeq ($(ENABLE_ASAN), 1)
		SHARED_LDFLAGS := -Wl,-undefined,dynamic_lookup
	endif
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

# Which search priority to use for libraries (system or local)
CUSTOM_LIB_SEARCH_PRIORITY := system

ifeq ($(CUSTOM_LIB_SEARCH_PRIORITY), local)
    LDFLAGS += -L/usr/local/lib
endif

# These will be relative to the file that includes this Makefile
SRC_DIR := .
# Use separate object directories for ASAN and non-ASAN builds to avoid conflicts.
# CI sets OBJ_DIR (e.g. .eobjs-ci-arch) so docker/native jobs do not share artifacts.
ifeq ($(ENABLE_ASAN), 1)
OBJ_DIR ?= .eobjs-asan
else
OBJ_DIR ?= .eobjs
endif

# This implements a recursive wildcard allowing us to iterate in subdirs
rwildcard=$(wildcard $1/$2) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))

# Default common settings
FALLBACK_INC :=
FALLBACK_LIB_PATH :=
FALLBACK_LDFLAGS_EXTRA :=

# Library list fallbacks
FALLBACK_LIBS_GRPC :=
FALLBACK_LIBS_PROTOBUF :=
FALLBACK_LIBS_YAML :=
FALLBACK_LIBS_PUGI :=
FALLBACK_LIBS_PNG :=
FALLBACK_LIBS_ZLIB :=

ifeq ($(OS),Linux)
  FALLBACK_INC := -I/usr/local/include
  FALLBACK_LIB_PATH := -L/usr/local/lib
  FALLBACK_LIBS_GRPC := -lgrpc++ -lgrpc -lcares -lssl -lcrypto -lpthread -ldl
  FALLBACK_LIBS_PROTOBUF := -lprotobuf
  FALLBACK_LIBS_YAML := -lyaml-cpp
  FALLBACK_LIBS_PUGI := -lpugixml
  FALLBACK_LIBS_PNG := -lpng
  FALLBACK_LIBS_ZLIB := -lz
  FALLBACK_LDFLAGS_EXTRA :=

else ifeq ($(OS),Darwin)
  FALLBACK_INC := -I$(LLVM_PREFIX)/include/c++/v1 -I$(LLVM_PREFIX)/include -I$(BREW_PREFIX)/include
  FALLBACK_LIB_PATH := -L$(LLVM_PREFIX)/lib/c++ -L$(BREW_PREFIX)/lib
  FALLBACK_LIBS_GRPC := -lgrpc++ -lgrpc -lssl -lcrypto -lcares -lpthread
  FALLBACK_LIBS_PROTOBUF := -lprotobuf
  FALLBACK_LIBS_YAML := -lyaml-cpp
  FALLBACK_LIBS_PUGI := -lpugixml
  FALLBACK_LIBS_PNG := -lpng
  FALLBACK_LIBS_ZLIB := -lz
  FALLBACK_LDFLAGS_EXTRA :=

else ifeq ($(IS_WINDOWS),1)
  MINGW_PREFIX ?= /mingw64
  FALLBACK_INC := -I$(MINGW_PREFIX)/include
  FALLBACK_LIB_PATH := -L$(MINGW_PREFIX)/lib
  FALLBACK_LIBS_GRPC := -lgrpc++ -lgrpc -lssl -lcrypto -lws2_32 -liphlpapi -lcares -lz -static-libgcc -static-libstdc++
  FALLBACK_LIBS_PROTOBUF := -lprotobuf
  FALLBACK_LIBS_YAML := -lyaml-cpp
  FALLBACK_LIBS_PUGI := -lpugixml
  FALLBACK_LIBS_PNG := -lpng
  FALLBACK_LIBS_ZLIB := -lz
  FALLBACK_LDFLAGS_EXTRA :=

else
  $(warning *** Unknown OS ($(OS)). Flags will be empty. ***)
endif

# gRPC
GRPC_PKGC_LIBS_TEST = $(shell pkg-config --libs grpc++ grpc 2>/dev/null)
ifeq ($(strip $(GRPC_PKGC_LIBS_TEST)),)
  $(info gRPC: pkg-config failed. Using fallback.)
  GRPC_CFLAGS := $(FALLBACK_INC)
  GRPC_LDFLAGS := $(FALLBACK_LIB_PATH)
  GRPC_LIBS := $(FALLBACK_LIBS_GRPC)
else
  $(info gRPC: pkg-config successful.)
  GRPC_PKGC_CFLGS = $(shell pkg-config --cflags-only-I grpc++ grpc 2>/dev/null)
  GRPC_CFLAGS := $(GRPC_PKGC_CFLGS)
  GRPC_LDFLAGS := $(filter -L%, $(GRPC_PKGC_LIBS_TEST))
  GRPC_LIBS := $(filter -l%, $(GRPC_PKGC_LIBS_TEST))
endif

# Protobuf
PROTOBUF_PKGC_LIBS_TEST = $(shell pkg-config --libs protobuf 2>/dev/null)
ifeq ($(strip $(PROTOBUF_PKGC_LIBS_TEST)),)
  $(info Protobuf: pkg-config failed. Using fallback.)
  PROTOBUF_CFLAGS := $(FALLBACK_INC)
  PROTOBUF_LDFLAGS := $(FALLBACK_LIB_PATH)
  PROTOBUF_LIBS := $(FALLBACK_LIBS_PROTOBUF)
else
  $(info Protobuf: pkg-config successful.)
  PROTOBUF_PKGC_CFLGS = $(shell pkg-config --cflags protobuf 2>/dev/null)
  PROTOBUF_CFLAGS := $(PROTOBUF_PKGC_CFLGS)
  PROTOBUF_LDFLAGS := $(filter -L%, $(PROTOBUF_PKGC_LIBS_TEST))
  PROTOBUF_LIBS := $(filter -l%, $(PROTOBUF_PKGC_LIBS_TEST))
endif

# yaml-cpp
YAML_PKGC_LIBS_TEST = $(shell pkg-config --libs yaml-cpp 2>/dev/null)
ifeq ($(strip $(YAML_PKGC_LIBS_TEST)),)
  $(info yaml-cpp: pkg-config failed. Using fallback.)
  YAML_CFLAGS := $(FALLBACK_INC)
  YAML_LDFLAGS := $(FALLBACK_LIB_PATH)
  YAML_LIBS := $(FALLBACK_LIBS_YAML)
else
  $(info yaml-cpp: pkg-config successful.)
  YAML_PKGC_CFLGS = $(shell pkg-config --cflags yaml-cpp 2>/dev/null)
  YAML_CFLAGS := $(YAML_PKGC_CFLGS)
  YAML_LDFLAGS := $(filter -L%, $(YAML_PKGC_LIBS_TEST))
  YAML_LIBS := $(filter -l%, $(YAML_PKGC_LIBS_TEST))
endif

# pugixml
PUGI_PKGC_LIBS_TEST = $(shell pkg-config --libs pugixml 2>/dev/null)
ifeq ($(strip $(PUGI_PKGC_LIBS_TEST)),)
  $(info pugixml: pkg-config failed. Using fallback.)
  PUGI_CFLAGS := $(FALLBACK_INC)
  PUGI_LDFLAGS := $(FALLBACK_LIB_PATH)
  PUGI_LIBS := $(FALLBACK_LIBS_PUGI)
else
  $(info pugixml: pkg-config successful.)
  PUGI_PKGC_CFLGS = $(shell pkg-config --cflags pugixml 2>/dev/null)
  PUGI_CFLAGS := $(PUGI_PKGC_CFLGS)
  PUGI_LDFLAGS := $(filter -L%, $(PUGI_PKGC_LIBS_TEST))
  PUGI_LIBS := $(filter -l%, $(PUGI_PKGC_LIBS_TEST))
endif

# libpng
PNG_PKGC_LIBS_TEST = $(shell pkg-config --libs libpng 2>/dev/null)
ifeq ($(strip $(PNG_PKGC_LIBS_TEST)),)
  $(info libpng: pkg-config failed. Using fallback.)
  PNG_CFLAGS := $(FALLBACK_INC)
  PNG_LDFLAGS := $(FALLBACK_LIB_PATH)
  PNG_LIBS := $(FALLBACK_LIBS_PNG)
else
  $(info libpng: pkg-config successful.)
  PNG_PKGC_CFLGS = $(shell pkg-config --cflags libpng 2>/dev/null)
  PNG_CFLAGS := $(PNG_PKGC_CFLGS)
  PNG_LDFLAGS := $(filter -L%, $(PNG_PKGC_LIBS_TEST))
  PNG_LIBS := $(filter -l%, $(PNG_PKGC_LIBS_TEST))
endif

# zlib
ZLIB_PKGC_LIBS_TEST = $(shell pkg-config --libs zlib 2>/dev/null)
ifeq ($(strip $(ZLIB_PKGC_LIBS_TEST)),)
  $(info zlib: pkg-config failed. Using fallback.)
  ZLIB_CFLAGS := $(FALLBACK_INC)
  ZLIB_LDFLAGS := $(FALLBACK_LIB_PATH)
  ZLIB_LIBS := $(FALLBACK_LIBS_ZLIB)
else
  $(info zlib: pkg-config successful.)
  ZLIB_PKGC_CFLGS = $(shell pkg-config --cflags zlib 2>/dev/null)
  ZLIB_CFLAGS := $(ZLIB_PKGC_CFLGS)
  ZLIB_LDFLAGS := $(filter -L%, $(ZLIB_PKGC_LIBS_TEST))
  ZLIB_LIBS := $(filter -l%, $(ZLIB_PKGC_LIBS_TEST))
endif
