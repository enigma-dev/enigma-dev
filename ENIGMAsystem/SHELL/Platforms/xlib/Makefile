include Platforms/General/Makefile

OS := $(shell uname -s)
ifeq ($(OS), Linux)
	SOURCES += $(wildcard Platforms/xlib/Linux/*.cpp)
	override CXXFLAGS += -IPlatforms/xlib/Linux/
	override CFLAGS += -IPlatforms/xlib/Linux/
else ifeq ($(OS), FreeBSD)
	SOURCES += $(wildcard Platforms/xlib/BSD/*.cpp)
	override CXXFLAGS += -IPlatforms/xlib/BSD/
	override CFLAGS += -IPlatforms/xlib/BSD/
else ifeq ($(OS), DragonFly)
	SOURCES += $(wildcard Platforms/xlib/BSD/*.cpp)
	override CXXFLAGS += -IPlatforms/xlib/BSD/
	override CFLAGS += -IPlatforms/xlib/BSD/
endif
SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp) Platforms/General/PFshell/PFshell.cpp
override CXXFLAGS += $(shell pkg-config x11 --cflags)
override CFLAGS += $(shell pkg-config x11 --cflags)
override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread
# RandR/Xinerama are optional (see XDisplayGetters.cpp's fallback).
ifeq ($(shell pkg-config --exists xrandr xinerama && echo yes),yes)
	override LDLIBS += -lXrandr -lXinerama
endif
