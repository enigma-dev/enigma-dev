SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override CXXFLAGS += -I/usr/X11R6/include $(shell pkg-config --cflags glib-2.0)
override LDLIBS += $(shell pkg-config --libs glib-2.0) -lz -lpthread -lX11 -lgd
override LDFLAGS += -L/usr/X11R6/lib
