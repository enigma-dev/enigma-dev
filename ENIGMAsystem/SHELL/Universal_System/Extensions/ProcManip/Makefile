ifeq ($(OS), Darwin)
	SOURCES += Universal_System/Extensions/ProcManip/MacOSX/cocoa.m
	SOURCES += Universal_System/Extensions/ProcManip/MacOSX/procmanip.cpp
else ifeq ($(UNIX_BASED), true)
	override CXXFLAGS += $(shell pkg-config x11 --cflags)
	override CFLAGS += $(shell pkg-config x11 --cflags)
	override LDLIBS += $(shell pkg-config x11 --libs)
	ifeq ($(OS), Linux)
		SOURCES += Universal_System/Extensions/ProcManip/Linux/procmanip.cpp
        	SOURCES += Universal_System/Extensions/ProcManip/xlib/procmanip.cpp
		override LDLIBS += -lprocps
	else ifeq ($(OS), FreeBSD)
		SOURCES += Universal_System/Extensions/ProcManip/BSD/procmanip.cpp
        	SOURCES += Universal_System/Extensions/ProcManip/xlib/procmanip.cpp
		override LDLIBS += -lutil
	endif
else
	SOURCES += Universal_System/Extensions/ProcManip/Win32/procmanip.cpp
endif
SOURCES += Universal_System/Extensions/ProcManip/procmanip.cpp
