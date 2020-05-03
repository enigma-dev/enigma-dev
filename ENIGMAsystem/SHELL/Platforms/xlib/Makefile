SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp) Platforms/General/PFshell/PFshell.cpp
override CXXFLAGS += $(shell pkg-config x11 --cflags)
override CFLAGS += $(shell pkg-config x11 --cflags)
override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread -lXrandr -lXinerama
