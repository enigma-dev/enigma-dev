include ../Config.mk

TARGET := ../libENIGMAShared$(LIB_EXT)
SHARED_SRC_DIR := .
SHARED_SOURCES := $(call rwildcard,event_reader,*.cpp) $(call rwildcard,eyaml,*.cpp) $(call rwildcard,libpng-util,*.cpp) $(call rwildcard,rectpacker,*.cpp) $(call rwildcard,ProtoYaml,*.cpp)
PROTO_DIR := ./protos/.eobjs

CXXFLAGS += -fPIC -I../CompilerSource -I$(PROTO_DIR)
LDFLAGS += -shared -lpng -lyaml-cpp -L../ -lProtocols$(SUFFIX) -lprotobuf

ifeq ($(UNIX_BASED), true)
	ifeq ($(OS), Darwin)
		override LDFLAGS += -framework Cocoa -framework CoreFoundation -framework CoreGraphics
		override CXXFLAGS += -ObjC++
		override CFLAGS += -ObjC++
	else ifeq ($(OS), Linux)
		override LDFLAGS += -lprocps $(shell pkg-config --libs x11)
		override CXXFLAGS += $(shell pkg-config --cflags x11)
		override CFLAGS += $(shell pkg-config --cflags x11)
	else ifeq ($(OS), FreeBSD)
		override LDFLAGS += -lprocstat -lutil -lc $(shell pkg-config --libs x11)
		override CXXFLAGS += $(shell pkg-config --cflags x11)
		override CFLAGS += $(shell pkg-config --cflags x11)
	else ifeq ($(OS), DragonFly)
		override LDFLAGS += -lkvm -lc $(shell pkg-config --libs x11)
		override CXXFLAGS += $(shell pkg-config --cflags x11)
		override CFLAGS += $(shell pkg-config --cflags x11)
	endif
endif

ifeq ($(UNIX_BASED), false)
	APIPROCESS := $(shell APIPROCESS=process32.h;if test -f "$APIPROCESS"; then echo $APIPROCESS exists;else "$(dirname $MSYSTEM_PREFIX)/msys2_shell.cmd" -defterm -mingw32 -no-start -here -lc "g++ apiprocess/process.cpp -o apiprocess/process32.exe -std=c++17 -static-libgcc -static-libstdc++ -static -m32";fi)
	APIPROCESS += $(shell APIPROCESS=process64.h;if test -f "$APIPROCESS"; then echo $APIPROCESS exists;else "$(dirname $MSYSTEM_PREFIX)/msys2_shell.cmd" -defterm -mingw64 -no-start -here -lc "g++ apiprocess/process.cpp -o apiprocess/process64.exe -std=c++17 -static-libgcc -static-libstdc++ -static -m64";fi)
	APIPROCESS += $(shell APIPROCESS=process32.h;if test -f "$APIPROCESS"; then echo $APIPROCESS exists;else cd apiprocess;xxd -i process32 > process32.h;fi)
	APIPROCESS += $(shell APIPROCESS=process64.h;if test -f "$APIPROCESS"; then echo $APIPROCESS exists;else cd apiprocess;xxd -i process64 > process64.h;fi)
	APIPROCESS += $(shell rm -f "apiprocess/process32.exe" "apiprocess/process64.exe")
	override CXXFLAGS += -DXPROCESS_WIN32EXE_INCLUDES -std=c++17
	override CFLAGS += -DXPROCESS_WIN32EXE_INCLUDES -std=c++17
endif

override CXXFLAGS += -std=c++17
override CFLAGS += -std=c++17
ifeq ($(UNIX_BASED), true)
	WEBMREPOS := $(shell cd libwebm;cmake .;make;cd ..)
endif
SHARED_SOURCES += $(call rwildcard,apiprocess,*.cpp) $(call rwildcard,apifilesystem,*.cpp) $(call rwildcard,tempdir,*.cpp)

include ../Default.mk
