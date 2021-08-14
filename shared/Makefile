include ../Config.mk

ifeq ($(UNIX_BASED), true)
	ifeq ($(OS), Darwin)
		override LDLIBS += -framework CoreFoundation -framework CoreGraphics -framework Cocoa
		override CXXFLAGS += -ObjC++
		override CFLAGS += -ObjC++
	else ifeq ($(OS), Linux)
		override LDLIBS += -lprocps
	else ifeq ($(OS), FreeBSD)
		override LDLIBS += -lprocstat -lutil -lc
	else ifeq ($(OS), DragonFly)
		override LDLIBS += -lkvm -lutil -lc
	endif
endif

TARGET := ../libENIGMAShared$(LIB_EXT)
SHARED_SRC_DIR := .
SHARED_SOURCES := $(call rwildcard,event_reader,*.cpp) $(call rwildcard,eyaml,*.cpp) $(call rwildcard,libpng-util,*.cpp) $(call rwildcard,rectpacker,*.cpp) $(call rwildcard,ProtoYaml,*.cpp)
PROTO_DIR := ./protos/.eobjs

CXXFLAGS += -fPIC -I../CompilerSource -I$(PROTO_DIR)
LDFLAGS += -shared -lpng -lyaml-cpp -L../ -lProtocols$(SUFFIX) -lprotobuf

ifeq ($(UNIX_BASED), false)
	CROSSPROCESS := $(shell CROSSPROCESS=crossprocess32.h;if test -f "$CROSSPROCESS"; then echo $CROSSPROCESS exists;else "$(dirname $MSYSTEM_PREFIX)/msys2_shell.cmd" -defterm -mingw32 -no-start -here -lc "g++ crossprocess/crossprocess.cpp -o crossprocess/crossprocess32.exe -DXPROCESS_GUIWINDOW_IMPL -std=c++17 -static-libgcc -static-libstdc++ -static -m32";fi)
	CROSSPROCESS += $(shell CROSSPROCESS=crossprocess64.h;if test -f "$CROSSPROCESS"; then echo $CROSSPROCESS exists;else "$(dirname $MSYSTEM_PREFIX)/msys2_shell.cmd" -defterm -mingw64 -no-start -here -lc "g++ crossprocess/crossprocess.cpp -o crossprocess/crossprocess64.exe -DXPROCESS_GUIWINDOW_IMPL -std=c++17 -static-libgcc -static-libstdc++ -static -m64";fi)
	CROSSPROCESS += $(shell CROSSPROCESS=crossprocess32.h;if test -f "$CROSSPROCESS"; then echo $CROSSPROCESS exists;else cd crossprocess;xxd -i crossprocess32 > crossprocess32.h;fi)
	CROSSPROCESS += $(shell CROSSPROCESS=crossprocess64.h;if test -f "$CROSSPROCESS"; then echo $CROSSPROCESS exists;else cd crossprocess;xxd -i crossprocess64 > crossprocess64.h;fi)
	CROSSPROCESS += $(shell rm -f "crossprocess/crossprocess32.exe" "crossprocess/crossprocess64.exe")
	override CXXFLAGS += -DXPROCESS_WIN32EXE_INCLUDES -std=c++17
	override CFLAGS += -DXPROCESS_WIN32EXE_INCLUDES -std=c++17
endif

override CXXFLAGS += -DXPROCESS_GUIWINDOW_IMPL -std=c++17
override CFLAGS += -DXPROCESS_GUIWINDOW_IMPL -std=c++17
SHARED_SOURCES += $(call rwildcard,crossprocess,*.cpp) $(call rwildcard,tempdir,*.cpp)

include ../Default.mk
