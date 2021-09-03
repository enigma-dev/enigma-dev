include ../Config.mk

TARGET := ../libENIGMAShared$(LIB_EXT)
SHARED_SRC_DIR := .
SHARED_SOURCES := $(call rwildcard,event_reader,*.cpp) $(call rwildcard,eyaml,*.cpp) $(call rwildcard,libpng-util,*.cpp) $(call rwildcard,rectpacker,*.cpp) $(call rwildcard,ProtoYaml,*.cpp)
PROTO_DIR := ./protos/.eobjs

CXXFLAGS += -fPIC -I../CompilerSource -I$(PROTO_DIR)
LDFLAGS += -shared -lpng -lyaml-cpp -L../ -lProtocols -lprotobuf

ifeq ($(UNIX_BASED), true)
	# if this can't go here, where do you suggest I put it then?
	WEBMREPOS := $(shell cd libwebm;cmake .;make;cd ..)
endif

include ../Default.mk
