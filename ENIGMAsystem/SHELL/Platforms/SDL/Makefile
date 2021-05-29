include Platforms/General/Makefile

ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif

SOURCES += $(wildcard Platforms/SDL/*.cpp) Platforms/General/POSIX/Utility.cpp

override CXXFLAGS += -DENIGMA_PLATFORM_SDL

ifneq ($(TARGET-PLATFORM), Android)
	override CXXFLAGS += $(shell $(PKG-CONFIG) --cflags sdl2)
	override LDLIBS += $(shell $(PKG-CONFIG) --libs --static sdl2)
endif

ifeq ($(TARGET-PLATFORM), Android)
	SOURCES += Platforms/SDL/Android/Window.cpp
  override ANDROID_MAKEFILES += SDL2/Android.mk
	override ANDROID_SHARED_LIBRARIES += SDL2
endif

ifeq ($(TARGET-PLATFORM), Windows)
	SOURCES += Platforms/Win32/WINDOWSfilemanip.cpp
	SOURCES += Platforms/SDL/Win32/Window.cpp
else
	ifneq ($(TARGET-PLATFORM), Android)
		SOURCES += Platforms/General/PFshell/PFshell.cpp
		SOURCES += Platforms/General/POSIX/shell.cpp
	endif

	SOURCES += Platforms/General/POSIX/POSIXfilemanip.cpp
	SOURCES += Platforms/General/POSIX/POSIXdirectory.cpp
	override LDLIBS += -lpthread

	ifeq ($(TARGET-PLATFORM), MacOSX)
		SOURCES += Platforms/SDL/Cocoa/Window.cpp
		SOURCES += Platforms/SDL/Cocoa/CocoaWindow.m
		override LDLIBS += -lobjc
	else ifeq ($(TARGET-PLATFORM), Linux)
		SOURCES += Platforms/SDL/xlib/Window.cpp
	else ifeq ($(TARGET-PLATFORM), FreeBSD)
		SOURCES += Platforms/SDL/xlib/Window.cpp
	endif
endif
