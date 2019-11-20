SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override CXXFLAGS +=  $(pkg-config x11 --cflags)
override CFLAGS += $(pkg-config x11 --cflags)
override LDLIBS += $(pkg-config x11 --libs)
override LDLIBS += -lz -lpthread -lX11
