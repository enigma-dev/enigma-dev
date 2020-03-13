SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override CXXFLAGS += $(shell pkg-config x11 --cflags)
override CFLAGS += $(shell pkg-config x11 --cflags)
OS := $(shell uname -s)
ifeq ($(OS), Linux)
	SOURCES += $(wildcard Platforms/xlib/Linux/*.cpp)
else ifeq ($(OS), FreeBSD)
	SOURCES += $(wildcard Platforms/xlib/BSD/*.cpp)
endif
override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread -lXrandr -lXinerama
