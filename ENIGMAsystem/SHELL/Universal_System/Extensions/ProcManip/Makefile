ifeq ($(UNIX_BASED), true)
	ifeq ($(OS), Darwin)
		SOURCES += Universal_System/Extensions/ProcManip/MacOSX/cocoa.m
		SOURCES += Universal_System/Extensions/ProcManip/MacOSX/procmanip.cpp
	else
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
	endif
	SOURCES += Universal_System/Extensions/ProcManip/POSIX/shell.cpp
else
	SOURCES += Universal_System/Extensions/ProcManip/Win32/procmanip.cpp
	SOURCES += Universal_System/Extensions/ProcManip/Win32/shell.cpp
endif
SOURCES += Universal_System/Extensions/ProcManip/procmanip.cpp
