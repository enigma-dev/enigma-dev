SOURCES += Widget_Systems/xlib/zenity.cpp
SOURCES += Widget_Systems/xlib/kdialog.cpp
SOURCES += Widget_Systems/xlib/dialogs.cpp
override CXXFLAGS +=  $(shell pkg-config x11 --cflags)
override CFLAGS += $(shell pkg-config x11 --cflags)
override LDLIBS += $(shell pkg-config x11 --libs) -lz -lpthread
