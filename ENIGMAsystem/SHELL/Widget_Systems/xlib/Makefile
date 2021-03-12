SOURCES += $(wildcard Widget_Systems/xlib/*.cpp) Widget_Systems/General/WSdialogs.cpp
override CXXFLAGS +=  $(shell pkg-config x11 --cflags)
override CFLAGS += $(shell pkg-config x11 --cflags)
override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread
ifeq ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lprocps
else ifeq ($(TARGET-PLATFORM), FreeBSD)
	override LDLIBS += -lutil -lc
endif
