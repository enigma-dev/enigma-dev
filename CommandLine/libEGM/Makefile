include ../../Config.mk

TARGET := ../../libEGM$(LIB_EXT)
SOURCES := $(filter-out $(call rwildcard,$(SRC_DIR)/tests,*.cpp),$(call rwildcard,$(SRC_DIR),*.cpp))
SHARED_SRC_DIR := ../../shared
PROTO_DIR := $(SHARED_SRC_DIR)/protos/$(OBJ_DIR)

ifeq ($(OS), Linux)
	FS_LIBS=-lstdc++fs
endif

CXXFLAGS += -I../ -I$(PROTO_DIR) -fPIC $(PROTOBUF_CFLAGS) $(PUGI_CFLAGS) $(YAML_CFLAGS) $(PNG_CFLAGS) $(ZLIB_CFLAGS) $(FALLBACK_INC)
LDFLAGS += -shared -L../../ $(SHARED_LDFLAGS) -lProtocols -lprotobuf -lENIGMAShared $(PROTOBUF_LDFLAGS) $(PROTOBUF_LIBS) $(PUGI_LDFLAGS) $(PUGI_LIBS) $(YAML_LDFLAGS) $(YAML_LIBS) $(PNG_LDFLAGS) $(PNG_LIBS) $(ZLIB_LDFLAGS) $(ZLIB_LIBS) $(FS_LIBS)

include ../../Default.mk
