ifndef TARGET-WINDOWING
	TARGET-WINDOWING := X11
endif

SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override CXXFLAGS += $(shell pkg-config x11 --cflags)
override CFLAGS += $(shell pkg-config x11 --cflags)
override LDLIBS += $(shell pkg-config x11 --libs) -lX11 -lz -lpthread -lXrandr -lXinerama
