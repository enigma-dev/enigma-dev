SOURCES += $(wildcard Widget_Systems/GTK+/*.cpp) Widget_Systems/General/WSdialogs.cpp
override LDLIBS += -lgthread-2.0

ifeq ($(PLATFORM), xlib)
	override CFLAGS += $(shell pkg-config --cflags gtk+-2.0)
	override CXXFLAGS += $(shell pkg-config --cflags gtk+-2.0)
	override LDLIBS += $(shell pkg-config --libs gtk+-2.0)
else ifeq ($(PLATFORM), Win32)
	override LDLIBS += -lGTK+ # hah
else ifeq ($(PLATFORM), Cocoa)
	override LDLIBS += -framework GTK+
endif
