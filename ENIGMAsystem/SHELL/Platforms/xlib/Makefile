SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
ifeq ($(OS), Darwin)
	override CXXFLAGS += -I/usr/X11R6/include
	override CFLAGS += -I/usr/X11R6/include
	override LDLIBS += -L/usr/X11R6/lib
endif
override CXXFLAGS += $(shell pkg-config x11 --cflags)
override CFLAGS += $(shell pkg-config x11 --cflags)
override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread -lXrandr -lXinerama
