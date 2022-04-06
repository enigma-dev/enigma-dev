SHARED_SRC_DIR := ../../shared
SOURCES += $(wildcard Widget_Systems/MacOSX/*.cpp)
SOURCES += Widget_Systems/General/WSdialogs.cpp
override CFLAGS += -I$(SHARED_SRC_DIR)/apifilesystem -IWidget_Systems/MacOSX
override CXXFLAGS += -I$(SHARED_SRC_DIR)/apifilesystem -IWidget_Systems/MacOSX
override LDLIBS += -lobjc -ObjC++
