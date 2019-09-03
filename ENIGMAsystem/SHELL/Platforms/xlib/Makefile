SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override CXXFLAGS += -I/usr/X11R6/include -I/usr/include -I/usr/include/libpng16
override CFLAGS += -I/usr/X11R6/include -I/usr/include -I/usr/include/libpng16
override LDLIBS += -Wl,-Bstatic -lm -lz -ltiff -lpng16 -lz -Wl,-Bdynamic -lz -Wl,-Bstatic -lgd -Wl,-Bdynamic -lpthread -lX11
override LDFLAGS += -L/usr/X11R6/lib -L/usr/lib
