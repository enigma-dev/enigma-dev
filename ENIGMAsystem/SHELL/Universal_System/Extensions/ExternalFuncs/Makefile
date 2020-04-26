# if Josh fucking merges my FreeBSD pr we'll then have a better approach to check platforms
OS := $(shell uname -s)
ifeq ($(OS), Linux)
	override CXXFLAGS += -IUniversal_System/Extensions/ExternalFuncs/POSIX/
	override CFLAGS += -IUniversal_System/Extensions/ExternalFuncs/POSIX/
	override LDLIBS += -lffi -ldl
else ifeq ($(OS), FreeBSD)
	override CXXFLAGS += -IUniversal_System/Extensions/ExternalFuncs/POSIX/
	override CFLAGS += -IUniversal_System/Extensions/ExternalFuncs/POSIX/
	override LDLIBS += -lffi -ldl
else ifeq ($(OS), Darwin)
	override CXXFLAGS += -IUniversal_System/Extensions/ExternalFuncs/POSIX/
	override CFLAGS += -IUniversal_System/Extensions/ExternalFuncs/POSIX/
	override LDLIBS += -lffi -ldl
else
	override CXXFLAGS += -IUniversal_System/Extensions/ExternalFuncs/Win32/
	override CFLAGS += -IUniversal_System/Extensions/ExternalFuncs/Win32/
	override LDLIBS += -lffi
endif
SOURCES += Universal_System/Extensions/ExternalFuncs/ExternalFuncs.cpp