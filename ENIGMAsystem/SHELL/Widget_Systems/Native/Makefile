SHARED_SRC_DIR := ../../shared
override CFLAGS += -IWidget_Systems/Native -DPROCESS_GUIWINDOW_IMPL -DUSE_SDL_POLLEVENT -DNULLIFY_STDERR
override CXXFLAGS += -IWidget_Systems/Native -DPROCESS_GUIWINDOW_IMPL -DUSE_SDL_POLLEVENT -DNULLIFY_STDERR
ifeq ($(TARGET-PLATFORM), Windows)
	SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/win32/*.cpp)
	SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/apiprocess/*.cpp)
	SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/*.cpp)
else
	ifeq ($(TARGET-PLATFORM), MacOSX)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/macos/*.mm)
		override LDLIBS += -lobjc -ObjC++
	else ifeq ($(TARGET-PLATFORM), Linux)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/xlib/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/apiprocess/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/*.cpp)		
	else ifeq ($(TARGET-PLATFORM), FreeBSD)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/xlib/*.cpp) 
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/apiprocess/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/*.cpp)		
	else ifeq ($(TARGET-PLATFORM), DragonFlyBSD)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/xlib/*.cpp) 
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/apiprocess/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/*.cpp)		
	else ifeq ($(TARGET-PLATFORM), OpenBSD)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/xlib/*.cpp) 
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/apiprocess/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/libdlgmod/general/*.cpp)		
	endif
endif
SOURCES += $(wildcard Widget_Systems/Native/*.cpp) $(SHARED_SRC_DIR)/xprocess.cpp
