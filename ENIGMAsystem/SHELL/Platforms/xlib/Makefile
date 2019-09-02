SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override CXXFLAGS += -I/usr/X11R6/include -I/usr/include $(shell pkg-config --cflags glib-2.0)
override CFLAGS += -I/usr/X11R6/include -I/usr/include $(shell pkg-config --cflags glib-2.0)
override LDLIBS += $(shell pkg-config --libs --static glib-2.0 libpng gdlib) -lpthread -lX11
override PKG_CONFIG_PATH += -L/usr/lib/x86_64-linux-gnu/pkgconfig/ -L/usr/lib/i386-linux-gnu/pkgconfig
override PKG_CONFIG_PATH += -L/usr/X11R6/lib -L/usr/lib 
