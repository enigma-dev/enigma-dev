include ../Config.mk

# "Shared" includes.
SHARED_SRC_DIR := ../shared

BASE = compileEGMf

ifeq ($(OS), Darwin)
	TARGET := ../lib$(BASE)$(SUFFIX).dylib
else ifeq ($(UNIX_BASED), true)
	TARGET := ../lib$(BASE)$(SUFFIX).so
else
	TARGET := ../lib$(BASE)$(SUFFIX).dll
	LDFLAGS += -static-libgcc
endif

PROTO_DIR := $(SHARED_SRC_DIR)/protos
CXXFLAGS += -fPIC -I./JDI/src -I$(SHARED_SRC_DIR) -I$(SHARED_SRC_DIR)/libpng-util -I$(PROTO_DIR)/.eobjs $(addprefix -I$(SHARED_SRC_DIR)/, $(SHARED_INCLUDES))
LDFLAGS += -shared -g -L../ -Wl,-rpath,./ -lProtocols$(SUFFIX) -lprotobuf -lENIGMAShared$(SUFFIX) -lz
ifeq ($(OS), Linux)
	LDFLAGS += -lstdc++fs
endif
ifeq ($(OS), FreeBSD)
	LDFLAGS += -lc -lutil
endif
ifeq ($(OS), DragonFly)
	LDFLAGS += -lc -lutil -lkvm
endif
SOURCES := $(filter-out ./standalone_main.cpp, $(call rwildcard,.,*.cpp))

ifeq ($(OS), Darwin)
	CPYCOMPEGM := $(shell cp -f ../libcompileEGMf-`uname -m`-`uname -n`.dylib ../libcompileEGMf.dylib)
else ifeq ($(UNIX_BASED), true)
	CPYCOMPEGM := $(shell /bin/sh -c "cp -f ../libcompileEGMf-`uname -n`-`uname -m`.so ../libcompileEGMf.so")
else
	CPYCOMPEGM := $(shell /bin/sh -c "cp -f ../libcompileEGMf-`uname -n`-`gcc -dumpmachine | sed 's/-w.*//'`.dll ../compileEGMf.dll")
endif

include ../Default.mk
