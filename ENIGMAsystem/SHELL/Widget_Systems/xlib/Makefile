SOURCES += $(wildcard Widget_Systems/xlib/*.cpp) Widget_Systems/General/WSdialogs.cpp
ifeq ($(OS), Linux)
	override LDLIBS += -lprocps
	override CXXFLAGS +=  $(shell pkg-config x11 --cflags)
	override CFLAGS += $(shell pkg-config x11 --cflags)
	override LDLIBS += $(shell pkg-config x11 --libs)
else ifeq ($(OS), FreeBSD)
	override LDLIBS += -lxcb /usr/local/lib/libX11.a /usr/local/lib/libXrandr.a /usr/local/lib/libXinerama.a /usr/local/lib/libXau.a /usr/local/lib/libXdmcp.a /usr/local/lib/libXext.a /usr/local/lib/libXrender.a -lc /usr/lib/libelf.a  /usr/lib/libkvm.a /usr/lib/libutil.a /usr/lib/libprocstat.a
else ifeq ($(OS), DragonFly)
	override LDLIBS += -lxcb /usr/local/lib/libX11.a /usr/local/lib/libXrandr.a /usr/local/lib/libXinerama.a /usr/local/lib/libXau.a /usr/local/lib/libXdmcp.a /usr/local/lib/libXext.a /usr/local/lib/libXrender.a-lc /usr/lib/libelf.a  /usr/lib/libkvm.a
else ifeq ($(OS), OpenBSD)
	override LDLIBS += -lxcb /usr/local/lib/libX11.a /usr/local/lib/libXrandr.a /usr/local/lib/libXinerama.a /usr/local/lib/libXau.a /usr/local/lib/libXdmcp.a /usr/local/lib/libXext.a /usr/local/lib/libXrender.a -lc /usr/lib/libelf.a  /usr/lib/libkvm.a
else ifeq ($(OS), Darwin)
	override CXXFLAGS +=  $(shell pkg-config x11 --cflags)
	override CFLAGS += $(shell pkg-config x11 --cflags)
	override LDLIBS += $(shell pkg-config x11 --libs)
endif
