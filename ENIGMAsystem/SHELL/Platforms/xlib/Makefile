ifeq ($(TARGET-PLATFORM), Linux)
	SOURCES += $(wildcard Platforms/xlib/Linux/*.cpp)
else ifeq ($(TARGET-PLATFORM), FreeBSD)
	SOURCES += $(wildcard Platforms/xlib/BSD/*.cpp)
endif
SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override CXXFLAGS += $(shell pkg-config x11 --cflags)
override CFLAGS += $(shell pkg-config x11 --cflags)
override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread -lXrandr -lXinerama

