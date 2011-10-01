SOURCES += $(wildcard Widget_Systems/GTK+/*.cpp)
CFLAGS += $(shell pkg-config --cflags gtk+-2.0)

ifeq ($(PLATFORM), xlib)
	LDLIBS += $(shell pkg-config --libs gtk+-2.0)
else ifeq ($(PLATFORM), Win32)
	LDLIBS += -lGTK+ # hah
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework GTK+
endif
