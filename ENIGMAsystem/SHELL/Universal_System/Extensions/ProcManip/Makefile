ifeq ($(OS), Darwin)
	SOURCES += Universal_System/Extensions/ProcManip/MacOSX/procmanip.cpp
else ifeq ($(OS), Linux)
	SOURCES += Universal_System/Extensions/ProcManip/Linux/procmanip.cpp
        SOURCES += Universal_System/Extensions/ProcManip/xlib/procmanip.cpp
	override CXXFLAGS += $(shell pkg-config x11 --cflags)
	override CFLAGS += $(shell pkg-config x11 --cflags)
	override LDLIBS += $(shell pkg-config x11 --libs) -lprocps
else ifeq ($(OS), FreeBSD)
	SOURCES += Universal_System/Extensions/ProcManip/BSD/procmanip.cpp
        SOURCES += Universal_System/Extensions/ProcManip/xlib/procmanip.cpp
	override CXXFLAGS += $(shell pkg-config x11 --cflags)
	override CFLAGS += $(shell pkg-config x11 --cflags)
	override LDLIBS += $(shell pkg-config x11 --libs) -lutil
else
	SOURCES += Universal_System/Extensions/ProcManip/Win32/procmanip.cpp
endif

