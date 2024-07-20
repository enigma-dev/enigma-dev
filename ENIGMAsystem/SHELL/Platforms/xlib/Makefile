ifndef TARGET-WINDOWING
	TARGET-WINDOWING := X11
endif

SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
ifeq ($(OS), Darwin)
	override CXXFLAGS += -I/opt/X11/include
	override CFLAGS += -I/opt/X11/include
	override LDLIBS += -L/opt/X11/lib
endif
override CXXFLAGS += $(shell pkg-config x11 --cflags)
override CFLAGS += $(shell pkg-config x11 --cflags)
override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread -lXrandr -lXinerama
