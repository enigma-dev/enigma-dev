SOURCES += $(wildcard Universal_System/Extensions/ttf/ttf.cpp)

ifeq ($(TARGET-PLATFORM), Windows)
  SOURCES += $(wildcard Universal_System/Extensions/ttf/win32_paths.cpp)
else
  SOURCES += $(wildcard Universal_System/Extensions/ttf/unix_paths.cpp)
endif

override CXXFLAGS += $(shell pkg-config --cflags freetype2)
override LDLIBS += $(shell pkg-config --libs freetype2)
