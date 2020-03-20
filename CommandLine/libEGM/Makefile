include ../../Config.mk

TARGET := ../../libEGM$(LIB_EXT)
SOURCES := $(call rwildcard,$(SRC_DIR),*.cpp)
SHARED_SRC_DIR := ../../shared
PROTO_DIR := $(SHARED_SRC_DIR)/protos/.eobjs

ifeq ($(OS), Linux)
	FS_LIBS=-lstdc++fs
endif

CXXFLAGS += -I../ $(shell pkg-config --cflags pugixml) -I$(PROTO_DIR) -I$(SHARED_SRC_DIR) -I$(SHARED_SRC_DIR)/libpng-util -fPIC
LDFLAGS += -shared -lz $(shell pkg-config --libs-only-L pugixml) -lpugixml -lyaml-cpp -L../../ -lProtocols -lprotobuf -L$(SHARED_SRC_DIR)/libpng-util -lpng-util -lpng $(FS_LIBS)

include ../../Default.mk
