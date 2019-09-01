SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override CXXFLAGS += $(shell pkg-config --cflags glib-2.0)
override LDLIBS += -lz -lpthread -lX11 $(shell pkg-config --libs glib-2.0) -lgd
