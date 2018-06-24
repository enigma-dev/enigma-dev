SOURCES += Universal_System/Extensions/ttf/ttf.cpp

ifeq ($(TARGET-PLATFORM), Windows)
	SOURCES += Universal_System/Extensions/ttf/find-win32.cpp
else ifeq ($(TARGET-PLATFORM), Linux)
	SOURCES += Universal_System/Extensions/ttf/find-xlib.cpp
else ifeq ($(TARGET-PLATFORM), MacOSX)
	SOURCES += Universal_System/Extensions/ttf/find-osx.cpp
endif

override CXXFLAGS += $(shell pkg-config --cflags freetype2)
override LDLIBS += $(shell pkg-config --libs --static freetype2) -lfreetype
