SOURCES += $(wildcard Widget_Systems/xlib/*.cpp)
override CXXFLAGS +=  $(shell pkg-config x11 --cflags)
override CFLAGS += $(shell pkg-config x11 --cflags)
override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread
ifeq ($(OS), Linux)
	override LDLIBS += -lprocps
else ifeq ($(OS), FreeBSD)
	override LDLIBS += -lutil -lc
endif
