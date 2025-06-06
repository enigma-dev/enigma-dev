include ../Config.mk

TARGET := ../libENIGMAShared$(LIB_EXT)
SHARED_SRC_DIR := .
SHARED_SOURCES := $(call rwildcard,event_reader,*.cpp) $(call rwildcard,eyaml,*.cpp) $(call rwildcard,libpng-util,*.cpp) $(call rwildcard,rectpacker,*.cpp) $(call rwildcard,ProtoYaml,*.cpp)
PROTO_DIR := ./protos/.eobjs

CXXFLAGS += -fPIC -I../CompilerSource -I$(PROTO_DIR) -fexceptions
LDFLAGS += -shared -lz -lpng -lyaml-cpp -L../ -lProtocols$(SUFFIX) -lprotobuf -fexceptions

ifeq ($(OS), Darwin)
	SDLMIXERREPOS := $(shell cd SDL_mixer;cd build-scripts;../configure --prefix=/usr/local;make;sudo make install;cd ../..;sudo chmod -R 755 /usr/local)
	override LDFLAGS += -framework CoreGraphics -framework CoreFoundation -framework AppKit
	override LDLIBS += -framework CoreGraphics -framework CoreFoundation -framework AppKit
	override CXXFLAGS += -I/usr/local/include -I/usr/local/include/SDL2 -I/opt/homebrew/include -I/opt/homebrew/include/SDL2 -ObjC++
	override CFLAGS += -I/usr/local/include -I/usr/local/include/SDL2 -I/opt/homebrew/include -I/opt/homebrew/include/SDL2 -ObjC++
	ifeq ($(shell uname -m), arm64)
 		override LDFLAGS += "/opt/homebrew/lib/libSDL2.a" "/opt/local/lib/libz.a" "/opt/local/lib/libpng.a" -liconv -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-weak_framework,CoreHaptics -Wl,-weak_framework,GameController -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal -framework CoreFoundation -framework CoreGraphics -framework AppKit -Xlinker -no_warn_duplicate_libraries
 		override LDLIBS += "/opt/homebrew/lib/libSDL2.a" "/opt/local/lib/libz.a" "/opt/local/lib/libpng.a" -liconv -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-weak_framework,CoreHaptics -Wl,-weak_framework,GameController -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal -framework CoreFoundation -framework CoreGraphics -framework AppKit -Xlinker -no_warn_duplicate_libraries
 	else
 		override LDFLAGS += "/usr/local/lib/libSDL2.a" "/opt/local/lib/libz.a" "/opt/local/lib/libpng.a" -liconv -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-weak_framework,CoreHaptics -Wl,-weak_framework,GameController -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal -framework CoreFoundation -framework CoreGraphics -framework AppKit -Xlinker -no_warn_duplicate_libraries
 		override LDLIBS += "/usr/local/lib/libSDL2.a" "/opt/local/lib/libz.a" "/opt/local/lib/libpng.a" -liconv -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-weak_framework,CoreHaptics -Wl,-weak_framework,GameController -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal -framework CoreFoundation -framework CoreGraphics -framework AppKit -Xlinker -no_warn_duplicate_libraries
 	endif
	LDLGREPOS := $(shell chmod +x libdlgmod/build.sh;libdlgmod/build.sh;chmod 755 libdlgmod/libdlgmod.dylib)
else ifeq ($(OS), Linux)
	override CXXFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override CFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override LDFLAGS += $(shell pkg-config --libs x11) $(shell pkg-config --libs sdl2) -lpthread -lz -lm -lpng -lxcb -lXrandr -lXinerama -lXext
	LDLGREPOS := $(shell chmod +x libdlgmod/build.sh;libdlgmod/build.sh;chmod 755 libdlgmod/libdlgmod.so)
else ifeq ($(OS), FreeBSD)  
	override CXXFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override CFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override LDFLAGS += $(shell pkg-config --libs x11) $(shell pkg-config --libs sdl2) -lc -lpthread -lelf -lkvm -lz -lm -linotify -lm -lpng -lxcb -lXrandr -lXinerama -lXext
	LDLGREPOS := $(shell chmod +x libdlgmod/build.sh;libdlgmod/build.sh;chmod 755 libdlgmod/libdlgmod.so)
else ifeq ($(OS), DragonFly)
	override CXXFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override CFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override LDFLAGS += $(shell pkg-config --libs x11) $(shell pkg-config --libs sdl2) -lc -lpthread -lkvm -lz -lm -linotify -lm -lpng -lxcb -lXrandr -lXinerama -lXext
	LDLGREPOS := $(shell chmod +x libdlgmod/build.sh;libdlgmod/build.sh;chmod 755 libdlgmod/libdlgmod.so)
else ifeq ($(OS), OpenBSD)
	ALUREREPOS := $(shell cd "alure";cmake .;make;cd ..)
	override CXXFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override CFLAGS += $(shell pkg-config --cflags x11) $(shell pkg-config --cflags sdl2)
	override LDFLAGS += $(shell pkg-config --libs x11) $(shell pkg-config --libs sdl2) -lc -lpthread -lkvm -lz -lm -linotify -lm -lpng -lxcb -lXrandr -lXinerama -lXext
	LDLGREPOS := $(shell chmod +x libdlgmod/build.sh;libdlgmod/build.sh;chmod 755 libdlgmod/libdlgmod.so)
else
	override CXXFLAGS += $(shell pkg-config --cflags sdl2)
	override CFLAGS += $(shell pkg-config --cflags sdl2)
	override LDLIBS += -lmingw32 -lshell32 -lole32 -luuid -lsetupapi -lImm32 -lversion -loleaut32 -lwinmm -luser32 -lntdll $(shell pkg-config --libs sdl2)
	override LDFLAGS += -lmingw32 -lshell32 -lole32 -luuid -lsetupapi -lImm32 -lversion -loleaut32 -lwinmm -luser32 -lntdll $(shell pkg-config --libs sdl2)
	LDLGREPOS := $(shell if [ -f "libdlgmod/libdlgmod.dll" ]; then echo '"libdlgmod.dll" exists'; else chmod +x libdlgmod/build.sh;libdlgmod/build.sh;chmod 755 libdlgmod/libdlgmod.dll; fi)
endif

ifeq ($(UNIX_BASED), true)
else
	STIGMA_LAUNCHER := $(shell chmod +x ../STIGMA.sh;../STIGMA.sh)
endif
override CXXFLAGS += -std=c++17 -DPROCESS_GUIWINDOW_IMPL -DUSE_SDL_POLLEVENT -DNULLIFY_STDERR $(shell pkg-config --cflags sdl2)
override CFLAGS += -std=c++17 -DPROCESS_GUIWINDOW_IMPL -DUSE_SDL_POLLEVENT -DNULLIFY_STDERR $(shell pkg-config --cflags sdl2)
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
EMAKEGUI := $(shell chmod +x ../emake.sh;../emake.sh)
SHARED_SOURCES += $(call rwildcard,apiprocess,*.cpp) $(call rwildcard,apifilesystem,*.cpp) $(call rwildcard,tempdir,*.cpp)

include ../Default.mk
