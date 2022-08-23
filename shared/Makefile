include ../Config.mk

TARGET := ../libENIGMAShared$(LIB_EXT)
SHARED_SRC_DIR := .
SHARED_SOURCES := $(call rwildcard,event_reader,*.cpp) $(call rwildcard,eyaml,*.cpp) $(call rwildcard,libpng-util,*.cpp) $(call rwildcard,rectpacker,*.cpp) $(call rwildcard,ProtoYaml,*.cpp)
PROTO_DIR := ./protos/.eobjs

CXXFLAGS += -fPIC -I../CompilerSource -I$(PROTO_DIR) -fexceptions
LDFLAGS += -shared -lpng -lyaml-cpp -L../ -lProtocols$(SUFFIX) -lprotobuf -fexceptions

ifeq ($(OS), Darwin)
	override LDFLAGS += -framework CoreGraphics -framework CoreFoundation -framework AppKit
	override LDLIBS += -framework CoreGraphics -framework CoreFoundation -framework AppKit
	override CXXFLAGS += -ObjC++
	override CFLAGS += -ObjC++
else ifeq ($(OS), Linux)
	override CXXFLAGS += $(shell pkg-config --cflags x11)
	override CFLAGS += $(shell pkg-config --cflags x11)
	override LDFLAGS += -lz -lpng $(shell pkg-config x11 --libs) -lXrandr -lXinerama
else ifeq ($(OS), FreeBSD)  
	override CXXFLAGS += $(shell pkg-config --cflags x11)
	override CFLAGS += $(shell pkg-config --cflags x11)
	override LDFLAGS += -lprocstat -lutil -lc -lpthread -lkvm -lz -lm -lusbhid /usr/local/lib/libSDL2.a /usr/local/lib/libinotify.a /usr/local/lib/libpng.a -lxcb -lX11 /usr/local/lib/libXrandr.a /usr/local/lib/libXinerama.a -lc
else ifeq ($(OS), DragonFly)
	override CXXFLAGS += $(shell pkg-config --cflags x11)
	override CFLAGS += $(shell pkg-config --cflags x11)
	override LDFLAGS += -lc -lpthread -lkvm -lz -lm -lusbhid /usr/local/lib/libSDL2.a /usr/lib/libm.a /usr/local/lib/libpng.a -lxcb -lX11 /usr/local/lib/libXrandr.a /usr/local/lib/libXinerama.a
else ifeq ($(OS), OpenBSD)
	override CXXFLAGS += $(shell pkg-config --cflags x11)
	override CFLAGS += $(shell pkg-config --cflags x11)
	override LDFLAGS += -lc -lpthread -lkvm -lz -lm -lusbhid /usr/local/lib/libSDL2.a /usr/local/lib/libinotify.a /usr/lib/libm.a /usr/local/lib/libpng.a -lxcb -lX11 /usr/local/lib/libXrandr.a /usr/local/lib/libXinerama.a -lc
endif

override CXXFLAGS += -std=c++17 -DPROCESS_GUIWINDOW_IMPL
override CFLAGS += -std=c++17 -DPROCESS_GUIWINDOW_IMPL
ifeq ($(UNIX_BASED), true)
	WEBMREPOS := $(shell cd libwebm;cmake .;make;cd ..)
else
	LDFLAGS += -lshell32 -lole32 -luuid
endif
SHARED_SOURCES += $(call rwildcard,lib/cproc,*.cpp) $(call rwildcard,lib/xproc,*.cpp) $(call rwildcard,apifilesystem,*.cpp) $(call rwildcard,tempdir,*.cpp)

include ../Default.mk
