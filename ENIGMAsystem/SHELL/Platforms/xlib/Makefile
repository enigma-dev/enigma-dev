SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override CXXFLAGS +=  -I/usr/X11R6/include -I/usr/include -I/usr/include/libpng16
override CFLAGS += -I/usr/X11R6/include -I/usr/include -I/usr/include/libpng16
override LDLIBS += -Wl,-Bstatic -lpng -lz -Wl,-Bdynamic -lz -lpthread -lX11
override LDLIBS += -L/usr/X11R6/lib -L/usr/lib
