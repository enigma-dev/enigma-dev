ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif

SOURCES += $(wildcard Platforms/SDL/*.cpp) Platforms/General/POSIX/Utility.cpp
override CXXFLAGS += $(shell $(PKG-CONFIG) --cflags sdl2) -DENIGMA_PLATFORM_SDL

ifneq ($(TARGET-PLATFORM), Android)
	SOURCES += Platforms/General/POSIX/shell.cpp
else
	SOURCES += Platforms/SDL/Android/Window.cpp
endif

ifeq ($(TARGET-PLATFORM), Windows)
	SOURCES += Platforms/Win32/WINDOWSfilemanip.cpp
	SOURCES += Platforms/SDL/Win32/Window.cpp
override LDLIBS += $(shell $(PKG-CONFIG) --libs --static sdl2)
else
	SOURCES += Platforms/General/POSIX/POSIXfilemanip.cpp
	SOURCES += Platforms/General/POSIX/POSIXdirectory.cpp
	override LDLIBS += -lpthread $(shell $(PKG-CONFIG) --libs --static sdl2)

	ifeq ($(TARGET-PLATFORM), MacOSX)
		SOURCES += Platforms/SDL/Cocoa/Window.cpp
		SOURCES += Platforms/SDL/Cocoa/CocoaWindow.m
	endif

	ifeq ($(TARGET-PLATFORM), Linux)
		SOURCES += Platforms/SDL/xlib/Window.cpp
	else ifeq ($(TARGET-PLATFORM), FreeBSD)
		SOURCES += Platforms/SDL/xlib/Window.cpp
	endif
endif
