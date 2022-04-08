SOURCES += $(wildcard Widget_Systems/xlib/*.cpp) Widget_Systems/General/WSdialogs.cpp
ifeq ($(OS), Linux)
	override LDLIBS += -lprocps
	override CXXFLAGS +=  $(shell pkg-config x11 --cflags)
	override CFLAGS += $(shell pkg-config x11 --cflags)
	override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread
endif
