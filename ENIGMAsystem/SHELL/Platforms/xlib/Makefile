SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
	override CXXFLAGS += $(shell pkg-config x11 --cflags)
	override CFLAGS += $(shell pkg-config x11 --cflags)
ifeq ($(OS), Linux)
	SOURCES += $(wildcard Platforms/xlib/Linux/*.cpp)
	override CXXFLAGS += -IPlatforms/xlib/Linux
	override CFLAGS += -IPlatforms/xlib/Linux
else ifeq ($(OS), FreeBSD)
	SOURCES += $(wildcard Platforms/xlib/BSD/*.cpp)
	override CXXFLAGS += -IPlatforms/xlib/BSD
	override CFLAGS += -IPlatforms/xlib/BSD
endif
override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread -lXrandr -lXinerama
