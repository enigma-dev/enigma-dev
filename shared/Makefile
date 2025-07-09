include ../Config.mk

TARGET := ../libENIGMAShared$(LIB_EXT)
SHARED_SRC_DIR := .
SHARED_SOURCES := $(call rwildcard,event_reader,*.cpp) $(call rwildcard,eyaml,*.cpp) $(call rwildcard,libpng-util,*.cpp) $(call rwildcard,rectpacker,*.cpp) $(call rwildcard,ProtoYaml,*.cpp) $(call rwildcard,apifilesystem,*.cpp)
PROTO_DIR := ./protos/.eobjs

CXXFLAGS += -fPIC -I../CompilerSource -I$(PROTO_DIR)
LDFLAGS += -shared -lpng -lyaml-cpp -L../ -lProtocols -lprotobuf
ifeq ($(UNIX_BASED), true)
else
	override LDFLAGS += -lshell32 -lole32 -luuid
	override LDLIBS += -lshell32 -lole32 -luuid
endif
CHMOD_IMGUIFD := $(shell chmod u+x ../ENIGMAsystem/SHELL/Widget_Systems/SDL/imgui.sh)

include ../Default.mk
