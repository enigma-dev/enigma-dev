include ../Config.mk

TARGET := ../libENIGMAShared$(LIB_EXT)
SHARED_SRC_DIR := .
SHARED_SOURCES := $(call rwildcard,event_reader,*.cpp) $(call rwildcard,eyaml,*.cpp) $(call rwildcard,libpng-util,*.cpp) $(call rwildcard,rectpacker,*.cpp) $(call rwildcard,ProtoYaml,*.cpp)
PROTO_DIR := ./protos/.eobjs

CXXFLAGS += -fPIC -I../CompilerSource -I$(PROTO_DIR) -fexceptions
LDFLAGS += -shared -lz -lpng -lyaml-cpp -L../ -lProtocols$(SUFFIX) -lprotobuf -fexceptions

ifeq ($(OS), Darwin)
	SDLMIXERREPOS := $(shell cd SDL_mixer;mkdir build;cd build;../configure --prefix="/opt/local/lib";make;make install;cd ../..)
	override LDFLAGS += -framework CoreGraphics -framework CoreFoundation -framework AppKit
	override LDLIBS += -framework CoreGraphics -framework CoreFoundation -framework AppKit
	override CXXFLAGS += -ObjC++
	override CFLAGS += -ObjC++
else ifeq ($(OS), Linux)
	override CXXFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override CFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override LDFLAGS += $(shell pkg-config --libs x11) $(shell pkg-config --libs sdl2) -lpthread -lz -lm -lpng -lxcb -lXrandr -lXinerama -lXext
else ifeq ($(OS), FreeBSD)  
	override CXXFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override CFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override LDFLAGS += $(shell pkg-config --libs x11) $(shell pkg-config --libs sdl2) -lc -lpthread -lelf -lkvm -lz -lm -linotify -lm -lpng -lxcb -lXrandr -lXinerama -lXext
else ifeq ($(OS), DragonFly)
	override CXXFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override CFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override LDFLAGS += $(shell pkg-config --libs x11) $(shell pkg-config --libs sdl2) -lc -lpthread -lkvm -lz -lm -linotify -lm -lpng -lxcb -lXrandr -lXinerama -lXext
else ifeq ($(OS), OpenBSD)
	ALUREREPOS := $(shell cd "alure";cmake .;make;cd ..)
	override CXXFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override CFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override LDFLAGS += $(shell pkg-config --libs x11) $(shell pkg-config --libs sdl2) -lc -lpthread -lkvm -lz -lm -linotify -lm -lpng -lxcb -lXrandr -lXinerama -lXext
endif

override CXXFLAGS += -std=c++17 -DPROCESS_GUIWINDOW_IMPL
override CFLAGS += -std=c++17 -DPROCESS_GUIWINDOW_IMPL
ifeq ($(UNIX_BASED), true)
	WEBMREPOS := $(shell cd libwebm;cmake .;make;cd ..)
	ifeq ($(OS), Darwin)
	else
		LSVGREPOS := $(shell cd "lunasvg";cmake .;make;cd ..)
		LSVGREPOS += $(shell chmod +x ../ENIGMAsystem/SHELL/Widget_Systems/SDL/imgui.sh)
	endif
else
	LDFLAGS += -lshell32 -lole32 -luuid -lntdll
endif
SHARED_SOURCES += $(call rwildcard,apiprocess,*.cpp) $(call rwildcard,apifilesystem,*.cpp) $(call rwildcard,tempdir,*.cpp)

include ../Default.mk
