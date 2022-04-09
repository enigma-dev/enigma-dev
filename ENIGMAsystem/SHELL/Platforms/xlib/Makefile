OS := $(shell uname -s)
ifeq ($(OS), Linux)
	override CXXFLAGS += $(shell pkg-config x11 --cflags) -IPlatforms/xlib/Linux/
	override CFLAGS += $(shell pkg-config x11 --cflags) -IPlatforms/xlib/Linux/
	override LDLIBS += $(shell pkg-config x11 --libs) -lXrandr -lXinerama
else ifeq ($(OS), FreeBSD)
	override CXXFLAGS += -IPlatforms/xlib/BSD/
	override CFLAGS += -IPlatforms/xlib/BSD/
	override LDLIBS += -lxcb /usr/local/lib/libX11.a /usr/local/lib/libXrandr.a /usr/local/lib/libXinerama.a /usr/local/lib/libXau.a /usr/local/lib/libXdmcp.a /usr/local/lib/libXext.a /usr/local/lib/libXrender.a
else ifeq ($(OS), DragonFly)
	override CXXFLAGS += -IPlatforms/xlib/BSD/
	override CFLAGS += -IPlatforms/xlib/BSD/
	override LDLIBS += -lxcb /usr/local/lib/libX11.a /usr/local/lib/libXrandr.a /usr/local/lib/libXinerama.a /usr/local/lib/libXau.a /usr/local/lib/libXdmcp.a /usr/local/lib/libXext.a /usr/local/lib/libXrender.a
else ifeq ($(OS), OpenBSD)
	override CXXFLAGS += -IPlatforms/xlib/BSD/
	override CFLAGS += -IPlatforms/xlib/BSD/
	override LDLIBS += -lxcb /usr/local/lib/libX11.a /usr/local/lib/libXrandr.a /usr/local/lib/libXinerama.a /usr/local/lib/libXau.a /usr/local/lib/libXdmcp.a /usr/local/lib/libXext.a /usr/local/lib/libXrender.a
endif
SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)

