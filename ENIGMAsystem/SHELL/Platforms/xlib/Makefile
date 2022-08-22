OS := $(shell uname -s)
ifeq ($(OS), Linux)
	override CXXFLAGS += $(shell pkg-config x11 --cflags) -IPlatforms/xlib/Linux/
	override CFLAGS += $(shell pkg-config x11 --cflags) -IPlatforms/xlib/Linux/
	override LDLIBS += $(shell pkg-config x11 --libs) -lXrandr -lXinerama
else ifeq ($(OS), FreeBSD)
	override CXXFLAGS += -IPlatforms/xlib/BSD/
	override CFLAGS += -IPlatforms/xlib/BSD/
else ifeq ($(OS), DragonFly)
	override CXXFLAGS += -IPlatforms/xlib/BSD/
	override CFLAGS += -IPlatforms/xlib/BSD/
else ifeq ($(OS), OpenBSD)
	override CXXFLAGS += -IPlatforms/xlib/BSD/
	override CFLAGS += -IPlatforms/xlib/BSD/
endif
SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)

