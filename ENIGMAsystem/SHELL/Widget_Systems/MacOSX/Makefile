SHARED_SRC_DIR := ../../shared
SOURCES += $(wildcard Widget_Systems/MacOSX/*.cpp)
SOURCES += Widget_Systems/MacOSX/lib/ImGuiFileDialog/ImGuiFileDialog.cpp
SOURCES += Widget_Systems/General/WSdialogs.cpp
override CFLAGS += -I$(SHARED_SRC_DIR)/apifilesystem -IWidget_Systems/MacOSX -DUSE_STD_FILESYSTEM
override CXXFLAGS += -I$(SHARED_SRC_DIR)/apifilesystem -IWidget_Systems/MacOSX -DUSE_STD_FILESYSTEM
override LDLIBS += -lobjc -ObjC++
