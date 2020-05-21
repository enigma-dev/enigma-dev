ifeq ($(UNIX_BASED), true)
	ifeq ($(OS), Darwin)
		SOURCES += Universal_System/Extensions/FileManip/MacOSX/fileman.cpp
	else
		ifeq ($(OS), Linux)
			SOURCES += Universal_System/Extensions/FileManip/Linux/fileman.cpp
		else ifeq ($(OS), FreeBSD)
			SOURCES += Universal_System/Extensions/FileManip/BSD/fileman.cpp
			override LDLIBS += -lc
		endif
	endif
	SOURCES += Universal_System/Extensions/FileManip/POSIX/fileman.cpp
else
	SOURCES += Universal_System/Extensions/FileManip/Win32/fileman.cpp
	override LDLIBS += -lshell32
endif
SOURCES += Universal_System/Extensions/FileManip/Universal/fileman.cpp
override CXXFLAGS += -fexceptions
override CFLAGS += -fexceptions
