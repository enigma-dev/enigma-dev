include ../Config.mk

# "Shared" includes.
SHARED_SRC_DIR := ../shared
include $(SHARED_SRC_DIR)/Makefile
include $(SHARED_SRC_DIR)/eyaml/Makefile
include $(SHARED_SRC_DIR)/event_reader/Makefile
include $(SHARED_SRC_DIR)/rectpacker/Makefile

BASE = compileEGMf
ifeq ($(OS), Linux)
	TARGET := ../lib$(BASE).so
else ifeq ($(OS), FreeBSD)
	TARGET := ../lib$(BASE).so
else ifeq ($(OS), Darwin)
	TARGET := ../lib$(BASE).dylib
else
	TARGET := ../$(BASE).dll
	LDFLAGS += -static-libgcc
endif

PROTO_DIR := $(SHARED_SRC_DIR)/protos
CXXFLAGS += -fPIC -I./JDI/src -I$(SHARED_SRC_DIR) -I$(SHARED_SRC_DIR)/libpng-util -I$(PROTO_DIR)/.eobjs $(addprefix -I$(SHARED_SRC_DIR)/, $(SHARED_INCLUDES))
LDFLAGS += -shared -g -L../ -Wl,-rpath,./ -lProtocols -lprotobuf -lz -L$(SHARED_SRC_DIR)/libpng-util -lpng-util -lpng
SOURCES := $(filter-out ./standalone_main.cpp, $(call rwildcard,.,*.cpp))

include ../Default.mk
