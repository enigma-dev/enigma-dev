ifeq ($(TARGET-PLATFORM), Windows)
	SOURCES += $(wildcard Widget_Systems/Native/win32/*.cpp)
	SOURCES += $(wildcard Widget_Systems/Native/general/apiprocess/*.cpp)
	SOURCES += $(wildcard Widget_Systems/Native/general/*.cpp)
else
	ifeq ($(TARGET-PLATFORM), MacOSX)
		SOURCES += $(wildcard Widget_Systems/Native/macos/*.mm)
		override LDLIBS += -lobjc -ObjC++
	else ifeq ($(TARGET-PLATFORM), Linux)
		SOURCES += $(wildcard Widget_Systems/Native/xlib/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/general/apiprocess/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/general/*.cpp)		
	else ifeq ($(TARGET-PLATFORM), FreeBSD)
		SOURCES += $(wildcard Widget_Systems/Native/xlib/*.cpp) 
		SOURCES += $(wildcard Widget_Systems/Native/general/apiprocess/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/general/*.cpp)		
	else ifeq ($(TARGET-PLATFORM), DragonFlyBSD)
		SOURCES += $(wildcard Widget_Systems/Native/xlib/*.cpp) 
		SOURCES += $(wildcard Widget_Systems/Native/general/apiprocess/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/general/*.cpp)		
	else ifeq ($(TARGET-PLATFORM), OpenBSD)
		SOURCES += $(wildcard Widget_Systems/Native/xlib/*.cpp) 
		SOURCES += $(wildcard Widget_Systems/Native/general/apiprocess/*.cpp)
		SOURCES += $(wildcard Widget_Systems/Native/general/*.cpp)		
	endif
endif
override CFLAGS += -IWidget_Systems/Native
override CXXFLAGS += -IWidget_Systems/Native
SOURCES += $(wildcard Widget_Systems/Native/*.cpp) 
