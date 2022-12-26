include ../../Config.mk

PROTO_SOURCES := $(wildcard *.proto) $(wildcard Configuration/*.proto)
SOURCES_GRPC := server.proto
LDFLAGS += -lgrpc++ --Wno-allow-shlib-undefined
ifeq ($(UNIX_BASED), true)
	GRPC_CPP_PLUGIN_PATH ?= $(shell which grpc_cpp_plugin)
else
	GRPC_CPP_PLUGIN_PATH ?= $(shell which grpc_cpp_plugin).exe
	CXXFLAGS += -D_WIN32_WINNT=0x0600
endif

TARGET := ../../libProtocols$(LIB_EXT)

CXXFLAGS += -I$(OBJ_DIR) -fPIC
LDFLAGS += -shared -lprotobuf

GENERATED := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.cc) $(PROTO_SOURCES:.proto=.pb.h) $(SOURCES_GRPC:.proto=.grpc.pb.cc) $(SOURCES_GRPC:.proto=.grpc.pb.h))
OBJECTS := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.o) $(SOURCES_GRPC:.proto=.grpc.pb.o))

include ../../Default.mk
