include ../Config.mk

# "Shared" includes.
SHARED_SRC_DIR := ../shared

BASE = compileEGMf
ifeq ($(OS), Linux)
	TARGET := ../lib$(BASE).so
else ifeq ($(OS), Darwin)
	TARGET := ../lib$(BASE).dylib
else
	TARGET := ../$(BASE).dll
	LDFLAGS += -static-libgcc
endif

PROTO_DIR := $(SHARED_SRC_DIR)/protos
CXXFLAGS += -fPIC -I./JDI/src -I$(SHARED_SRC_DIR) -I$(SHARED_SRC_DIR)/libpng-util -I$(PROTO_DIR)/.eobjs $(addprefix -I$(SHARED_SRC_DIR)/, $(SHARED_INCLUDES))
LDFLAGS += -shared -g -L../ -Wl,-rpath,./ -lProtocols -lprotobuf -lENIGMAShared -lyaml-cpp -lz 
SOURCES := $(filter-out ./standalone_main.cpp, $(call rwildcard,.,*.cpp))

include ../Default.mk
