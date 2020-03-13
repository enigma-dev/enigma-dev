OS := $(shell uname -s)
ifeq ($(OS), Linux)
	SOURCES += $(wildcard Platforms/General/POSIX/Linux/*.cpp)
else ifeq ($(OS), FreeBSD)
	SOURCES += $(wildcard Platforms/General/POSIX/BSD/*.cpp)
endif
SOURCES += $(wildcard Platforms/None/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override LDLIBS += -lz -lpthread