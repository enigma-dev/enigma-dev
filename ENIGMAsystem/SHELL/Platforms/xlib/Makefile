SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
ifeq ($(OS), Linux)
override CXXFLAGS += $(shell pkg-config x11 --cflags) -IPlatforms/xlib/joysticks/linux
override CFLAGS += $(shell pkg-config x11 --cflags) -IPlatforms/xlib/joysticks/linux
else
override CXXFLAGS += $(shell pkg-config x11 --cflags) -IPlatforms/xlib/joysticks/bsd
override CFLAGS += $(shell pkg-config x11 --cflags) -IPlatforms/xlib/joysticks/bsd
endif
override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread -lXrandr -lXinerama
