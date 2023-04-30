include ../../Config.mk

PROTO_SOURCES := $(wildcard *.proto) $(wildcard Configuration/*.proto)
SOURCES_GRPC := server.proto

CXXFLAGS += $(shell pkg-config --cflags protobuf grpc++) -I$(OBJ_DIR) -static -fPIC
LDFLAG += $(shell pkg-config --libs protobuf grpc++) -shared

ifeq ($(UNIX_BASED), true)
	GRPC_CPP_PLUGIN_PATH ?= $(shell which grpc_cpp_plugin)
else
	GRPC_CPP_PLUGIN_PATH ?= $(shell which grpc_cpp_plugin).exe
	CXXFLAGS += -D_WIN32_WINNT=0x0600
endif

TARGET := ../../libProtocols$(LIB_EXT)

GENERATED := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.cc) $(PROTO_SOURCES:.proto=.pb.h) $(SOURCES_GRPC:.proto=.grpc.pb.cc) $(SOURCES_GRPC:.proto=.grpc.pb.h))
OBJECTS := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.o) $(SOURCES_GRPC:.proto=.grpc.pb.o))

include ../../Default.mk
