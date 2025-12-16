include ../Config.mk

TARGET := ../libENIGMAShared$(LIB_EXT)
SHARED_SRC_DIR := .
SHARED_SOURCES := $(call rwildcard,event_reader,*.cpp) $(call rwildcard,eyaml,*.cpp) $(call rwildcard,libpng-util,*.cpp) $(call rwildcard,rectpacker,*.cpp) $(call rwildcard,ProtoYaml,*.cpp)
PROTO_DIR := ./protos/.eobjs

CXXFLAGS += -fPIC -I../CompilerSource -I$(PROTO_DIR) $(PROTOBUF_CFLAGS) $(FALLBACK_INC) $(YAML_CFLAGS) $(PNG_CFLAGS)
LDFLAGS += -shared -L../ $(SHARED_LDFLAGS) -lProtocols $(PROTOBUF_LDFLAGS) $(PROTOBUF_LIBS) $(YAML_LDFLAGS) $(YAML_LIBS) $(PNG_LDFLAGS) $(PNG_LIBS)

include ../Default.mk
