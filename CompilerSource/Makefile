include ../Config.mk

# "Shared" includes.
SHARED_SRC_DIR := ../shared

BASE = compileEGMf

ifeq ($(OS), Darwin)
	TARGET := ../lib$(BASE).dylib
else ifeq ($(UNIX_BASED), true)
	TARGET := ../lib$(BASE).so
else
	TARGET := ../$(BASE).dll
	LDFLAGS += -static-libgcc
endif

PROTO_DIR := $(SHARED_SRC_DIR)/protos
CXXFLAGS += -fPIC -I./JDI/src -I$(PROTO_DIR)/.eobjs $(addprefix -I$(SHARED_SRC_DIR)/, $(SHARED_INCLUDES))
LDFLAGS += -shared -g -L../ -Wl,-rpath,./ -lProtocols -lprotobuf -lENIGMAShared -lz 
SOURCES := $(filter-out ./standalone_main.cpp, $(call rwildcard,.,*.cpp))

include ../Default.mk
