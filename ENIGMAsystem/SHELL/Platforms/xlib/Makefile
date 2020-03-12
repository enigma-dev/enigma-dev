SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
ifeq ($(OS), Linux)
	override CXXFLAGS += $(shell pkg-config x11 --cflags) -IPlatforms/xlib/Linux
	override CFLAGS += $(shell pkg-config x11 --cflags) -IPlatforms/xlib/Linux
else
	override CXXFLAGS += $(shell pkg-config x11 --cflags) -IPlatforms/xlib/BSD
	override CFLAGS += $(shell pkg-config x11 --cflags) -IPlatforms/xlib/BSD
endif
override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread -lXrandr -lXinerama
