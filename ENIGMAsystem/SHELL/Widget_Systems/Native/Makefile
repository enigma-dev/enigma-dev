override CFLAGS += -IWidget_Systems/Native -std=c++17 $(shell pkg-config --cflags sdl2) -DPROCESS_GUIWINDOW_IMPL -DUSE_SDL_POLLEVENT -DNULLIFY_STDERR
override CXXFLAGS += -IWidget_Systems/Native -std=c++17 $(shell pkg-config --cflags sdl2) -DPROCESS_GUIWINDOW_IMPL -DUSE_SDL_POLLEVENT -DNULLIFY_STDERR
ifeq ($(TARGET-PLATFORM), Windows)
	SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/win32/*.cpp)
	override LDLIBS += -lntdll -lgdiplus -lcomctl32 -lshlwapi -lcomdlg32 -lole32 -loleaut32 -luuid
else
	ifeq ($(TARGET-PLATFORM), MacOSX)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/macos/*.cpp)
		override LDLIBS += -lobjc -ObjC++ -framework AppKit -framework UniformTypeIdentifiers
	else ifeq ($(TARGET-PLATFORM), Linux)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/xlib/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/*.cpp)
	else ifeq ($(TARGET-PLATFORM), FreeBSD)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/xlib/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/*.cpp)
	else ifeq ($(TARGET-PLATFORM), DragonFlyBSD)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/xlib/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/*.cpp)	
	else ifeq ($(TARGET-PLATFORM), OpenBSD)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/xlib/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/*.cpp)
	endif
endif
SOURCES += $(wildcard Widget_Systems/Native/*.cpp)
