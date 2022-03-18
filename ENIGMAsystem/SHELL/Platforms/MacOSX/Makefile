ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif

SOURCES += $(wildcard Platforms/MacOSX/*.cpp) Platforms/General/POSIX/Utility.cpp
SOURCES += Platforms/MacOSX/Cocoa/Window.cpp
SOURCES += Platforms/MacOSX/Desktop/Window.cpp
override CFLAGS += -DENIGMA_PLATFORM_SDL
override CXXFLAGS += -DENIGMA_PLATFORM_SDL
