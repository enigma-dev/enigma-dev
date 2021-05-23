ifeq ($(OS), Linux)
	SOURCES += $(wildcard Platforms/General/POSIX/Linux/*.cpp)
else ifeq ($(OS), FreeBSD)
	SOURCES += $(wildcard Platforms/General/POSIX/BSD/*.cpp)
endif
ifeq ($(UNIX_BASED), true)
	SOURCES += $(wildcard Platforms/General/POSIX/*.cpp) Platforms/General/PFshell/PFshell.cpp
else
	SOURCES += Platforms/General/POSIX/main.cpp Platforms/Win32/WINDOWSfilemanip.cpp Platforms/Win32/WINDOWSshell.cpp
	SOURCES += Platforms/General/POSIX/POSIXthreads.cpp Platforms/Win32/WINDOWSregistry.cpp
	override LDLIBS += -lffi -lcomdlg32 -lgdi32 -lwinmm -lwininet
endif
SOURCES += $(wildcard Platforms/None/*.cpp)
override LDLIBS += -lz -lpthread
