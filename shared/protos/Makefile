include ../../Config.mk

PROTO_SOURCES := $(wildcard *.proto) $(wildcard Configuration/*.proto)
SOURCES_GRPC := server.proto
CXXFLAGS += -D_WIN32_WINNT=0x0600
GRPC_CPP_PLUGIN_PATH ?= $(shell which grpc_cpp_plugin)$(BIN_EXT)

TARGET := ../../libProtocols$(LIB_EXT)

CXXFLAGS += -I$(OBJ_DIR) -fPIC
LDFLAGS += -shared $(shell pkg-config --libs pkg-config --libs grpc++ protobuf)

GENERATED := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.cc) $(PROTO_SOURCES:.proto=.pb.h) $(SOURCES_GRPC:.proto=.grpc.pb.cc) $(SOURCES_GRPC:.proto=.grpc.pb.h))
OBJECTS := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.o) $(SOURCES_GRPC:.proto=.grpc.pb.o))

include ../../Default.mk
