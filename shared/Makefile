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
	override LDFLAGS += -lz -lpng $(shell pkg-config x11 --libs) -lXrandr -lXinerama
else ifeq ($(OS), FreeBSD)  
	override LDFLAGS += /usr/lib/libprocstat.a /usr/lib/libutil.a /usr/lib/libkvm.a /usr/lib/libc.a /usr/lib/libelf.a /usr/lib/libz.a /usr/lib/libdl.a /usr/local/lib/libSDL2.a /usr/lib/libusbhid.a /usr/lib/libc++.a /usr/lib/libcxxrt.a /usr/local/lib/libinotify.a /usr/lib/libm.a /usr/lib/libusbhid.a /usr/lib/librt.a /usr/lib/libthr.a /usr/local/lib/libpng.a -lxcb /usr/local/lib/libX11.a /usr/local/lib/libXrandr.a /usr/local/lib/libXinerama.a /usr/local/lib/libXau.a /usr/local/lib/libXdmcp.a /usr/local/lib/libXext.a /usr/local/lib/libXrender.a
else ifeq ($(OS), DragonFly)
	override LDFLAGS += /usr/lib/libkvm.a /usr/lib/libc.a /usr/lib/libelf.a /usr/lib/libz.a /usr/lib/libdl.a /usr/local/lib/libSDL2.a /usr/lib/libusbhid.a /usr/lib/libc++.a /usr/lib/libcxxrt.a /usr/local/lib/libinotify.a /usr/lib/libm.a /usr/lib/libusbhid.a /usr/lib/librt.a /usr/lib/libthr.a /usr/local/lib/libpng.a -lxcb /usr/local/lib/libX11.a /usr/local/lib/libXrandr.a /usr/local/lib/libXinerama.a /usr/local/lib/libXau.a /usr/local/lib/libXdmcp.a /usr/local/lib/libXext.a /usr/local/lib/libXrender.a
else ifeq ($(OS), OpenBSD)
	override LDFLAGS += /usr/lib/libkvm.a /usr/lib/libc.a /usr/lib/libelf.a /usr/lib/libz.a /usr/lib/libdl.a /usr/local/lib/libSDL2.a /usr/lib/libusbhid.a /usr/lib/libc++.a /usr/lib/libcxxrt.a /usr/local/lib/libinotify.a /usr/lib/libm.a /usr/lib/libusbhid.a /usr/lib/librt.a /usr/lib/libthr.a /usr/local/lib/libpng.a -lxcb /usr/local/lib/libX11.a /usr/local/lib/libXrandr.a /usr/local/lib/libXinerama.a /usr/local/lib/libXau.a /usr/local/lib/libXdmcp.a /usr/local/lib/libXext.a /usr/local/lib/libXrender.a
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
