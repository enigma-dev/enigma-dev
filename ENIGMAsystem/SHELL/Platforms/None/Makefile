include Platforms/General/Makefile

ifeq ($(OS), Linux)
	SOURCES += $(wildcard Platforms/General/POSIX/Linux/*.cpp)
else ifeq ($(OS), FreeBSD)
	SOURCES += $(wildcard Platforms/General/POSIX/BSD/*.cpp)
endif
SOURCES += $(wildcard Platforms/None/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp) Platforms/General/PFshell/PFshell.cpp
override LDLIBS += -lz -lpthread
