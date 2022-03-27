include Platforms/General/Makefile

ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif

SOURCES += $(wildcard Platforms/SDL/*.cpp) Platforms/General/POSIX/Utility.cpp

override CXXFLAGS += -DENIGMA_PLATFORM_SDL

ifeq ($(TARGET-PLATFORM), Web)
	override CXXFLAGS += -sUSE_SDL=2 -I$(ENIGMA_ROOT)/android/external/glm/
endif

ifneq ($(TARGET-PLATFORM), Android)
	override CXXFLAGS += $(shell $(PKG-CONFIG) --cflags sdl2)
	override LDLIBS += $(shell $(PKG-CONFIG) --libs --static sdl2)
endif

ifeq ($(TARGET-PLATFORM), Android)
    override CXXFLAGS += -I$(ENIGMA_ROOT)/android/external/SDL2/src/core/android/
	SOURCES += $(wildcard Platforms/SDL/Android/*.cpp)
	override ANDROID_MAKEFILES += SDL2/Android.mk
	override ANDROID_SHARED_LIBRARIES += SDL2
else
	SOURCES += Platforms/SDL/Desktop/Window.cpp
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
	else ifeq ($(TARGET-PLATFORM), Web)
		SOURCES += Platforms/SDL/Web/Window.cpp
	endif
endif
