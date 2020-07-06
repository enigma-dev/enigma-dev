ifeq ($(UNIX_BASED), true)
	ifeq ($(OS), Darwin)
		SOURCES += Universal_System/Extensions/ProcInfo/MacOSX/procinfo.cpp
		SOURCES += Universal_System/Extensions/ProcInfo/MacOSX/Cocoa/procinfo.mm
		SOURCES += Universal_System/Extensions/ProcInfo/MacOSX/Cocoa/subclass.mm
		override CXXFLAGS += -ObjC
		override CFLAGS += -ObjC
		override LDLIBS += -framework Cocoa
	else
		ifeq ($(OS), Linux)
			SOURCES += Universal_System/Extensions/ProcInfo/Linux/procinfo.cpp
			override LDLIBS += -lprocps
		else ifeq ($(OS), FreeBSD)
			SOURCES += Universal_System/Extensions/ProcInfo/BSD/procinfo.cpp
			override LDLIBS += -lc -lutil -lprocstat
		endif
		SOURCES += Universal_System/Extensions/ProcInfo/xlib/procinfo.cpp
		override CXXFLAGS += $(shell pkg-config x11 --cflags)
		override CFLAGS += $(shell pkg-config x11 --cflags)
		override LDLIBS += $(shell pkg-config x11 --libs)
	endif
	SOURCES += Universal_System/Extensions/ProcInfo/POSIX/procinfo.cpp
else
	SOURCES += Universal_System/Extensions/ProcInfo/Win32/procinfo.cpp
	override LDLIBS += -lole32 -lwbemuuid
endif
SOURCES += Universal_System/Extensions/ProcInfo/Universal/procinfo.cpp
