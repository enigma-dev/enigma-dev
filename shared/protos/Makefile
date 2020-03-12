include ../../Config.mk

PROTO_SOURCES := $(wildcard *.proto)
SOURCES_GRPC :=
ifeq ($(CLI_ENABLE_SERVER), TRUE)
	SOURCES_GRPC += server.proto
	LDFLAGS += -lgrpc++
	CXXFLAGS += -D_WIN32_WINNT=0x0600

	GRPC_CPP_PLUGIN_PATH ?= $(shell which grpc_cpp_plugin)$(BIN_EXT)
else
	# older protobuf does not even support the RPC syntax
	# so don't even run protoc on server.proto when off
	PROTO_SOURCES := $(filter-out server.proto,$(PROTO_SOURCES))
endif

TARGET := ../../libProtocols$(LIB_EXT)

CXXFLAGS += -I$(OBJ_DIR) -fPIC
LDFLAGS += -shared -lprotobuf

GENERATED := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.cc) $(PROTO_SOURCES:.proto=.pb.h) $(SOURCES_GRPC:.proto=.grpc.pb.cc) $(SOURCES_GRPC:.proto=.grpc.pb.h))
OBJECTS := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.o) $(SOURCES_GRPC:.proto=.grpc.pb.o))

include ../../Default.mk


