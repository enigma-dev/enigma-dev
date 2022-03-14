SHARED_SRC_DIR := ../../shared
SOURCES += $(wildcard Widget_Systems/MacOSX/*.cpp)
SOURCES += Widget_Systems/MacOSX/cocoaDialogs.m
SOURCES += Widget_Systems/General/WSdialogs.cpp
override CFLAGS += -I$(SHARED_SRC_DIR)/apifilesystem -DUSE_STD_FILESYSTEM
override CXXFLAGS += -I$(SHARED_SRC_DIR)/apifilesystem -DUSE_STD_FILESYSTEM
override LDLIBS += -lobjc
