include Platforms/General/Makefile

OS := $(shell uname -s)
ifeq ($(OS), Linux)
	SOURCES += $(wildcard Platforms/General/POSIX/Linux/*.cpp)
	SOURCES += $(wildcard Platforms/xlib/Linux/*.cpp)
	override CXXFLAGS += -IPlatforms/xlib/Linux/
	override CFLAGS += -IPlatforms/xlib/Linux/
else ifeq ($(OS), FreeBSD)
	SOURCES += $(wildcard Platforms/General/POSIX/BSD/*.cpp)
	SOURCES += $(wildcard Platforms/xlib/BSD/*.cpp)
	override CXXFLAGS += -IPlatforms/xlib/BSD/
	override CFLAGS += -IPlatforms/xlib/BSD/
else ifeq ($(OS), DragonFly)
	SOURCES += $(wildcard Platforms/General/POSIX/BSD/*.cpp)
	SOURCES += $(wildcard Platforms/xlib/BSD/*.cpp)
	override CXXFLAGS += -IPlatforms/xlib/BSD/
	override CFLAGS += -IPlatforms/xlib/BSD/
endif
SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp) Platforms/General/PFshell/PFshell.cpp
override CXXFLAGS += $(shell pkg-config x11 --cflags)
override CFLAGS += $(shell pkg-config x11 --cflags)
override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread -lXrandr -lXinerama

