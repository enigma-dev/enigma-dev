SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override CXXFLAGS +=  -I/usr/X11R6/include -I/usr/include
override CFLAGS += -I/usr/X11R6/include -I/usr/include
override LDLIBS += -lz -lpthread -lX11
override LDLIBS += -L/usr/X11R6/lib -L/usr/lib
