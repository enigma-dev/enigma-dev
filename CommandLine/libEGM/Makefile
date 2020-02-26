include ../../Config.mk

TARGET := ../../libEGM$(LIB_EXT)
SOURCES := $(call rwildcard,$(SRC_DIR),*.cpp)
SHARED_SRC_DIR := ../../shared
PROTO_DIR := $(SHARED_SRC_DIR)/protos/.eobjs

#if gcc >= 8 use std::filesystem instead of boost
ifeq ($(OS), Darwin)
	CXXFLAGS += -std=c++17
else ifeq ($(shell expr $(GCCVER) \>= 8), 1)
	CXXFLAGS += -std=c++17
	FS_LIBS=-lstdc++fs
else
	CXXFLAGS += -std=c++14 -DUSE_BOOST_FS
	ifeq ($(OS), Linux)
		FS_LIBS=-lboost_system -lboost_filesystem
	else ifeq ($(OS), Darwin)
		FS_LIBS=-lboost_system -lboost_filesystem
	else
		FS_LIBS=-lboost_system-mt -lboost_filesystem-mt
	endif
endif

CXXFLAGS += -I../ $(shell pkg-config --cflags pugixml) -I$(PROTO_DIR) -I$(SHARED_SRC_DIR) -I$(SHARED_SRC_DIR)/libpng-util -fPIC
LDFLAGS += -shared -lz $(shell pkg-config --libs-only-L pugixml) -lpugixml -lyaml-cpp -L../../ -lProtocols -lprotobuf -L$(SHARED_SRC_DIR)/libpng-util -lpng-util -lpng $(FS_LIBS)

include ../../Default.mk
