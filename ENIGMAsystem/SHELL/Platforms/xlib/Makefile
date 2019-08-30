GLIB_CFLAGS=$(shell pkg-config --cflags glib-2.0)
GLIB_LIBS=$(shell pkg-config --libs glib-2.0)
XLIB_CFLAGS=-I /usr/X11R6/include
XLIB_LIBS=-L /usr/X11R6/lib 
GD_LIBS=-lgd

SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override CXXFLAGS += ${GLIB_CFLAGS} ${XLIB_CFLAGS}
override LDLIBS += -lz -lpthread -lX11 ${GLIB_LIBS} ${XLIB_LIBS} ${GD_LIBS}
