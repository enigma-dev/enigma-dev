SHARED_SRC_DIR := ../../shared
SOURCES += $(wildcard Widget_Systems/MacOSX/*.cpp)
SOURCES += Widget_Systems/MacOSX/cocoaDialogs.m
SOURCES += Widget_Systems/General/WSdialogs.cpp
override CFLAGS += -I$(SHARED_SRC_DIR)/apifilesystem -IWidget_Systems/MacOSX -DUSE_STD_FILESYSTEM $(shell pkg-config sdl2 --cflags)
override CXXFLAGS += -I$(SHARED_SRC_DIR)/apifilesystem -IWidget_Systems/MacOSX -DUSE_STD_FILESYSTEM $(shell pkg-config sdl2 --cflags)
override LDLIBS += -lobjc
