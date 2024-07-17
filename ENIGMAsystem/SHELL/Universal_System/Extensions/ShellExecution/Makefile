SHARED_SRC_DIR := ../../shared
override CXXFLAGS += -I$(SHARED_SRC_DIR) -std=c++17 -DPROCESS_GUIWINDOW_IMPL $(shell pkg-config --cflags sdl2) -DUSE_SDL_POLLEVENT
override CFLAGS += -I$(SHARED_SRC_DIR) -std=c++17 -DPROCESS_GUIWINDOW_IMPL $(shell pkg-config --cflags sdl2) -DUSE_SDL_POLLEVENT
SOURCES += Universal_System/Extensions/ShellExecution/xprocess.cpp
SOURCES += Universal_System/Extensions/ShellExecution/shell.cpp
